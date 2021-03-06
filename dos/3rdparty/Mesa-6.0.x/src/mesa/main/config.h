/**
 * \file config.h
 * Tunable configuration parameters.
 */

/*
 * Mesa 3-D graphics library
 * Version:  6.0
 *
 * Copyright (C) 1999-2004  Brian Paul   All Rights Reserved.
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


#ifndef CONFIG_H
#define CONFIG_H

#ifdef HAVE_CONFIG_H
#include "conf.h"
#endif


/**
 * \name OpenGL implementation limits
 */
/*@{*/

/** Maximum modelview matrix stack depth */
#define MAX_MODELVIEW_STACK_DEPTH 32

/** Maximum projection matrix stack depth */
#define MAX_PROJECTION_STACK_DEPTH 32

/** Maximum texture matrix stack depth */
#define MAX_TEXTURE_STACK_DEPTH 10

/** Maximum color matrix stack depth */
#define MAX_COLOR_STACK_DEPTH 4

/** Maximum attribute stack depth */
#define MAX_ATTRIB_STACK_DEPTH 16

/** Maximum client attribute stack depth */
#define MAX_CLIENT_ATTRIB_STACK_DEPTH 16

/** Maximum recursion depth of display list calls */
#define MAX_LIST_NESTING 64

/** Maximum number of lights */
#define MAX_LIGHTS 8

/** Maximum user-defined clipping planes */
#define MAX_CLIP_PLANES 6

/** Maximum pixel map lookup table size */
#define MAX_PIXEL_MAP_TABLE 256

/** Number of auxillary color buffers */
#define NUM_AUX_BUFFERS 0

/** Maximum order (degree) of curves */
#ifdef AMIGA
#   define MAX_EVAL_ORDER 12
#else
#   define MAX_EVAL_ORDER 30
#endif

/** Maximum Name stack depth */
#define MAX_NAME_STACK_DEPTH 64

/** Minimum point size */
#define MIN_POINT_SIZE 1.0
/** Maximum point size */
#define MAX_POINT_SIZE 20.0
/** Point size granularity */
#define POINT_SIZE_GRANULARITY 0.1

/** Minimum line width */
#define MIN_LINE_WIDTH 1.0
/** Maximum line width */
#define MAX_LINE_WIDTH 10.0
/** Line width granularity */
#define LINE_WIDTH_GRANULARITY 0.1

/** Max texture palette / color table size */
#define MAX_COLOR_TABLE_SIZE 256

/** Number of 1D/2D texture mipmap levels */
#define MAX_TEXTURE_LEVELS 12

/** Number of 3D texture mipmap levels */
#define MAX_3D_TEXTURE_LEVELS 9

/** Number of cube texture mipmap levels - GL_ARB_texture_cube_map */
#define MAX_CUBE_TEXTURE_LEVELS 12

/** Maximum rectangular texture size - GL_NV_texture_rectangle */
#define MAX_TEXTURE_RECT_SIZE 2048

/** Number of texture units - GL_ARB_multitexture */
#define MAX_TEXTURE_UNITS 8

/**
 * \name Separate numbers of texture coordinates and texture image units.
 *
 * These values will eventually replace most instances of MAX_TEXTURE_UNITS.
 * We should always have MAX_TEXTURE_COORD_UNITS <= MAX_TEXTURE_IMAGE_UNITS.
 * And, GL_MAX_TEXTURE_UNITS <= MAX_TEXTURE_COORD_UNITS.
 */
/*@{*/
#define MAX_TEXTURE_COORD_UNITS 8
#define MAX_TEXTURE_IMAGE_UNITS 8
/*@}*/

/** 
 * Maximum viewport/image width. Must accomodate all texture sizes too. 
 */
#define MAX_WIDTH 4096
/** Maximum viewport/image height */
#define MAX_HEIGHT 4096

/** Maxmimum size for CVA.  May be overridden by the drivers.  */
#define MAX_ARRAY_LOCK_SIZE 3000

/** Subpixel precision for antialiasing, window coordinate snapping */
#define SUB_PIXEL_BITS 4

/** Size of histogram tables */
#define HISTOGRAM_TABLE_SIZE 256

/** Max convolution filter width */
#define MAX_CONVOLUTION_WIDTH 9
/** Max convolution filter height */
#define MAX_CONVOLUTION_HEIGHT 9

/** GL_ARB_texture_compression */
#define MAX_COMPRESSED_TEXTURE_FORMATS 25

/** GL_EXT_texture_filter_anisotropic */
#define MAX_TEXTURE_MAX_ANISOTROPY 16.0

/** GL_EXT_texture_lod_bias */
#define MAX_TEXTURE_LOD_BIAS 4.0

/* GL_NV_vertex_program */
#define MAX_NV_VERTEX_PROGRAM_INSTRUCTIONS 128
#define MAX_NV_VERTEX_PROGRAM_TEMPS         12
#define MAX_NV_VERTEX_PROGRAM_PARAMS        96
#define MAX_NV_VERTEX_PROGRAM_INPUTS        16
#define MAX_NV_VERTEX_PROGRAM_OUTPUTS       15

/* GL_NV_fragment_program */
#define MAX_NV_FRAGMENT_PROGRAM_INSTRUCTIONS 128
#define MAX_NV_FRAGMENT_PROGRAM_TEMPS         96
#define MAX_NV_FRAGMENT_PROGRAM_PARAMS        64
#define MAX_NV_FRAGMENT_PROGRAM_INPUTS        12
#define MAX_NV_FRAGMENT_PROGRAM_OUTPUTS        3
#define MAX_NV_FRAGMENT_PROGRAM_WRITE_ONLYS    2

/* GL_ARB_vertex_program */
#define MAX_VERTEX_PROGRAM_ADDRESS_REGS 1
#define MAX_VERTEX_PROGRAM_ATTRIBS     16

/* GL_ARB_fragment_program */
#define MAX_FRAGMENT_PROGRAM_ADDRESS_REGS 1
#define MAX_FRAGMENT_PROGRAM_ALU_INSTRUCTIONS 48
#define MAX_FRAGMENT_PROGRAM_TEX_INSTRUCTIONS 24
#define MAX_FRAGMENT_PROGRAM_TEX_INDIRECTIONS  4

/* Any program target/extension */
#define MAX_PROGRAM_LOCAL_PARAMS 96
#define MAX_PROGRAM_MATRICES 8
#define MAX_PROGRAM_MATRIX_STACK_DEPTH 4

/*@}*/


/**
 * \name Mesa-specific parameters
 */
/*@{*/


/**
 * Bits per accumulation buffer color component:  8, 16 or 32
 */
#define ACCUM_BITS 16

/**
 * Bits per depth buffer value.  
 *
 * Any reasonable value up to 31 will work.  32 doesn't work because of integer
 * overflow problems in the rasterizer code.
 */
#ifndef DEFAULT_SOFTWARE_DEPTH_BITS
#define DEFAULT_SOFTWARE_DEPTH_BITS 16
#endif
/** Depth buffer data type */
#if DEFAULT_SOFTWARE_DEPTH_BITS <= 16
#define DEFAULT_SOFTWARE_DEPTH_TYPE GLushort
#else
#define DEFAULT_SOFTWARE_DEPTH_TYPE GLuint
#endif


/**
 * Bits per stencil value: 8
 */
#define STENCIL_BITS 8


/**
 * Bits per color channel:  8, 16 or 32
 */
#ifndef CHAN_BITS
#define CHAN_BITS 8
#endif


/*
 * Color channel component order
 * 
 * \note Changes will almost certainly cause problems at this time.
 */
#define RCOMP 0
#define GCOMP 1
#define BCOMP 2
#define ACOMP 3


/*
 * Enable/disable features (blocks of code) by setting FEATURE_xyz to 0 or 1.
 */
#ifndef _HAVE_FULL_GL
#define _HAVE_FULL_GL 1
#endif

#define FEATURE_ARB_vertex_buffer_object  _HAVE_FULL_GL
#define FEATURE_ARB_vertex_program  _HAVE_FULL_GL
#define FEATURE_ARB_fragment_program  _HAVE_FULL_GL
#define FEATURE_ARB_occlusion_query  _HAVE_FULL_GL
#define FEATURE_MESA_program_debug  _HAVE_FULL_GL
#define FEATURE_NV_fence  _HAVE_FULL_GL
#define FEATURE_NV_fragment_program  _HAVE_FULL_GL
#define FEATURE_NV_vertex_program  _HAVE_FULL_GL
#define FEATURE_userclip  _HAVE_FULL_GL
#define FEATURE_texgen  _HAVE_FULL_GL
#define FEATURE_windowpos  _HAVE_FULL_GL

/*@}*/


#endif /* CONFIG_H */
