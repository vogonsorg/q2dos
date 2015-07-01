/******
gserverlist.c
GameSpy C Engine SDK
  
Copyright 1999 GameSpy Industries, Inc

Suite E-204
2900 Bristol Street
Costa Mesa, CA 92626
(714)549-7689
Fax(714)549-0757

******

 Please see the GameSpy C Engine SDK documentation for more 
 information

******/
#include "goaceng.h"
#include "gserver.h"
#include "darray.h"
#include "../nonport.h"
//#include "../../server/server.h" // FS
#include "../../client/client.h" // FS
#include "gutil.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> // FS
#include <errno.h> // FS

#ifndef _WIN32
#include <sys/ioctl.h>
#endif
extern char *NET_ErrorString (void);

#define MSHOST	"maraakate.org" // FS: Gamespy dead "master.gamespy.com"
#define MSPORT	28900
#define SERVER_GROWBY 32
#define LAN_SEARCH_TIME 3000 //3 sec

#ifdef __cplusplus
extern "C" {
#endif


//todo: check state changes on error
typedef struct 
{
	SOCKET s;
	int serverindex;
	unsigned long starttime;
} UpdateInfo;



struct GServerListImplementation
{
	GServerListState state;
	DArray servers;
	UpdateInfo *updatelist; //dynamic array of updateinfos
	char gamename[32];
	char seckey[32];
	char enginename[32];
	int maxupdates;
	int nextupdate;
	int abortupdate;
	ListCallBackFn CallBackFn;
	void *instance;
	char *sortkey;
	gbool sortascending;
	SOCKET slsocket;
	unsigned long lanstarttime;
};

GServerList g_sortserverlist; //global serverlist for sorting info!!


/* ServerListNew
----------------
Creates and returns a new (empty) GServerList. */
GServerList ServerListNew (char *gamename, char *enginename, char *seckey, int maxconcupdates, void *CallBackFn, int CallBackFnType, void *instance)
{
	GServerList list;

	list = (GServerList) malloc(sizeof(struct GServerListImplementation));
	assert(list != NULL);
	list->state = sl_idle;
	list->servers = ArrayNew(sizeof(GServer), SERVER_GROWBY, ServerFree);
	list->maxupdates = maxconcupdates;
	list->updatelist = calloc(maxconcupdates, sizeof(UpdateInfo));
	assert(list->updatelist != NULL);
	strcpy(list->gamename, gamename);
	strcpy(list->seckey, seckey);
	strcpy(list->enginename, enginename);
	list->CallBackFn = CallBackFn;
	assert(CallBackFn != NULL);
	list->instance = instance;
	list->sortkey = "";
	SocketStartUp();
	return list;	
}

/* ServerListFree
-----------------
Free a GServerList and all internal sturctures and servers */
void ServerListFree(GServerList serverlist)
{
	ArrayFree(serverlist->servers);
	serverlist->servers = NULL;
	free(serverlist->updatelist);
	serverlist->updatelist = NULL;
	
	free(serverlist);
	serverlist = NULL;
	SocketShutDown();
}

 //create update sockets and init structures
static GError InitUpdateList(GServerList serverlist)
{
	int i;
	qboolean _true = true;

	for (i = 0 ; i < serverlist->maxupdates ; i++)
	{
		serverlist->updatelist[i].s = socket (AF_INET, SOCK_DGRAM,IPPROTO_UDP);
		if (serverlist->updatelist[i].s == INVALID_SOCKET)
			return GE_NOSOCKET;

		// FS: Set non-blocking sockets
		if (ioctlsocket( serverlist->updatelist[i].s, FIONBIO,IOCTLARG_T &_true) == SOCKET_ERROR)
		{
			Com_Printf("ERROR: InitUpdateList: ioctl FIOBNIO:%s\n", NET_ErrorString());
			return GE_NOSOCKET;
		}

		serverlist->updatelist[i].serverindex = -1;
		serverlist->updatelist[i].starttime = 0;
	}
	return 0;
}

//free update sockets 
static GError FreeUpdateList(GServerList serverlist) 
{
	int i;

#ifdef __DJGPP__
	Com_Printf("Freeing gamespy query sockets, please wait. . .\n"); // FS: This has a little bit of a delay in DOS, so give a warning about it
#endif
	for (i = 0 ; i < serverlist->maxupdates ; i++)
	{
		closesocket(serverlist->updatelist[i].s);
	}
	return 0;


}

//create and connect a server list socket
static GError CreateServerListSocket(GServerList serverlist)
{
	struct   sockaddr_in saddr;
	struct hostent *hent;

	if (cl_master_server_ip->string[0] == '\0')
	{
		Com_Printf("Error: cl_master_server_ip is blank!  Setting to default: %s\n", CL_MASTER_ADDR);
		Cvar_Set("cl_master_server_ip", CL_MASTER_ADDR);
	}
	if (cl_master_server_port->value <= 0)
	{
		Com_Printf("Error: cl_master_server_port is invalid!  Setting to default: %s\n", CL_MASTER_PORT);
		Cvar_Set("cl_master_server_port", CL_MASTER_PORT);
	}

	serverlist->slsocket = socket ( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if (serverlist->slsocket == INVALID_SOCKET)
		return GE_NOSOCKET;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons((unsigned short)cl_master_server_port->value);
	hent = gethostbyname(cl_master_server_ip->string);
	if (!hent)
		return GE_NODNS; 
	saddr.sin_addr.s_addr = *(u_long *)hent->h_addr_list[0];
	memset ( saddr.sin_zero, 0, 8 ); 
	if (connect ( serverlist->slsocket, (struct sockaddr *) &saddr, sizeof saddr ) != 0)
		return GE_NOCONNECT; 

	//else we are connected
	return 0;


}


//create and connect a server list socket
static GError CreateServerListLANSocket(GServerList serverlist)
{
	int optval = 1;

	serverlist->slsocket = socket ( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if (serverlist->slsocket == INVALID_SOCKET)
		return GE_NOSOCKET;
	if (setsockopt(serverlist->slsocket, SOL_SOCKET, SO_BROADCAST, (char *)&optval, sizeof(optval)) != 0)
		return GE_NOSOCKET;

	//else we are ready to broadcast
	return 0;


}

//trigger the callback and set the new mode
static void ServerListModeChange(GServerList serverlist, GServerListState newstate)
{
	serverlist->state = newstate;
	serverlist->CallBackFn(serverlist, LIST_STATECHANGED, serverlist->instance, NULL, NULL);

}


// validate us to the master and send a list request
#define SECURE "\\secure\\"
static GError SendListRequest(GServerList serverlist)
{
	char data[256], *ptr, result[64];
	int len;

	
	len = recv(serverlist->slsocket, data, sizeof(data) - 1, 0);
	if (len == SOCKET_ERROR)
		return GE_NOCONNECT;
	data[len] = '\0'; //null terminate it
	
	ptr = strstr ( data, SECURE ) + strlen(SECURE);
	gs_encrypt   ( (uchar *) serverlist->seckey, 6, (uchar *)ptr, 6 );
	gs_encode ( (uchar *)ptr, 6, (uchar *)result );

	//validate to the master
	sprintf(data, "\\gamename\\%s\\gamever\\%s\\location\\0\\validate\\%s\\final\\\\queryid\\1.1\\",
			serverlist->enginename, ENGINE_VERSION, result); //validate us		
	
	len = send ( serverlist->slsocket, data, strlen(data), 0 );
	if (len == SOCKET_ERROR || len == 0)
		return GE_NOCONNECT;

	//send the list request
	sprintf(data, "\\list\\gamename\\%s\\final\\", serverlist->gamename);
	len = send ( serverlist->slsocket, data, strlen(data), 0 );
	if (len == SOCKET_ERROR || len == 0)
		return GE_NOCONNECT;

	ServerListModeChange(serverlist, sl_listxfer);
	return 0;
}


static GError SendBroadcastRequest(GServerList serverlist, int startport, int endport, int delta)
{
	struct   sockaddr_in saddr;
	short i;

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = 0xFFFFFFFF; //broadcast
	for (i = startport ; i <= endport ; i += delta)
	{
		saddr.sin_port = htons(i);
		sendto(serverlist->slsocket, "\\status\\",8,0,(struct sockaddr *)&saddr,sizeof(saddr));
	}
	ServerListModeChange(serverlist, sl_lanlist);
	serverlist->lanstarttime = current_time();
	return 0;



}

//just wait for the server list to become idle
static void DoSyncLoop(GServerList serverlist)
{
	while (serverlist->state != sl_idle)
	{
		ServerListThink(serverlist);
		msleep(10);
		
	}
	
}

/* ServerListUpdate
-------------------
Start updating a GServerList. */
GError ServerListUpdate(GServerList serverlist, gbool async)
{
	GError error;

	if (serverlist->state != sl_idle)
		return GE_BUSY;

	error = InitUpdateList(serverlist);

	Com_DPrintf(DEVELOPER_MSG_GAMESPY, "Gamespy ServerListUpdate: Created Update list\n");
	if (error)
		return error;
	error = CreateServerListSocket(serverlist);
	Com_DPrintf(DEVELOPER_MSG_GAMESPY, "Gamespy ServerListUpdate: Created ServerListSocket list\n");
	if (error)
		return error;
	error = SendListRequest(serverlist);
	Com_DPrintf(DEVELOPER_MSG_GAMESPY, "Gamespy ServerListUpdate: Send List Request\n");
	if (error)
		return error;

	serverlist->nextupdate = 0;
	serverlist->abortupdate = 0;
	if (!async)
		DoSyncLoop(serverlist);

	return 0;
}

/* ServerListLANUpdate
-------------------
Start updating a GServerList from servers on the LAN. */
GError ServerListLANUpdate(GServerList serverlist, gbool async, int startsearchport, int endsearchport, int searchdelta)
{
	GError error;

	assert(searchdelta > 0);

	if (serverlist->state != sl_idle)
		return GE_BUSY;

	error = InitUpdateList(serverlist);
	if (error) return error;
	error = CreateServerListLANSocket(serverlist);
	if (error) return error;
	error = SendBroadcastRequest(serverlist, startsearchport, endsearchport, searchdelta);
	if (error) return error;
	serverlist->nextupdate = 0;
	serverlist->abortupdate = 0;
	if (!async)
		DoSyncLoop(serverlist);

	return 0;
}


//add the server to the list with the given ip, port
static void ServerListAddServer(GServerList serverlist, char *ip, int port)
{
	GServer server;
	server =  ServerNew(ip, port);
	ArrayAppend(serverlist->servers,&server);
 //printf("%d %s:%d\n",++count, ip,port);
}


static GError ServerListLANList(GServerList serverlist)
{
	struct timeval timeout = {0,0};
	fd_set set;
	char indata[1500];
	struct   sockaddr_in saddr;
	int saddrlen = sizeof(saddr);
	int error;

	while (1) //we break if the select fails
	{
		FD_ZERO(&set);
		FD_SET( serverlist->slsocket, &set);
		error = select(FD_SETSIZE, &set, NULL, NULL, &timeout);
		if (SOCKET_ERROR == error || 0 == error) //no data
			break;
		error = recvfrom(serverlist->slsocket, indata, sizeof(indata) - 1, 0, (struct sockaddr *)&saddr, &saddrlen );
		if (SOCKET_ERROR == error)
			continue; 
		//we got data, add the server to the list to update
		if (strstr(indata,"\\final\\") != NULL)
			ServerListAddServer(serverlist, inet_ntoa(saddr.sin_addr), ntohs(saddr.sin_port));
	}
	if (current_time() - serverlist->lanstarttime > LAN_SEARCH_TIME) //done waiting for replies
	{
		closesocket(serverlist->slsocket);
		serverlist->slsocket = INVALID_SOCKET;
		ServerListModeChange(serverlist, sl_querying);
	}
	return 0;

}
							 
//reads the server list from the socket and parses it
static GError ServerListReadList(GServerList serverlist)
{
	static char data[2048]; //static input buffer
	int len, oldlen;
	char *p, ip[32], port[16],*q, *lastip;
//append to data
	oldlen = strlen(data);
	len = recv(serverlist->slsocket, data + oldlen, sizeof(data) - oldlen - 1, 0);
	if (len == SOCKET_ERROR || len == 0)
		return GE_NOCONNECT;

	data[len + oldlen] = 0; //null terminate it
	// data is in the form of '\ip\1.2.3.4:1234\ip\1.2.3.4:1234\final\'
	Com_DPrintf(DEVELOPER_MSG_GAMESPY, "List xfer data: %s\n", data);

	lastip = data;
	while (*lastip != '\0')
	{
		p = lastip;
		if (*(p+1) == 'f' || serverlist->abortupdate) //\final\!!
		{
			closesocket(serverlist->slsocket);
			serverlist->slsocket = INVALID_SOCKET;
			data[0] = 0; //clear data so it can be used again
			ServerListModeChange(serverlist, sl_querying);
			return 0; //get out!!
		}
		if (strlen(p) < 14) //no way it could be a full IP, quit
			break;
		p += 4; //skip the '\ip\'
		if (strchr(p,':') == NULL || strchr(p,'\\') == NULL)
			break; //it's not the full ip:port
		q = ip; //fill the ip buffer
		while (*p != 0 && *p != ':')
			*q++ = *p++;
		
		*q = '\0'; //null terminate the ip
		p++; //skip the :
		q = port;
		while (*p != 0 && *p != '\\')
		*q++ = *p++;
		*q = '\0';
		lastip = p; //store the new position
		ServerListAddServer(serverlist, ip, atoi(port));
	}
	memmove(data,lastip,strlen(lastip) + 1); //shift it over
	return 0;

}

//loop through pending queries and send out new ones
#define OOB_SEQ "\xff\xff\xff\xff" //32 bit integer (-1) as string sequence for out of band data
#define STATUS "\xff\xff\xff\xffstatus"

#ifdef WIN32
static GError ServerListQueryLoop(GServerList serverlist)
{
	int i, scount = 0, error;
	fd_set set;
	struct timeval timeout = {1,0};
	char indata[1500];
	struct sockaddr_in saddr;
	int saddrlen = sizeof(saddr);
	float percent = 0; // FS
	GServer server;

//first, check for available data
	FD_ZERO(&set);
	for (i = 0 ; i < serverlist->maxupdates ; i++)
	{
		if (serverlist->updatelist[i].serverindex >= 0) //there is a server waiting
		{
			scount++;
			FD_SET( serverlist->updatelist[i].s, &set);
		}
	}

	if (scount > 0) //there are sockets to check for data
	{
		error = select(serverlist->maxupdates + 1, &set, NULL, NULL, &timeout);
		if (SOCKET_ERROR != error && 0 != error)
		{
			for (i = 0 ; i < serverlist->maxupdates ; i++)
			{
				if (serverlist->updatelist[i].serverindex >= 0 && FD_ISSET(serverlist->updatelist[i].s, &set) ) //there is a server waiting
				{ //we can read data!!
					error = recvfrom(serverlist->updatelist[i].s, indata, sizeof(indata) - 1, 0, (struct sockaddr *)&saddr, &saddrlen );
					if (SOCKET_ERROR != error) //we got data
					{
						indata[error] = 0; //truncate and parse it
						server = *(GServer *)ArrayNth(serverlist->servers,serverlist->updatelist[i].serverindex);
						if (server->ping == 9999) //set the ping
						{
							server->ping = current_time() - serverlist->updatelist[i].starttime;
						}
						ServerParseKeyVals(server, indata); 
						serverlist->CallBackFn(serverlist, 
												LIST_PROGRESS, 
												serverlist->instance,
												server,
												(void *)((serverlist->nextupdate * 100) / ArrayLength(serverlist->servers))); //percent done
						serverlist->updatelist[i].serverindex = -1; //reuse the updatelist
					}
					else
					{
						serverlist->updatelist[i].serverindex = -1; //reuse the updatelist
					}
				}
				Sys_SendKeyEvents (); // FS: Check for aborts
			}
		}
	}

	//kill expired ones
	for (i = 0 ; i < serverlist->maxupdates ; i++)
	{
		if (serverlist->updatelist[i].serverindex >= 0 && current_time() - serverlist->updatelist[i].starttime > SERVER_TIMEOUT ) 
		{
			serverlist->updatelist[i].serverindex = -1; //reuse the updatelist
		}
	}

	if (serverlist->abortupdate || ((serverlist->nextupdate >= ArrayLength(serverlist->servers) && scount == 0))) 
	{ //we are done!!
		if(!serverlist->abortupdate) // FS: Don't print if this was a forced abort
		{
			Com_Printf("\x02Server scan complete!\n");
			S_GamespySound ("gamespy/complete.wav");
		}
		FreeUpdateList(serverlist);
		ServerListModeChange(serverlist, sl_idle);
		return 0;
	}

//now, send out queries on available sockets
	for (i = 0 ; i < serverlist->maxupdates && serverlist->nextupdate < ArrayLength(serverlist->servers) ; i++)
	{
		if (serverlist->updatelist[i].serverindex < 0) //it's availalbe
		{
			serverlist->updatelist[i].serverindex = serverlist->nextupdate++;

			server = *(GServer *)ArrayNth(serverlist->servers,serverlist->updatelist[i].serverindex);
			saddr.sin_family = AF_INET;
			saddr.sin_addr.s_addr = inet_addr(ServerGetAddress(server));
			saddr.sin_port = htons((short)ServerGetQueryPort(server));

			error = sendto(serverlist->updatelist[i].s,STATUS,strlen(STATUS), 0, (struct sockaddr *) &saddr, saddrlen);
			serverlist->updatelist[i].starttime = current_time();
		}
		percent = ((float)serverlist->nextupdate/(float)ServerListCount(serverlist)) * 100;
		cls.gamespypercent = percent;
		SCR_UpdateScreen(); // FS: Force an update so the percentage bar shows some progress
		Sys_SendKeyEvents (); // FS: Check for aborts
	}
	return 0;
}
#else // DOS
// FS: DOS is all sorts of fucked with sockets.  This will have to do sadly :(
static GError ServerListQueryLoop(GServerList serverlist)
{
	int i, error;
	fd_set read_fd;
	fd_set write_fd;
	struct timeval timeout = {0,0}; // FS: One full second, after that fuck it
	char indata[1500];
	struct sockaddr_in saddr;
	int saddrlen = sizeof(saddr);
	int server_timeout = bound(100, cl_master_server_timeout->intValue, 9000); // FS: Now a CVAR
	float percent = 0.0f; // FS
	GServer server;

	for (i = 0 ; i < serverlist->maxupdates && serverlist->nextupdate < ArrayLength(serverlist->servers) ; i++)
	{
		if (serverlist->abortupdate) // FS: Check if we want to stop.
		{
			FreeUpdateList(serverlist);
			ServerListModeChange(serverlist, sl_idle);
			return 0;
		}

		if (serverlist->updatelist[i].serverindex < 0) //it's availalbe
		{
			FD_ZERO(&write_fd);
			FD_SET(serverlist->updatelist[i].s, &read_fd);

			serverlist->updatelist[i].serverindex = serverlist->nextupdate++;
			server = *(GServer *)ArrayNth(serverlist->servers,serverlist->updatelist[i].serverindex);
			saddr.sin_family = AF_INET;
			saddr.sin_addr.s_addr = inet_addr(ServerGetAddress(server));

			Com_DPrintf(DEVELOPER_MSG_GAMESPY, "Attempting to ping[%i]: %s:%i\n", i, ServerGetAddress(server), ServerGetQueryPort(server));

			saddr.sin_port = htons((short)ServerGetQueryPort(server));

			error = sendto(serverlist->updatelist[i].s,STATUS,strlen(STATUS), 0, (struct sockaddr *) &saddr, saddrlen);

			if (SOCKET_ERROR != error)
			{
				serverlist->updatelist[i].starttime = current_time();
			}
			else
			{
				Com_DPrintf(DEVELOPER_MSG_GAMESPY, "Error pinging server: %i\n", errno);
				serverlist->updatelist[i].serverindex = -1; // reuse the update index
			}

			FD_ZERO(&read_fd);
			FD_SET(serverlist->updatelist[i].s, &write_fd);
			select_s(serverlist->updatelist[i].s + 1, NULL, &write_fd, NULL, &timeout);

			if (FD_ISSET(serverlist->updatelist[i].s, &write_fd))
			{
retry:
				error = recvfrom(serverlist->updatelist[i].s, indata, sizeof(indata) - 1, 0, (struct sockaddr *)&saddr, &saddrlen );

				if (SOCKET_ERROR != error) //we got data
				{
					indata[error] = 0; //truncate and parse it
					server = *(GServer *)ArrayNth(serverlist->servers,serverlist->updatelist[i].serverindex);

					if (server->ping == 9999) //set the ping
					{
						server->ping = current_time() - serverlist->updatelist[i].starttime;
					}

					ServerParseKeyVals(server, indata); 
					serverlist->CallBackFn(serverlist, 
										LIST_PROGRESS, 
										serverlist->instance,
										server,
										(void *)((serverlist->nextupdate * 100) / ArrayLength(serverlist->servers))); //percent done
						serverlist->updatelist[i].serverindex = -1; //reuse the updatelist
				}
				else
				{
					if ( (errno == EWOULDBLOCK) && (current_time() - serverlist->updatelist[i].starttime < server_timeout) )

					{
						Sys_SendKeyEvents (); // FS: Check for aborts because this takes a while in DOS
//						Com_DPrintf(DEVELOPER_MSG_GAMESPY, "Slow down cowboy[]..\n");
						goto retry;
					}
					else // FS: If we got didn't get EWOULDBLOCK or if it just kept going past the server_timeout threshold then just give up.
					{
						Com_DPrintf(DEVELOPER_MSG_GAMESPY, "Error during gamespy recv %d\n", errno);
						serverlist->updatelist[i].serverindex = -1; //reuse the updatelist
					}
				}
			}
		}

		percent = ((float)serverlist->nextupdate/(float)ServerListCount(serverlist)) * 100.0f;
		cls.gamespypercent = percent;
		SCR_UpdateScreen(); // FS: Force an update so the percentage bar shows some progress
		Sys_SendKeyEvents (); // FS: Check for aborts because this takes a while in DOS
	}

#if 0
	//kill expired ones
	for (i = 0 ; i < serverlist->maxupdates ; i++)
	{
		if (serverlist->updatelist[i].serverindex >= 0 && current_time() - serverlist->updatelist[i].starttime > SERVER_TIMEOUT ) 
		{
			serverlist->updatelist[i].serverindex = -1; //reuse the updatelist
		}
	}
#endif

	if (serverlist->abortupdate || (serverlist->nextupdate >= ArrayLength(serverlist->servers))) 
	{ //we are done!!
		if(!serverlist->abortupdate) // FS: Don't print if this was a forced abort
		{
			Com_Printf("\x02Server scan complete!\n");
			S_GamespySound ("gamespy/complete.wav");
		}
		FreeUpdateList(serverlist);
		ServerListModeChange(serverlist, sl_idle);
		return 0;
	}
	return 0;
}
#endif // WIN32

/* ServerListThink
------------------
For use with Async Updates. This needs to be called every ~10ms for list processing and
updating to occur during async server list updates */
GError ServerListThink(GServerList serverlist)
{

	switch (serverlist->state)
	{
		case sl_idle:
			return 0;
		case sl_listxfer:
				Com_DPrintf(DEVELOPER_MSG_GAMESPY, "Gamespy ServerListThink: Server List xfer\n");
				 //read the data
				return ServerListReadList(serverlist);
				break;
		case sl_lanlist:
				Com_DPrintf(DEVELOPER_MSG_GAMESPY, "Gamespy ServerListThink: Server Lan List Query\n");
				return ServerListLANList(serverlist);
		case sl_querying: 
				//do some queries
				Com_DPrintf(DEVELOPER_MSG_GAMESPY, "Gamespy ServerListThink: Server List Query Loop\n");
				return ServerListQueryLoop(serverlist);
				break;
	}


	return 0;
}

/* ServerListHalt
-----------------
Halts the current update batch */
GError ServerListHalt(GServerList serverlist)
{
//	if (serverlist->state != sl_idle) // FS: Immediately abort.
		serverlist->abortupdate = 1;

	return 0;
}

/* ServerListClear
------------------
Clear and free all of the servers from the server list.
List must be in the sl_idle state */
GError ServerListClear(GServerList serverlist)
{
	
	if (serverlist->state != sl_idle)
		return GE_BUSY;
	//fastest way to clear is kill and recreate
	ArrayFree(serverlist->servers);
	serverlist->servers = ArrayNew(sizeof(GServer), SERVER_GROWBY, ServerFree);
	return 0;
}

/* ServerListState
------------------
Returns the current state of the server list */
GServerListState ServerListState(GServerList serverlist)
{
	return serverlist->state;
}

/* ServerListErrorDesc
----------------------
Returns a static string description of the specified error */
char *ServerListErrorDesc(GServerList serverlist, GError error)
{
	switch (error)
	{
	case GE_NOERROR:
		return "";
	case GE_NOSOCKET:
		return "Unable to create socket";
	case GE_NODNS:
		return "Unable to resolve master";
	case GE_NOCONNECT:
		return "Connection to master reset";
	case GE_BUSY:
		return "Server List is busy";
	}
	return "UNKNOWN ERROR CODE";

}

/* ServerListGetServer
----------------------
Returns the server at the specified index, or NULL if the index is out of bounds */
GServer ServerListGetServer(GServerList serverlist, int index)
{
	if (index < 0 || index >= ArrayLength(serverlist->servers))
		return NULL;
	return *(GServer *)ArrayNth(serverlist->servers,index);
}

/* ServerListCount
------------------
Returns the number of servers on the specified list. Indexing is 0 based, so
the actual server indexes are 0 <= valid index < Count */
int ServerListCount(GServerList serverlist)
{
	return ArrayLength(serverlist->servers);
}

/****
Comparision Functions
***/
static int IntKeyCompare(const void *entry1, const void *entry2)
{
	GServer server1 = *(GServer *)entry1, server2 = *(GServer *)entry2;
	int diff;
	diff = ServerGetIntValue(server1, g_sortserverlist->sortkey, 0) - 
			ServerGetIntValue(server2, g_sortserverlist->sortkey, 0);
	if (!g_sortserverlist->sortascending) 
		diff = -diff;
	return diff;
	
}

static int FloatKeyCompare(const void *entry1, const void *entry2)
{
    GServer server1 = *(GServer *)entry1, server2 = *(GServer *)entry2;
	double f = ServerGetFloatValue(server1, g_sortserverlist->sortkey, 0) - 
			ServerGetFloatValue(server2, g_sortserverlist->sortkey, 0);
	if (!g_sortserverlist->sortascending) 
		f = -f;
	if (f > 0) return 1; else if (f < 0) return -1; else return 0;
}

static int StrCaseKeyCompare(const void *entry1, const void *entry2)
{
	
    GServer server1 = *(GServer *)entry1, server2 = *(GServer *)entry2;
    int diff = strcmp(ServerGetStringValue(server1, g_sortserverlist->sortkey, ""),
				ServerGetStringValue(server2, g_sortserverlist->sortkey, ""));
	if (!g_sortserverlist->sortascending) 
		diff = -diff;
	return diff;
}

static int StrNoCaseKeyCompare(const void *entry1, const void *entry2)
{
	GServer server1 = *(GServer *)entry1, server2 = *(GServer *)entry2;
	int diff = strcasecmp(ServerGetStringValue(server1, g_sortserverlist->sortkey, ""),
				ServerGetStringValue(server2, g_sortserverlist->sortkey, ""));
	if (!g_sortserverlist->sortascending) 
		diff = -diff;
	return diff;
}

/* ServerListSort
-----------------
Sort the server list in either ascending or descending order using the 
specified comparemode.
sortkey can be a normal server key, or "ping" or "hostaddr" */
void ServerListSort(GServerList serverlist, gbool ascending, char *sortkey, GCompareMode comparemode)
{
	ArrayCompareFn comparator = 0; // FS: Compiler warning
	switch (comparemode)
	{
		case cm_int:
			comparator = IntKeyCompare;
			break;
		case cm_float:
			comparator = FloatKeyCompare;
			break;
		case cm_strcase:
			comparator = StrCaseKeyCompare;
			break;
		case cm_stricase:
			comparator = StrNoCaseKeyCompare;
			break;
	}
	serverlist->sortkey = sortkey;
	serverlist->sortascending = ascending;
	g_sortserverlist = serverlist;
	ArraySort(serverlist->servers,comparator);

}


#ifdef __cplusplus
}
#endif

