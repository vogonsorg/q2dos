/*
** GL_DMESA.C
**
** This file contains DOS specific stuff having to do with the
** OpenGL refresh using DMesa interface.  The following functions
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

#if defined(REF_HARD_LINKED) && !defined(REFGL_MESA)
int DMESA_ScanIFace (void)
{
	return -1;
}

#else

#include <GL/dmesa.h>

#ifndef REF_HARD_LINKED
#include <dlfcn.h>
static DMesaVisual (*DMesaCreateVisual_fp) (GLint, GLint, GLint, GLint, GLboolean, GLboolean, GLint, GLint, GLint, GLint);
static void (*DMesaDestroyVisual_fp) (DMesaVisual);
static DMesaContext (*DMesaCreateContext_fp) (DMesaVisual, DMesaContext);
static void (*DMesaDestroyContext_fp) (DMesaContext);
static DMesaBuffer (*DMesaCreateBuffer_fp) (DMesaVisual, GLint, GLint, GLint, GLint);
static void (*DMesaDestroyBuffer_fp) (DMesaBuffer b);
static void (*DMesaSwapBuffers_fp) (DMesaBuffer);
static GLboolean (*DMesaMakeCurrent_fp) (DMesaContext, DMesaBuffer);
static DMesaProc (*DMesaGetProcAddress_fp) (const char *);
#else
#define DMesaCreateVisual_fp DMesaCreateVisual
#define DMesaDestroyVisual_fp DMesaDestroyVisual
#define DMesaCreateContext_fp DMesaCreateContext
#define DMesaDestroyContext_fp DMesaDestroyContext
#define DMesaCreateBuffer_fp DMesaCreateBuffer
#define DMesaDestroyBuffer_fp DMesaDestroyBuffer
#define DMesaSwapBuffers_fp DMesaSwapBuffers
#define DMesaMakeCurrent_fp DMesaMakeCurrent
#define DMesaGetProcAddress_fp DMesaGetProcAddress
#endif

static DMesaVisual dv;
static DMesaContext dc;
static DMesaBuffer db;

static int DMESA_InitCtx (int *width, int *height, int *bpp)
{
	dv = DMesaCreateVisual_fp(*width, *height, *bpp, 0, true, true, 2, 16, 0, 0);
	if (!dv) return -1;

	dc = DMesaCreateContext_fp(dv, NULL);
	if (!dc) return -1;

	db = DMesaCreateBuffer_fp(dv, 0, 0, *width, *height);
	if (!db) return -1;

	DMesaMakeCurrent_fp(dc, db);

	return 0;
}

static void DMESA_Shutdown (void)
{
	if (db) {
		DMesaDestroyBuffer_fp(db);
		db = NULL;
	}
	if (dc) {
		DMesaDestroyContext_fp(dc);
		dc = NULL;
	}
	if (dv) {
		DMesaDestroyVisual_fp(dv);
		dv = NULL;
	}
}

static void DMESA_EndFrame (void)
{
	qglFlush();
	DMesaSwapBuffers_fp(db);
}

static void *DMESA_GetProcAddress (const char *sym)
{
	if (DMesaGetProcAddress_fp)
		return DMesaGetProcAddress_fp (sym);
	return NULL;
}

static const char *DMESA_IFaceName (void)
{
	return "DMesa";
}

int DMESA_ScanIFace (void)
{
#ifndef REF_HARD_LINKED
	DOSGL_InitCtx  = NULL;
	DOSGL_Shutdown = NULL;
	DOSGL_EndFrame = NULL;
	DOSGL_GetProcAddress = NULL;
	DOSGL_IFaceName = NULL;
	DMesaCreateVisual_fp = dlsym(RTLD_DEFAULT,"_DMesaCreateVisual");
	DMesaDestroyVisual_fp = dlsym(RTLD_DEFAULT,"_DMesaDestroyVisual");
	DMesaCreateContext_fp = dlsym(RTLD_DEFAULT,"_DMesaCreateContext");
	DMesaDestroyContext_fp = dlsym(RTLD_DEFAULT,"_DMesaDestroyContext");
	DMesaCreateBuffer_fp = dlsym(RTLD_DEFAULT,"_DMesaCreateBuffer");
	DMesaDestroyBuffer_fp = dlsym(RTLD_DEFAULT,"_DMesaDestroyBuffer");
	DMesaSwapBuffers_fp = dlsym(RTLD_DEFAULT,"_DMesaSwapBuffers");
	DMesaMakeCurrent_fp = dlsym(RTLD_DEFAULT,"_DMesaMakeCurrent");
	DMesaGetProcAddress_fp = dlsym(RTLD_DEFAULT,"_DMesaGetProcAddress");
	if (!DMesaCreateVisual_fp || !DMesaDestroyVisual_fp ||
	    !DMesaCreateContext_fp || !DMesaDestroyContext_fp ||
	    !DMesaCreateBuffer_fp || !DMesaDestroyBuffer_fp ||
	    !DMesaSwapBuffers_fp || !DMesaMakeCurrent_fp) {
		return -1;
	}
#endif

	DOSGL_InitCtx  = DMESA_InitCtx;
	DOSGL_Shutdown = DMESA_Shutdown;
	DOSGL_EndFrame = DMESA_EndFrame;
	DOSGL_GetProcAddress = DMESA_GetProcAddress;
	DOSGL_IFaceName = DMESA_IFaceName;

	return 0;
}

#endif
