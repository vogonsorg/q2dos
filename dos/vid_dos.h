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
// vid_dos.h: header file for DOS-specific video stuff

/* FS: Moved this here so we don't have to call extern to places with copy/paste */
#define MAX_RESOLUTIONS 20
#define VIDNAME_LEN 30

typedef struct vid_resolutions_s {
	int mode;
	int vesa_mode;
	int height;
	int width;
	void *address;
	byte *planarAddress;
	qboolean isLFB;
	qboolean isBanked;
	qboolean isPlanar;
	char menuname[VIDNAME_LEN];
} vid_resolutions_t;

extern int num_vid_resolutions;
extern vid_resolutions_t vid_resolutions[MAX_RESOLUTIONS];

extern int		VGA_width, VGA_height, VGA_rowbytes, VGA_bufferrowbytes;
extern byte		*VGA_pagebase;

void VID_InitExtra (void);
void VGA_UpdateLinearScreen (void *srcptr, void *destptr, int width,
	int height, int srcrowbytes, int destrowbytes);

