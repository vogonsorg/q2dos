// in_dos.c -- dos mouse code.  adapated from Quake 1 and uHexen2.

#include <dos.h>
#include "dosisms.h"
#include "../client/client.h"

extern	double		sys_msg_time;
extern	double		sys_frame_time;

static	qboolean	mouse_avail;
static	qboolean	mouseactive;
static	qboolean	mouse_wheel;
static	int		mouse_buttons;
static	int		mouse_oldbuttonstate;
static	int		mouse_buttonstate;
static	int		mouse_wheelcounter;
static	float	mouse_x, mouse_y;
static	float	old_mouse_x, old_mouse_y;
static	qboolean	mlooking;

cvar_t *m_filter;
cvar_t *in_joystick;


static void IN_MLookDown (void) {
	mlooking = true;
}

static void IN_MLookUp (void) {
	mlooking = false;
	if (!freelook->value && lookspring->value)
		IN_CenterView ();
}

static void IN_StartupMouse (void)
{
	if (COM_CheckParm ("-nomouse"))
		return;

// check for mouse
	regs.x.ax = 0;
	dos_int86(0x33);
	mouse_avail = regs.x.ax;
	if (!mouse_avail)
	{
		Com_Printf ("No mouse found\n");
		return;
	}

	mouse_buttons = regs.x.bx;
	if (mouse_buttons > 3)
		mouse_buttons = 3;
	Com_Printf("%d-button mouse available\n", mouse_buttons);
	mouseactive = true;
	if (COM_CheckParm ("-nowheel"))
		return;
	regs.x.ax = 0x11;
	dos_int86(0x33);
	if (regs.x.ax == 0x574D && regs.h.cl == 1)
	{
		mouse_wheel = true;
		Com_Printf("mouse wheel support available\n");
	}
}

void IN_Init (void)
{
	m_filter = Cvar_Get ("m_filter", "0", 0);
	in_joystick = Cvar_Get ("in_joystick", "0", CVAR_ARCHIVE);
	Cmd_AddCommand ("+mlook", IN_MLookDown);
	Cmd_AddCommand ("-mlook", IN_MLookUp);

	IN_StartupMouse ();
}

void IN_Shutdown (void)
{
}

void IN_Commands (void)
{
	int		i;

	sys_msg_time = Sys_Milliseconds();

	if (mouse_avail)
	{
		regs.x.ax = 3;		// read buttons
		dos_int86(0x33);
		mouse_buttonstate = regs.x.bx;	// regs.h.bl
		mouse_wheelcounter = (signed char) regs.h.bh;
	// perform button actions
		for (i = 0; i < mouse_buttons; i++)
		{
			if ( (mouse_buttonstate & (1<<i)) && !(mouse_oldbuttonstate & (1<<i)) )
			{
				Key_Event (K_MOUSE1 + i, true, sys_msg_time);
			}
			if ( !(mouse_buttonstate & (1<<i)) && (mouse_oldbuttonstate & (1<<i)) )
			{
				Key_Event (K_MOUSE1 + i, false, sys_msg_time);
			}
		}
		if (mouse_wheel)
		{
			if (mouse_wheelcounter < 0)
			{
				Key_Event (K_MWHEELUP, true, sys_msg_time);
				Key_Event (K_MWHEELUP, false, sys_msg_time);
			}
			else if (mouse_wheelcounter > 0)
			{
				Key_Event (K_MWHEELDOWN, true, sys_msg_time);
				Key_Event (K_MWHEELDOWN, false, sys_msg_time);
			}
		}

		mouse_oldbuttonstate = mouse_buttonstate;
	}
	sys_frame_time = Sys_Milliseconds();
}

static void IN_ReadMouseMove (int *x, int *y)
{
	regs.x.ax = 0x0B;	/* read move */
	dos_int86(0x33);
	if (x)	*x = (short) regs.x.cx;
	if (y)	*y = (short) regs.x.dx;
}

void IN_Move (usercmd_t *cmd)
{
	int		mx, my;

	if (!mouse_avail || !mouseactive)
		return;

	IN_ReadMouseMove (&mx, &my);

	if (m_filter->value)
	{
		mouse_x = (mx + old_mouse_x) * 0.5;
		mouse_y = (my + old_mouse_y) * 0.5;
	}
	else
	{
		mouse_x = mx;
		mouse_y = my;
	}
	old_mouse_x = mx;
	old_mouse_y = my;

	mouse_x *= sensitivity->value;
	mouse_y *= sensitivity->value;

// add mouse X/Y movement to cmd
	if ( (in_strafe.state & 1) || (lookstrafe->value && mlooking))
		cmd->sidemove += m_side->value * mouse_x;
	else
		cl.viewangles[YAW] -= m_yaw->value * mouse_x;

	if ((mlooking || freelook->value) && !(in_strafe.state & 1))
	{
		cl.viewangles[PITCH] += m_pitch->value * mouse_y;
	}
	else
	{
		cmd->forwardmove -= m_forward->value * mouse_y;
	}
}

void IN_ActivateMouse (void)
{
	if (mouse_avail && !mouseactive)
	{
		old_mouse_x = old_mouse_y = 0;
		IN_ReadMouseMove (NULL, NULL);
		mouseactive = true;
	}
}

void IN_DeactivateMouse (void)
{
	mouseactive = false;
}

void IN_Frame (void)
{
	if (!cl.refresh_prepped ||
	    cls.key_dest == key_console || cls.key_dest == key_menu)
	{
		IN_DeactivateMouse ();
		return;
	}

	IN_ActivateMouse ();
}
