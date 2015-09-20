/*
** GL_FXMESA.C
**
** This file contains DOS specific stuff having to do with the
** OpenGL refresh using fxMesa interface.  The following functions
** must be implemented:
**
** DOSGL_InitCtx
** DOSGL_Shutdown
** DOSGL_EndFrame
** DOSGL_GetProcAddress
** DOSGL_IFaceName
*/

#include "../ref_gl/gl_local.h"
#include "glimp_dos.h"

#if defined(REF_HARD_LINKED) && !defined(REFGL_FXMESA)
int FXMESA_ScanIFace (void)
{
	return -1;
}

#else

#include <GL/fxmesa.h>

#ifndef REF_HARD_LINKED
#include <dlfcn.h>
static fxMesaContext (*fxMesaCreateContext_fp) (GLuint, GrScreenResolution_t, GrScreenRefresh_t, const GLint attribList[]);
static fxMesaContext (*fxMesaCreateBestContext_fp) (GLuint, GLint, GLint, const GLint attribList[]);
static void (*fxMesaMakeCurrent_fp) (fxMesaContext);
static void (*fxMesaDestroyContext_fp) (fxMesaContext);
static void (*fxMesaSwapBuffers_fp) (void);
#else
#define fxMesaCreateContext_fp fxMesaCreateContext
#define fxMesaCreateBestContext_fp fxMesaCreateBestContext
#define fxMesaMakeCurrent_fp fxMesaMakeCurrent
#define fxMesaDestroyContext_fp fxMesaDestroyContext
#define fxMesaSwapBuffers_fp fxMesaSwapBuffers
#endif

static fxMesaContext fc = NULL;

#if 0
static int findres (int *width, int *height)
{
	static int fx_resolutions[][3] = {
		{ 320,200,  GR_RESOLUTION_320x200 },
		{ 320,240,  GR_RESOLUTION_320x240 },
		{ 400,256,  GR_RESOLUTION_400x256 },
		{ 400,300,  GR_RESOLUTION_400x300 },
		{ 512,384,  GR_RESOLUTION_512x384 },
		{ 640,200,  GR_RESOLUTION_640x200 },
		{ 640,350,  GR_RESOLUTION_640x350 },
		{ 640,400,  GR_RESOLUTION_640x400 },
		{ 640,480,  GR_RESOLUTION_640x480 },
		{ 800,600,  GR_RESOLUTION_800x600 },
		{ 960,720,  GR_RESOLUTION_960x720 },
		{ 856,480,  GR_RESOLUTION_856x480 },
		{ 512,256,  GR_RESOLUTION_512x256 },
		{ 1024,768, GR_RESOLUTION_1024x768 },
		{ 1280,1024,GR_RESOLUTION_1280x1024 },
		{ 1600,1200,GR_RESOLUTION_1600x1200 },
		{ 0, 0, 0 }
	};

	int i;

	for (i = 0; fx_resolutions[i][0]; i++) {
		if (*width <= fx_resolutions[i][0] && *height <= fx_resolutions[i][1]) {
			*width = fx_resolutions[i][0];
			*height = fx_resolutions[i][1];
			return fx_resolutions[i][2];
		}
	}

	*width = 640;
	*height = 480;
	return GR_RESOLUTION_640x480;
}
#endif

static int FXMESA_InitCtx (int *width, int *height, int *bpp)
{
	GLint attribs[32];

	attribs[0] = FXMESA_DOUBLEBUFFER;
	attribs[1] = FXMESA_ALPHA_SIZE;
	attribs[2] = 1;
	attribs[3] = FXMESA_DEPTH_SIZE;
	attribs[4] = 1;
	attribs[5] = FXMESA_NONE;

//	fc = fxMesaCreateContext_fp(0, findres(width, height), GR_REFRESH_60Hz, attribs);
	fc = fxMesaCreateBestContext_fp(0, *width, *height, attribs); /* FS: allows us to use SST_SCREENREFRESH to set the refresh rate */
	if (!fc)
		return -1;

	fxMesaMakeCurrent_fp(fc);

	return 0;
}

static void FXMESA_Shutdown (void)
{
	if (fc) {
		fxMesaDestroyContext_fp(fc);
		fc = NULL;
	}
}

static void FXMESA_EndFrame (void)
{
	qglFlush();
	fxMesaSwapBuffers_fp();
}

static void *FXMESA_GetProcAddress (const char *sym)
{
	return NULL; /* hmph.. */
}

static const char *FXMESA_IFaceName (void)
{
	return "fxMesa";
}

int FXMESA_ScanIFace (void)
{
#ifndef REF_HARD_LINKED
	DOSGL_InitCtx  = NULL;
	DOSGL_Shutdown = NULL;
	DOSGL_EndFrame = NULL;
	DOSGL_GetProcAddress = NULL;
	DOSGL_IFaceName = NULL;
	fxMesaCreateContext_fp = dlsym(RTLD_DEFAULT,"_fxMesaCreateContext");
	fxMesaCreateBestContext_fp = dlsym(RTLD_DEFAULT,"_fxMesaCreateBestContext");
	fxMesaMakeCurrent_fp = dlsym(RTLD_DEFAULT,"_fxMesaMakeCurrent");
	fxMesaDestroyContext_fp = dlsym(RTLD_DEFAULT,"_fxMesaDestroyContext");
	fxMesaSwapBuffers_fp = dlsym(RTLD_DEFAULT,"_fxMesaSwapBuffers");
	if (!fxMesaCreateContext_fp || !fxMesaCreateBestContext_fp ||
	    !fxMesaMakeCurrent_fp || !fxMesaDestroyContext_fp ||
	    !fxMesaSwapBuffers_fp) {
		return -1;
	}
#endif

	DOSGL_InitCtx  = FXMESA_InitCtx;
	DOSGL_Shutdown = FXMESA_Shutdown;
	DOSGL_EndFrame = FXMESA_EndFrame;
	DOSGL_GetProcAddress = FXMESA_GetProcAddress;
	DOSGL_IFaceName = FXMESA_IFaceName;

	return 0;
}

#endif
