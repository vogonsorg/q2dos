#include "../ref_soft/r_local.h"
#include <dpmi.h>
#include <pc.h>

int whatmodearewe = 0;

//My stupid thing for video modes.
extern int num_vid_resolutions;
struct vid_resolutions_t {
        int mode;
        int vesa_mode;
        int height;
        int width;
	int address;
        char menuname[30];
} ;
extern struct vid_resolutions_t vid_resolutions[10];


void	SWimp_BeginFrame( float camera_separation )
{
}

void	SWimp_EndFrame (void)
{
	//It's LFB only
if(whatmodearewe==0)	//VGA mode 13
	dosmemput(vid.buffer,320*200,0xA0000);
else
	dosmemput(vid.buffer,(vid.height*vid.width),vid_resolutions[whatmodearewe].address);
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
	return;

shiftcomponents=2;
outp(0x3c8,0);	//this means we are going to set the pallette
for(i=0;i<1024;i++){	//we do it this way to skip a byte since it's padded
        outp(0x3c9,palette[i]>>shiftcomponents);i++;
        outp(0x3c9,palette[i]>>shiftcomponents);i++;
        outp(0x3c9,palette[i]>>shiftcomponents);i++;
	}

}


void		SWimp_Shutdown( void )
{
      __dpmi_regs r;

      r.x.ax = 3;
      __dpmi_int(0x10, &r);
	//return to text mode
}

rserr_t		SWimp_SetMode( int *pwidth, int *pheight, int mode, qboolean fullscreen )
{
Com_Printf("SWimp_SetMode %d fullscreen %d\n",mode,fullscreen); 
        if ( !ri.Vid_GetModeInfo( pwidth, pheight, mode ) )
        {
                ri.Con_Printf( PRINT_ALL, " invalid mode\n" );
                return rserr_invalid_mode;
        }
        ri.Con_Printf( PRINT_ALL, "SWimp_SetMode setting to %dx%d\n", *pwidth, *pheight);

whatmodearewe=mode;
if(mode==0) {
	vid.height=200;
	vid.width=320;
	vid.rowbytes=320;
	vid.buffer=malloc(320*200*1);
   {	//mode 13
      __dpmi_regs r;

      r.x.ax = 0x13;
      __dpmi_int(0x10, &r);
   }
  }
else {
        vid.height=vid_resolutions[mode].height;
        vid.width=vid_resolutions[mode].width;
        vid.rowbytes=vid.width;
        vid.buffer=malloc(vid.width*vid.height*1);
   {    //VESA 
      __dpmi_regs r;
      r.x.ax = 0x4F02;
      r.x.bx = vid_resolutions[mode].vesa_mode;
      __dpmi_int(0x10, &r);
      if (r.h.ah)
         Sys_Error("Error setting VESA mode 0x%0x",vid_resolutions[mode].vesa_mode);
   }
 }
	ri.Vid_NewWindow(vid.width,vid.height);
	return rserr_ok;
}

void		SWimp_AppActivate( qboolean active )
{
}

