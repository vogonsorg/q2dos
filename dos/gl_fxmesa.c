/*
** GLW_IMP.C
**
** This file contains ALL Linux specific stuff having to do with the
** OpenGL refresh.  When a port is being made the following functions
** must be implemented by the port:
**
** GLimp_EndFrame
** GLimp_Init
** GLimp_Shutdown
** GLimp_SwitchFullscreen
**
*/

#include <termios.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <stdio.h>

#include "../ref_3dfx/gl_local.h"
#include "../client/keys.h"
#include "vid_dos.h"

#include <GL/fxmesa.h>

/*****************************************************************************/

qboolean GLimp_InitGL (void);

extern cvar_t *vid_fullscreen;
extern cvar_t *vid_ref;

static fxMesaContext fc = NULL;

#define NUM_GL_RESOLUTIONS 8

vmodeinfo_t resolutions[NUM_GL_RESOLUTIONS]={ 
	{ 240,320,  "[320x240]" },
	{ 300,400,  "[400x300]" },
	{ 384,512,  "[512x384]" },
	{ 480,640,  "[640x480]" },
	{ 600,800,  "[800x600]" },
	{ 768,1024,  "[1024x768]" },
	{ 1024,1280,  "[1280x1024]" },
	{ 1200,1600,  "[1600x1200]" }
};

/*
** GLimp_SetMode
*/
rserr_t GLimp_SetMode( int *pwidth, int *pheight, int mode, qboolean fullscreen )
{
	int width, height;
	GLint attribs[32];

	if(gl_mode->value < 0.0f)
	{
		width = r_customwidth->intValue;
		height = r_customheight->intValue;
	}

	ri.Con_Printf( PRINT_ALL, "Initializing OpenGL display\n");

	ri.Con_Printf (PRINT_ALL, "...setting mode %d:", mode );

	if ( !ri.Vid_GetModeInfo( &width, &height, mode ) )
	{
		ri.Con_Printf( PRINT_ALL, " invalid mode\n" );
		return rserr_invalid_mode;
	}

	ri.Con_Printf( PRINT_ALL, " %d %d\n", width, height );

	// destroy the existing window
	GLimp_Shutdown ();

	// set fx attribs
	attribs[0] = FXMESA_DOUBLEBUFFER;
	attribs[1] = FXMESA_ALPHA_SIZE;
	attribs[2] = 1;
	attribs[3] = FXMESA_DEPTH_SIZE;
	attribs[4] = 1;
	attribs[5] = FXMESA_NONE;

//	fc = fxMesaCreateContext(0, findres(&width, &height), GR_REFRESH_60Hz, attribs);
	fc = fxMesaCreateBestContext(0, (GLint)width, (GLint)height, attribs); /* FS: This will allow us to use SST_SCREENREFRESH to set the refresh rate */
	if (!fc)
		return rserr_invalid_mode;

	*pwidth = width;
	*pheight = height;

	// let the sound and input subsystems know about the new window
	ri.Vid_NewWindow (width, height);

	fxMesaMakeCurrent(fc);

	return rserr_ok;
}

/*
** GLimp_Shutdown
**
** This routine does all OS specific shutdown procedures for the OpenGL
** subsystem.  Under OpenGL this means NULLing out the current DC and
** HGLRC, deleting the rendering context, and releasing the DC acquired
** for the window.  The state structure is also nulled out.
**
*/
void GLimp_Shutdown( void )
{
	if (fc) {
		fxMesaDestroyContext(fc);
		fc = NULL;
	}
}

/*
** GLimp_Init
**
** This routine is responsible for initializing the OS specific portions
** of OpenGL.  
*/

int GLimp_Init( void *nummodes, void *modeinfos )
{
	vmodeinfo_t *vi;
	int	i;

	/* HACK HACK HACK: sending the video mode infos to vid_dos.c
	 * by exploiting our params. See: vid_dos.c:VID_LoadRefresh() */
	vi = malloc(sizeof(resolutions) * sizeof(vmodeinfo_t));
	for (i = 0; i < NUM_GL_RESOLUTIONS; ++i)
	{
		vi[i].height = resolutions[i].height;
		vi[i].width = resolutions[i].width;
		strcpy(vi[i].menuname, resolutions[i].menuname);
	}
	*(int *) nummodes = NUM_GL_RESOLUTIONS;
	*(vmodeinfo_t **) modeinfos = vi;

	return true;
}

/*
** GLimp_BeginFrame
*/
void GLimp_BeginFrame( float camera_seperation )
{
}

/*
** GLimp_EndFrame
** 
** Responsible for doing a swapbuffers and possibly for other stuff
** as yet to be determined.  Probably better not to make this a GLimp
** function and instead do a call to GLimp_SwapBuffers.
*/
void GLimp_EndFrame (void)
{
	glFlush();
	fxMesaSwapBuffers();
}

/*
** GLimp_AppActivate
*/
void GLimp_AppActivate( qboolean active )
{
}

extern void gl3DfxSetPaletteEXT(GLuint *pal);

void Fake_glColorTableEXT( GLenum target, GLenum internalformat,
                             GLsizei width, GLenum format, GLenum type,
                             const GLvoid *table )
{
	byte temptable[256][4];
	byte *intbl;
	int i;

	for (intbl = (byte *)table, i = 0; i < 256; i++) {
		temptable[i][2] = *intbl++;
		temptable[i][1] = *intbl++;
		temptable[i][0] = *intbl++;
		temptable[i][3] = 255;
	}
	gl3DfxSetPaletteEXT((GLuint *)temptable);
}

/* FS: TODO FIXME: These are just stubs so Knightmare's ref_gl can link until GetProcAddress is working for extensions */
void UpdateGammaRamp (void)
{
}

void qglClientActiveTextureARB (GLenum foo)
{
}
void qwglGetProcAddress (char * foo)
{
}

void qglActiveTextureARB (GLenum foo)
{
}

void qglMultiTexCoord2f (GLenum foo, GLfloat x, GLfloat y)
{
}