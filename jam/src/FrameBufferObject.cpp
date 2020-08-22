/**********************************************************************************
* 
* FrameBufferObject.cpp
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
 
#include "stdafx.h"

#include "jam/FrameBufferObject.h"
#include "jam/RenderBufferObject.h"
#include "jam/Texture2D.h"

namespace jam
{

	FrameBufferObject::FrameBufferObject( GLenum target /* = GL_FRAMEBUFFER */ ) : m_fbo(0), m_target(target)
	{
	}

	FrameBufferObject::~FrameBufferObject()
	{
		if( m_fbo ) {
			destroy() ;
			m_fbo = 0 ;
		}
	}

	void FrameBufferObject::create()
	{
		glGenFramebuffers(1,&m_fbo) ;
	}
	
	void FrameBufferObject::destroy()
	{
		glDeleteFramebuffers(1, &m_fbo);
	}
 
	void FrameBufferObject::bind()
	{
		if( m_fbo == 0 ) {
			create() ;
		}
	 	glBindFramebuffer(m_target,m_fbo) ;
	}

	void FrameBufferObject::unbind()
	{
	 	glBindFramebuffer(m_target,0) ;
	}

	void FrameBufferObject::attachRenderBuffer( RenderBufferObject * rbo, GLenum attachment )
	{
		glFramebufferRenderbuffer( m_target, attachment, GL_RENDERBUFFER, rbo->getId() ) ;
	}

	void FrameBufferObject::attachTexture2D( const Ref<Texture2D>& tex, GLenum attachment, GLint level )
	{
		glFramebufferTexture2D( m_target, attachment, GL_TEXTURE_2D, tex->getId(), level ) ;
	}

	bool FrameBufferObject::isGood() const
	{
		return checkStatus() == GL_FRAMEBUFFER_COMPLETE ; 
	}

	GLenum FrameBufferObject::checkStatus() const
	{
		return glCheckFramebufferStatus(m_target) ;
	}

	void FrameBufferObject::invalidate( GLsizei numAttachments, const GLenum *attachments )
	{
		glInvalidateFramebuffer( m_target, numAttachments, attachments ) ;
	}

	void FrameBufferObject::invalidateRegion( GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height )
	{
		glInvalidateSubFramebuffer( m_target, numAttachments, attachments, x, y, width, height ) ;
	}

}
