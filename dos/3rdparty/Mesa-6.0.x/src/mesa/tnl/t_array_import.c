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
 *
 * Authors:
 *    Keith Whitwell <keith@tungstengraphics.com>
 */

#include "glheader.h"
#include "context.h"
#include "macros.h"
#include "imports.h"
#include "state.h"
#include "mtypes.h"

#include "array_cache/acache.h"
#include "math/m_translate.h"

#include "t_array_import.h"
#include "t_context.h"


static void _tnl_import_vertex( GLcontext *ctx,
				GLboolean writeable,
				GLboolean stride )
{
   struct gl_client_array *tmp;
   GLboolean is_writeable = 0;
   struct tnl_vertex_arrays *inputs = &TNL_CONTEXT(ctx)->array_inputs;
   const GLubyte *data;

   tmp = _ac_import_vertex(ctx,
			   GL_FLOAT,
			   stride ? 4*sizeof(GLfloat) : 0,
			   0,
			   writeable,
			   &is_writeable);

   data = tmp->Ptr;
   inputs->Obj.data = (GLfloat (*)[4]) data;
   inputs->Obj.start = (GLfloat *) data;
   inputs->Obj.stride = tmp->StrideB;
   inputs->Obj.size = tmp->Size;
}

static void _tnl_import_normal( GLcontext *ctx,
				GLboolean writeable,
				GLboolean stride )
{
   struct gl_client_array *tmp;
   GLboolean is_writeable = 0;
   struct tnl_vertex_arrays *inputs = &TNL_CONTEXT(ctx)->array_inputs;
   const GLubyte *data;

   tmp = _ac_import_normal(ctx, GL_FLOAT,
			   stride ? 3*sizeof(GLfloat) : 0, writeable,
			   &is_writeable);

   data = tmp->Ptr;
   inputs->Normal.data = (GLfloat (*)[4]) data;
   inputs->Normal.start = (GLfloat *) data;
   inputs->Normal.stride = tmp->StrideB;
   inputs->Normal.size = 3;
}


static void _tnl_import_color( GLcontext *ctx,
			       GLboolean writeable,
			       GLboolean stride )
{
   struct gl_client_array *tmp;
   GLboolean is_writeable = 0;
   struct tnl_vertex_arrays *inputs = &TNL_CONTEXT(ctx)->array_inputs;
   const GLubyte *data;

   tmp = _ac_import_color(ctx,
			  GL_FLOAT,
			  stride ? 4*sizeof(GLfloat) : 0,
			  4,
			  writeable,
			  &is_writeable);

   data = tmp->Ptr;
   inputs->Color.data = (GLfloat (*)[4]) data;
   inputs->Color.start = (GLfloat *) data;
   inputs->Color.stride = tmp->StrideB;
   inputs->Color.size = tmp->Size;
}


static void _tnl_import_secondarycolor( GLcontext *ctx,
					GLboolean writeable,
					GLboolean stride )
{
   struct gl_client_array *tmp;
   GLboolean is_writeable = 0;
   struct tnl_vertex_arrays *inputs = &TNL_CONTEXT(ctx)->array_inputs;
   const GLubyte *data;

   tmp = _ac_import_secondarycolor(ctx, 
				   GL_FLOAT,
				   stride ? 4*sizeof(GLfloat) : 0,
				   4,
				   writeable,
				   &is_writeable);

   data = tmp->Ptr;
   inputs->SecondaryColor.data = (GLfloat (*)[4]) data;
   inputs->SecondaryColor.start = (GLfloat *) data;
   inputs->SecondaryColor.stride = tmp->StrideB;
   inputs->SecondaryColor.size = tmp->Size;
}

static void _tnl_import_fogcoord( GLcontext *ctx,
				  GLboolean writeable,
				  GLboolean stride )
{
   struct tnl_vertex_arrays *inputs = &TNL_CONTEXT(ctx)->array_inputs;
   struct gl_client_array *tmp;
   GLboolean is_writeable = 0;
   const GLubyte *data;

   tmp = _ac_import_fogcoord(ctx, GL_FLOAT,
			     stride ? sizeof(GLfloat) : 0, writeable,
			     &is_writeable);

   data = tmp->Ptr;
   inputs->FogCoord.data = (GLfloat (*)[4]) data;
   inputs->FogCoord.start = (GLfloat *) data;
   inputs->FogCoord.stride = tmp->StrideB;
}

static void _tnl_import_index( GLcontext *ctx,
			       GLboolean writeable,
			       GLboolean stride )
{
   struct tnl_vertex_arrays *inputs = &TNL_CONTEXT(ctx)->array_inputs;
   struct gl_client_array *tmp;
   GLboolean is_writeable = 0;
   const GLubyte *data;

   tmp = _ac_import_index(ctx, GL_FLOAT,
			  stride ? sizeof(GLfloat) : 0, writeable,
			  &is_writeable);

   data = tmp->Ptr;
   inputs->Index.data = (GLfloat (*)[4]) data;
   inputs->Index.start = (GLfloat *) data;
   inputs->Index.stride = tmp->StrideB;
}


static void _tnl_import_texcoord( GLcontext *ctx,
				  GLuint unit,
				  GLboolean writeable,
				  GLboolean stride )
{
   struct tnl_vertex_arrays *inputs = &TNL_CONTEXT(ctx)->array_inputs;
   struct gl_client_array *tmp;
   GLboolean is_writeable = 0;
   const GLubyte *data;

   tmp = _ac_import_texcoord(ctx, unit, GL_FLOAT,
			     stride ? 4 * sizeof(GLfloat) : 0,
			     0,
			     writeable,
			     &is_writeable);

   data = tmp->Ptr;
   inputs->TexCoord[unit].data = (GLfloat (*)[4]) data;
   inputs->TexCoord[unit].start = (GLfloat *) data;
   inputs->TexCoord[unit].stride = tmp->StrideB;
   inputs->TexCoord[unit].size = tmp->Size;
}


static void _tnl_import_edgeflag( GLcontext *ctx,
				  GLboolean writeable,
				  GLboolean stride )
{
   struct tnl_vertex_arrays *inputs = &TNL_CONTEXT(ctx)->array_inputs;
   struct gl_client_array *tmp;
   GLboolean is_writeable = 0;
   const GLubyte *data;

   tmp = _ac_import_edgeflag(ctx, GL_UNSIGNED_BYTE,
			     sizeof(GLubyte),
			     0,
			     &is_writeable);

   data = tmp->Ptr;
   inputs->EdgeFlag = (GLubyte *) data;
}



static void _tnl_import_attrib( GLcontext *ctx,
                                GLuint index,
                                GLboolean writeable,
                                GLboolean stride )
{
   struct tnl_vertex_arrays *inputs = &TNL_CONTEXT(ctx)->array_inputs;
   struct gl_client_array *tmp;
   GLboolean is_writeable = 0;
   const GLubyte *data;

   tmp = _ac_import_attrib(ctx, index, GL_FLOAT,
                           stride ? 4 * sizeof(GLfloat) : 0,
                           4,  /* want GLfloat[4] */
                           writeable,
                           &is_writeable);

   data = tmp->Ptr;
   inputs->Attribs[index].data = (GLfloat (*)[4]) data;
   inputs->Attribs[index].start = (GLfloat *) data;
   inputs->Attribs[index].stride = tmp->StrideB;
   inputs->Attribs[index].size = tmp->Size;
}




void _tnl_vb_bind_arrays( GLcontext *ctx, GLint start, GLint end)
{
   TNLcontext *tnl = TNL_CONTEXT(ctx);
   struct vertex_buffer *VB = &tnl->vb;
   GLuint inputs = tnl->pipeline.inputs;
   struct tnl_vertex_arrays *tmp = &tnl->array_inputs;
   GLuint i, index;

   VB->Count = end - start;
   VB->Elts = NULL;

   _ac_import_range( ctx, start, end );

   /* When vertex program mode is enabled, the generic vertex program
    * attribute arrays have priority over the conventional attributes.
    * Try to use them now.
    */
   for (index = 0; index < VERT_ATTRIB_MAX; index++) {
      /* When vertex program mode is enabled, the generic vertex attribute
       * arrays have priority over the conventional vertex arrays.
       */
      if (ctx->VertexProgram.Enabled
          && ctx->Array.VertexAttrib[index].Enabled) {
         /* Use generic attribute array */
         _tnl_import_attrib( ctx, index, GL_FALSE, GL_TRUE );
         VB->AttribPtr[index] = &tmp->Attribs[index];
      }
      /* use conventional arrays... */
      else if (index == VERT_ATTRIB_POS) {
         if (inputs & _TNL_BIT_POS) {
            _tnl_import_vertex( ctx, 0, 0 );
            tmp->Obj.count = VB->Count;
            VB->AttribPtr[_TNL_ATTRIB_POS] = &tmp->Obj;
         }
      }
      else if (index == VERT_ATTRIB_NORMAL) {
         if (inputs & _TNL_BIT_NORMAL) {
            _tnl_import_normal( ctx, 0, 0 );
            tmp->Normal.count = VB->Count;
            VB->AttribPtr[_TNL_ATTRIB_NORMAL] = &tmp->Normal;
         }
      }
      else if (index == VERT_ATTRIB_COLOR0) {
         if (inputs & _TNL_BIT_COLOR0) {
            _tnl_import_color( ctx, 0, 0 );
            tmp->Color.count = VB->Count;
            VB->AttribPtr[_TNL_ATTRIB_COLOR0] = &tmp->Color;
         }
      }
      else if (index == VERT_ATTRIB_COLOR1) {
         if (inputs & _TNL_BIT_COLOR1) {
            _tnl_import_secondarycolor( ctx, 0, 0 );
            tmp->SecondaryColor.count = VB->Count;
            VB->AttribPtr[_TNL_ATTRIB_COLOR1] = &tmp->SecondaryColor;
         }
      }
      else if (index == VERT_ATTRIB_FOG) {
         if (inputs & _TNL_BIT_FOG) {
            _tnl_import_fogcoord( ctx, 0, 0 );
            tmp->FogCoord.count = VB->Count;
            VB->AttribPtr[_TNL_ATTRIB_FOG] = &tmp->FogCoord;
         }
      }
      else if (index >= VERT_ATTRIB_TEX0 && index <= VERT_ATTRIB_TEX7) {
         if (inputs & _TNL_BITS_TEX_ANY) {
            for (i = 0; i < ctx->Const.MaxTextureUnits; i++) {
               if (inputs & _TNL_BIT_TEX(i)) {
                  _tnl_import_texcoord( ctx, i, GL_FALSE, GL_FALSE );
                  tmp->TexCoord[i].count = VB->Count;
                  VB->AttribPtr[_TNL_ATTRIB_TEX0 + i] = &tmp->TexCoord[i];
               }
            }
         }
      }
   }

   /* odd-ball vertex attributes */
   if (inputs & _TNL_BIT_INDEX) {
      _tnl_import_index( ctx, 0, 0 );
      tmp->Index.count = VB->Count;
      VB->AttribPtr[_TNL_ATTRIB_INDEX] = &tmp->Index;
   }

   if (inputs & _TNL_BIT_EDGEFLAG) {
      _tnl_import_edgeflag( ctx, GL_TRUE, sizeof(GLboolean) );
      VB->EdgeFlag = (GLboolean *) tmp->EdgeFlag;
   }

   /* These are constant & can be precalculated:
    */
   if (inputs & _TNL_BITS_MAT_ANY) {
      for (i = _TNL_ATTRIB_MAT_FRONT_AMBIENT; i < _TNL_ATTRIB_INDEX; i++) {
	 tmp->Attribs[i].count = VB->Count;
	 tmp->Attribs[i].data = (GLfloat (*)[4]) tnl->vtx.current[i];
	 tmp->Attribs[i].start = tnl->vtx.current[i];
	 tmp->Attribs[i].size = 4; 
	 tmp->Attribs[i].stride = 0;
	 VB->AttribPtr[i] = &tmp->Attribs[i];
      }      
   }


   /* Legacy pointers -- remove one day.
    */
   VB->ObjPtr = VB->AttribPtr[_TNL_ATTRIB_POS];
   VB->NormalPtr = VB->AttribPtr[_TNL_ATTRIB_NORMAL];
   VB->ColorPtr[0] = VB->AttribPtr[_TNL_ATTRIB_COLOR0];
   VB->ColorPtr[1] = 0;
   VB->IndexPtr[0] = VB->AttribPtr[_TNL_ATTRIB_INDEX];
   VB->IndexPtr[1] = 0;
   VB->SecondaryColorPtr[0] = VB->AttribPtr[_TNL_ATTRIB_COLOR1];
   VB->SecondaryColorPtr[1] = 0;
   VB->FogCoordPtr = VB->AttribPtr[_TNL_ATTRIB_FOG];

   for (i = 0; i < ctx->Const.MaxTextureCoordUnits; i++) {
      VB->TexCoordPtr[i] = VB->AttribPtr[_TNL_ATTRIB_TEX0 + i];
   }

}
