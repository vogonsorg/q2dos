#include <dpmi.h>
#include <pc.h>
#include <sys/nearptr.h>
#include "../ref_soft/r_local.h"
#include "vid_dos.h"

int currentvideomode = 0;

void	SWimp_BeginFrame( float camera_separation )
{
}

void	SWimp_EndFrame (void)
{
	//It's LFB only
	if(!vid_resolutions[currentvideomode].isLFB)	//VGA mode 13
	{
		dosmemput(vid.buffer,vid_resolutions[currentvideomode].height*vid_resolutions[currentvideomode].width,0xA0000);
	}
	else
	{
#ifdef OLD_RELIABLE_CODE
		__djgpp_nearptr_enable();
		memcpy(vid_resolutions[currentvideomode].address+__djgpp_conventional_base,vid.buffer,(vid.height*vid.width));
//		__djgpp_nearptr_disable(); // FS: FIXME TODO -- DON'T DISABLE.  WILL STOMP DMA.BUFFER!
#endif
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
		vid.height=vid_resolutions[mode].height;
		vid.width=vid_resolutions[mode].width;
		vid.rowbytes=vid.width;

		if(vid.buffer)
		{
			free(vid.buffer);
		}

		vid.buffer=malloc(vid.width*vid.height*1);
		{	//mode 13
			__dpmi_regs r;

			r.x.ax = 0x13;
			__dpmi_int(0x10, &r);
		}
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
