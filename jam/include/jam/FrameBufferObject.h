/**********************************************************************************
* 
* FrameBufferObject.h
* 
* This file is part of Jam
* 
* Copyright (c) 2014-2019 Giovanni Zito.
* Copyright (c) 2014-2019 Jam contributors (cf. AUTHORS.md)
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
* 
**********************************************************************************/

#ifndef __JAM_FRAME_BUFFER_OBJECT_H__
#define __JAM_FRAME_BUFFER_OBJECT_H__

#include <GL/glew.h>

#include <jam/jam.h>
#include <jam/RefCountedObject.h>

namespace jam
{
class RenderBufferObject ;
class Texture2D ;

/**
*	Encapsulates OpenGL FBO
*/
class JAM_API FrameBufferObject
{
public:
							FrameBufferObject( GLenum target = GL_FRAMEBUFFER ) ;
	virtual					~FrameBufferObject() ;

	void					create() ;
	void					destroy() ;

	void					bind() ;
	void					unbind() ;

	void					attachRenderBuffer( RenderBufferObject* rbo, GLenum attachment ) ;
	void					attachTexture2D( const Ref<Texture2D>& tex, GLenum attachment, GLint level ) ;

	bool					isGood() const ;
	GLenum					checkStatus() const ;

	void					invalidate( GLsizei numAttachments, const GLenum *attachments ) ;
	void					invalidateRegion( GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height ) ;

	GLuint					getId() const ;
	GLenum					getTarget() const ;

private:
	GLuint					m_fbo ;
	GLenum					m_target ;
};			

JAM_INLINE GLuint FrameBufferObject::getId() const { return m_fbo; }
JAM_INLINE GLuint FrameBufferObject::getTarget() const { return m_target; }

}

#endif

