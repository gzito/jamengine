/**********************************************************************************
* 
* StridedVertexBuffer.h
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

#ifndef __JAM_STRIDEDVERTEXBUFFER_H__
#define __JAM_STRIDEDVERTEXBUFFER_H__


#include <jam/jam.h>
#include <jam/RefCountedObject.h>
#include <jam/Draw2d.h>
#include <jam/IVertexBuffer.hpp>


namespace jam
{

/**
	Represent interleaved vertex buffer
	Vertex attributes are interleaved in array of struct
*/
class JAM_API StridedVertexBuffer : public IVertexBuffer
{
public:
							StridedVertexBuffer( U16 vertexCount = StridedVertexBuffer::DefaultMaxVertexCount, U16 indexCount = 0 ) ;
							~StridedVertexBuffer() ;

	V3F_C4B_T2F*			getVertexArray() { return &m_vertices[m_startVertexCount]; }
	U16*					getIndexArray() { return &m_index[m_startIndexCount]; }

	U16						getNumOfVertices() const override { return m_vertexCount; }
	U16						getNumOfIndices() const override { return m_indexCount; }

	U16						getMaxNumOfVertices() const { return m_maxVertexCount; } 
	U16						getMaxNumOfIndices() const { return m_maxIndexCount; }

	U16						addVertex( float x, float y, uint32_t c, float tu=0.0f, float tv=0.0f );
	void					addQuad( float x1, float y1, float x2, float y2, uint32_t diffuse, float u1, float v1, float u2, float v2 );
	void					addQuad( const Polygon2f& poly, uint32_t diffuse, float u1, float v1, float u2, float v2 );

	void					addIndex( U16 v ) ;
	void					addTriIndices( U16 v0, U16 v1, U16 v2 );
	void					addQuadIndices( U16 v0, U16 v1, U16 v2, int16_t v3 );

	void					setNewBlock() ;
	void					reset() ;

	bool					isSpaceAvailable( U16 vertexCount, U16 indexCount ) const ;

	void					upload() override ;
	void					bindVao() override ;
	void					unbindVao() override ;
	void					update() ;

	U16						getStartVertexCount() const ;
	U16						getStartIndexCount() const ;

public:
	static const U16		DefaultMaxVertexCount ;

protected:
	V3F_C4B_T2F*			m_vertices ;
	U16*					m_index ;

	U16						m_vertexCount ;
	U16						m_indexCount ;

	U16						m_startVertexCount ;
	U16						m_startIndexCount ;

	U16						m_maxVertexCount ;
	U16						m_maxIndexCount ;

	GLuint					m_vbo ;
};

JAM_INLINE U16 StridedVertexBuffer::getStartVertexCount() const { return m_startVertexCount; }
JAM_INLINE U16 StridedVertexBuffer::getStartIndexCount() const { return m_startIndexCount; }

}

#endif // __JAM_STRIDEDVERTEXBUFFER_H__
