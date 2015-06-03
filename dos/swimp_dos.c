#include "../ref_soft/r_local.h"
#include <dpmi.h>
#include <pc.h>

int whatmodearewe = 0;


void	SWimp_BeginFrame( float camera_separation )
{
//	printf("b");
//	fflush(stdout);
}

void	SWimp_EndFrame (void)
{
	//It's LFB only
if(whatmodearewe==0)	//VGA mode 13
	dosmemput(vid.buffer,320*200,0xA0000);
else
	dosmemput(vid.buffer,848*480,0xC0000000);
}

int	SWimp_Init( void *hInstance, void *wndProc )
{
//	printf("SWimp_Init windows only?\n");
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
}

rserr_t		SWimp_SetMode( int *pwidth, int *pheight, int mode, qboolean fullscreen )
{
Com_Printf("SWimp_SetMode %d x %d mode %d fs %d\n",*pwidth,*pheight,mode,fullscreen); 
        if ( !ri.Vid_GetModeInfo( pwidth, pheight, mode ) )
        {
                ri.Con_Printf( PRINT_ALL, " invalid mode\n" );
                return rserr_invalid_mode;
        }
        ri.Con_Printf( PRINT_ALL, " %d %d\n", *pwidth, *pheight);

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
        vid.height=480;
        vid.width=848;
        vid.rowbytes=848;
        vid.buffer=malloc(848*480*1);
   {    //VESA 222
      __dpmi_regs r;
      r.x.ax = 0x4F02;
      r.x.bx = 0x222;
      __dpmi_int(0x10, &r);
      if (r.h.ah)
         Sys_Error("Error setting VESA mode 0x222");
   }
 }
	ri.Vid_NewWindow(vid.width,vid.height);
	return rserr_ok;
}

void		SWimp_AppActivate( qboolean active )
{
}

