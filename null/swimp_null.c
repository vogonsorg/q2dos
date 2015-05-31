#include "../ref_soft/r_local.h"

void		SWimp_BeginFrame( float camera_separation )
{
	printf("b");
	fflush(stdout);
}

void		SWimp_EndFrame (void)
{
	printf(".");
	fflush(stdout);
}

int			SWimp_Init( void *hInstance, void *wndProc )
{
	printf("SWimp_Init windows only?\n");
	return 0;
}

void		SWimp_SetPalette( const unsigned char *palette)
{
	printf("SWimp_SetPalette\n");
}

void		SWimp_Shutdown( void )
{
}

rserr_t		SWimp_SetMode( int *pwidth, int *pheight, int mode, qboolean fullscreen )
{
	printf("SWimp_SetMode %d x %d mode %d fs %d\n",&pwidth,&pheight,mode,fullscreen);
	vid.height=200;
	vid.width=320;
	vid.rowbytes=320;
	vid.buffer=malloc(320*240*8);

	return rserr_ok;
}

void		SWimp_AppActivate( qboolean active )
{
}

