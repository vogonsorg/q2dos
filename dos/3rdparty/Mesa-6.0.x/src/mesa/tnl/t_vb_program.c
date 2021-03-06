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


/**
 * \file tnl/t_vb_program.c
 * \brief Pipeline stage for executing NVIDIA vertex programs.
 * \author Brian Paul,  Keith Whitwell
 */


#include "glheader.h"
#include "api_noop.h"
#include "colormac.h"
#include "context.h"
#include "dlist.h"
#include "hash.h"
#include "light.h"
#include "macros.h"
#include "imports.h"
#include "simple_list.h"
#include "mtypes.h"
#include "nvvertprog.h"
#include "nvvertexec.h"
#include "nvprogram.h"

#include "math/m_translate.h"

#include "t_context.h"
#include "t_pipeline.h"


/**
 * \warning These values _MUST_ match the values in the OutputRegisters[]
 * array in vpparse.c!!!
 */
#define VERT_RESULT_HPOS 0
#define VERT_RESULT_COL0 1
#define VERT_RESULT_COL1 2
#define VERT_RESULT_BFC0 3
#define VERT_RESULT_BFC1 4
#define VERT_RESULT_FOGC 5
#define VERT_RESULT_PSIZ 6
#define VERT_RESULT_TEX0 7
#define VERT_RESULT_TEX1 8
#define VERT_RESULT_TEX2 9
#define VERT_RESULT_TEX3 10
#define VERT_RESULT_TEX4 11
#define VERT_RESULT_TEX5 12
#define VERT_RESULT_TEX6 13
#define VERT_RESULT_TEX7 14


/*!
 * Private storage for the vertex program pipeline stage.
 */
struct vp_stage_data {
   /** The results of running the vertex program go into these arrays. */
   GLvector4f attribs[15];

   GLvector4f ndcCoords;              /**< normalized device coords */
   GLubyte *clipmask;                 /**< clip flags */
   GLubyte ormask, andmask;           /**< for clipping */
};


#define VP_STAGE_DATA(stage) ((struct vp_stage_data *)(stage->privatePtr))


/**
 * This function executes vertex programs
 */
static GLboolean run_vp( GLcontext *ctx, struct tnl_pipeline_stage *stage )
{
   TNLcontext *tnl = TNL_CONTEXT(ctx);
   struct vp_stage_data *store = VP_STAGE_DATA(stage);
   struct vertex_buffer *VB = &tnl->vb;
   struct vertex_program *program = ctx->VertexProgram.Current;
   GLuint i;

   _mesa_init_tracked_matrices(ctx); /* load registers with matrices */
   _mesa_init_vp_registers(ctx);     /* init temp and result regs */

   for (i = 0; i < VB->Count; i++) {
      GLuint attr;

#if 0
      printf("Input  %d: %f, %f, %f, %f\n", i,
             VB->AttribPtr[0]->data[i][0],
             VB->AttribPtr[0]->data[i][1],
             VB->AttribPtr[0]->data[i][2],
             VB->AttribPtr[0]->data[i][3]);
      printf("   color: %f, %f, %f, %f\n",
             VB->AttribPtr[3]->data[i][0],
             VB->AttribPtr[3]->data[i][1],
             VB->AttribPtr[3]->data[i][2],
             VB->AttribPtr[3]->data[i][3]);
      printf("  normal: %f, %f, %f, %f\n",
             VB->AttribPtr[2]->data[i][0],
             VB->AttribPtr[2]->data[i][1],
             VB->AttribPtr[2]->data[i][2],
             VB->AttribPtr[2]->data[i][3]);
#endif

      /* the vertex array case */
      for (attr = 0; attr < VERT_ATTRIB_MAX; attr++) {
	 if (program->InputsRead & (1 << attr)) {
	    const GLubyte *ptr = (const GLubyte*) VB->AttribPtr[attr]->data;
	    const GLuint size = VB->AttribPtr[attr]->size;
	    const GLuint stride = VB->AttribPtr[attr]->stride;
	    const GLfloat *data = (GLfloat *) (ptr + stride * i);
	    ASSIGN_4V(ctx->VertexProgram.Inputs[attr], 0, 0, 0, 1);
	    COPY_SZ_4V(ctx->VertexProgram.Inputs[attr], size, data);
	 }
      }

      /* execute the program */
      ASSERT(program);
      _mesa_exec_vertex_program(ctx, program);

      /* Fixup fog an point size results if needed */
      if (ctx->Fog.Enabled &&
          (program->OutputsWritten & (1 << VERT_RESULT_FOGC)) == 0) {
         ctx->VertexProgram.Outputs[VERT_RESULT_FOGC][0] = 1.0;
      }

      if (ctx->VertexProgram.PointSizeEnabled &&
          (program->OutputsWritten & (1 << VERT_RESULT_PSIZ)) == 0) {
         ctx->VertexProgram.Outputs[VERT_RESULT_PSIZ][0] = ctx->Point.Size;
      }

      /* copy the output registers into the VB->attribs arrays */
      /* XXX (optimize) could use a conditional and smaller loop limit here */
      for (attr = 0; attr < 15; attr++) {
         COPY_4V(store->attribs[attr].data[i],
                 ctx->VertexProgram.Outputs[attr]);
      }
   }

   /* Setup the VB pointers so that the next pipeline stages get
    * their data from the right place (the program output arrays).
    */
   VB->ClipPtr = &store->attribs[VERT_RESULT_HPOS];
   VB->ClipPtr->size = 4;
   VB->ClipPtr->count = VB->Count;
   VB->ColorPtr[0] = &store->attribs[VERT_RESULT_COL0];
   VB->ColorPtr[1] = &store->attribs[VERT_RESULT_BFC0];
   VB->SecondaryColorPtr[0] = &store->attribs[VERT_RESULT_COL1];
   VB->SecondaryColorPtr[1] = &store->attribs[VERT_RESULT_BFC1];
   VB->FogCoordPtr = &store->attribs[VERT_RESULT_FOGC];
   VB->PointSizePtr = &store->attribs[VERT_RESULT_PSIZ];

   VB->AttribPtr[VERT_ATTRIB_COLOR0] = VB->ColorPtr[0];
   VB->AttribPtr[VERT_ATTRIB_COLOR1] = VB->SecondaryColorPtr[0];
   VB->AttribPtr[VERT_ATTRIB_FOG] = VB->FogCoordPtr;
   VB->AttribPtr[_TNL_ATTRIB_POINTSIZE] = &store->attribs[VERT_RESULT_PSIZ];

   for (i = 0; i < ctx->Const.MaxTextureUnits; i++) {
      VB->AttribPtr[VERT_ATTRIB_TEX0+i] = VB->TexCoordPtr[i] = 
	 &store->attribs[VERT_RESULT_TEX0 + i];
   }



   /* Cliptest and perspective divide.  Clip functions must clear
    * the clipmask.
    */
   store->ormask = 0;
   store->andmask = CLIP_ALL_BITS;

   if (tnl->NeedNdcCoords) {
      VB->NdcPtr =
         _mesa_clip_tab[VB->ClipPtr->size]( VB->ClipPtr,
                                            &store->ndcCoords,
                                            store->clipmask,
                                            &store->ormask,
                                            &store->andmask );
   }
   else {
      VB->NdcPtr = 0;
      _mesa_clip_np_tab[VB->ClipPtr->size]( VB->ClipPtr,
                                            0,
                                            store->clipmask,
                                            &store->ormask,
                                            &store->andmask );
   }

   if (store->andmask)  /* All vertices are outside the frustum */
      return GL_FALSE;


   /* This is where we'd do clip testing against the user-defined
    * clipping planes, but they're not supported by vertex programs.
    */

   VB->ClipOrMask = store->ormask;
   VB->ClipMask = store->clipmask;

   return GL_TRUE;
}


/**
 * This function validates stuff.
 */
static GLboolean run_validate_program( GLcontext *ctx,
					struct tnl_pipeline_stage *stage )
{
#if 000
   /* XXX do we need any validation for vertex programs? */
   GLuint ind = 0;
   light_func *tab;

   if (ctx->Visual.rgbMode) {
      if (ctx->Light._NeedVertices) {
	 if (ctx->Light.Model.ColorControl == GL_SEPARATE_SPECULAR_COLOR)
	    tab = _tnl_light_spec_tab;
	 else
	    tab = _tnl_light_tab;
      }
      else {
	 if (ctx->Light.EnabledList.next == ctx->Light.EnabledList.prev)
	    tab = _tnl_light_fast_single_tab;
	 else
	    tab = _tnl_light_fast_tab;
      }
   }
   else
      tab = _tnl_light_ci_tab;

   if (ctx->Light.ColorMaterialEnabled)
      ind |= LIGHT_COLORMATERIAL;

   if (ctx->Light.Model.TwoSide)
      ind |= LIGHT_TWOSIDE;

   VP_STAGE_DATA(stage)->light_func_tab = &tab[ind];

   /* This and the above should only be done on _NEW_LIGHT:
    */
   _mesa_validate_all_lighting_tables( ctx );
#endif

   /* Now run the stage...
    */
   stage->run = run_vp;
   return stage->run( ctx, stage );
}



/**
 * Called the first time stage->run is called.  In effect, don't
 * allocate data until the first time the stage is run.
 */
static GLboolean run_init_vp( GLcontext *ctx,
                              struct tnl_pipeline_stage *stage )
{
   TNLcontext *tnl = TNL_CONTEXT(ctx);
   struct vertex_buffer *VB = &(tnl->vb);
   struct vp_stage_data *store;
   const GLuint size = VB->Size;
   GLuint i;

   stage->privatePtr = MALLOC(sizeof(*store));
   store = VP_STAGE_DATA(stage);
   if (!store)
      return GL_FALSE;

   /* Allocate arrays of vertex output values */
   for (i = 0; i < 15; i++) {
      _mesa_vector4f_alloc( &store->attribs[i], 0, size, 32 );
      store->attribs[i].size = 4;
   }

   /* a few other misc allocations */
   _mesa_vector4f_alloc( &store->ndcCoords, 0, size, 32 );
   store->clipmask = (GLubyte *) ALIGN_MALLOC(sizeof(GLubyte)*size, 32 );

   /* Now validate the stage derived data...
    */
   stage->run = run_validate_program;
   return stage->run( ctx, stage );
}



/**
 * Check if vertex program mode is enabled. 
 * If so, configure the pipeline stage's type, inputs, and outputs.
 */
static void check_vp( GLcontext *ctx, struct tnl_pipeline_stage *stage )
{
   stage->active = ctx->VertexProgram.Enabled;

   if (stage->active) {
      /* Set stage->inputs equal to the bitmask of vertex attributes
       * which the program needs for inputs.
       */
      stage->inputs = ctx->VertexProgram.Current->InputsRead;
   }
}


/**
 * Destructor for this pipeline stage.
 */
static void dtr( struct tnl_pipeline_stage *stage )
{
   struct vp_stage_data *store = VP_STAGE_DATA(stage);

   if (store) {
      GLuint i;

      /* free the vertex program result arrays */
      for (i = 0; i < 15; i++)
         _mesa_vector4f_free( &store->attribs[i] );

      /* free misc arrays */
      _mesa_vector4f_free( &store->ndcCoords );
      ALIGN_FREE( store->clipmask );

      FREE( store );
      stage->privatePtr = 0;
   }
}

/**
 * Public description of this pipeline stage.
 */
const struct tnl_pipeline_stage _tnl_vertex_program_stage =
{
   "vertex-program",
   _NEW_ALL,	/*XXX FIX */	/* recheck */
   _NEW_ALL,	/*XXX FIX */    /* recalc */
   GL_FALSE,			/* active */
   0,				/* inputs - calculated on the fly */
   _TNL_BITS_PROG_ANY,		/* outputs -- could calculate */
   0,				/* changed_inputs */
   NULL,			/* private_data */
   dtr,				/* destroy */
   check_vp,			/* check */
   run_init_vp			/* run -- initially set to ctr */
};
