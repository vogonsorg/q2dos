#include "../ref_soft/r_local.h"
#include <dpmi.h>
#include <pc.h>


void		SWimp_BeginFrame( float camera_separation )
{
//	printf("b");
//	fflush(stdout);
}

static int wtf;
void		SWimp_EndFrame (void)
{
unsigned lol[256*3];
int j;
	dosmemput(vid.buffer,320*200,0xA0000);
//Mess with the palette to prove we didn't crash
memcpy(lol,vid.buffer,256*3);
for(j=0;j<256*3;j++)
	lol[j]=lol[j]+wtf;

wtf++;

SWimp_SetPalette(lol);
}

int			SWimp_Init( void *hInstance, void *wndProc )
{
	printf("SWimp_Init windows only?\n");
	return 0;
}

void		SWimp_SetPalette( const unsigned char *pal)
{
	int shiftcomponents=2;
	int i;

	outportb(0x3c8, 0);
	for (i=0 ; i<768 ; i++)
		outportb(0x3c9, (pal[i]>>shiftcomponents));
}

void		SWimp_Shutdown( void )
{
}

rserr_t		SWimp_SetMode( int *pwidth, int *pheight, int mode, qboolean fullscreen )
{
	printf("SWimp_SetMode %d x %d mode %d fs %d\n",&pwidth,&pheight,mode,fullscreen);

        if ( !ri.Vid_GetModeInfo( pwidth, pheight, mode ) )
        {
                ri.Con_Printf( PRINT_ALL, " invalid mode\n" );
                return rserr_invalid_mode;
        }
        ri.Con_Printf( PRINT_ALL, " %d %d\n", *pwidth, *pheight);


	vid.height=200;
	vid.width=320;
	vid.rowbytes=320;
	vid.buffer=malloc(320*200*1);
   {	//mode 13
      __dpmi_regs r;

      r.x.ax = 0x13;
      __dpmi_int(0x10, &r);
   }

	return rserr_ok;
}

void		SWimp_AppActivate( qboolean active )
{
}

