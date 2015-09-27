/*
Copyright (C) 1996-1997 Id Software, Inc.

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
// glimp_dos.h: header file for DOS-specific OpenGL video stuff

#ifndef REF_HARD_LINKED
#define GL_DLSYM
#endif

extern void *opengl_dxe;

extern int  (*DOSGL_InitCtx ) (int *width, int *height, int *bpp);
extern void (*DOSGL_Shutdown) (void);
extern void (*DOSGL_EndFrame) (void);
extern void * (*DOSGL_GetProcAddress) (const char *);
extern const char * (*DOSGL_APIName) (void);

int DMESA_LoadAPI (void *handle);
int SAGE_LoadAPI (void *handle);
int FXMESA_LoadAPI (void *handle);
