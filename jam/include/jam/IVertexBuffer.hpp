/**********************************************************************************
* 
* IVertexBuffer.hpp
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

#ifndef __IWGE_IVERTEXBUFFER_H__
#define __IWGE_IVERTEXBUFFER_H__


#include <jam/jam.h>
#include <GL/glew.h>

namespace jam
{

class JAM_API IVertexBuffer
{
public:
	IVertexBuffer() : m_vao(0), m_ebo(0), m_uploaded(false) {} ;

	virtual uint16_t	getNumOfVertices() const = 0 ;
	virtual uint16_t	getNumOfIndices() const = 0 ;

	virtual void		upload() = 0 ;
	virtual void		bindVao() = 0 ;
	virtual void		unbindVao() = 0 ;
	bool				isUploaded() const { return m_uploaded; }

protected:
	GLuint				m_vao ;
	GLuint				m_ebo ;
	bool				m_uploaded ;
};

}

#endif // __IWGE_IVERTEXBUFFER_H__ 
