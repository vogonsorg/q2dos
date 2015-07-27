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
// vid_dos.c -- DOS video driver adapated from Q1

#ifndef REF_HARD_LINKED
#include <dlfcn.h>
#endif
#include "../client/client.h"
#include "../client/qmenu.h"
#include "vid_dos.h"

#define NUM_VID_DRIVERS 1
#define REF_SOFT	0

cvar_t *vid_ref;
static cvar_t *vid_fullscreen;
static cvar_t *vid_gamma;
extern cvar_t *scr_viewsize;	// client/cl_scrn.c

/* cvars for vga/vesa code */
static cvar_t *vid_bankedvga;
static cvar_t *vid_vgaonly;

static cvar_t *sw_mode;
static cvar_t *sw_stipplealpha;
static cvar_t *sw_waterwarp; /* FS */
static cvar_t *r_contentblend; /* FS */

extern void M_ForceMenuOff (void);
static void VID_ListModes_f (void);
static void VID_Restart_f (void);

#define SOFTWARE_MENU 0

static menuframework_s  s_software_menu;
static menuframework_s *s_current_menu;
static int		s_current_menu_index;

static menulist_s		s_mode_list[NUM_VID_DRIVERS];
static menuslider_s		s_screensize_slider[NUM_VID_DRIVERS];
static menuslider_s		s_brightness_slider[NUM_VID_DRIVERS];
static menulist_s		s_stipple_box;
static menulist_s		s_contentblend_box;	/* FS */
static menulist_s		s_waterwarp_box;	/* FS */
static menuaction_s		s_cancel_action[NUM_VID_DRIVERS];
static menuaction_s		s_defaults_action[NUM_VID_DRIVERS];

static const vid_resolutions_t	*vid_modes;
static int			vid_nummodes;

static const char *		resolution_names[MAX_RESOLUTIONS + 1];
				// namelist initialized by VID_Init()

viddef_t	viddef;				// global video state

refexport_t	re;
#ifdef REF_HARD_LINKED
refexport_t GetRefAPI (refimport_t rimp);
#else
static void *reflib_library; /* Handle to refresh DLL. */
#endif
static qboolean reflib_active = false;


/*
==========================================================================

DIRECT LINK GLUE

==========================================================================
*/

#define	MAXPRINTMSG	4096
static void VID_Printf (int print_level, char *fmt, ...)
{
	va_list		argptr;
	char		msg[MAXPRINTMSG];

	va_start (argptr,fmt);
	vsnprintf (msg,sizeof(msg),fmt,argptr);
	va_end (argptr);

	if (print_level == PRINT_ALL)
		Com_Printf ("%s", msg);
	else
		Com_DPrintf(DEVELOPER_MSG_GFX, "%s", msg);
}

static void VID_Error (int err_level, char *fmt, ...)
{
	va_list		argptr;
	char		msg[MAXPRINTMSG];

	va_start (argptr,fmt);
	vsnprintf (msg,sizeof(msg),fmt,argptr);
	va_end (argptr);

	Com_Error (err_level, "%s", msg);
}

static void VID_NewWindow (int width, int height)
{
	viddef.width = width;
	viddef.height = height;

	cl.force_refdef = true;		// can't use a paused refdef
}

/*
** VID_GetModeInfo
*/
static qboolean VID_GetModeInfo(int *width, int *height, int mode)
{
	if (mode < 0 || mode >= vid_nummodes)
		return false;

	*width  = vid_modes[mode].width;
	*height = vid_modes[mode].height;

//	Com_Printf("VID_GetModeInfo %dx%d mode %d\n",*width,*height,mode);
	return true;
}

static void ScreenSizeCallback(void *s)
{
	menuslider_s *slider = (menuslider_s *) s;
	Cvar_SetValue("viewsize", slider->curvalue * 10);
}

static void BrightnessCallback(void *s)
{
	menuslider_s *slider = (menuslider_s *) s;

	if (stricmp(vid_ref->string,"soft") == 0)
	{
		float gamma = (0.8 - (slider->curvalue/10.0 - 0.5)) + 0.5;
		Cvar_SetValue( "vid_gamma", gamma );
	}
}

static void ResetDefaults(void *unused)
{
	VID_MenuInit();
}

static void ApplyChanges(void *unused)
{
	float gamma;

	/*
	** make values consistent
	*/
	s_brightness_slider[!s_current_menu_index].curvalue = s_brightness_slider[s_current_menu_index].curvalue;

	/*
	** invert sense so greater = brighter, and scale to a range of 0.5 to 1.3
	*/
	gamma = (0.8 - (s_brightness_slider[s_current_menu_index].curvalue/10.0 - 0.5)) + 0.5;

	Cvar_SetValue("vid_gamma", gamma);
	Cvar_SetValue("sw_stipplealpha", s_stipple_box.curvalue);
	Cvar_SetValue("sw_mode", s_mode_list[SOFTWARE_MENU].curvalue);
	Cvar_SetValue("r_contentblend", s_contentblend_box.curvalue); // FS
	Cvar_SetValue("sw_waterwarp", s_waterwarp_box.curvalue); // FS

	Cvar_Set("vid_ref", "soft");

	M_ForceMenuOff();
}

static void CancelChanges(void *unused)
{
	extern void M_PopMenu(void);

	M_PopMenu();
}

static void VID_FreeReflib (void)
{
#ifndef REF_HARD_LINKED
	if (reflib_library)
		dlclose(reflib_library);
	reflib_library = NULL;
#endif
	memset (&re, 0, sizeof(re));
	reflib_active  = false;
	vid_modes = NULL;
	vid_nummodes = 0;
}

static void VID_LoadRefresh (const char *name)
{
	refimport_t	ri;
#ifndef REF_HARD_LINKED
	GetRefAPI_t	GetRefAPI;
#endif

	if (reflib_active)
	{
		re.Shutdown();
		VID_FreeReflib ();
	}

#ifndef REF_HARD_LINKED
	if ((reflib_library = dlopen(name, RTLD_LAZY|RTLD_GLOBAL)) == NULL)
		Sys_Error("dlopen(\"%s\") failed\n",name);
	if ((GetRefAPI = (void *) dlsym(reflib_library, "_GetRefAPI")) == NULL)
		Sys_Error("dlsym() failed on %s",name);
#endif

	ri.Cmd_AddCommand = Cmd_AddCommand;
	ri.Cmd_RemoveCommand = Cmd_RemoveCommand;
	ri.Cmd_Argc = Cmd_Argc;
	ri.Cmd_Argv = Cmd_Argv;
	ri.Cmd_ExecuteText = Cbuf_ExecuteText;
	ri.Con_Printf = VID_Printf;
	ri.Sys_Error = VID_Error;
	ri.FS_LoadFile = FS_LoadFile;
	ri.FS_FreeFile = FS_FreeFile;
	ri.FS_Gamedir = FS_Gamedir;
	ri.Vid_NewWindow = VID_NewWindow;
	ri.Cvar_Get = Cvar_Get;
	ri.Cvar_Set = Cvar_Set;
	ri.Cvar_SetValue = Cvar_SetValue;
	ri.Vid_GetModeInfo = VID_GetModeInfo;

	re = GetRefAPI(ri);

	if (re.api_version != API_VERSION)
	{
		VID_FreeReflib ();
		Com_Error (ERR_FATAL, "Re has incompatible api_version");
	}

	/* HACK HACK HACK: retrieving the video modes list into here
	 * using the hInstance and wndProc parameters of SWimp_Init().
	 * See: swimp_dos.c:SWimp_Init()
	 */
	if (re.Init (&vid_nummodes, &vid_modes) == -1)
		Com_Error (ERR_FATAL, "Couldn't start refresh");

	reflib_active = true;
	vidref_val = VIDREF_SOFT;

	vid_ref = Cvar_Get ("vid_ref", "soft", CVAR_ARCHIVE);
	vid_fullscreen = Cvar_Get ("vid_fullscreen", "1", CVAR_ARCHIVE);
	vid_gamma = Cvar_Get("vid_gamma", "1", CVAR_ARCHIVE);
}

void	VID_Init (void)
{
	int		i;

	viddef.width = 320;
	viddef.height = 240;

	vid_ref = Cvar_Get ("vid_ref", "soft", CVAR_ARCHIVE);
	vid_ref->description = "Video renderer to use.  Locked to software in Q2DOS.";
	vid_fullscreen = Cvar_Get ("vid_fullscreen", "1", CVAR_ARCHIVE);
	vid_fullscreen->description = "Enable fullscreen video.  Locked to fullscreen in Q2DOS.";
	vid_gamma = Cvar_Get("vid_gamma", "1", CVAR_ARCHIVE);

	vid_vgaonly = Cvar_Get("vid_vgaonly", (COM_CheckParm("-vgaonly"))? "1" : "0", 0);
	vid_bankedvga = Cvar_Get("vid_vgaonly", (COM_CheckParm("-bankedvga"))? "1" : "0", 0);

	VID_CheckChanges ();

	for (i = 0; i < MAX_RESOLUTIONS; ++i)
	{
		resolution_names[i] = (i < vid_nummodes)?
					vid_modes[i].menuname : NULL;
	}
	resolution_names[MAX_RESOLUTIONS] = NULL;

	Cmd_AddCommand("vid_restart", VID_Restart_f);
	Cmd_AddCommand("vid_listmodes", VID_ListModes_f); // FS: Added
}

void	VID_Shutdown (void)
{
	if (re.Shutdown)
		re.Shutdown ();
}

void	VID_CheckChanges (void)
{
	char name[100];

	if ( vid_ref->modified)
	{
		cl.force_refdef = true;		// can't use a paused refdef
		S_StopAllSounds();
	}
	while (vid_ref->modified)
	{
		/*
		** refresh has changed
		*/
		vid_ref->modified = false;
		vid_fullscreen->modified = true;
		cl.refresh_prepped = false;
		cls.disable_screen = true;

		Com_sprintf(name, sizeof(name), "ref_%s.dxe", vid_ref->string);
		VID_LoadRefresh(name);

		Cvar_Set("vid_ref", "soft");
		cls.disable_screen = false;
	}
}

void	VID_MenuInit (void)
{
	static const char *yesno_names[] = {
		"no",
		"yes",
		NULL
	};

	int i;

	if (!sw_mode)
		sw_mode = Cvar_Get("sw_mode", "0", 0);

	if (!sw_stipplealpha)
		sw_stipplealpha = Cvar_Get("sw_stipplealpha", "0", CVAR_ARCHIVE);

	if (!r_contentblend) /* FS */
		r_contentblend = Cvar_Get( "r_contentblend", "1", CVAR_ARCHIVE);

	if (!sw_waterwarp) /* FS */
		sw_waterwarp = Cvar_Get( "sw_waterwarp", "1", CVAR_ARCHIVE);

	s_mode_list[SOFTWARE_MENU].curvalue = sw_mode->value;

	if (!scr_viewsize)
		scr_viewsize = Cvar_Get("viewsize", "100", CVAR_ARCHIVE);

	s_screensize_slider[SOFTWARE_MENU].curvalue = scr_viewsize->value/10;

	s_current_menu_index = SOFTWARE_MENU;

	s_software_menu.x = viddef.width * 0.50;
	s_software_menu.nitems = 0;

	for (i = 0; i < NUM_VID_DRIVERS; i++)
	{
		s_mode_list[i].generic.type = MTYPE_SPINCONTROL;
		s_mode_list[i].generic.name = "video resolution";
		s_mode_list[i].generic.x = 0;
		s_mode_list[i].generic.y = 10; /* FS: Y position for where the spin control ends up */
		s_mode_list[i].generic.callback = NULL;
		s_mode_list[i].itemnames = resolution_names;

		s_screensize_slider[i].generic.type	= MTYPE_SLIDER;
		s_screensize_slider[i].generic.x		= 0;
		s_screensize_slider[i].generic.y		= 20;
		s_screensize_slider[i].generic.name	= "screen size";
		s_screensize_slider[i].minvalue = 3;
		s_screensize_slider[i].maxvalue = 12;
		s_screensize_slider[i].generic.callback = ScreenSizeCallback;

		s_brightness_slider[i].generic.type	= MTYPE_SLIDER;
		s_brightness_slider[i].generic.x	= 0;
		s_brightness_slider[i].generic.y	= 30;
		s_brightness_slider[i].generic.name	= "brightness";
		s_brightness_slider[i].generic.callback = BrightnessCallback;
		s_brightness_slider[i].minvalue = 5;
		s_brightness_slider[i].maxvalue = 13;
		s_brightness_slider[i].curvalue = ( 1.3 - vid_gamma->value + 0.5 ) * 10;

		s_defaults_action[i].generic.type = MTYPE_ACTION;
		s_defaults_action[i].generic.name = "reset to defaults";
		s_defaults_action[i].generic.x    = 0;
		s_defaults_action[i].generic.y    = 100;
		s_defaults_action[i].generic.callback = ResetDefaults;

		s_cancel_action[i].generic.type = MTYPE_ACTION;
		s_cancel_action[i].generic.name = "cancel";
		s_cancel_action[i].generic.x    = 0;
		s_cancel_action[i].generic.y    = 110;
		s_cancel_action[i].generic.callback = CancelChanges;
	}

	s_stipple_box.generic.type = MTYPE_SPINCONTROL;
	s_stipple_box.generic.x	= 0;
	s_stipple_box.generic.y	= 60;
	s_stipple_box.generic.name	= "stipple alpha";
	s_stipple_box.curvalue = sw_stipplealpha->value;
	s_stipple_box.itemnames = yesno_names;

	/* FS */
	s_contentblend_box.generic.type = MTYPE_SPINCONTROL;
	s_contentblend_box.generic.x	= 0;
	s_contentblend_box.generic.y	= 70;
	s_contentblend_box.generic.name	= "content blending";
	s_contentblend_box.curvalue = r_contentblend->intValue;
	s_contentblend_box.itemnames = yesno_names;

	/* FS */
	s_waterwarp_box.generic.type = MTYPE_SPINCONTROL;
	s_waterwarp_box.generic.x	= 0;
	s_waterwarp_box.generic.y	= 80;
	s_waterwarp_box.generic.name	= "water warping";
	s_waterwarp_box.curvalue = sw_waterwarp->intValue;
	s_waterwarp_box.itemnames = yesno_names;

	/* FS: ATTN  AddItem order has to be the order you want to see it in or else the cursor gets wonky! */
	Menu_AddItem(&s_software_menu, (void *) &s_mode_list[SOFTWARE_MENU]);
	Menu_AddItem(&s_software_menu, (void *) &s_screensize_slider[SOFTWARE_MENU]);
	Menu_AddItem(&s_software_menu, (void *) &s_brightness_slider[SOFTWARE_MENU]);
	Menu_AddItem(&s_software_menu, (void *) &s_stipple_box);
	Menu_AddItem(&s_software_menu, (void *) &s_contentblend_box); // FS
	Menu_AddItem(&s_software_menu, (void *) &s_waterwarp_box); // FS

	Menu_AddItem(&s_software_menu, (void *) &s_defaults_action[SOFTWARE_MENU]);
	Menu_AddItem(&s_software_menu, (void *) &s_cancel_action[SOFTWARE_MENU]);

	Menu_Center(&s_software_menu);
	s_software_menu.x -= 8;
}

void	VID_MenuDraw (void)
{
	int w, h;

	s_current_menu = &s_software_menu;

	/*
	** draw the banner
	*/
	re.DrawGetPicSize( &w, &h, "m_banner_video" );
	re.DrawPic( viddef.width / 2 - w / 2, viddef.height /2 - 110, "m_banner_video" );

	/*
	** move cursor to a reasonable starting position
	*/
	Menu_AdjustCursor( s_current_menu, 1 );

	/*
	** draw the menu
	*/
	Menu_Draw( s_current_menu );
}

const char *VID_MenuKey (int k)
{
	menuframework_s *m = s_current_menu;
	static const char *sound = "misc/menu1.wav";

	switch (k)
	{
	case K_ESCAPE:
		ApplyChanges(0);
		return NULL;
	case K_KP_UPARROW:
	case K_UPARROW:
		m->cursor--;
		Menu_AdjustCursor(m, -1);
		break;
	case K_KP_DOWNARROW:
	case K_DOWNARROW:
		m->cursor++;
		Menu_AdjustCursor(m, 1);
		break;
	case K_KP_LEFTARROW:
	case K_LEFTARROW:
		Menu_SlideItem(m, -1);
		break;
	case K_KP_RIGHTARROW:
	case K_RIGHTARROW:
		Menu_SlideItem(m, 1);
		break;
	case K_KP_ENTER:
	case K_ENTER:
		if (!Menu_SelectItem(m))
			ApplyChanges(NULL);
		break;
	}

	return sound;//NULL
}

static void VID_Restart_f (void)
{
	vid_ref->modified = true;
	return;
}

static void VID_ListModes_f (void)
{
	int i = 0;

	for(i = 0; i < vid_nummodes; i++)
	{
		if(vid_modes[i].menuname[0] != 0)
		{
			Com_Printf("[Mode %02d] %s\n", i, vid_modes[i].menuname);
		}
	}
	Com_Printf("Available modes: %d\n", vid_nummodes);
}
