/**********************************************************************************
* 
* VertexBufferObject.cpp
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

#include "jam/VertexBufferObject.h"

namespace jam
{

	VertexBufferObject::VertexBufferObject( GLenum target /*= GL_ARRAY_BUFFER*/ ) : m_vbo(0), m_type(target)
	{
	}

	VertexBufferObject::~VertexBufferObject()
	{
		if( m_vbo ) {
			destroy() ;
			m_vbo = 0 ;
		}
	}

	void VertexBufferObject::create()
	{
		glGenBuffers(1,&m_vbo) ;
	}

	void VertexBufferObject::destroy()
	{
		glDeleteBuffers(1, &m_vbo);
		m_vbo = 0 ;
	}
	
	void VertexBufferObject::bind()
	{
		if( m_vbo == 0 ) {
			create() ;
		}
		glBindBuffer( m_type, m_vbo ) ;
	}

	void VertexBufferObject::unbind()
	{
		glBindBuffer(m_type,0) ;
	}

	void VertexBufferObject::bufferData( GLsizeiptr size, const GLvoid * data, GLenum usage /*= GL_STATIC_DRAW*/ )
	{
		// creates and initializes a buffer object's data store
		glBufferData( m_type, size, data, usage) ;
	}

	void VertexBufferObject::bufferSubData( GLintptr offset, GLsizeiptr size, const GLvoid * data )
	{
		// updates a subset of a buffer object's data store
		glBufferSubData( m_type, offset, size, data ) ;
	}

}
