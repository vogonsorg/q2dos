/*
** GL_SAGE.C
**
** This file contains DOS specific stuff having to do with the
** OpenGL refresh using sage interface.  The following functions
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

#if defined(REF_HARD_LINKED) && !defined(REFGL_SAGE)
int SAGE_ScanIFace (void)
{
	return -1;
}

#else

#include <GL/sage.h>

#ifndef REF_HARD_LINKED
#include <dlfcn.h>
typedef int (*sage_init_f) (void);
typedef sageContext* (*sage_open_f) (int, int, int, int, int, int, int);
typedef int (*sage_bind_f) (sageContext *, void *, int, int);
typedef void (*sage_shut_f) (sageContext *);
typedef void (*sage_fini_f) (void);
typedef void (*sage_swap_f) (int);
typedef SageProc (*sage_GetProcAddress_f) (const char *);
static sage_init_f sage_init_fp;
static sage_open_f sage_open_fp;
static sage_bind_f sage_bind_fp;
static sage_shut_f sage_shut_fp;
static sage_fini_f sage_fini_fp;
static sage_swap_f sage_swap_fp;
static sage_GetProcAddress_f sage_GetProcAddress_fp;
#else
#define sage_init_fp sage_init
#define sage_open_fp sage_open
#define sage_bind_fp sage_bind
#define sage_shut_fp sage_shut
#define sage_fini_fp sage_fini
#define sage_swap_fp sage_swap
#define sage_GetProcAddress_fp sage_GetProcAddress
#endif

static sageContext *ctx;
static qboolean firstRunHack = true;
static void SAGE_Shutdown (void);

static int SAGE_InitCtx (int *width, int *height, int *bpp)
{
	int hwbpp;

	hwbpp = sage_init_fp ();
	if (hwbpp <= 0)
		return -1;

	if (hwbpp < *bpp) {
		ri.Con_Printf(PRINT_ALL, "%d bpp not supported. Falling back to 16 bpp.\n", *bpp);
		*bpp = 16;
		ri.Cvar_Set("vid_glbpp", "16");
	}

	/* request either 16 or 32 bpp visual */
	if (*bpp <= 16)
		ctx = sage_open_fp(true, 5, 6, 5, 0, 16, 0);
	else	ctx = sage_open_fp(true, 8, 8, 8, 8, 24, 8);
	if (!ctx) {
		sage_fini_fp ();
		return -1;
	}

	if (sage_bind_fp(ctx, (void *)0xB16B00B5, *width, *height) != 0) {
		sage_bind_fp (NULL, NULL, 0, 0);
		sage_shut_fp (ctx);
		sage_fini_fp ();
		ctx = NULL;
		return -1;
	}

	/* FS:  HACK HACK HACK:  For some reason SAGE has bad
	 * performance unless it's loaded twice.  SLI or not. :( */
	if (firstRunHack) {
		firstRunHack = false;
		ri.Con_Printf(PRINT_DEVELOPER, "Running SAGE performance hack\n");
		SAGE_Shutdown();
		return SAGE_InitCtx(width, height, bpp);
	}

	return 0;
}

static void SAGE_Shutdown (void)
{
	if (ctx)
	{
		sage_bind_fp (NULL, NULL, 0, 0);
		sage_shut_fp (ctx);
		sage_fini_fp ();
		ctx = NULL;
	}
}

static void SAGE_EndFrame (void)
{
	sage_swap_fp (1);
}

#ifndef REF_HARD_LINKED
static void *SAGE_GetProcAddress (const char *sym)
{
	if (sage_GetProcAddress_fp)
		return sage_GetProcAddress_fp (sym);
	return NULL;
}
#else /* assume the function is present */
static void *SAGE_GetProcAddress (const char *sym) {
	return sage_GetProcAddress (sym);
}
#endif

static const char *SAGE_IFaceName (void)
{
	return "sage";
}

int SAGE_ScanIFace (void)
{
#ifndef REF_HARD_LINKED
	DOSGL_InitCtx  = NULL;
	DOSGL_Shutdown = NULL;
	DOSGL_EndFrame = NULL;
	DOSGL_GetProcAddress = NULL;
	DOSGL_IFaceName = NULL;
	sage_init_fp = (sage_init_f) dlsym(RTLD_DEFAULT,"_sage_init");
	sage_open_fp = (sage_open_f) dlsym(RTLD_DEFAULT,"_sage_open");
	sage_bind_fp = (sage_bind_f) dlsym(RTLD_DEFAULT,"_sage_bind");
	sage_shut_fp = (sage_shut_f) dlsym(RTLD_DEFAULT,"_sage_shut");
	sage_fini_fp = (sage_fini_f) dlsym(RTLD_DEFAULT,"_sage_fini");
	sage_swap_fp = (sage_swap_f) dlsym(RTLD_DEFAULT,"_sage_swap");
	sage_GetProcAddress_fp = (sage_GetProcAddress_f) dlsym(RTLD_DEFAULT,"_sage_GetProcAddress");
	if (!sage_init_fp || !sage_open_fp ||
	    !sage_bind_fp || !sage_shut_fp ||
	    !sage_fini_fp || !sage_swap_fp) {
		return -1;
	}
#endif

	DOSGL_InitCtx  = SAGE_InitCtx;
	DOSGL_Shutdown = SAGE_Shutdown;
	DOSGL_EndFrame = SAGE_EndFrame;
	DOSGL_GetProcAddress = SAGE_GetProcAddress;
	DOSGL_IFaceName = SAGE_IFaceName;

	return 0;
}

#endif
