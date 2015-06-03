// in_null.c -- for systems without a mouse

#include "dosisms.h"
#include "../client/client.h"

cvar_t	*in_joystick;
qboolean	mouse_avail;
int		mouse_buttons;
int		mouse_oldbuttonstate;
int		mouse_buttonstate;
float	mouse_x, mouse_y;
float	old_mouse_x, old_mouse_y;
extern	unsigned	sys_msg_time;
// mouse variables
cvar_t	*m_filter;
qboolean	mlooking;

void IN_MLookDown (void) { mlooking = true; }
void IN_MLookUp (void) {
mlooking = false;
if (!freelook->value && lookspring->value)
		IN_CenterView ();
}

void IN_StartupMouse (void)
{
	if ( COM_CheckParm ("-nomouse") ) 
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
}

void IN_Init (void)
{
	m_filter				= Cvar_Get ("m_filter",					"0",		0);
	Cmd_AddCommand ("+mlook", IN_MLookDown);
	Cmd_AddCommand ("-mlook", IN_MLookUp);
	in_joystick = Cvar_Get ("in_joystick", "0", CVAR_ARCHIVE);
	IN_StartupMouse ();
}

void IN_Shutdown (void)
{
}

void IN_Commands (void)
{
	int		i;

	if (mouse_avail)
	{
		regs.x.ax = 3;		// read buttons
		dos_int86(0x33);
		mouse_buttonstate = regs.x.bx;
	
	// perform button actions
		for (i=0 ; i<mouse_buttons ; i++)
		{
			if ( (mouse_buttonstate & (1<<i)) &&
			!(mouse_oldbuttonstate & (1<<i)) )
			{
				Key_Event (K_MOUSE1 + i, true, sys_msg_time);
			}
			if ( !(mouse_buttonstate & (1<<i)) &&
			(mouse_oldbuttonstate & (1<<i)) )
			{
				Key_Event (K_MOUSE1 + i, false, sys_msg_time);
			}
		}	
		
		mouse_oldbuttonstate = mouse_buttonstate;
	}
}

void IN_Frame (void)
{
}

void IN_Move (usercmd_t *cmd)
{
	int		mx, my;

	if (!mouse_avail)
		return;

	regs.x.ax = 11;		// read move
	dos_int86(0x33);
	mx = (short)regs.x.cx;
	my = (short)regs.x.dx;

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
	if ( (in_strafe.state & 1) || (lookstrafe->value && mlooking ))
		cmd->sidemove += m_side->value * mouse_x;
	else
		cl.viewangles[YAW] -= m_yaw->value * mouse_x;

	if ( (mlooking || freelook->value) && !(in_strafe.state & 1))
	{
		cl.viewangles[PITCH] += m_pitch->value * mouse_y;
	}
	else
	{
		cmd->forwardmove -= m_forward->value * mouse_y;
	}

}

void IN_Activate (qboolean active)
{
}

void IN_ActivateMouse (void)
{
}

void IN_DeactivateMouse (void)
{
}

void IN_MouseEvent (int mstate)
{
	int		i;

	if (!mouse_avail)
		return;

// perform button actions
	for (i=0 ; i<mouse_buttons ; i++)
	{
		if ( (mstate & (1<<i)) &&
			!(mouse_oldbuttonstate & (1<<i)) )
		{
			Key_Event (K_MOUSE1 + i, true, sys_msg_time);
		}

		if ( !(mstate & (1<<i)) &&
			(mouse_oldbuttonstate & (1<<i)) )
		{
				Key_Event (K_MOUSE1 + i, false, sys_msg_time);
		}
	}	
		
	mouse_oldbuttonstate = mstate;
}
