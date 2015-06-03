#include "../ref_soft/r_local.h"


void	SWimp_BeginFrame( float camera_separation )
{
}

void	SWimp_EndFrame (void)
{
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

	return rserr_ok;
}

void		SWimp_AppActivate( qboolean active )
{
}

