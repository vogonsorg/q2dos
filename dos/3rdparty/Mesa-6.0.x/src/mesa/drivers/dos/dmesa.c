/*
 * Mesa 3-D graphics library
 * Version:  5.1
 * 
 * Copyright (C) 1999-2003  Brian Paul   All Rights Reserved.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * DOS/DJGPP device driver v1.5 for Mesa
 *
 *  Copyright (c) 2003 - Borca Daniel
 *  Email : dborca@users.sourceforge.net
 *  Web   : http://www.geocities.com/dborca
 */


#include "glheader.h"
#include "context.h"
#ifndef FX
#include "bufferobj.h"
#include "extensions.h"
#include "macros.h"
#include "matrix.h"
#include "mtypes.h"
#include "texformat.h"
#include "teximage.h"
#include "texstore.h"
#include "imports.h"
#include "array_cache/acache.h"
#include "swrast/s_context.h"
#include "swrast/s_depth.h"
#include "swrast/s_lines.h"
#include "swrast/s_triangle.h"
#include "swrast/swrast.h"
#include "swrast_setup/swrast_setup.h"
#include "tnl/tnl.h"
#include "tnl/t_context.h"
#include "tnl/t_pipeline.h"
#include "video.h"
#else  /* FX */
#include "GL/fxmesa.h"
#endif /* FX */

#include "GL/dmesa.h"



/*
 * In C++ terms, this class derives from the GLvisual class.
 * Add system-specific fields to it.
 */
struct dmesa_visual {
   GLvisual gl_visual;
   GLboolean db_flag;           /* double buffered? */
   GLboolean rgb_flag;          /* RGB mode? */
   GLboolean sw_alpha;          /* use Mesa's alpha buffer? */
   GLuint depth;                /* bits per pixel (1, 8, 24, etc) */
   int zbuffer;                 /* Z=buffer: 0=no, 1=SW, -1=HW */
};

/*
 * In C++ terms, this class derives from the GLframebuffer class.
 * Add system-specific fields to it.
 */
struct dmesa_buffer {
   GLframebuffer gl_buffer;     /* The depth, stencil, accum, etc buffers */
   void *the_window;            /* your window handle, etc */

   int xpos, ypos;              /* position */
   int width, height;           /* size in pixels */
};

/*
 * In C++ terms, this class derives from the GLcontext class.
 * Add system-specific fields to it.
 */
struct dmesa_context {
   GLcontext gl_ctx;            /* the core library context */
   DMesaVisual visual;
   DMesaBuffer Buffer;
   GLuint ClearColor;
   GLuint ClearIndex;
   /* etc... */
};



#ifndef FX
/****************************************************************************
 * Read/Write pixels
 ***************************************************************************/
#define FLIP(y)  (dmesa->Buffer->height - (y) - 1)
#define FLIP2(y) (_b_ - (y))

#define DSTRIDE dmesa->Buffer->width

/****************************************************************************
 * RGB[A]
 ***************************************************************************/
static void write_rgba_span (const GLcontext *ctx, GLuint n, GLint x, GLint y,
                             const GLubyte rgba[][4], const GLubyte mask[])
{
 const DMesaContext dmesa = (DMesaContext)ctx;
 GLuint i, offset;

 offset = DSTRIDE * FLIP(y) + x;
 if (mask) {
    /* draw some pixels */
    for (i=0; i<n; i++, offset++) {
        if (mask[i]) {
           vl_putpixel(offset, vl_mixrgba(rgba[i]));
        }
    }
 } else {
    /* draw all pixels */
    for (i=0; i<n; i++, offset++) {
        vl_putpixel(offset, vl_mixrgba(rgba[i]));
    }
 }
}



static void write_rgb_span (const GLcontext *ctx, GLuint n, GLint x, GLint y,
                            const GLubyte rgb[][3], const GLubyte mask[])
{
 const DMesaContext dmesa = (DMesaContext)ctx;
 GLuint i, offset;

 offset = DSTRIDE * FLIP(y) + x;
 if (mask) {
    /* draw some pixels */
    for (i=0; i<n; i++, offset++) {
        if (mask[i]) {
           vl_putpixel(offset, vl_mixrgb(rgb[i]));
        }
    }
 } else {
    /* draw all pixels */
    for (i=0; i<n; i++, offset++) {
        vl_putpixel(offset, vl_mixrgb(rgb[i]));
    }
 }
}



static void write_mono_rgba_span (const GLcontext *ctx,
                                  GLuint n, GLint x, GLint y,
                                  const GLchan color[4], const GLubyte mask[])
{
 const DMesaContext dmesa = (DMesaContext)ctx;
 GLuint i, offset, rgba = vl_mixrgba(color);

 offset = DSTRIDE * FLIP(y) + x;
 if (mask) {
    /* draw some pixels */
    for (i=0; i<n; i++, offset++) {
        if (mask[i]) {
           vl_putpixel(offset, rgba);
        }
    }
 } else {
    /* draw all pixels */
    for (i=0; i<n; i++, offset++) {
        vl_putpixel(offset, rgba);
    }
 }
}



static void read_rgba_span (const GLcontext *ctx, GLuint n, GLint x, GLint y,
                            GLubyte rgba[][4])
{
 const DMesaContext dmesa = (DMesaContext)ctx;
 GLuint i, offset;

 offset = DSTRIDE * FLIP(y) + x;
 /* read all pixels */
 for (i=0; i<n; i++, offset++) {
     vl_getrgba(offset, rgba[i]);
 }
}



static void write_rgba_pixels (const GLcontext *ctx,
                               GLuint n, const GLint x[], const GLint y[],
                               const GLubyte rgba[][4], const GLubyte mask[])
{
 const DMesaContext dmesa = (DMesaContext)ctx;
 GLuint i, _w_ = DSTRIDE, _b_ = dmesa->Buffer->height - 1;

 if (mask) {
    /* draw some pixels */
    for (i=0; i<n; i++) {
        if (mask[i]) {
           vl_putpixel(FLIP2(y[i])*_w_ + x[i], vl_mixrgba(rgba[i]));
        }
    }
 } else {
    /* draw all pixels */
    for (i=0; i<n; i++) {
        vl_putpixel(FLIP2(y[i])*_w_ + x[i], vl_mixrgba(rgba[i]));
    }
 }
}



static void write_mono_rgba_pixels (const GLcontext *ctx,
                                    GLuint n, const GLint x[], const GLint y[],
                                    const GLchan color[4], const GLubyte mask[])
{
 const DMesaContext dmesa = (DMesaContext)ctx;
 GLuint i, _w_ = DSTRIDE, _b_ = dmesa->Buffer->height - 1, rgba = vl_mixrgba(color);

 if (mask) {
    /* draw some pixels */
    for (i=0; i<n; i++) {
        if (mask[i]) {
           vl_putpixel(FLIP2(y[i])*_w_ + x[i], rgba);
        }
    }
 } else {
    /* draw all pixels */
    for (i=0; i<n; i++) {
        vl_putpixel(FLIP2(y[i])*_w_ + x[i], rgba);
    }
 }
}



static void read_rgba_pixels (const GLcontext *ctx,
                              GLuint n, const GLint x[], const GLint y[],
                              GLubyte rgba[][4], const GLubyte mask[])
{
 const DMesaContext dmesa = (DMesaContext)ctx;
 GLuint i, _w_ = DSTRIDE, _b_ = dmesa->Buffer->height - 1;

 if (mask) {
    /* read some pixels */
    for (i=0; i<n; i++) {
        if (mask[i]) {
           vl_getrgba(FLIP2(y[i])*_w_ + x[i], rgba[i]);
        }
    }
 } else {
    /* read all pixels */
    for (i=0; i<n; i++) {
        vl_getrgba(FLIP2(y[i])*_w_ + x[i], rgba[i]);
    }
 }
}



/****************************************************************************
 * Index
 ***************************************************************************/
static void write_index_span (const GLcontext *ctx, GLuint n, GLint x, GLint y,
                              const GLuint index[], const GLubyte mask[])
{
 const DMesaContext dmesa = (DMesaContext)ctx;
 GLuint i, offset;

 offset = DSTRIDE * FLIP(y) + x;
 if (mask) {
    /* draw some pixels */
    for (i=0; i<n; i++, offset++) {
        if (mask[i]) {
           vl_putpixel(offset, index[i]);
        }
    }
 } else {
    /* draw all pixels */
    for (i=0; i<n; i++, offset++) {
        vl_putpixel(offset, index[i]);
    }
 }
}



static void write_index8_span (const GLcontext *ctx, GLuint n, GLint x, GLint y,
                               const GLubyte index[], const GLubyte mask[])
{
 const DMesaContext dmesa = (DMesaContext)ctx;
 GLuint i, offset;

 offset = DSTRIDE * FLIP(y) + x;
 if (mask) {
    /* draw some pixels */
    for (i=0; i<n; i++, offset++) {
        if (mask[i]) {
           vl_putpixel(offset, index[i]);
        }
    }
 } else {
    /* draw all pixels */
    for (i=0; i<n; i++, offset++) {
        vl_putpixel(offset, index[i]);
    }
 }
}



static void write_mono_index_span (const GLcontext *ctx,
                                   GLuint n, GLint x, GLint y,
                                   GLuint colorIndex, const GLubyte mask[])
{
 const DMesaContext dmesa = (DMesaContext)ctx;
 GLuint i, offset;

 offset = DSTRIDE * FLIP(y) + x;
 if (mask) {
    /* draw some pixels */
    for (i=0; i<n; i++, offset++) {
        if (mask[i]) {
           vl_putpixel(offset, colorIndex);
        }
    }
 } else {
    /* draw all pixels */
    for (i=0; i<n; i++, offset++) {
        vl_putpixel(offset, colorIndex);
    }
 }
}



static void read_index_span (const GLcontext *ctx, GLuint n, GLint x, GLint y,
                             GLuint index[])
{
 const DMesaContext dmesa = (DMesaContext)ctx;
 GLuint i, offset;

 offset = DSTRIDE * FLIP(y) + x;
 /* read all pixels */
 for (i=0; i<n; i++, offset++) {
     index[i] = vl_getpixel(offset);
 }
}



static void write_index_pixels (const GLcontext *ctx,
                                GLuint n, const GLint x[], const GLint y[],
                                const GLuint index[], const GLubyte mask[])
{
 const DMesaContext dmesa = (DMesaContext)ctx;
 GLuint i, _w_ = DSTRIDE, _b_ = dmesa->Buffer->height - 1;

 if (mask) {
    /* draw some pixels */
    for (i=0; i<n; i++) {
        if (mask[i]) {
           vl_putpixel(FLIP2(y[i])*_w_ + x[i], index[i]);
        }
    }
 } else {
    /* draw all pixels */
    for (i=0; i<n; i++) {
        vl_putpixel(FLIP2(y[i])*_w_ + x[i], index[i]);
    }
 }
}



static void write_mono_index_pixels (const GLcontext *ctx,
                                     GLuint n, const GLint x[], const GLint y[],
                                     GLuint colorIndex, const GLubyte mask[])
{
 const DMesaContext dmesa = (DMesaContext)ctx;
 GLuint i, _w_ = DSTRIDE, _b_ = dmesa->Buffer->height - 1;

 if (mask) {
    /* draw some pixels */
    for (i=0; i<n; i++) {
        if (mask[i]) {
           vl_putpixel(FLIP2(y[i])*_w_ + x[i], colorIndex);
        }
    }
 } else {
    /* draw all pixels */
    for (i=0; i<n; i++) {
        vl_putpixel(FLIP2(y[i])*_w_ + x[i], colorIndex);
    }
 }
}



static void read_index_pixels (const GLcontext *ctx,
                               GLuint n, const GLint x[], const GLint y[],
                               GLuint index[], const GLubyte mask[])
{
 const DMesaContext dmesa = (DMesaContext)ctx;
 GLuint i, _w_ = DSTRIDE, _b_ = dmesa->Buffer->height - 1;

 if (mask) {
    /* read some pixels */
    for (i=0; i<n; i++) {
        if (mask[i]) {
           index[i] = vl_getpixel(FLIP2(y[i])*_w_ + x[i]);
        }
    }
 } else {
    /* read all pixels */
    for (i=0; i<n; i++) {
        index[i] = vl_getpixel(FLIP2(y[i])*_w_ + x[i]);
    }
 }
}



/****************************************************************************
 * Z-buffer
 ***************************************************************************/



/****************************************************************************
 * Optimized triangle rendering
 ***************************************************************************/

/*
 * NON-depth-buffered flat triangle.
 */
#define NAME tri_rgb_flat

#define SETUP_CODE \
 const DMesaContext dmesa = (DMesaContext)ctx; \
 GLuint _b_ = dmesa->Buffer->height - 1; \
 GLuint _w_ = dmesa->Buffer->width; \
 GLuint rgb = vl_mixrgb(v2->color);

#define RENDER_SPAN(span) \
 GLuint i, offset = FLIP2(span.y)*_w_ + span.x;	\
 for (i = 0; i < span.end; i++, offset++) {	\
     vl_putpixel(offset, rgb);			\
 }

#include "swrast/s_tritemp.h"



/*
 * Z-less flat triangle.
 */
#define NAME tri_rgb_flat_zless

#define INTERP_Z 1
#define DEPTH_TYPE DEFAULT_SOFTWARE_DEPTH_TYPE

#define SETUP_CODE \
 const DMesaContext dmesa = (DMesaContext)ctx; \
 GLuint _b_ = dmesa->Buffer->height - 1; \
 GLuint _w_ = dmesa->Buffer->width; \
 GLuint rgb = vl_mixrgb(v2->color);

#define RENDER_SPAN(span) \
 GLuint i, offset = FLIP2(span.y)*_w_ + span.x;	\
 for (i = 0; i < span.end; i++, offset++) {	\
     const DEPTH_TYPE z = FixedToDepth(span.z);	\
     if (z < zRow[i]) {				\
        vl_putpixel(offset, rgb);		\
        zRow[i] = z;				\
     }						\
     span.z += span.zStep;			\
 }

#include "swrast/s_tritemp.h"



/*
 * NON-depth-buffered iterated triangle.
 */
#define NAME tri_rgb_iter

#define INTERP_RGB 1

#define SETUP_CODE \
 const DMesaContext dmesa = (DMesaContext)ctx; \
 GLuint _b_ = dmesa->Buffer->height - 1; \
 GLuint _w_ = dmesa->Buffer->width;

#define RENDER_SPAN(span) \
 GLuint i, offset = FLIP2(span.y)*_w_ + span.x;				\
 for (i = 0; i < span.end; i++, offset++) {				\
     vl_putpixel(offset, vl_mixfix(span.red, span.green, span.blue));	\
     span.red += span.redStep;						\
     span.green += span.greenStep;					\
     span.blue += span.blueStep;					\
 }

#include "swrast/s_tritemp.h"



/*
 * Z-less iterated triangle.
 */
#define NAME tri_rgb_iter_zless

#define INTERP_Z 1
#define DEPTH_TYPE DEFAULT_SOFTWARE_DEPTH_TYPE
#define INTERP_RGB 1

#define SETUP_CODE \
 const DMesaContext dmesa = (DMesaContext)ctx; \
 GLuint _b_ = dmesa->Buffer->height - 1; \
 GLuint _w_ = dmesa->Buffer->width;

#define RENDER_SPAN(span) \
 GLuint i, offset = FLIP2(span.y)*_w_ + span.x;				\
 for (i = 0; i < span.end; i++, offset++) {				\
     const DEPTH_TYPE z = FixedToDepth(span.z);				\
     if (z < zRow[i]) {							\
        vl_putpixel(offset, vl_mixfix(span.red, span.green, span.blue));\
        zRow[i] = z;							\
     }									\
     span.red += span.redStep;						\
     span.green += span.greenStep;					\
     span.blue += span.blueStep;					\
     span.z += span.zStep;						\
 }

#include "swrast/s_tritemp.h"



/*
 * Analyze context state to see if we can provide a fast triangle function
 * Otherwise, return NULL.
 */
static swrast_tri_func dmesa_choose_tri_function (GLcontext *ctx)
{
 const SWcontext *swrast = SWRAST_CONTEXT(ctx);

 if ((ctx->RenderMode != GL_RENDER)
     || (ctx->Polygon.SmoothFlag)
     || (ctx->Polygon.StippleFlag)
     || (ctx->Texture._EnabledUnits)
     || (swrast->_RasterMask & MULTI_DRAW_BIT)
     || (ctx->Polygon.CullFlag && ctx->Polygon.CullFaceMode == GL_FRONT_AND_BACK)) {
    return (swrast_tri_func)NULL;
 }

 if (swrast->_RasterMask==DEPTH_BIT
     && ctx->Depth.Func==GL_LESS
     && ctx->Depth.Mask==GL_TRUE
     && ctx->Visual.depthBits == DEFAULT_SOFTWARE_DEPTH_BITS) {
    return (ctx->Light.ShadeModel==GL_SMOOTH) ? tri_rgb_iter_zless : tri_rgb_flat_zless;
 }

 if (swrast->_RasterMask==0) { /* no depth test */
    return (ctx->Light.ShadeModel==GL_SMOOTH) ? tri_rgb_iter : tri_rgb_flat;
 }

 return (swrast_tri_func)NULL;
}



/* Override for the swrast triangle-selection function.  Try to use one
 * of our internal triangle functions, otherwise fall back to the
 * standard swrast functions.
 */
static void dmesa_choose_tri (GLcontext *ctx)
{
 SWcontext *swrast = SWRAST_CONTEXT(ctx);

 if (!(swrast->Triangle=dmesa_choose_tri_function(ctx)))
    _swrast_choose_triangle(ctx);
}



/****************************************************************************
 * Optimized line rendering
 ***************************************************************************/

/*
 * NON-depth-buffered flat line.
 */
#define NAME line_rgb_flat

#define INTERP_XY 1
#define CLIP_HACK 1

#define SETUP_CODE \
 const DMesaContext dmesa = (DMesaContext)ctx; \
 GLuint _b_ = dmesa->Buffer->height - 1; \
 GLuint _w_ = dmesa->Buffer->width; \
 GLuint rgb = vl_mixrgb(vert1->color);

#define PLOT(X,Y) vl_putpixel(FLIP2(Y) * _w_ + X, rgb);

#include "swrast/s_linetemp.h"



/*
 * Z-less flat line.
 */
#define NAME line_rgb_flat_zless

#define INTERP_XY 1
#define INTERP_Z 1
#define DEPTH_TYPE DEFAULT_SOFTWARE_DEPTH_TYPE
#define CLIP_HACK 1

#define SETUP_CODE \
 const DMesaContext dmesa = (DMesaContext)ctx; \
 GLuint _b_ = dmesa->Buffer->height - 1; \
 GLuint _w_ = dmesa->Buffer->width; \
 GLuint rgb = vl_mixrgb(vert1->color);

#define PLOT(X,Y) \
 if (Z < *zPtr) {				\
    *zPtr = Z;					\
    vl_putpixel(FLIP2(Y) * _w_ + X, rgb);	\
 }

#include "swrast/s_linetemp.h"



/*
 * NON-depth-buffered iterated line.
 */
#define line_rgb_iter NULL



/*
 * Z-less iterated line.
 */
#define line_rgb_iter_zless NULL



/*
 * Analyze context state to see if we can provide a fast line function
 * Otherwise, return NULL.
 */
static swrast_line_func dmesa_choose_line_function (GLcontext *ctx)
{
 const SWcontext *swrast = SWRAST_CONTEXT(ctx);

 if ((ctx->RenderMode != GL_RENDER)
     || (ctx->Line.SmoothFlag)
     || (ctx->Texture._EnabledUnits)
     || (ctx->Line.StippleFlag)
     || (swrast->_RasterMask & MULTI_DRAW_BIT)
     || (ctx->Line.Width!=1.0F)) {
    return (swrast_line_func)NULL;
 }

 if (swrast->_RasterMask==DEPTH_BIT
     && ctx->Depth.Func==GL_LESS
     && ctx->Depth.Mask==GL_TRUE
     && ctx->Visual.depthBits == DEFAULT_SOFTWARE_DEPTH_BITS) {
    return (ctx->Light.ShadeModel==GL_SMOOTH) ? line_rgb_iter_zless : line_rgb_flat_zless;
 }

 if (swrast->_RasterMask==0) { /* no depth test */
    return (ctx->Light.ShadeModel==GL_SMOOTH) ? line_rgb_iter : line_rgb_flat;
 }

 return (swrast_line_func)NULL;
}



/* Override for the swrast line-selection function.  Try to use one
 * of our internal line functions, otherwise fall back to the
 * standard swrast functions.
 */
static void dmesa_choose_line (GLcontext *ctx)
{
 SWcontext *swrast = SWRAST_CONTEXT(ctx);

 if (!(swrast->Line=dmesa_choose_line_function(ctx)))
    _swrast_choose_line(ctx);
}



/****************************************************************************
 * Miscellaneous device driver funcs
 ***************************************************************************/

static void clear_index (GLcontext *ctx, GLuint index)
{
 ((DMesaContext)ctx)->ClearIndex = index;
}

static void clear_color (GLcontext *ctx, const GLfloat color[4])
{
 GLubyte col[4];
 CLAMPED_FLOAT_TO_UBYTE(col[0], color[0]);
 CLAMPED_FLOAT_TO_UBYTE(col[1], color[1]);
 CLAMPED_FLOAT_TO_UBYTE(col[2], color[2]);
 CLAMPED_FLOAT_TO_UBYTE(col[3], color[3]);
 ((DMesaContext)ctx)->ClearColor = vl_mixrgba(col);
}



static void clear (GLcontext *ctx, GLbitfield mask, GLboolean all,
                   GLint x, GLint y, GLint width, GLint height)
{
 const DMesaContext c = (DMesaContext)ctx;
 const GLuint *colorMask = (GLuint *)&ctx->Color.ColorMask;

/*
 * Clear the specified region of the buffers indicated by 'mask'
 * using the clear color or index as specified by one of the two
 * functions above.
 * If all==GL_TRUE, clear whole buffer, else just clear region defined
 * by x,y,width,height
 */

 /* we can't handle color or index masking */
 if ((*colorMask == 0xffffffff) && (ctx->Color.IndexMask == 0xffffffff)) {
    if (mask & DD_BACK_LEFT_BIT) {
       int color = c->visual->rgb_flag ? c->ClearColor : c->ClearIndex;

       if (all) {
          vl_clear(color);
       } else {
          vl_rect(x, y, width, height, color);
       }

       mask &= ~DD_BACK_LEFT_BIT;
    }
 }

 if (mask) {
    _swrast_Clear(ctx, mask, all, x, y, width, height);
 }
}



/*
 * This function is called to specify which buffer to read and write
 * for software rasterization (swrast) fallbacks.  This doesn't necessarily
 * correspond to glDrawBuffer() or glReadBuffer() calls.
 */
static void set_buffer (GLcontext *ctx, GLframebuffer *colorBuffer, GLuint bufferBit)
{
 /*
  * XXX todo - examine bufferBit and set read/write pointers
  */
 /* Normally, we would have
  *    ctx->Driver.ReadBuffer == set_read_buffer
  *    ctx->Driver.DrawBuffer == set_draw_buffer
  * and make sure set_draw_buffer calls _swrast_DrawBuffer,
  * which in turn will call this routine via dd->SetBuffer.
  */
}



/*
 * Return the width and height of the current buffer.
 * If anything special has to been done when the buffer/window is
 * resized, do it now.
 */
static void get_buffer_size (GLframebuffer *buffer, GLuint *width, GLuint *height)
{
 DMesaBuffer b = (DMesaBuffer)buffer;

 *width  = b->width;
 *height = b->height;
}



static const GLubyte* get_string (GLcontext *ctx, GLenum name)
{
 switch (name) {
        case GL_RENDERER:
             return (const GLubyte *)"Mesa DJGPP"
                                     "\0port (c) Borca Daniel dec-2003";
        default:
             return NULL;
 }
}



static void finish (GLcontext *ctx)
{
 /*
  * XXX todo - OPTIONAL FUNCTION: implements glFinish if possible
  */
}



static void flush (GLcontext *ctx)
{
 /*
  * XXX todo - OPTIONAL FUNCTION: implements glFlush if possible
  */
}



/****************************************************************************
 * State
 ***************************************************************************/
#define DMESA_NEW_LINE   (_NEW_LINE | \
                          _NEW_TEXTURE | \
                          _NEW_LIGHT | \
                          _NEW_DEPTH | \
                          _NEW_RENDERMODE | \
                          _SWRAST_NEW_RASTERMASK)

#define DMESA_NEW_TRIANGLE (_NEW_POLYGON | \
                            _NEW_TEXTURE | \
                            _NEW_LIGHT | \
                            _NEW_DEPTH | \
                            _NEW_RENDERMODE | \
                            _SWRAST_NEW_RASTERMASK)

/* Extend the software rasterizer with our line and triangle
 * functions.
 */
static void dmesa_register_swrast_functions (GLcontext *ctx)
{
 SWcontext *swrast = SWRAST_CONTEXT(ctx);

 swrast->choose_line = dmesa_choose_line;
 swrast->choose_triangle = dmesa_choose_tri;

 swrast->invalidate_line |= DMESA_NEW_LINE;
 swrast->invalidate_triangle |= DMESA_NEW_TRIANGLE;
}



/* Setup pointers and other driver state that is constant for the life
 * of a context.
 */
static void dmesa_init_pointers (GLcontext *ctx)
{
 TNLcontext *tnl;
 struct swrast_device_driver *dd = _swrast_GetDeviceDriverReference(ctx);

 ctx->Driver.GetString = get_string;
 ctx->Driver.GetBufferSize = get_buffer_size;
 ctx->Driver.Flush = flush;
 ctx->Driver.Finish = finish;
    
 /* Software rasterizer pixel paths:
  */
 ctx->Driver.Accum = _swrast_Accum;
 ctx->Driver.Bitmap = _swrast_Bitmap;
 ctx->Driver.Clear = clear;
 ctx->Driver.ResizeBuffers = _swrast_alloc_buffers;
 ctx->Driver.CopyPixels = _swrast_CopyPixels;
 ctx->Driver.DrawPixels = _swrast_DrawPixels;
 ctx->Driver.ReadPixels = _swrast_ReadPixels;
 ctx->Driver.DrawBuffer = _swrast_DrawBuffer;

 /* Software texture functions:
  */
 ctx->Driver.ChooseTextureFormat = _mesa_choose_tex_format;
 ctx->Driver.TexImage1D = _mesa_store_teximage1d;
 ctx->Driver.TexImage2D = _mesa_store_teximage2d;
 ctx->Driver.TexImage3D = _mesa_store_teximage3d;
 ctx->Driver.TexSubImage1D = _mesa_store_texsubimage1d;
 ctx->Driver.TexSubImage2D = _mesa_store_texsubimage2d;
 ctx->Driver.TexSubImage3D = _mesa_store_texsubimage3d;
 ctx->Driver.TestProxyTexImage = _mesa_test_proxy_teximage;

 ctx->Driver.CopyTexImage1D = _swrast_copy_teximage1d;
 ctx->Driver.CopyTexImage2D = _swrast_copy_teximage2d;
 ctx->Driver.CopyTexSubImage1D = _swrast_copy_texsubimage1d;
 ctx->Driver.CopyTexSubImage2D = _swrast_copy_texsubimage2d;
 ctx->Driver.CopyTexSubImage3D = _swrast_copy_texsubimage3d;

 ctx->Driver.CompressedTexImage1D = _mesa_store_compressed_teximage1d;
 ctx->Driver.CompressedTexImage2D = _mesa_store_compressed_teximage2d;
 ctx->Driver.CompressedTexImage3D = _mesa_store_compressed_teximage3d;
 ctx->Driver.CompressedTexSubImage1D = _mesa_store_compressed_texsubimage1d;
 ctx->Driver.CompressedTexSubImage2D = _mesa_store_compressed_texsubimage2d;
 ctx->Driver.CompressedTexSubImage3D = _mesa_store_compressed_texsubimage3d;

 /* Swrast hooks for imaging extensions:
  */
 ctx->Driver.CopyColorTable = _swrast_CopyColorTable;
 ctx->Driver.CopyColorSubTable = _swrast_CopyColorSubTable;
 ctx->Driver.CopyConvolutionFilter1D = _swrast_CopyConvolutionFilter1D;
 ctx->Driver.CopyConvolutionFilter2D = _swrast_CopyConvolutionFilter2D;

 /* Statechange callbacks:
  */
 ctx->Driver.ClearColor = clear_color;
 ctx->Driver.ClearIndex = clear_index;

 /* Initialize the TNL driver interface:
  */
 tnl = TNL_CONTEXT(ctx);
 tnl->Driver.RunPipeline = _tnl_run_pipeline;

 dd->SetBuffer = set_buffer;
   
 /* Install swsetup for tnl->Driver.Render.*:
  */
 _swsetup_Wakeup(ctx);

 /* The span functions should be in `dmesa_update_state', but I'm
  * pretty sure they will never change during the life of the Visual
  */

 /* Index span/pixel functions */
 dd->WriteCI32Span = write_index_span;
 dd->WriteCI8Span = write_index8_span;
 dd->WriteMonoCISpan = write_mono_index_span;
 dd->WriteCI32Pixels = write_index_pixels;
 dd->WriteMonoCIPixels = write_mono_index_pixels;
 dd->ReadCI32Span = read_index_span;
 dd->ReadCI32Pixels = read_index_pixels;

 /* RGB(A) span/pixel functions */
 dd->WriteRGBASpan = write_rgba_span;
 dd->WriteRGBSpan = write_rgb_span;
 dd->WriteMonoRGBASpan = write_mono_rgba_span;
 dd->WriteRGBAPixels = write_rgba_pixels;
 dd->WriteMonoRGBAPixels = write_mono_rgba_pixels;
 dd->ReadRGBASpan = read_rgba_span;
 dd->ReadRGBAPixels = read_rgba_pixels;
}



static void dmesa_update_state (GLcontext *ctx, GLuint new_state)
{
 /* Propagate statechange information to swrast and swrast_setup
  * modules. The DMesa driver has no internal GL-dependent state.
  */
 _swrast_InvalidateState( ctx, new_state );
 _ac_InvalidateState( ctx, new_state );
 _tnl_InvalidateState( ctx, new_state );
 _swsetup_InvalidateState( ctx, new_state );
}
#endif /* FX */



/****************************************************************************
 * DMesa Public API Functions
 ***************************************************************************/

/*
 * The exact arguments to this function will depend on your window system
 */
DMesaVisual DMesaCreateVisual (GLint width,
                               GLint height,
                               GLint colDepth,
                               GLint refresh,
                               GLboolean dbFlag,
                               GLboolean rgbFlag,
                               GLint alphaSize,
                               GLint depthSize,
                               GLint stencilSize,
                               GLint accumSize)
{
#ifndef FX
 DMesaVisual v;
 GLint redBits, greenBits, blueBits, alphaBits, indexBits;
 GLboolean sw_alpha;

 if (!dbFlag) {
    return NULL;
 }

 alphaBits = 0;

 if (!rgbFlag) {
    indexBits = 8;
    redBits = 0;
    greenBits = 0;
    blueBits = 0;
 } else {
    indexBits = 0;
    switch (colDepth) {
           case 8:
                redBits = 8;
                greenBits = 8;
                blueBits = 8;
                break;
           case 15:
                alphaBits = 1;
                redBits = 5;
                greenBits = 5;
                blueBits = 5;
                break;
           case 16:
                redBits = 5;
                greenBits = 6;
                blueBits = 5;
                break;
           case 32:
                alphaBits = 8;
           case 24:
                redBits = 8;
                greenBits = 8;
                blueBits = 8;
                break;
           default:
                return NULL;
    }
 }

 /* Okay,
  * `alphaBits' is what we can provide
  * `alphaSize' is what app requests
  *
  * Note that alpha buffering is required only if destination alpha is used
  * in alpha blending; alpha blending modes that do not use destination alpha
  * can be used w/o alpha buffer.
  *
  * We will use whatever ALPHA app requests. Later, in `CreateBuffer' we'll
  * instruct Mesa to use its own ALPHA buffer, by passing a non-FALSE value
  * for ALPHA to `_mesa_initialize_framebuffer'.
  *
  * Basically, 32bit modes provide ALPHA storage, but can we rely on this?
  */
 alphaBits = alphaSize;
 sw_alpha = (alphaBits > 0);

 if ((colDepth=vl_video_init(width, height, colDepth, rgbFlag, refresh)) <= 0) {
    return NULL;
 }

 if ((v=(DMesaVisual)CALLOC_STRUCT(dmesa_visual)) != NULL) {
    /* Create core visual */
    _mesa_initialize_visual((GLvisual *)v,
                            rgbFlag,		/* rgb */
                            dbFlag,
                            GL_FALSE,		/* stereo */
                            redBits,
                            greenBits,
                            blueBits,
                            alphaBits,
                            indexBits,		/* indexBits */
                            depthSize,
                            stencilSize,
                            accumSize,		/* accumRed */
                            accumSize,		/* accumGreen */
                            accumSize,		/* accumBlue */
                            alphaBits?accumSize:0,	/* accumAlpha */
                            1);			/* numSamples */

    v->depth = colDepth;
    v->db_flag = dbFlag;
    v->rgb_flag = rgbFlag;
    v->sw_alpha = sw_alpha;

    v->zbuffer = (depthSize > 0) ? 1 : 0;
 }

 return v;

#else  /* FX */

 char *env;
 int i = 0, fx_attrib[32];

 if (!rgbFlag) {
    return NULL;
 }

 if (dbFlag) fx_attrib[i++] = FXMESA_DOUBLEBUFFER;
 if (depthSize > 0) { fx_attrib[i++] = FXMESA_DEPTH_SIZE; fx_attrib[i++] = depthSize; }
 if (stencilSize > 0) { fx_attrib[i++] = FXMESA_STENCIL_SIZE; fx_attrib[i++] = stencilSize; }
 if (accumSize > 0) { fx_attrib[i++] = FXMESA_ACCUM_SIZE; fx_attrib[i++] = accumSize; }
 if (alphaSize) { fx_attrib[i++] = FXMESA_ALPHA_SIZE; fx_attrib[i++] = alphaSize; }
 fx_attrib[i++] = FXMESA_COLORDEPTH;
 fx_attrib[i++] = colDepth;
 fx_attrib[i] = FXMESA_NONE;

 if ((env = getenv("MESA_FX_INFO")) && (env[0] == 'r')) {
    freopen("MESA.LOG", "w", stderr);
 }

 if (refresh && (((env = getenv("FX_GLIDE_REFRESH")) == NULL) || !atoi(env))) {
    /* if we are passed non-zero value for refresh, we need to override
     * default refresh rate. However, if FX_GLIDE_REFRESH is already set
     * to 0, we won't override it, because it has a special meaning for
     * DJGPP Glide3x (switch via VESA, using BIOS default refresh).
     */
    char tmp[256];
    sprintf(tmp, "FX_GLIDE_REFRESH=%u", refresh);
    putenv(tmp);
 }

 return (DMesaVisual)fxMesaCreateBestContext(-1, width, height, fx_attrib);
#endif /* FX */
}



void DMesaDestroyVisual (DMesaVisual v)
{
#ifndef FX
 _mesa_destroy_visual((GLvisual *)v);

 vl_video_exit();

#else
 fxMesaDestroyContext((fxMesaContext)v);
#endif
}



DMesaBuffer DMesaCreateBuffer (DMesaVisual visual,
                               GLint xpos, GLint ypos,
                               GLint width, GLint height)
{
#ifndef FX
 DMesaBuffer b;

 if ((b=(DMesaBuffer)CALLOC_STRUCT(dmesa_buffer)) != NULL) {
    _mesa_initialize_framebuffer((GLframebuffer *)b,
                                 (GLvisual *)visual,
                                 visual->zbuffer == 1,
                                 ((GLvisual *)visual)->stencilBits > 0,
                                 ((GLvisual *)visual)->accumRedBits > 0,
                                 visual->sw_alpha);
    b->xpos = xpos;
    b->ypos = ypos;
    b->width = width;
    b->height = height;
 }

 return b;
#else
 return (DMesaBuffer)visual;
#endif
}



void DMesaDestroyBuffer (DMesaBuffer b)
{
#ifndef FX
 free(b->the_window);
 _mesa_destroy_framebuffer((GLframebuffer *)b);
#endif
}



DMesaContext DMesaCreateContext (DMesaVisual visual,
                                 DMesaContext share)
{
#ifndef FX
 DMesaContext c;
 GLboolean direct = GL_FALSE;

 if ((c=(DMesaContext)CALLOC_STRUCT(dmesa_context)) != NULL) {
    _mesa_initialize_context((GLcontext *)c,
                             (GLvisual *)visual,
                             (GLcontext *)share,
                             (void *)c, direct);

    _mesa_enable_sw_extensions((GLcontext *)c);
    _mesa_enable_1_3_extensions((GLcontext *)c);
    _mesa_enable_1_4_extensions((GLcontext *)c);
    _mesa_enable_1_5_extensions((GLcontext *)c);

    /* you probably have to do a bunch of other initializations here. */
    c->visual = visual;

    ((GLcontext *)c)->Driver.UpdateState = dmesa_update_state;

    /* Initialize the software rasterizer and helper modules.
     */
    _swrast_CreateContext((GLcontext *)c);
    _ac_CreateContext((GLcontext *)c);
    _tnl_CreateContext((GLcontext *)c);
    _swsetup_CreateContext((GLcontext *)c);
    if (visual->rgb_flag) dmesa_register_swrast_functions((GLcontext *)c);
    dmesa_init_pointers((GLcontext *)c);
 }

 return c;

#else  /* FX */
 return (DMesaContext)visual;
#endif /* FX */
}



void DMesaDestroyContext (DMesaContext c)
{
#ifndef FX
 if (c) {
    _swsetup_DestroyContext((GLcontext *)c);
    _swrast_DestroyContext((GLcontext *)c);
    _tnl_DestroyContext((GLcontext *)c);
    _ac_DestroyContext((GLcontext *)c);
    _mesa_destroy_context((GLcontext *)c);
 }
#endif
}



GLboolean DMesaMoveBuffer (GLint xpos, GLint ypos)
{
#ifndef FX
 GET_CURRENT_CONTEXT(ctx);
 DMesaBuffer b = ((DMesaContext)ctx)->Buffer;

 if (vl_sync_buffer(&b->the_window, xpos, ypos, b->width, b->height) != 0) {
    return GL_FALSE;
 } else {
    b->xpos = xpos;
    b->ypos = ypos;
    return GL_TRUE;
 }

#else
 return GL_FALSE;
#endif
}



GLboolean DMesaResizeBuffer (GLint width, GLint height)
{
#ifndef FX
 GET_CURRENT_CONTEXT(ctx);
 DMesaBuffer b = ((DMesaContext)ctx)->Buffer;

 if (vl_sync_buffer(&b->the_window, b->xpos, b->ypos, width, height) != 0) {
    return GL_FALSE;
 } else {
    b->width = width;
    b->height = height;
    return GL_TRUE;
 }

#else
 return GL_FALSE;
#endif
}



/*
 * Make the specified context and buffer the current one.
 */
GLboolean DMesaMakeCurrent (DMesaContext c, DMesaBuffer b)
{
#ifndef FX
 if ((c != NULL) && (b != NULL)) {
    if (vl_sync_buffer(&b->the_window, b->xpos, b->ypos, b->width, b->height) != 0) {
       return GL_FALSE;
    }

    c->Buffer = b;

    _mesa_make_current((GLcontext *)c, (GLframebuffer *)b);
    if (((GLcontext *)c)->Viewport.Width == 0) {
       /* initialize viewport to window size */
       _mesa_Viewport(0, 0, b->width, b->height);
    }
 } else {
    /* Detach */
    _mesa_make_current(NULL, NULL);
 }

#else
 fxMesaMakeCurrent((fxMesaContext)c);
#endif

 return GL_TRUE;
}



void DMesaSwapBuffers (DMesaBuffer b)
{
 /* copy/swap back buffer to front if applicable */
#ifndef FX
 GET_CURRENT_CONTEXT(ctx);
 _mesa_notifySwapBuffers(ctx);
 vl_flip();
#else
 fxMesaSwapBuffers();
#endif
}



void DMesaSetCI (int ndx, GLfloat red, GLfloat green, GLfloat blue)
{
#ifndef FX
 vl_setCI(ndx, red, green, blue);
#endif
}



void *DMesaGetCurrentContext (void)
{
#ifndef FX
 GET_CURRENT_CONTEXT(ctx);
 return ctx;
#else
 return fxMesaGetCurrentContext();
#endif
}



DMesaProc
DMesaGetProcAddress (const char *name)
{
   DMesaProc p = (DMesaProc)_glapi_get_proc_address(name);
   return p;
}



int DMesaGetIntegerv (GLenum pname, GLint *params)
{
 switch (pname) {
        case DMESA_GET_SCREEN_SIZE:
             #ifndef FX
             vl_get(VL_GET_SCREEN_SIZE, params);
             #else
             fxGetScreenGeometry(&params[0], &params[1]);
             #endif
             break;
        case DMESA_GET_DRIVER_CAPS:
             #ifndef FX
             params[0] = DMESA_DRIVER_SWDB_BIT;
             #else
             params[0] = DMESA_DRIVER_LLWO_BIT;
             #endif
             break;
        case DMESA_GET_VIDEO_MODES:
             #ifndef FX
             return vl_get(VL_GET_VIDEO_MODES, params);
             #else
             {
              /* `vmode' struct must be sync'ed with `internal.h' (vl_mode)
               * `vmode' list must be sync'ed with `fxapi.c'
               * `hw >= 6' means Napalm and can do 32bit rendering
               * TODO: we should take an envvar for `fxMesaSelectCurrentBoard'
               */
              static struct {
                     int width, height;
                     int bpp;
              } vmode[] = {
                     { 320,  200, 16 },
                     { 320,  240, 16 },
                     { 512,  384, 16 },
                     { 640,  400, 16 },
                     { 640,  480, 16 },
                     { 800,  600, 16 },
                     {1024,  768, 16 },

                     { 320,  200, 32 },
                     { 320,  240, 32 },
                     { 512,  384, 32 },
                     { 640,  400, 32 },
                     { 640,  480, 32 },
                     { 800,  600, 32 },
                     {1024,  768, 32 }
              };
              int hw = fxMesaSelectCurrentBoard(0);
              int i, n = sizeof(vmode) / sizeof(vmode[0]);
              if (hw < 6) {
                 n /= 2;
              }
              if (params) {
                 for (i = 0; i < n; i++) {
                     params[i] = (GLint)(&vmode[i]);
                 }
              }
              return n;
             }
             #endif
        default:
             return -1;
 }

 return 0;
}
