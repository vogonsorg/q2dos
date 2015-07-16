#ifndef GAME_HARD_LINKED

#include <assert.h>
#include <ctype.h>
#include <dlfcn.h>
#include <errno.h>
#include <unistd.h>
#include <sys/dxe.h>

#ifdef GAMESPY
#include <io.h>
#include <signal.h>
#include <tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "../client/gspy.h"
#endif

#include "../qcommon/qcommon.h"

/* FS: The following is gross, but I just figured this out. */
DXE_EXPORT_TABLE (syms)
	DXE_EXPORT (__dj_assert)
	DXE_EXPORT (__dj_ctype_tolower)
	DXE_EXPORT (__dj_ctype_toupper)
	DXE_EXPORT (__dj_huge_val)
	DXE_EXPORT (__dj_stderr)
	DXE_EXPORT (_doprnt)
	DXE_EXPORT (acos)
	DXE_EXPORT (asin)
	DXE_EXPORT (atan)
	DXE_EXPORT (atan2)
	DXE_EXPORT (atof)
	DXE_EXPORT (atoi)
	DXE_EXPORT (ceil)
	DXE_EXPORT (cos)
	DXE_EXPORT (crand)
	DXE_EXPORT (errno)
	DXE_EXPORT (exit)
	DXE_EXPORT (fclose)
	DXE_EXPORT (feof)
	DXE_EXPORT (fgetc)
	DXE_EXPORT (fgets)
	DXE_EXPORT (floor)
	DXE_EXPORT (fopen)
	DXE_EXPORT (fprintf)
	DXE_EXPORT (fputc)
	DXE_EXPORT (fputs)
	DXE_EXPORT (fread)
	DXE_EXPORT (free)
	DXE_EXPORT (fscanf)
	DXE_EXPORT (fseek)
	DXE_EXPORT (fwrite)
	DXE_EXPORT (getc)
	DXE_EXPORT (localtime)
	DXE_EXPORT (malloc)
	DXE_EXPORT (memcmp)
	DXE_EXPORT (memcpy)
	DXE_EXPORT (memset)
	DXE_EXPORT (printf)
	DXE_EXPORT (putc)
	DXE_EXPORT (puts)
	DXE_EXPORT (qsort)
	DXE_EXPORT (rand)
	DXE_EXPORT (sin)
	DXE_EXPORT (sprintf)
	DXE_EXPORT (sqrt)
	DXE_EXPORT (srand)
	DXE_EXPORT (sscanf)
	DXE_EXPORT (stpcpy)
	DXE_EXPORT (strcasecmp)
	DXE_EXPORT (strcat)
	DXE_EXPORT (strchr)
	DXE_EXPORT (strcmp)
	DXE_EXPORT (strcpy)
	DXE_EXPORT (strdup)
	DXE_EXPORT (strftime)
	DXE_EXPORT (stricmp)
	DXE_EXPORT (strlen)
	DXE_EXPORT (strncmp)
	DXE_EXPORT (strncpy)
	DXE_EXPORT (strnicmp)
	DXE_EXPORT (strrchr)
	DXE_EXPORT (strstr)
	DXE_EXPORT (strtod)
	DXE_EXPORT (strtok)
	DXE_EXPORT (strtol)
	DXE_EXPORT (tan)
	DXE_EXPORT (time)
	DXE_EXPORT (tolower)
	DXE_EXPORT (vsprintf)
#ifdef GAMESPY
	DXE_EXPORT(usleep)
	DXE_EXPORT(vsnprintf)
	DXE_EXPORT(bsearch)
	DXE_EXPORT(Q_strncpyz)
	DXE_EXPORT(calloc)
	DXE_EXPORT(realloc)
	DXE_EXPORT(Com_sprintf)
	DXE_EXPORT(gettimeofday)
	DXE_EXPORT(memmove)
	DXE_EXPORT(sendto)
	DXE_EXPORT(socket)
	DXE_EXPORT(inet_ntoa)
	DXE_EXPORT(recvfrom)
	DXE_EXPORT(gethostbyname)
	DXE_EXPORT(connect)
	DXE_EXPORT(setsockopt)
	DXE_EXPORT(send)
	DXE_EXPORT(recv)
	DXE_EXPORT(ioctlsocket)
	DXE_EXPORT(closesocket)
	DXE_EXPORT(select_s)
	DXE_EXPORT(inet_addr)
#endif
DXE_EXPORT_END

static void (*game_library)(void);
static int	firsttime = 1; /* FS: Moved this to the outside so gamespy will load if we do q2.exe +disconnect */

void Sys_UnloadGame (void)
{
	if (game_library)
		dlclose (game_library);
	game_library = NULL;
}

static int lastresort ()
{
	printf ("last resort function called!\n");
	return 0;
}

static void *dxe_res (const char *symname)
{
	union
	{
		int (*from)(void);
		void *to;
	} func_ptr_cast;

	printf ("%s: undefined symbol in dynamic module.  Please report this as a bug!\n", symname);
	Com_Printf ("%s: undefined symbol in dynamic module.  Please report this as a bug!\n", symname);

	func_ptr_cast.from = lastresort;
	return func_ptr_cast.to;
}

void *Sys_GetGameAPI (void *parms)
{
	void	*(*GetGameAPI) (void *);
	char	name[MAX_OSPATH];
	char	curpath[MAX_OSPATH];
	char	*path;
	const char *gamename = "gamex86.dxe";

	getcwd(curpath, sizeof(curpath));

	Com_Printf("------- Loading %s -------\n", gamename);

	if (firsttime)
	{
		firsttime = 0;
		/* Set the error callback function */
		_dlsymresolver = dxe_res;
		/* Register the symbols exported into dynamic modules */
		dlregsym (syms);
	}

	/* now run through the search paths */
	for (path = NULL; ; )
	{
		path = FS_NextPath (path);
		if (!path) return NULL; /* couldn't find one anywhere */

		Com_sprintf (name, sizeof(name), "%s/%s/%s", curpath, path, gamename);
		game_library = dlopen (name, RTLD_LAZY);
		if (game_library)
		{
			Com_Printf ("LoadLibrary (%s)\n",name);
			break;
		}
	}

	GetGameAPI = (void *) dlsym (game_library, "_GetGameAPI");
	if (!GetGameAPI)
	{
		Sys_UnloadGame ();
		return NULL;
	}

	return GetGameAPI (parms);
}

#ifdef GAMESPY
#ifndef GAMESPY_HARD_LINKED
static void (*gamespy_library)(void);
gspyexport_t	gspye;

extern void S_GamespySound (char *sound);
extern char* NET_ErrorString(void);
extern void CL_Gamespy_Update_Num_Servers(int numServers);
#endif

qboolean Sys_LoadGameSpy(char *name)
{
#ifdef GAMESPY
	char curpath[MAX_OSPATH];
	char gspyLoad[MAX_OSPATH];

	gspyimport_t gspyi;
	GetGameSpyAPI_t GetGameSpyAPI;

	getcwd(curpath, sizeof(curpath));

	Com_Printf("------- Loading %s -------\n", name);

	if (firsttime)
	{
		firsttime = 0;
		/* Set the error callback function */
		_dlsymresolver = dxe_res;
		/* Register the symbols exported into dynamic modules */
		dlregsym (syms);
	}

	Com_sprintf(gspyLoad, sizeof(gspyLoad), "%s/%s", curpath, name);
	gamespy_library = dlopen(gspyLoad, RTLD_LAZY);

	if (!gamespy_library)
	{
		Com_Printf( "LoadLibrary(\"%s\") failed\n", gspyLoad );

		return false;
	}

	GetGameSpyAPI = (void *) dlsym (gamespy_library, "_GetGameSpyAPI");

	if (!GetGameSpyAPI)
	{
		Com_Error( ERR_FATAL, "GetProcAddress failed on %s", gspyLoad );
	}

	gspyi.Cvar_Get = Cvar_Get;
	gspyi.Cvar_Set = Cvar_Set;
	gspyi.Cvar_SetValue = Cvar_SetValue;
	gspyi.Con_Printf = Com_Printf;
	gspyi.Con_DPrintf = Com_DPrintf;
	gspyi.NET_ErrorString = NET_ErrorString;
	gspyi.Sys_SendKeyEvents = Sys_SendKeyEvents;
	gspyi.S_GamespySound = S_GamespySound;
	gspyi.CL_Gamespy_Update_Num_Servers = CL_Gamespy_Update_Num_Servers;

	gspye = GetGameSpyAPI(gspyi);

	if(gspye.api_version != GAMESPY_API_VERSION)
	{
		gspye.Shutdown();
		Com_Printf("Error: GameSpy DXE reported version %i.  Supported version by Q2DOS %i\n", gspye.api_version, GAMESPY_API_VERSION);
		return false;
	}

	gspye.Init(); /* FS: Grab the cl_master_server_* CVARs */

	return true;
#else
	return false;
#endif /* GAMESPY_HARD_LINKED */
#endif /* GAMESPY */
}

#endif /* GAME_HARD_LINKED */
