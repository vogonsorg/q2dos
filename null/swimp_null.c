#include "../ref_soft/r_local.h"

void	SWimp_BeginFrame( float camera_separation )
{
	printf("b");
	fflush(stdout);
}

void	SWimp_EndFrame (void)
{
	printf(".");
	fflush(stdout);
}

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

