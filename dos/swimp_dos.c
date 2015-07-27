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

#include <dpmi.h>
#include <pc.h>
#include <sys/nearptr.h>
#include "../ref_soft/r_local.h"
#include "vid_dos.h"

/* FS: For planar modes */
#include "vregset.h"
#include "vgamodes.h"
#include "dosisms.h"

static int currentvideomode = 0;

void VID_DrawBanked(void);
void VGA_UpdatePlanarScreen (void *srcbuffer);
void VGA_UpdateLinearScreen (void *srcptr, void *destptr, int width, int height, int srcrowbytes, int destrowbytes); /* FS: Unused, may be unnecessary */
extern vrect_t		scr_vrect; /* position of render window on screen */

#if	!id386
void MaskExceptions (void) {
}
void Sys_SetFPCW (void) {
}

void VGA_UpdatePlanarScreen (void *srcbuffer)
{
/* based on vga_copytoplanar256() of SVGAlib */
	byte *p;
	int ofs;
	int plane, x, y;

	plane = 0;
	/* Copy pixels that belong in plane. */
	_docopy:
	outportb(SC_INDEX, MAP_MASK);
	outportb(SC_DATA, 1 << plane);
	p = (byte *) srcbuffer;
	ofs = 0;
	for (y = 0; y < VGA_height; y++)
	{
		x = 0;
		while (x * 4 + 32 < VGA_width)
		{
			VGA_pagebase[ofs + x + 0] = p[x * 4 + plane + 0];
			VGA_pagebase[ofs + x + 1] = p[x * 4 + plane + 4];
			VGA_pagebase[ofs + x + 2] = p[x * 4 + plane + 8];
			VGA_pagebase[ofs + x + 3] = p[x * 4 + plane + 12];
			VGA_pagebase[ofs + x + 4] = p[x * 4 + plane + 16];
			VGA_pagebase[ofs + x + 5] = p[x * 4 + plane + 20];
			VGA_pagebase[ofs + x + 6] = p[x * 4 + plane + 24];
			VGA_pagebase[ofs + x + 7] = p[x * 4 + plane + 28];
			x += 8;
		}
		while (x * 4 < VGA_width)
		{
			VGA_pagebase[ofs + x] = p[x * 4 + plane];
			x++;
		}
		p += VGA_bufferrowbytes;	/* Next line. */
		ofs += VGA_rowbytes;
	}
	if (++plane < 4)
		goto _docopy;
}

void VGA_UpdateLinearScreen (	void *srcptr, void *destptr,
				int width, int height,
				int srcrowbytes, int destrowbytes)
{
/* quick'n'dirty C-only version */
	byte *s, *d;
	int y;

	s = (byte *) srcptr;
	d = (byte *) destptr;
	for (y = 0; y < height; y++)
	{
		memcpy (d, s, width);
		s += srcrowbytes;
		d += destrowbytes;
	}
}
#endif	/* !id386 */

void Sys_MakeCodeWriteable (unsigned long startaddr, unsigned long length)
{
/* MS-DOS is always writeable */
}

void	SWimp_BeginFrame( float camera_separation )
{
}

void	SWimp_EndFrame (void)
{
	if(!vid_resolutions[currentvideomode].isLFB)
	{
		if(!(vid_resolutions[currentvideomode].isBanked) && !(vid_resolutions[currentvideomode].isPlanar))
		{
			/* JASON: VGA Mode 13 */
			dosmemput(vid.buffer,vid_resolutions[currentvideomode].height*vid_resolutions[currentvideomode].width,0xA0000);
		}
		else /* FS: Credit to ggorts */
		{
			if(vid_resolutions[currentvideomode].isBanked)
			{
				VID_DrawBanked();
			}
			else
			{
				VGA_UpdatePlanarScreen(vid.buffer); /* FS: Abrash's code */
			}
		}
	}
	else
	{
		memcpy(vid_resolutions[currentvideomode].address, vid.buffer, (vid.height*vid.width));
	}
}

/* Windows style hook */
int	SWimp_Init( void *vid_nummodes, void *vid_modes )
{
	currentvideomode = 0;

	VID_InitExtra(); /* probe VESA */

	/* HACK HACK HACK: sending the video modes list to vid_dos.c
	 * by exploiting our hInstance and wndProc parameters.  See:
	 * vid_dos.c:VID_LoadRefresh()
	 */
	*(int *) vid_nummodes = num_vid_resolutions;
	*(void **) vid_modes = vid_resolutions;

	return 0;
}

/*
** SWimp_SetPalette
**
** System specific palette setting routine.  A NULL palette means
** to use the existing palette.  The palette is expected to be in
** a padded 4-byte xRGB format.
*/
void	SWimp_SetPalette( const unsigned char *palette)
{
	int shiftcomponents;
	int i;

	if(palette==NULL)
	{
		return;
	}

	shiftcomponents=2;
	outportb(0x3c8,0);	/* JASON: this means we are going to set the pallette */

	for(i=0;i<1024;i++)	/* JASON: we do it this way to skip a byte since it's padded */
	{
		outportb(0x3c9,palette[i]>>shiftcomponents);i++;
		outportb(0x3c9,palette[i]>>shiftcomponents);i++;
		outportb(0x3c9,palette[i]>>shiftcomponents);i++;
	}
}

void	SWimp_Shutdown( void )
{
	/* JASON: return to text mode */
	__dpmi_regs r;
	vid.buffer = 0;
	vid.rowbytes = 0;

	r.x.ax = 3;
	__dpmi_int(0x10, &r);
}

rserr_t SWimp_SetMode( int *pwidth, int *pheight, int mode, qboolean fullscreen )
{
	__dpmi_regs r;

	ri.Con_Printf(PRINT_ALL, "SWimp_SetMode %d fullscreen %d\n",mode,fullscreen);

	if ( !ri.Vid_GetModeInfo( pwidth, pheight, mode ) )
	{
		ri.Con_Printf( PRINT_ALL, " invalid mode\n" );

		return rserr_invalid_mode;
	}

	ri.Con_Printf( PRINT_ALL, "SWimp_SetMode setting to %s %dx%d\n",vid_resolutions[mode].menuname, *pwidth, *pheight);

	currentvideomode=mode;

	vid.height=vid_resolutions[mode].height;
	vid.width=vid_resolutions[mode].width;
	vid.rowbytes=vid.width;

	if(vid.buffer)
	{
		free(vid.buffer);
	}

	vid.buffer=malloc(vid.width*vid.height*1);

	r.x.ax = 0x13; /* FS: Make sure mode 13 is set again because if we go to high res mode and back it gets funky with banked modes.  Also, Win9x wants this to happen before going to LFB */		
	__dpmi_int(0x10, &r);

	if(!vid_resolutions[mode].isLFB)
	{
		if(!vid_resolutions[mode].isBanked && !vid_resolutions[mode].isPlanar)
		{	/* JASON: VGA Mode 13 */
			/* FS: Do nothing, let it fall through */
//			r.x.ax = 0x13;
		}
		else
		{
			if(vid_resolutions[mode].isBanked)
			{
				ri.Con_Printf(PRINT_ALL, "\x02Setting banked mode!\n");
			}
			else
			{
				ri.Con_Printf(PRINT_DEVELOPER, "\x02Setting VGA-X mode!\n");
			}

			r.x.ax = 0x4F02;
			r.x.bx = vid_resolutions[mode].vesa_mode;

			if(vid_resolutions[mode].isPlanar)
			{
				/* enable all planes for writing */
				outportb (SC_INDEX, MAP_MASK);
				outportb (SC_DATA, 0x0F);
				VideoRegisterSet(vrs320x240x256planar); /* FS: 320x240x8 only. */
				VGA_pagebase = vid_resolutions[mode].planarAddress;
				VGA_rowbytes = vid_resolutions[mode].width / 4;
				VGA_bufferrowbytes = vid.rowbytes;
				VGA_width = vid.width;
				VGA_height = vid.height;
			}
			__dpmi_int(0x10, &r);
		}
	}
	else
	{
	    //VESA LFB
		r.x.ax = 0x4F02;
		r.x.bx = vid_resolutions[mode].vesa_mode+0x4000; /* JASON: 0x4000 for Linear access */
		__dpmi_int(0x10, &r);

		if (r.h.ah)
		{
			Sys_Error("Error setting VESA LFB mode 0x%0x",vid_resolutions[mode].vesa_mode);
		}
	}

	ri.Vid_NewWindow(vid.width,vid.height);

	return rserr_ok;
}

void		SWimp_AppActivate( qboolean active )
{
}

void VID_DrawBanked(void)
{
	const unsigned char *buf = vid.buffer;
	int bank = 0;
	int h = vid_resolutions[currentvideomode].height;
	int w = vid_resolutions[currentvideomode].width;
	int todo = h * w;
	int copy;
	__dpmi_regs r;

	while (todo > 0)
	{
#if 0 /* FS: For page flipping? */
		r.x.ax = 0x4F07;
		r.x.bx = 0;
		r.x.cx = bank % w;
		r.x.dx = bank / w;
		__dpmi_int(0x10, &r);
#endif

		r.x.ax = 0x4F05;
		r.x.bx = 0;
		r.x.dx = bank;
		__dpmi_int(0x10, &r);

		copy = (todo > 65536)? 65536 : todo;
		dosmemput(buf, copy, 0xA0000);

		todo -= copy;
		buf += copy;
		bank++;
	}

	r.x.ax = 0x4f05;
	r.x.bx = 0;
	r.x.dx = 0;
	__dpmi_int(0x10, &r);
}
