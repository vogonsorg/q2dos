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

#ifdef GAMESPY /* FS: This way we can still leave the files in the VS6 makefile and not hear complaints about missing CVARs */

#include "goaceng.h"
#include "gserver.h"
#include "darray.h"
#include "../nonport.h"
#include "../../client/client.h" /* FS: For the cl_gamespy CVARs */
#include "gutil.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h> /* FS: Needed for non-blocking sockets */
#include <errno.h> /* FS: Needed for non-blocking sockets */

#ifndef _WIN32
#include <sys/ioctl.h>
#endif

#define SERVER_GROWBY 32
#define LAN_SEARCH_TIME 3000 /* 3 sec */

static cvar_t	*cl_master_server_retries;
static cvar_t	*cl_master_server_port;
static cvar_t	*cl_master_server_ip;
static cvar_t	*cl_master_server_timeout;
static cvar_t	*cl_master_server_optout;
static cvar_t	*playerName;

gspyimport_t gspyi;

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

static GServerList g_sortserverlist; /* global serverlist for sorting info */
static int totalRetry = 20; /* FS: Total retry attempts waiting for the gamespy validate stuff */

void InitGamespy(void)
{
	cl_master_server_retries = gspyi.cvar("cl_master_server_retries", "20", CVAR_ARCHIVE);
	cl_master_server_port = gspyi.cvar("cl_master_server_port", "28900", CVAR_ARCHIVE);
	cl_master_server_ip = gspyi.cvar("cl_master_server_ip", "maraakate.org", CVAR_ARCHIVE);
	cl_master_server_timeout = gspyi.cvar("cl_master_server_timeout", "3000", CVAR_ARCHIVE);
	cl_master_server_optout = gspyi.cvar("cl_master_server_optout", "0", CVAR_ARCHIVE);
	playerName = gspyi.cvar("name", "unnamed", CVAR_USERINFO | CVAR_ARCHIVE);
}

void ShutdownGamespy (void)
{
	/* FS: No clean-up afaik */
}

gspyexport_t *GetGameSpyAPI (gspyimport_t *import)
{
	static gspyexport_t gspye;

	gspyi = *import;

	gspye.api_version = GAMESPY_API_VERSION;
	gspye.Init = InitGamespy;
	gspye.Shutdown = ShutdownGamespy;
	gspye.ServerListNew = ServerListNew;
	gspye.ServerListFree = ServerListFree;
	gspye.ServerListUpdate = ServerListUpdate;
	gspye.ServerListThink = ServerListThink;
	gspye.ServerListHalt = ServerListHalt;
	gspye.ServerListClear = ServerListClear;
	gspye.ServerListState = ServerListState;
	gspye.ServerListErrorDesc = ServerListErrorDesc;
	gspye.ServerListSort = ServerListSort;
	gspye.ServerListGetServer = ServerListGetServer;

	gspye.ServerGetPing = ServerGetPing;
	gspye.ServerGetAddress = ServerGetAddress;
	gspye.ServerGetIntValue = ServerGetIntValue;
	gspye.ServerGetQueryPort = ServerGetQueryPort;
	gspye.ServerGetStringValue = ServerGetStringValue;

	return &gspye;
}

/* FS: Set a socket to be non-blocking */
#ifdef _WIN32
#define TCP_BLOCKING_ERROR WSAEWOULDBLOCK
static int Set_Non_Blocking_Socket (SOCKET s) {
	u_long _true = true;

	return ioctlsocket( s, FIONBIO, &_true);
}

static __inline int Get_Last_Error(void) {
	return WSAGetLastError();
}
#else
#define TCP_BLOCKING_ERROR EWOULDBLOCK
static int Set_Non_Blocking_Socket (SOCKET s) {
	int _true = true;
	return ioctlsocket( s, FIONBIO, IOCTLARG_T &_true);
}

static __inline int Get_Last_Error(void) {
	return errno;
}
#endif

static void Close_TCP_Socket(SOCKET *s)
{
	closesocket(*s);
	*s = INVALID_SOCKET;
}

/* ServerListNew
----------------
Creates and returns a new (empty) GServerList. */
GServerList ServerListNew (char *gamename, char *enginename, char *seckey, int maxconcupdates, void *CallBackFn, int CallBackFnType, void *instance)
{
	GServerList list;

	list = (GServerList) malloc(sizeof(struct GServerListImplementation));
	if(!list)
		gspyi.error("ServerListNew: list is NULL.");
	list->state = sl_idle;
	list->servers = ArrayNew(sizeof(GServer), SERVER_GROWBY, ServerFree);
	list->maxupdates = maxconcupdates;
	list->updatelist = calloc(maxconcupdates, sizeof(UpdateInfo));
	if(!list->updatelist)
		gspyi.error("ServerListNew: list->updatelist is NULL.");
	strcpy(list->gamename, gamename);
	strcpy(list->seckey, seckey);
	strcpy(list->enginename, enginename);
	list->CallBackFn = CallBackFn;
	if(!CallBackFn)
		gspyi.error("ServerListNew: CallBackFn is NULL.");
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

	for (i = 0 ; i < serverlist->maxupdates ; i++)
	{
		serverlist->updatelist[i].s = socket (AF_INET, SOCK_DGRAM,IPPROTO_UDP);
		if (serverlist->updatelist[i].s == INVALID_SOCKET)
		{
			return GE_NOSOCKET;
		}

		/* FS: Set non-blocking sockets */
		if (Set_Non_Blocking_Socket(serverlist->updatelist[i].s) == SOCKET_ERROR)
		{
//			gspyi.print("ERROR: InitUpdateList: ioctl FIOBNIO:%s\n", gspyi.net_strerror());
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
	gspyi.print("Freeing gamespy query sockets, please wait. . .\n"); /* FS: This can have a little bit of a delay in DOS, so give a warning about it */
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
		gspyi.print("Error: cl_master_server_ip is blank!  Setting to default: %s\n", CL_MASTER_ADDR);
		gspyi.cvar_set("cl_master_server_ip", CL_MASTER_ADDR);
	}
	if (cl_master_server_port->intValue <= 0)
	{
		gspyi.print("Error: cl_master_server_port is invalid!  Setting to default: %s\n", CL_MASTER_PORT);
		gspyi.cvar_set("cl_master_server_port", CL_MASTER_PORT);
	}

	serverlist->slsocket = socket ( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if (serverlist->slsocket == INVALID_SOCKET)
		return GE_NOSOCKET;

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons((unsigned short)cl_master_server_port->value);
	hent = gethostbyname(cl_master_server_ip->string);

	if (!hent)
	{
		Close_TCP_Socket(&serverlist->slsocket);
		return GE_NODNS;
	}

	saddr.sin_addr.s_addr = *(u_long *)hent->h_addr_list[0];
	memset ( saddr.sin_zero, 0, 8 );

	if (connect ( serverlist->slsocket, (struct sockaddr *) &saddr, sizeof saddr ) != 0 )
	{
		Close_TCP_Socket(&serverlist->slsocket);
		return GE_NOSOCKET;
	}

	if(Set_Non_Blocking_Socket(serverlist->slsocket) == SOCKET_ERROR)
	{
		gspyi.print("ERROR: CreateServerListSocket: ioctl FIOBNIO:%s\n", gspyi.net_strerror());
		Close_TCP_Socket(&serverlist->slsocket);
		return GE_NOSOCKET;
	}

	totalRetry = bound(1,cl_master_server_retries->intValue,100);

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
	{
		gspyi.print("ERROR: CreateServerListLANSocket: setsockopt SOL_SOCKET, SO_BROADCAST:%s\n", gspyi.net_strerror());
		Close_TCP_Socket(&serverlist->slsocket);
		return GE_NOSOCKET;
	}

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
	char *clientName = "OPT-OUT"; /* FS: Allow people to opt-out of me getting their username in list requests */
	int len;
	int error = 0;
	int retry = 0;
	int sleepMs = 50;

retryRecv:
	len = recv(serverlist->slsocket, data, sizeof(data) - 1, 0);

	if (len == SOCKET_ERROR)
	{
		error = Get_Last_Error();

		if(error == TCP_BLOCKING_ERROR && (retry < totalRetry))
		{
			retry++;
			gspyi.dprint(DEVELOPER_MSG_GAMESPY, "Retrying Gamespy TCP Validate Handshake, Attempt %i of %i.\n", retry, totalRetry);
			msleep(sleepMs);
			sleepMs = sleepMs + 10;
			gspyi.run_keyevents (); /* FS: Check for aborts */
			goto retryRecv;
		}
		else
		{
			Close_TCP_Socket(&serverlist->slsocket);
			return GE_NOCONNECT;
		}
	}
	data[len] = '\0'; //null terminate it
	gspyi.dprint(DEVELOPER_MSG_GAMESPY, "Gamespy validate server key: %s\n", data);

	ptr = strstr ( data, SECURE ) + strlen(SECURE);
	gs_encrypt   ( (uchar *) serverlist->seckey, 6, (uchar *)ptr, 6 );
	gs_encode ( (uchar *)ptr, 6, (uchar *)result );

	 /* FS: Allow people to opt-out of me getting their username in list requests */
	if(!cl_master_server_optout->intValue)
	{
		clientName = playerName->string;
	}

	//validate to the master
	sprintf(data, "\\gamename\\%s\\gamever\\%s\\location\\0\\clientname\\%s\\validate\\%s\\final\\\\queryid\\1.1\\",
			serverlist->enginename, ENGINE_VERSION, clientName, result); //validate us		
	gspyi.dprint(DEVELOPER_MSG_GAMESPY, "Gamespy validate to the master: %s\n", data);

	len = send ( serverlist->slsocket, data, strlen(data), 0 );
	if (len == SOCKET_ERROR || len == 0)
	{
		Close_TCP_Socket(&serverlist->slsocket);
		return GE_NOCONNECT;
	}

	//send the list request
	sprintf(data, "\\list\\cmp\\gamename\\%s\\final\\", serverlist->gamename);
	len = send ( serverlist->slsocket, data, strlen(data), 0 );
	if (len == SOCKET_ERROR || len == 0)
	{
		Close_TCP_Socket(&serverlist->slsocket);
		return GE_NOCONNECT;
	}

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

	gspyi.dprint(DEVELOPER_MSG_GAMESPY, "Gamespy ServerListUpdate: Created Update list\n");
	if (error)
		return error;
	error = CreateServerListSocket(serverlist);
	gspyi.dprint(DEVELOPER_MSG_GAMESPY, "Gamespy ServerListUpdate: Created ServerListSocket list\n");
	if (error)
		return error;
	error = SendListRequest(serverlist);
	gspyi.dprint(DEVELOPER_MSG_GAMESPY, "Gamespy ServerListUpdate: Send List Request\n");
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

	if(searchdelta <= 0)
		gspyi.error("ServerListLANUpdate: searchdelta <= 0.");

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
static void ServerListAddServer(GServerList serverlist, unsigned int ip, unsigned short port)
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
	char indata[GS_MSGLEN];
	struct   sockaddr_in saddr;
	socklen_t saddrlen = sizeof(saddr);
	int error;

	while (1) //we break if the select fails
	{
		FD_ZERO(&set);
		FD_SET( serverlist->slsocket, &set);
		error = selectsocket(FD_SETSIZE, &set, NULL, NULL, &timeout);
		if (SOCKET_ERROR == error || 0 == error) //no data
			break;
		error = recvfrom(serverlist->slsocket, indata, sizeof(indata) - 1, 0, (struct sockaddr *)&saddr, &saddrlen );
		if (SOCKET_ERROR == error)
			continue;
		//we got data, add the server to the list to update
		if (strstr(indata,"\\final\\") != NULL)
			ServerListAddServer(serverlist, saddr.sin_addr.s_addr, ntohs(saddr.sin_port));
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
	static int oldlen = 0;
	int len;
	char *p;
	unsigned int ip;
	unsigned short port;
	int retry = 0;
	int error = 0;
	int sleepMs = 50;

	if(serverlist->abortupdate)
	{
		goto abort;
	}

retryRecv:
	len = recv(serverlist->slsocket, data + oldlen, sizeof(data) - oldlen - 1, 0);

	if (len == SOCKET_ERROR || len == 0)
	{
		error = Get_Last_Error();

		if(error == TCP_BLOCKING_ERROR && (retry < totalRetry))
		{
			retry++;
			gspyi.dprint(DEVELOPER_MSG_GAMESPY, "Retrying Gamespy TCP List RECV, Attempt %i of %i.\n", retry, totalRetry);
			msleep(sleepMs);
			sleepMs = sleepMs + 10;
			gspyi.run_keyevents (); /* FS: Check for aborts */
			goto retryRecv;
		}
		else
		{
			gspyi.dprint(DEVELOPER_MSG_GAMESPY,"Error during TCP List RECV: %s\n", gspyi.net_strerror());
abort:
			Close_TCP_Socket(&serverlist->slsocket);
			serverlist->slsocket = INVALID_SOCKET;
			oldlen = 0; //clear data so it can be used again
			ServerListHalt(serverlist);
			ServerListModeChange(serverlist, sl_querying);
			return GE_NOCONNECT;
		}
	}

	// data is in the form of '<unsigned int>ip<unsigned short>port<unsigned int>ip<unsigned short>port\final\'
	oldlen += len;

	gspyi.dprint(DEVELOPER_MSG_GAMESPY, "List xfer data: %s\n", data);

	p = data;
	while (p - data <= oldlen - 6)
	{
		if (strncmp(p,"\\final\\",7) == 0 || serverlist->abortupdate)
		{
			closesocket(serverlist->slsocket);
			serverlist->slsocket = INVALID_SOCKET;
			oldlen = 0; //clear data so it can be used again
			ServerListModeChange(serverlist, sl_querying);
			return 0; //get out!!
		}
		if (oldlen < 6) //no way it could be a full IP, quit
			break;

		memcpy(&ip,p,4);
		p += 4;
		memcpy(&port,p,2);
		p += 2;

		ServerListAddServer(serverlist, ip, ntohs(port));
	}
	oldlen = oldlen - (p - data);
	memmove(data,p,oldlen); //shift it over
	return 0;
}

//loop through pending queries and send out new ones
#define STATUS "\xff\xff\xff\xffstatus"

/* FS: Redone.  Now works properly at the same speed in DOS and WIN32 */
static GError ServerListQueryLoop(GServerList serverlist)
{
	int i, error;
	fd_set read_fd;
	struct timeval timeout = {0,0};
	char indata[GS_MSGLEN];
	struct sockaddr_in saddr;
	socklen_t saddrlen = sizeof(saddr);
	int server_timeout = bound(100, cl_master_server_timeout->intValue, 9000); /* FS: Now a CVAR */
	int scount = 0;
	static qboolean firsttime = true;
	GServer server;

	FD_ZERO(&read_fd);

	if(!firsttime)
	{
		for (i = 0 ; i < serverlist->maxupdates; i++)
		{
			FD_SET(serverlist->updatelist[i].s, &read_fd);
			selectsocket(serverlist->updatelist[i].s + 1, NULL, &read_fd, NULL, &timeout);

			if (FD_ISSET(serverlist->updatelist[i].s, &read_fd))
			{
				if (serverlist->updatelist[i].serverindex >= 0) //there is a server waiting
				{
					scount++;
				}
				else
				{
					continue;
				}

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
										(void *)(intptr_t)((serverlist->nextupdate * 100) / ArrayLength(serverlist->servers))); //percent done
					serverlist->updatelist[i].serverindex = -1; //reuse the updatelist
				}
				else
				{
					error = Get_Last_Error();
					if ((error == TCP_BLOCKING_ERROR) && (current_time() - serverlist->updatelist[i].starttime < server_timeout) )
					{
//						gspyi.dprint(DEVELOPER_MSG_GAMESPY, "Slow down cowboy..\n");
						continue;
					}
					else /* FS: If we didn't get WOULDBLOCK or if it just kept going past the server_timeout threshold then just give up. */
					{
						gspyi.dprint(DEVELOPER_MSG_GAMESPY, "Error during gamespy recv %s.  Removing server from list.\n", gspyi.net_strerror());
						serverlist->updatelist[i].serverindex = -1; //reuse the updatelist
					}
				}
			}
		}
	}

	if ( (serverlist->abortupdate) || (serverlist->nextupdate >= ArrayLength(serverlist->servers) && scount == 0) ) 
	{ //we are done!!
		if(!serverlist->abortupdate) /* FS: Don't print that the scan is complete if this was a forced abort */
		{
			gspyi.print("\x02Server scan complete!\n");
			gspyi.play_sound ("gamespy/complete.wav");
		}
		gspyi.update_numservers(ArrayLength(serverlist->servers));
		firsttime = true;
		FreeUpdateList(serverlist);
		ServerListModeChange(serverlist, sl_idle);
		return 0;
	}

	for (i = 0 ; i < serverlist->maxupdates && serverlist->nextupdate < ArrayLength(serverlist->servers) ; i++)
	{
		if (serverlist->updatelist[i].serverindex < 0) //it's available
		{
			serverlist->updatelist[i].serverindex = serverlist->nextupdate++;

			server = *(GServer *)ArrayNth(serverlist->servers,serverlist->updatelist[i].serverindex);
			saddr.sin_family = AF_INET;
			saddr.sin_addr.s_addr = inet_addr(ServerGetAddress(server));

			gspyi.dprint(DEVELOPER_MSG_GAMESPY, "Attempting to ping[%i]: %s:%i\n", i, ServerGetAddress(server), ServerGetQueryPort(server));

			saddr.sin_port = htons((short)ServerGetQueryPort(server));

			error = sendto(serverlist->updatelist[i].s,STATUS,strlen(STATUS), 0, (struct sockaddr *) &saddr, saddrlen);

			if (SOCKET_ERROR != error)
			{
				serverlist->updatelist[i].starttime = current_time();
			}
			else
			{
				gspyi.dprint(DEVELOPER_MSG_GAMESPY, "Error pinging server[%i] %s: %s\n", i, ServerGetAddress(server), gspyi.net_strerror());
				serverlist->updatelist[i].serverindex = -1; // reuse the update index
			}
		}
	}

	firsttime = false;
	return 0;
}

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
				gspyi.dprint(DEVELOPER_MSG_GAMESPY, "Gamespy ServerListThink: Server List xfer\n");
				 //read the data
				return ServerListReadList(serverlist);
		case sl_lanlist:
				gspyi.dprint(DEVELOPER_MSG_GAMESPY, "Gamespy ServerListThink: Server Lan List Query\n");
				return ServerListLANList(serverlist);
		case sl_querying: 
				//do some queries
				gspyi.dprint(DEVELOPER_MSG_GAMESPY, "Gamespy ServerListThink: Server List Query Loop\n");
				return ServerListQueryLoop(serverlist);
	}

	return 0;
}

/* ServerListHalt
-----------------
Halts the current update batch */
GError ServerListHalt(GServerList serverlist)
{
	if (serverlist->state != sl_idle)
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
	ArrayCompareFn comparator = 0; /* FS: Compiler warning */

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

#endif /* GAMESPPY */
