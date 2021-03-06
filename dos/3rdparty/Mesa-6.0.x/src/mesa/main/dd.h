/**
 * \file dd.h
 * Device driver interfaces.
 */

/*
 * Mesa 3-D graphics library
 * Version:  4.1
 *
 * Copyright (C) 1999-2002  Brian Paul   All Rights Reserved.
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


#ifndef DD_INCLUDED
#define DD_INCLUDED

/* THIS FILE ONLY INCLUDED BY mtypes.h !!!!! */

struct gl_pixelstore_attrib;

/* Mask bits sent to the driver Clear() function */
#define DD_FRONT_LEFT_BIT  FRONT_LEFT_BIT         /* 1 */
#define DD_FRONT_RIGHT_BIT FRONT_RIGHT_BIT        /* 2 */
#define DD_BACK_LEFT_BIT   BACK_LEFT_BIT          /* 4 */
#define DD_BACK_RIGHT_BIT  BACK_RIGHT_BIT         /* 8 */
#define DD_AUX0            AUX0_BIT               /* future use */
#define DD_AUX1            AUX1_BIT               /* future use */
#define DD_AUX2            AUX2_BIT               /* future use */
#define DD_AUX3            AUX3_BIT               /* future use */
#define DD_DEPTH_BIT       GL_DEPTH_BUFFER_BIT    /* 0x00000100 */
#define DD_ACCUM_BIT       GL_ACCUM_BUFFER_BIT    /* 0x00000200 */
#define DD_STENCIL_BIT     GL_STENCIL_BUFFER_BIT  /* 0x00000400 */


/**
 * Device driver function table.
 * Core Mesa uses these function pointers to call into device drivers.
 * Most of these functions directly correspond to OpenGL state commands.
 * Core Mesa will call these functions after error checking has been done
 * so that the drivers don't have to worry about error testing.
 *
 * Vertex transformation/clipping/lighting is patched into the T&L module.
 * Rasterization functions are patched into the swrast module.
 */
struct dd_function_table {
   /**
    * Return a string as needed by glGetString().
    *
    * Only the GL_RENDERER token must be implemented.  Otherwise, NULL can be
    * returned.
    */
   const GLubyte * (*GetString)( GLcontext *ctx, GLenum name );

   /**
    * Notify the driver after Mesa has made some internal state changes.  
    *
    * This is in addition to any state change callbacks Mesa may already have
    * made.
    */
   void (*UpdateState)( GLcontext *ctx, GLuint new_state );

   /**
    * Clear the color/depth/stencil/accum buffer(s).
    *
    * \param mask a bitmask of the DD_*_BIT values defined above that indicates
    * which buffers need to be cleared.
    * \param all if true then clear the whole buffer, else clear only the
    * region defined by <tt>(x, y, width, height)</tt>.
    * 
    * This function must obey the glColorMask(), glIndexMask() and glStencilMask()
    * settings!
    * Software Mesa can do masked clears if the device driver can't.
    */
   void (*Clear)( GLcontext *ctx, GLbitfield mask, GLboolean all,
		  GLint x, GLint y, GLint width, GLint height );

   /**
    * Specify the current buffer for writing.  
    *
    * Called via glDrawBuffer().  Note the driver must organize fallbacks (e.g.
    * with swrast) if it cannot implement the requested mode.
    */
   void (*DrawBuffer)( GLcontext *ctx, GLenum buffer );

   /**
    * Specifies the current buffer for reading.  
    *
    * Called via glReadBuffer().
    */
   void (*ReadBuffer)( GLcontext *ctx, GLenum buffer );

   /**
    * Get the width and height of the named buffer/window.
    *
    * Mesa uses this to determine when the driver's window size has changed.
    */
   void (*GetBufferSize)( GLframebuffer *buffer,
                          GLuint *width, GLuint *height );

   /**
    * Resize the driver's depth/stencil/accum/back buffers to match the
    * size given in the GLframebuffer struct.  
    *
    * This is typically called when Mesa detects that a window size has changed.
    */
   void (*ResizeBuffers)( GLframebuffer *buffer );

   /**
    * This is called whenever glFinish() is called.
    */
   void (*Finish)( GLcontext *ctx );

   /**
    * This is called whenever glFlush() is called.
    */
   void (*Flush)( GLcontext *ctx );

   /**
    * Called whenever an error is generated.  
    *
    * __GLcontextRec::ErrorValue contains the error value.
    */
   void (*Error)( GLcontext *ctx );


   /**
    * \name For hardware accumulation buffer
    */
   /*@{*/
   /**
    * Execute glAccum command within the given scissor region.
    */
   void (*Accum)( GLcontext *ctx, GLenum op, GLfloat value,
		  GLint xpos, GLint ypos, GLint width, GLint height );
   /*@}*/


   /**
    * \name glDraw(), glRead(), glCopyPixels() and glBitmap() functions
    */
   /*@{*/

   /**
    * This is called by glDrawPixels().
    *
    * \p unpack describes how to unpack the source image data.
    */
   void (*DrawPixels)( GLcontext *ctx,
		       GLint x, GLint y, GLsizei width, GLsizei height,
		       GLenum format, GLenum type,
		       const struct gl_pixelstore_attrib *unpack,
		       const GLvoid *pixels );

   /**
    * Called by glReadPixels().
    */
   void (*ReadPixels)( GLcontext *ctx,
		       GLint x, GLint y, GLsizei width, GLsizei height,
		       GLenum format, GLenum type,
		       const struct gl_pixelstore_attrib *unpack,
		       GLvoid *dest );

   /**
    * Do a glCopyPixels().  
    *
    * This function must respect all rasterization state, glPixelTransfer(),
    * glPixelZoom(), etc.
    */
   void (*CopyPixels)( GLcontext *ctx,
                            GLint srcx, GLint srcy,
                            GLsizei width, GLsizei height,
                            GLint dstx, GLint dsty, GLenum type );

   /**
    * This is called by glBitmap().  
    *
    * Works the same as dd_function_table::DrawPixels, above.
    */
   void (*Bitmap)( GLcontext *ctx,
		   GLint x, GLint y, GLsizei width, GLsizei height,
		   const struct gl_pixelstore_attrib *unpack,
		   const GLubyte *bitmap );
   /*@}*/

   
   /**
    * \name Texture image functions
    */
   /*@{*/

   /**
    * Choose texture format.
    * 
    * This is called by the \c _mesa_store_tex[sub]image[123]d() fallback
    * functions.  The driver should examine \p internalFormat and return a
    * pointer to an appropriate gl_texture_format.
    */
   const struct gl_texture_format *
   (*ChooseTextureFormat)( GLcontext *ctx, GLint internalFormat,
                           GLenum srcFormat, GLenum srcType );

   /**
    * Called by glTexImage1D().
    * 
    * \param target user specified.
    * \param format user specified.
    * \param type user specified.
    * \param pixels user specified.
    * \param packing indicates the image packing of pixels.
    * \param texObj is the target texture object.
    * \param texImage is the target texture image.  It will have the texture \p
    * width, \p height, \p depth, \p border and \p internalFormat information.
    * 
    * \p retainInternalCopy is returned by this function and indicates whether
    * core Mesa should keep an internal copy of the texture image.
    *
    * Drivers should call a fallback routine from texstore.c if needed.
    */
   void (*TexImage1D)( GLcontext *ctx, GLenum target, GLint level,
                       GLint internalFormat,
                       GLint width, GLint border,
                       GLenum format, GLenum type, const GLvoid *pixels,
                       const struct gl_pixelstore_attrib *packing,
                       struct gl_texture_object *texObj,
                       struct gl_texture_image *texImage );

   /**
    * Called by glTexImage2D().
    * 
    * \sa dd_function_table::TexImage1D.
    */
   void (*TexImage2D)( GLcontext *ctx, GLenum target, GLint level,
                       GLint internalFormat,
                       GLint width, GLint height, GLint border,
                       GLenum format, GLenum type, const GLvoid *pixels,
                       const struct gl_pixelstore_attrib *packing,
                       struct gl_texture_object *texObj,
                       struct gl_texture_image *texImage );
   
   /**
    * Called by glTexImage3D().
    * 
    * \sa dd_function_table::TexImage1D.
    */
   void (*TexImage3D)( GLcontext *ctx, GLenum target, GLint level,
                       GLint internalFormat,
                       GLint width, GLint height, GLint depth, GLint border,
                       GLenum format, GLenum type, const GLvoid *pixels,
                       const struct gl_pixelstore_attrib *packing,
                       struct gl_texture_object *texObj,
                       struct gl_texture_image *texImage );

   /**
    * Called by glTexSubImage1D().
    *
    * \param target user specified.
    * \param level user specified.
    * \param xoffset user specified.
    * \param yoffset user specified.
    * \param zoffset user specified.
    * \param width user specified.
    * \param height user specified.
    * \param depth user specified.
    * \param format user specified.
    * \param type user specified.
    * \param pixels user specified.
    * \param packing indicates the image packing of pixels.
    * \param texObj is the target texture object.
    * \param texImage is the target texture image.  It will have the texture \p
    * width, \p height, \p border and \p internalFormat information.
    *
    * The driver should use a fallback routine from texstore.c if needed.
    */
   void (*TexSubImage1D)( GLcontext *ctx, GLenum target, GLint level,
                          GLint xoffset, GLsizei width,
                          GLenum format, GLenum type,
                          const GLvoid *pixels,
                          const struct gl_pixelstore_attrib *packing,
                          struct gl_texture_object *texObj,
                          struct gl_texture_image *texImage );
   
   /**
    * Called by glTexSubImage2D().
    *
    * \sa dd_function_table::TexSubImage1D.
    */
   void (*TexSubImage2D)( GLcontext *ctx, GLenum target, GLint level,
                          GLint xoffset, GLint yoffset,
                          GLsizei width, GLsizei height,
                          GLenum format, GLenum type,
                          const GLvoid *pixels,
                          const struct gl_pixelstore_attrib *packing,
                          struct gl_texture_object *texObj,
                          struct gl_texture_image *texImage );
   
   /**
    * Called by glTexSubImage3D().
    *
    * \sa dd_function_table::TexSubImage1D.
    */
   void (*TexSubImage3D)( GLcontext *ctx, GLenum target, GLint level,
                          GLint xoffset, GLint yoffset, GLint zoffset,
                          GLsizei width, GLsizei height, GLint depth,
                          GLenum format, GLenum type,
                          const GLvoid *pixels,
                          const struct gl_pixelstore_attrib *packing,
                          struct gl_texture_object *texObj,
                          struct gl_texture_image *texImage );

   /**
    * Called by glCopyTexImage1D().
    * 
    * Drivers should use a fallback routine from texstore.c if needed.
    */
   void (*CopyTexImage1D)( GLcontext *ctx, GLenum target, GLint level,
                           GLenum internalFormat, GLint x, GLint y,
                           GLsizei width, GLint border );

   /**
    * Called by glCopyTexImage2D().
    * 
    * Drivers should use a fallback routine from texstore.c if needed.
    */
   void (*CopyTexImage2D)( GLcontext *ctx, GLenum target, GLint level,
                           GLenum internalFormat, GLint x, GLint y,
                           GLsizei width, GLsizei height, GLint border );

   /**
    * Called by glCopyTexSubImage1D().
    * 
    * Drivers should use a fallback routine from texstore.c if needed.
    */
   void (*CopyTexSubImage1D)( GLcontext *ctx, GLenum target, GLint level,
                              GLint xoffset,
                              GLint x, GLint y, GLsizei width );
   /**
    * Called by glCopyTexSubImage2D().
    * 
    * Drivers should use a fallback routine from texstore.c if needed.
    */
   void (*CopyTexSubImage2D)( GLcontext *ctx, GLenum target, GLint level,
                              GLint xoffset, GLint yoffset,
                              GLint x, GLint y,
                              GLsizei width, GLsizei height );
   /**
    * Called by glCopyTexSubImage3D().
    * 
    * Drivers should use a fallback routine from texstore.c if needed.
    */
   void (*CopyTexSubImage3D)( GLcontext *ctx, GLenum target, GLint level,
                              GLint xoffset, GLint yoffset, GLint zoffset,
                              GLint x, GLint y,
                              GLsizei width, GLsizei height );

   /**
    * Called by glTexImage[123]D when user specifies a proxy texture
    * target.  
    *
    * \return GL_TRUE if the proxy test passes, or GL_FALSE if the test fails.
    */
   GLboolean (*TestProxyTexImage)(GLcontext *ctx, GLenum target,
                                  GLint level, GLint internalFormat,
                                  GLenum format, GLenum type,
                                  GLint width, GLint height,
                                  GLint depth, GLint border);
   /*@}*/

   
   /**
    * \name Compressed texture functions
    */
   /*@{*/

   /**
    * Called by glCompressedTexImage1D().
    *
    * \param target user specified.
    * \param format user specified.
    * \param type user specified.
    * \param pixels user specified.
    * \param packing indicates the image packing of pixels.
    * \param texObj is the target texture object.
    * \param texImage is the target texture image.  It will have the texture \p
    * width, \p height, \p depth, \p border and \p internalFormat information.
    *      
    * \a retainInternalCopy is returned by this function and indicates whether
    * core Mesa should keep an internal copy of the texture image.
    */
   void (*CompressedTexImage1D)( GLcontext *ctx, GLenum target,
                                 GLint level, GLint internalFormat,
                                 GLsizei width, GLint border,
                                 GLsizei imageSize, const GLvoid *data,
                                 struct gl_texture_object *texObj,
                                 struct gl_texture_image *texImage );
   /**
    * Called by glCompressedTexImage2D().
    *
    * \sa dd_function_table::CompressedTexImage1D.
    */
   void (*CompressedTexImage2D)( GLcontext *ctx, GLenum target,
                                 GLint level, GLint internalFormat,
                                 GLsizei width, GLsizei height, GLint border,
                                 GLsizei imageSize, const GLvoid *data,
                                 struct gl_texture_object *texObj,
                                 struct gl_texture_image *texImage );
   /**
    * Called by glCompressedTexImage3D().
    *
    * \sa dd_function_table::CompressedTexImage3D.
    */
   void (*CompressedTexImage3D)( GLcontext *ctx, GLenum target,
                                 GLint level, GLint internalFormat,
                                 GLsizei width, GLsizei height, GLsizei depth,
                                 GLint border,
                                 GLsizei imageSize, const GLvoid *data,
                                 struct gl_texture_object *texObj,
                                 struct gl_texture_image *texImage );

   /**
    * Called by glCompressedTexSubImage1D().
    * 
    * \param target user specified.
    * \param level user specified.
    * \param xoffset user specified.
    * \param yoffset user specified.
    * \param zoffset user specified.
    * \param width user specified.
    * \param height user specified.
    * \param depth user specified.
    * \param imageSize user specified.
    * \param data user specified.
    * \param texObj is the target texture object.
    * \param texImage is the target texture image.  It will have the texture \p
    * width, \p height, \p depth, \p border and \p internalFormat information.
    */
   void (*CompressedTexSubImage1D)(GLcontext *ctx, GLenum target, GLint level,
                                   GLint xoffset, GLsizei width,
                                   GLenum format,
                                   GLsizei imageSize, const GLvoid *data,
                                   struct gl_texture_object *texObj,
                                   struct gl_texture_image *texImage);
   /**
    * Called by glCompressedTexSubImage2D().
    *
    * \sa dd_function_table::CompressedTexImage3D.
    */
   void (*CompressedTexSubImage2D)(GLcontext *ctx, GLenum target, GLint level,
                                   GLint xoffset, GLint yoffset,
                                   GLsizei width, GLint height,
                                   GLenum format,
                                   GLsizei imageSize, const GLvoid *data,
                                   struct gl_texture_object *texObj,
                                   struct gl_texture_image *texImage);
   /**
    * Called by glCompressedTexSubImage3D().
    *
    * \sa dd_function_table::CompressedTexImage3D.
    */
   void (*CompressedTexSubImage3D)(GLcontext *ctx, GLenum target, GLint level,
                                   GLint xoffset, GLint yoffset, GLint zoffset,
                                   GLsizei width, GLint height, GLint depth,
                                   GLenum format,
                                   GLsizei imageSize, const GLvoid *data,
                                   struct gl_texture_object *texObj,
                                   struct gl_texture_image *texImage);
   /**
    * Called to validate a certain compressed format.
    */
   GLboolean (*IsCompressedFormat)( GLcontext *ctx, GLenum internalFormat );
   /**
    * Called to get bytes of storage needed for the given texture size and
    * compressed format.
    */
   GLuint (*CompressedTextureSize)( GLcontext *ctx,
                                    GLsizei width, GLsizei height, GLsizei depth,
                                    GLenum format );
   /*@}*/

   /**
    * \name Texture object functions
    */
   /*@{*/

   /**
    * Called by glBindTexture().
    */
   void (*BindTexture)( GLcontext *ctx, GLenum target,
                        struct gl_texture_object *tObj );

   /**
    * Called when a texture object is created.
    */
   void (*CreateTexture)( GLcontext *ctx, struct gl_texture_object *tObj );

   /**
    * Called to allocate a new texture object.
    * 
    * \note This function pointer should be initialized by drivers \e before
    * calling _mesa_initialize_context() since context initialization involves
    * allocating some texture objects!
    */
   struct gl_texture_object * (*NewTextureObject)( GLcontext *ctx, GLuint name,
                                                   GLenum target );
   /**
    * Called when a texture object is about to be deallocated.  
    *
    * Driver should free anything attached to the DriverData pointers.
    */
   void (*DeleteTexture)( GLcontext *ctx, struct gl_texture_object *tObj );

   /**
    * Called to allocate a new texture image object.
    */
   struct gl_texture_image * (*NewTextureImage)( GLcontext *ctx );

   /**
    * Called by glAreTextureResident().
    */
   GLboolean (*IsTextureResident)( GLcontext *ctx,
                                   struct gl_texture_object *t );

   /**
    * Called by glPrioritizeTextures().
    */
   void (*PrioritizeTexture)( GLcontext *ctx,  struct gl_texture_object *t,
                              GLclampf priority );

   /**
    * Called by glActiveTextureARB() to set current texture unit.
    */
   void (*ActiveTexture)( GLcontext *ctx, GLuint texUnitNumber );

   /**
    * Called when the texture's color lookup table is changed.
    * 
    * If \p tObj is NULL then the shared texture palette
    * gl_texture_object::Palette is to be updated.
    */
   void (*UpdateTexturePalette)( GLcontext *ctx,
                                 struct gl_texture_object *tObj );
   /*@}*/

   
   /**
    * \name Imaging functionality
    */
   /*@{*/
   void (*CopyColorTable)( GLcontext *ctx,
			   GLenum target, GLenum internalformat,
			   GLint x, GLint y, GLsizei width );

   void (*CopyColorSubTable)( GLcontext *ctx,
			      GLenum target, GLsizei start,
			      GLint x, GLint y, GLsizei width );

   void (*CopyConvolutionFilter1D)( GLcontext *ctx, GLenum target,
				    GLenum internalFormat,
				    GLint x, GLint y, GLsizei width );

   void (*CopyConvolutionFilter2D)( GLcontext *ctx, GLenum target,
				    GLenum internalFormat,
				    GLint x, GLint y,
				    GLsizei width, GLsizei height );
   /*@}*/


   /**
    * \name State-changing functions.
    *
    * \note drawing functions are above.
    *
    * These functions are called by their corresponding OpenGL API functions.
    * They are \e also called by the gl_PopAttrib() function!!!
    * May add more functions like these to the device driver in the future.
    */
   /*@{*/
   /** Specify the alpha test function */
   void (*AlphaFunc)(GLcontext *ctx, GLenum func, GLfloat ref);
   /** Set the blend color */
   void (*BlendColor)(GLcontext *ctx, const GLfloat color[4]);
   /** Set the blend equation */
   void (*BlendEquation)(GLcontext *ctx, GLenum mode);
   /** Specify pixel arithmetic */
   void (*BlendFunc)(GLcontext *ctx, GLenum sfactor, GLenum dfactor);
   void (*BlendFuncSeparate)(GLcontext *ctx,
                             GLenum sfactorRGB, GLenum dfactorRGB,
                             GLenum sfactorA, GLenum dfactorA);
   /** Specify clear values for the color buffers */
   void (*ClearColor)(GLcontext *ctx, const GLfloat color[4]);
   /** Specify the clear value for the depth buffer */
   void (*ClearDepth)(GLcontext *ctx, GLclampd d);
   /** Specify the clear value for the color index buffers */
   void (*ClearIndex)(GLcontext *ctx, GLuint index);
   /** Specify the clear value for the stencil buffer */
   void (*ClearStencil)(GLcontext *ctx, GLint s);
   /** Specify a plane against which all geometry is clipped */
   void (*ClipPlane)(GLcontext *ctx, GLenum plane, const GLfloat *equation );
   /** Enable and disable writing of frame buffer color components */
   void (*ColorMask)(GLcontext *ctx, GLboolean rmask, GLboolean gmask,
                     GLboolean bmask, GLboolean amask );
   /** Cause a material color to track the current color */
   void (*ColorMaterial)(GLcontext *ctx, GLenum face, GLenum mode);
   /** Specify whether front- or back-facing facets can be culled */
   void (*CullFace)(GLcontext *ctx, GLenum mode);
   /** Define front- and back-facing polygons */
   void (*FrontFace)(GLcontext *ctx, GLenum mode);
   /** Specify the value used for depth buffer comparisons */
   void (*DepthFunc)(GLcontext *ctx, GLenum func);
   /** Enable or disable writing into the depth buffer */
   void (*DepthMask)(GLcontext *ctx, GLboolean flag);
   /** Specify mapping of depth values from normalized device coordinates to window coordinates */
   void (*DepthRange)(GLcontext *ctx, GLclampd nearval, GLclampd farval);
   /** Enable or disable server-side gl capabilities */
   void (*Enable)(GLcontext *ctx, GLenum cap, GLboolean state);
   /** Specify fog parameters */
   void (*Fogfv)(GLcontext *ctx, GLenum pname, const GLfloat *params);
   /** Specify implementation-specific hints */
   void (*Hint)(GLcontext *ctx, GLenum target, GLenum mode);
   /** Control the writing of individual bits in the color index buffers */
   void (*IndexMask)(GLcontext *ctx, GLuint mask);
   /** Set light source parameters */
   void (*Lightfv)(GLcontext *ctx, GLenum light,
		   GLenum pname, const GLfloat *params );
   /** Set the lighting model parameters */
   void (*LightModelfv)(GLcontext *ctx, GLenum pname, const GLfloat *params);
   /** Specify the line stipple pattern */
   void (*LineStipple)(GLcontext *ctx, GLint factor, GLushort pattern );
   /** Specify the width of rasterized lines */
   void (*LineWidth)(GLcontext *ctx, GLfloat width);
   /** Specify a logical pixel operation for color index rendering */
   void (*LogicOpcode)(GLcontext *ctx, GLenum opcode);
   void (*PointParameterfv)(GLcontext *ctx, GLenum pname,
                            const GLfloat *params);
   /** Specify the diameter of rasterized points */
   void (*PointSize)(GLcontext *ctx, GLfloat size);
   /** Select a polygon rasterization mode */
   void (*PolygonMode)(GLcontext *ctx, GLenum face, GLenum mode);
   /** Set the scale and units used to calculate depth values */
   void (*PolygonOffset)(GLcontext *ctx, GLfloat factor, GLfloat units);
   /** Set the polygon stippling pattern */
   void (*PolygonStipple)(GLcontext *ctx, const GLubyte *mask );
   /** Set rasterization mode */
   void (*RenderMode)(GLcontext *ctx, GLenum mode );
   /** Define the scissor box */
   void (*Scissor)(GLcontext *ctx, GLint x, GLint y, GLsizei w, GLsizei h);
   /** Select flat or smooth shading */
   void (*ShadeModel)(GLcontext *ctx, GLenum mode);
   /** Set function and reference value for stencil testing */
   void (*StencilFunc)(GLcontext *ctx, GLenum func, GLint ref, GLuint mask);
   /** Control the writing of individual bits in the stencil planes */
   void (*StencilMask)(GLcontext *ctx, GLuint mask);
   /** Set stencil test actions */
   void (*StencilOp)(GLcontext *ctx, GLenum fail, GLenum zfail, GLenum zpass);
   void (*ActiveStencilFace)(GLcontext *ctx, GLuint face);
   /** Control the generation of texture coordinates */
   void (*TexGen)(GLcontext *ctx, GLenum coord, GLenum pname,
		  const GLfloat *params);
   /** Set texture environment parameters */
   void (*TexEnv)(GLcontext *ctx, GLenum target, GLenum pname,
                  const GLfloat *param);
   /** Set texture parameters */
   void (*TexParameter)(GLcontext *ctx, GLenum target,
                        struct gl_texture_object *texObj,
                        GLenum pname, const GLfloat *params);
   void (*TextureMatrix)(GLcontext *ctx, GLuint unit, const GLmatrix *mat);
   /** Set the viewport */
   void (*Viewport)(GLcontext *ctx, GLint x, GLint y, GLsizei w, GLsizei h);
   /*@}*/


   /**
    * \name Vertex array functions
    *
    * Called by the corresponding OpenGL functions.
    */
   /*@{*/
   void (*VertexPointer)(GLcontext *ctx, GLint size, GLenum type,
			 GLsizei stride, const GLvoid *ptr);
   void (*NormalPointer)(GLcontext *ctx, GLenum type,
			 GLsizei stride, const GLvoid *ptr);
   void (*ColorPointer)(GLcontext *ctx, GLint size, GLenum type,
			GLsizei stride, const GLvoid *ptr);
   void (*FogCoordPointer)(GLcontext *ctx, GLenum type,
			   GLsizei stride, const GLvoid *ptr);
   void (*IndexPointer)(GLcontext *ctx, GLenum type,
			GLsizei stride, const GLvoid *ptr);
   void (*SecondaryColorPointer)(GLcontext *ctx, GLint size, GLenum type,
				 GLsizei stride, const GLvoid *ptr);
   void (*TexCoordPointer)(GLcontext *ctx, GLint size, GLenum type,
			   GLsizei stride, const GLvoid *ptr);
   void (*EdgeFlagPointer)(GLcontext *ctx, GLsizei stride, const GLvoid *ptr);
   void (*VertexAttribPointer)(GLcontext *ctx, GLuint index, GLint size,
                               GLenum type, GLsizei stride, const GLvoid *ptr);
   /*@}*/


   /** 
    * \name State-query functions
    *
    * Return GL_TRUE if query was completed, GL_FALSE otherwise.
    */
   /*@{*/
   /** Return the value or values of a selected parameter */
   GLboolean (*GetBooleanv)(GLcontext *ctx, GLenum pname, GLboolean *result);
   /** Return the value or values of a selected parameter */
   GLboolean (*GetDoublev)(GLcontext *ctx, GLenum pname, GLdouble *result);
   /** Return the value or values of a selected parameter */
   GLboolean (*GetFloatv)(GLcontext *ctx, GLenum pname, GLfloat *result);
   /** Return the value or values of a selected parameter */
   GLboolean (*GetIntegerv)(GLcontext *ctx, GLenum pname, GLint *result);
   /** Return the value or values of a selected parameter */
   GLboolean (*GetPointerv)(GLcontext *ctx, GLenum pname, GLvoid **result);
   /*@}*/
   

   /**
    * \name Vertex buffer object functions
    */
#if FEATURE_ARB_vertex_buffer_object
   /*@{*/
   void (*BindBuffer)( GLcontext *ctx, GLenum target,
		       struct gl_buffer_object *obj );

   struct gl_buffer_object * (*NewBufferObject)( GLcontext *ctx, GLuint buffer,
						 GLenum target );
   
   void (*DeleteBuffer)( GLcontext *ctx, struct gl_buffer_object *obj );

   void (*BufferData)( GLcontext *ctx, GLenum target, GLsizeiptrARB size,
		       const GLvoid *data, GLenum usage,
		       struct gl_buffer_object *obj );

   void (*BufferSubData)( GLcontext *ctx, GLenum target, GLintptrARB offset,
			  GLsizeiptrARB size, const GLvoid *data,
			  struct gl_buffer_object *obj );

   void (*GetBufferSubData)( GLcontext *ctx, GLenum target,
			     GLintptrARB offset, GLsizeiptrARB size,
			     GLvoid *data, struct gl_buffer_object *obj );

   void * (*MapBuffer)( GLcontext *ctx, GLenum target, GLenum access,
			struct gl_buffer_object *obj );

   GLboolean (*UnmapBuffer)( GLcontext *ctx, GLenum target,
			     struct gl_buffer_object *obj );
   /*@}*/
#endif

   /**
    * \name Support for multiple T&L engines
    */
   /*@{*/

   /**
    * Bitmask of state changes that require the current T&L module to be
    * validated, using ValidateTnlModule() below.
    */
   GLuint NeedValidate;

   /**
    * Validate the current T&L module. 
    *
    * This is called directly after UpdateState() when a state change that has
    * occurred matches the dd_function_table::NeedValidate bitmask above.  This
    * ensures all computed values are up to date, thus allowing the driver to
    * decide if the current T&L module needs to be swapped out.
    *
    * This must be non-NULL if a driver installs a custom T&L module and sets
    * the dd_function_table::NeedValidate bitmask, but may be NULL otherwise.
    */
   void (*ValidateTnlModule)( GLcontext *ctx, GLuint new_state );


#define PRIM_OUTSIDE_BEGIN_END   GL_POLYGON+1
#define PRIM_INSIDE_UNKNOWN_PRIM GL_POLYGON+2
#define PRIM_UNKNOWN             GL_POLYGON+3

   /**
    * Set by the driver-supplied T&L engine.  
    *
    * Set to PRIM_OUTSIDE_BEGIN_END when outside glBegin()/glEnd().
    */
   GLuint CurrentExecPrimitive;

   /**
    * Current state of an in-progress compilation.  
    *
    * May take on any of the additional values PRIM_OUTSIDE_BEGIN_END,
    * PRIM_INSIDE_UNKNOWN_PRIM or PRIM_UNKNOWN defined above.
    */
   GLuint CurrentSavePrimitive;


#define FLUSH_STORED_VERTICES 0x1
#define FLUSH_UPDATE_CURRENT  0x2
   /**
    * Set by the driver-supplied T&L engine whenever vertices are buffered
    * between glBegin()/glEnd() objects or __GLcontextRec::Current is not
    * updated.
    *
    * The dd_function_table::FlushVertices call below may be used to resolve
    * these conditions.
    */
   GLuint NeedFlush;
   GLuint SaveNeedFlush;

   /**
    * If inside glBegin()/glEnd(), it should ASSERT(0).  Otherwise, if
    * FLUSH_STORED_VERTICES bit in \p flags is set flushes any buffered
    * vertices, if FLUSH_UPDATE_CURRENT bit is set updates
    * __GLcontextRec::Current and gl_light_attrib::Material
    *
    * Note that the default T&L engine never clears the
    * FLUSH_UPDATE_CURRENT bit, even after performing the update.
    */
   void (*FlushVertices)( GLcontext *ctx, GLuint flags );
   void (*SaveFlushVertices)( GLcontext *ctx );

   /**
    * Give the driver the opportunity to hook in its own vtxfmt for
    * compiling optimized display lists.  This is called on each valid
    * glBegin() during list compilation.
    */
   GLboolean (*NotifySaveBegin)( GLcontext *ctx, GLenum mode );

   /**
    * Notify driver that the special derived value _NeedEyeCoords has
    * changed.
    */
   void (*LightingSpaceChange)( GLcontext *ctx );

   /**
    * Called by glNewList().
    *
    * Let the T&L component know what is going on with display lists
    * in time to make changes to dispatch tables, etc.
    */
   void (*NewList)( GLcontext *ctx, GLuint list, GLenum mode );
   /**
    * Called by glEndList().
    *
    * \sa dd_function_table::NewList.
    */
   void (*EndList)( GLcontext *ctx );

   /**
    * Called by glCallList(s), but not recursively.
    *
    * Notify the T&L component before and after calling a display list.
    * Called by glCallList(s), but not recursively.
    */
   void (*BeginCallList)( GLcontext *ctx, GLuint list );
   /**
    * Called by glEndCallList().
    *
    * \sa dd_function_table::BeginCallList.
    */
   void (*EndCallList)( GLcontext *ctx );

   /**
    * Let the T&L component know when the context becomes current.
    */
   void (*MakeCurrent)( GLcontext *ctx, GLframebuffer *drawBuffer,
			GLframebuffer *readBuffer );

   /**
    * Called by glLockArraysEXT().
    */
   void (*LockArraysEXT)( GLcontext *ctx, GLint first, GLsizei count );
   /**
    * Called by UnlockArraysEXT().
    */
   void (*UnlockArraysEXT)( GLcontext *ctx );
   /*@}*/

};


/**
 * Transform/Clip/Lighting interface
 *
 * Drivers present a reduced set of the functions possible in
 * glBegin()/glEnd() objects.  Core mesa provides translation stubs for the
 * remaining functions to map down to these entry points.
 *
 * These are the initial values to be installed into dispatch by
 * mesa.  If the T&L driver wants to modify the dispatch table
 * while installed, it must do so itself.  It would be possible for
 * the vertexformat to install it's own initial values for these
 * functions, but this way there is an obvious list of what is
 * expected of the driver.
 *
 * If the driver wants to hook in entry points other than those
 * listed, it must restore them to their original values in
 * the disable() callback, below.
 */
typedef struct {
   /**
    * \name Vertex
    */
   /*@{*/
   void (GLAPIENTRYP ArrayElement)( GLint ); /* NOTE */
   void (GLAPIENTRYP Color3f)( GLfloat, GLfloat, GLfloat );
   void (GLAPIENTRYP Color3fv)( const GLfloat * );
   void (GLAPIENTRYP Color4f)( GLfloat, GLfloat, GLfloat, GLfloat );
   void (GLAPIENTRYP Color4fv)( const GLfloat * );
   void (GLAPIENTRYP EdgeFlag)( GLboolean );
   void (GLAPIENTRYP EdgeFlagv)( const GLboolean * );
   void (GLAPIENTRYP EvalCoord1f)( GLfloat );          /* NOTE */
   void (GLAPIENTRYP EvalCoord1fv)( const GLfloat * ); /* NOTE */
   void (GLAPIENTRYP EvalCoord2f)( GLfloat, GLfloat ); /* NOTE */
   void (GLAPIENTRYP EvalCoord2fv)( const GLfloat * ); /* NOTE */
   void (GLAPIENTRYP EvalPoint1)( GLint );             /* NOTE */
   void (GLAPIENTRYP EvalPoint2)( GLint, GLint );      /* NOTE */
   void (GLAPIENTRYP FogCoordfEXT)( GLfloat );
   void (GLAPIENTRYP FogCoordfvEXT)( const GLfloat * );
   void (GLAPIENTRYP Indexf)( GLfloat );
   void (GLAPIENTRYP Indexfv)( const GLfloat * );
   void (GLAPIENTRYP Materialfv)( GLenum face, GLenum pname, const GLfloat * ); /* NOTE */
   void (GLAPIENTRYP MultiTexCoord1fARB)( GLenum, GLfloat );
   void (GLAPIENTRYP MultiTexCoord1fvARB)( GLenum, const GLfloat * );
   void (GLAPIENTRYP MultiTexCoord2fARB)( GLenum, GLfloat, GLfloat );
   void (GLAPIENTRYP MultiTexCoord2fvARB)( GLenum, const GLfloat * );
   void (GLAPIENTRYP MultiTexCoord3fARB)( GLenum, GLfloat, GLfloat, GLfloat );
   void (GLAPIENTRYP MultiTexCoord3fvARB)( GLenum, const GLfloat * );
   void (GLAPIENTRYP MultiTexCoord4fARB)( GLenum, GLfloat, GLfloat, GLfloat, GLfloat );
   void (GLAPIENTRYP MultiTexCoord4fvARB)( GLenum, const GLfloat * );
   void (GLAPIENTRYP Normal3f)( GLfloat, GLfloat, GLfloat );
   void (GLAPIENTRYP Normal3fv)( const GLfloat * );
   void (GLAPIENTRYP SecondaryColor3fEXT)( GLfloat, GLfloat, GLfloat );
   void (GLAPIENTRYP SecondaryColor3fvEXT)( const GLfloat * );
   void (GLAPIENTRYP TexCoord1f)( GLfloat );
   void (GLAPIENTRYP TexCoord1fv)( const GLfloat * );
   void (GLAPIENTRYP TexCoord2f)( GLfloat, GLfloat );
   void (GLAPIENTRYP TexCoord2fv)( const GLfloat * );
   void (GLAPIENTRYP TexCoord3f)( GLfloat, GLfloat, GLfloat );
   void (GLAPIENTRYP TexCoord3fv)( const GLfloat * );
   void (GLAPIENTRYP TexCoord4f)( GLfloat, GLfloat, GLfloat, GLfloat );
   void (GLAPIENTRYP TexCoord4fv)( const GLfloat * );
   void (GLAPIENTRYP Vertex2f)( GLfloat, GLfloat );
   void (GLAPIENTRYP Vertex2fv)( const GLfloat * );
   void (GLAPIENTRYP Vertex3f)( GLfloat, GLfloat, GLfloat );
   void (GLAPIENTRYP Vertex3fv)( const GLfloat * );
   void (GLAPIENTRYP Vertex4f)( GLfloat, GLfloat, GLfloat, GLfloat );
   void (GLAPIENTRYP Vertex4fv)( const GLfloat * );
   void (GLAPIENTRYP CallList)( GLuint );	/* NOTE */
   void (GLAPIENTRYP CallLists)( GLsizei, GLenum, const GLvoid * );	/* NOTE */
   void (GLAPIENTRYP Begin)( GLenum );
   void (GLAPIENTRYP End)( void );
   void (GLAPIENTRYP VertexAttrib1fNV)( GLuint index, GLfloat x );
   void (GLAPIENTRYP VertexAttrib1fvNV)( GLuint index, const GLfloat *v );
   void (GLAPIENTRYP VertexAttrib2fNV)( GLuint index, GLfloat x, GLfloat y );
   void (GLAPIENTRYP VertexAttrib2fvNV)( GLuint index, const GLfloat *v );
   void (GLAPIENTRYP VertexAttrib3fNV)( GLuint index, GLfloat x, GLfloat y, GLfloat z );
   void (GLAPIENTRYP VertexAttrib3fvNV)( GLuint index, const GLfloat *v );
   void (GLAPIENTRYP VertexAttrib4fNV)( GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w );
   void (GLAPIENTRYP VertexAttrib4fvNV)( GLuint index, const GLfloat *v );
   /*@}*/

   /*
    */
   void (GLAPIENTRYP Rectf)( GLfloat, GLfloat, GLfloat, GLfloat );

   /**
    * \name Array
    */
   /*@{*/
   void (GLAPIENTRYP DrawArrays)( GLenum mode, GLint start, GLsizei count );
   void (GLAPIENTRYP DrawElements)( GLenum mode, GLsizei count, GLenum type,
			 const GLvoid *indices );
   void (GLAPIENTRYP DrawRangeElements)( GLenum mode, GLuint start,
			      GLuint end, GLsizei count,
			      GLenum type, const GLvoid *indices );
   /*@}*/

   /**
    * \name Eval
    *
    * If you don't support eval, fallback to the default vertex format
    * on receiving an eval call and use the pipeline mechanism to
    * provide partial T&L acceleration.
    *
    * Mesa will provide a set of helper functions to do eval within
    * accelerated vertex formats, eventually...
    */
   /*@{*/
   void (GLAPIENTRYP EvalMesh1)( GLenum mode, GLint i1, GLint i2 );
   void (GLAPIENTRYP EvalMesh2)( GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2 );
   /*@}*/

} GLvertexformat;


#endif /* DD_INCLUDED */
