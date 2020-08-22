/**********************************************************************************
* 
* VertexBufferObject.h
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

#ifndef __JAM_VERTEX_BUFFER_OBJECT_H__
#define __JAM_VERTEX_BUFFER_OBJECT_H__

#include <GL/glew.h>

#include <jam/jam.h>

namespace jam
{

/**
*	Encapsulates OpenGL VBO
*/
class JAM_API VertexBufferObject
{
public:
	// target is OpenGL target, e.g. GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER, etc.
							VertexBufferObject( GLenum target = GL_ARRAY_BUFFER ) ;
	virtual					~VertexBufferObject() ;

	void					setTarget( GLenum target ) ;
	GLenum					getTarget() const ;

	void					create() ;
	void					destroy() ;

	void					bind() ;
	void					unbind() ;

	void					bufferData( GLsizeiptr size, const GLvoid * data, GLenum usage = GL_STATIC_DRAW ) ;
		
private:

private:
	GLuint					m_vbo ;
	GLenum					m_type ;
};			

JAM_INLINE void VertexBufferObject::setTarget( GLenum target ) { m_type = target; }
JAM_INLINE GLenum VertexBufferObject::getTarget() const { return m_type; }

}

#endif

