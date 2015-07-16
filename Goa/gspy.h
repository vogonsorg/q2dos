/*
Copyright (C) 1997-2001 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
#ifndef __GSPY_H
#define __GSPY_H

#include "../qcommon/qcommon.h"
#include "CEngine/goaceng.h"

/* FS: IF THIS FILE IS CHANGED IT MUST BE CHANGED IN CLIENT/GSPY.H! */

#define	GAMESPY_API_VERSION		1

/*
 * these are the functions exported by the gamespy module
 */
typedef struct
{
	/* if api_version is different, the dll cannot be used */
	int		api_version;

	/* called when the library is loaded */
	void	(*Init) (void);

	/* called before the library is unloaded */
	void	(*Shutdown) (void);

	GServerList	(*ServerListNew)(char *gamename,char *enginename, char *seckey, int maxconcupdates, void *CallBackFn, int CallBackFnType, void *instance);
	void (*ServerListFree)(GServerList serverlist);
	GError (*ServerListUpdate)(GServerList serverlist, gbool async);
	GError (*ServerListThink)(GServerList serverlist);
	GError (*ServerListHalt)(GServerList serverlist);
	GError (*ServerListClear)(GServerList serverlist);
	GServer (*ServerListGetServer)(GServerList serverlist, int index);
	GServerListState (*ServerListState)(GServerList serverlist);
	char *(*ServerGetAddress)(GServer server);
	char *(*ServerListErrorDesc)(GServerList serverlist, GError error);
	void (*ServerListSort)(GServerList serverlist, gbool ascending, char *sortkey, GCompareMode comparemode);
	int (*ServerGetPing)(GServer server);
	char *(*ServerGetStringValue)(GServer server, char *key, char *sdefault);
	int (*ServerGetIntValue)(GServer server, char *key, int idefault);
	int (*ServerGetQueryPort)(GServer server);

} gspyexport_t;

/*
 * these are the functions imported by the gamespy module
 */
typedef struct
{
	void	(*Sys_Error)(int err_level, char *str, ...);

	void	(*Con_Printf)(char *str, ...);
	void	(*Con_DPrintf)(unsigned long developerFlags, char *fmt, ...);
	cvar_t	*(*Cvar_Get)(char *name, char *value, int flags);
	cvar_t	*(*Cvar_Set)(char *name, char *value );
	void	 (*Cvar_SetValue)(char *name, float value );
	char	*(*NET_ErrorString)(void);
	void	(*Sys_SendKeyEvents)(void);
	void	(*S_GamespySound)(char *s);
	void	(*CL_Gamespy_Update_Num_Servers)(int numServers);
} gspyimport_t;


#endif /* __GSPY_H */
