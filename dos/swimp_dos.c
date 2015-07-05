#include <dpmi.h>
#include <pc.h>
#include <sys/nearptr.h>
#include "../ref_soft/r_local.h"
#include "vid_dos.h"

// FS: For planar modes
#include "vregset.h"
#include "vgamodes.h"
#include "dosisms.h"

int currentvideomode = 0;

void VID_DrawBanked(void);
void VID_DrawPlanar(void);
void VGA_UpdatePlanarScreen (void *srcbuffer);

void	SWimp_BeginFrame( float camera_separation )
{
}

void	SWimp_EndFrame (void)
{
	//It's LFB only
	if(!vid_resolutions[currentvideomode].isLFB)
	{
		if(!(vid_resolutions[currentvideomode].isBanked) && !(vid_resolutions[currentvideomode].isPlanar))	// VGA mode 13
		{
			dosmemput(vid.buffer,vid_resolutions[currentvideomode].height*vid_resolutions[currentvideomode].width,0xA0000);
		}
		else // FS: Credit to ggorts
		{
			if(vid_resolutions[currentvideomode].isBanked)
			{
				VID_DrawBanked();
			}
			else
			{
				VGA_UpdatePlanarScreen(vid.buffer); // FS: Abrashes code
			}
		}
	}
	else
	{
		memcpy(vid_resolutions[currentvideomode].address, vid.buffer, (vid.height*vid.width));
	}
}

//Windows style hook
int	SWimp_Init( void *hInstance, void *wndProc )
{
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
	outp(0x3c8,0);	//this means we are going to set the pallette

	for(i=0;i<1024;i++)	//we do it this way to skip a byte since it's padded
	{
		outp(0x3c9,palette[i]>>shiftcomponents);i++;
		outp(0x3c9,palette[i]>>shiftcomponents);i++;
		outp(0x3c9,palette[i]>>shiftcomponents);i++;
	}
}

void	SWimp_Shutdown( void )
{
	//return to text mode
	__dpmi_regs r;
	vid.buffer = 0;
	vid.rowbytes = 0;

	r.x.ax = 3;
	__dpmi_int(0x10, &r);
}

rserr_t		SWimp_SetMode( int *pwidth, int *pheight, int mode, qboolean fullscreen )
{
	Com_Printf("SWimp_SetMode %d fullscreen %d\n",mode,fullscreen); 

	if ( !ri.Vid_GetModeInfo( pwidth, pheight, mode ) )
	{
		ri.Con_Printf( PRINT_ALL, " invalid mode\n" );

		return rserr_invalid_mode;
	}

	ri.Con_Printf( PRINT_ALL, "SWimp_SetMode setting to %s %dx%d\n",vid_resolutions[mode].menuname, *pwidth, *pheight);

	currentvideomode=mode;

	if(!vid_resolutions[mode].isLFB)
	{
		__dpmi_regs r;

		vid.height=vid_resolutions[mode].height;
		vid.width=vid_resolutions[mode].width;
		vid.rowbytes=vid.width;

		if(vid.buffer)
		{
			free(vid.buffer);
		}

		vid.buffer=malloc(vid.width*vid.height*1);

		if(!vid_resolutions[mode].isBanked && !vid_resolutions[mode].isPlanar)
		{	//mode 13
			r.x.ax = 0x13;
		}
		else
		{
			if(vid_resolutions[mode].isBanked)
			{
				ri.Con_Printf(PRINT_ALL, "\x02Setting banked mode!\n");
			}
			else
			{
				r.x.ax = 0x13;
				__dpmi_int(0x10, &r);
				ri.Con_Printf(PRINT_ALL, "\x02Setting VGA-X mode!\n");
			}

			r.x.ax = 0x4F02;
			r.x.bx = vid_resolutions[mode].vesa_mode;

			if(vid_resolutions[mode].isPlanar)
			{
				// enable all planes for writing
				outportb (SC_INDEX, MAP_MASK);
				outportb (SC_DATA, 0x0F);
				VideoRegisterSet(vrs320x240x256planar);
				VGA_pagebase = vid_resolutions[mode].planarAddress;
				VGA_rowbytes = vid_resolutions[mode].width / 4;
				VGA_bufferrowbytes = vid.rowbytes;
				VGA_width = vid.width;
				VGA_height = vid.height;
				
			}

		}

		__dpmi_int(0x10, &r);
	}
	else
	{
		vid.height=vid_resolutions[mode].height;
		vid.width=vid_resolutions[mode].width;
		vid.rowbytes=vid.width;

		if(vid.buffer)
		{
			free(vid.buffer);
		}

		vid.buffer=malloc(vid.width*vid.height*1);
		{    //VESA 
			__dpmi_regs r;
			r.x.ax = 0x4F02;
			r.x.bx = vid_resolutions[mode].vesa_mode+0x4000; //0x4000 for Linear access
			__dpmi_int(0x10, &r);

			if (r.h.ah)
			{
				Sys_Error("Error setting VESA LFB mode 0x%0x",vid_resolutions[mode].vesa_mode);
			}
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
	int bank_number=0;
	int todo=vid_resolutions[currentvideomode].height*vid_resolutions[currentvideomode].width;
	int copy_size;
	__dpmi_regs r;

	while (todo>0)
	{
#if 0 // FS: For page flipping?
		r.x.ax = 0x4F07;
		r.x.bx = 0;
		r.x.cx = bank_number % vid_resolutions[currentvideomode].width;
		r.x.dx = bank_number / vid_resolutions[currentvideomode].width;
		__dpmi_int(0x10, &r);
#endif

		r.x.ax = 0x4F05;
		r.x.bx = 0;
		r.x.dx = bank_number; // FS: FIXME bank buffer need to be swapped!
		__dpmi_int(0x10, &r);

		if (todo>65536) // FS: 320x240
		{
			copy_size=65536;
		}
		else
		{
			copy_size=todo;
		}

		dosmemput(vid.buffer, copy_size, 0xA0000);

		todo-=copy_size;
		vid.buffer+=copy_size;
		bank_number++;
	}
	// FS: FIXME: I don't think the next line is the right thing to do?
	vid.buffer-=vid_resolutions[currentvideomode].height*vid_resolutions[currentvideomode].width; // FS: Move back to the beginning, this was the cause of the crash after a few frames
	r.x.ax = 0x4f05;
	r.x.bx = 0;
	r.x.dx = 0;
	__dpmi_int(0x10, &r);
}
