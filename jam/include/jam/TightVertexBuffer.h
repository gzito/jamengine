/**********************************************************************************
* 
* TightVertexBuffer.h
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

#ifndef __JAM_TIGHTVERTEXBUFFER_H__
#define __JAM_TIGHTVERTEXBUFFER_H__


#include <jam/jam.h>
#include <jam/Draw2d.h>
#include <jam/IVertexBuffer.hpp>

namespace jam
{

/**
	Represents tight packed vertex buffer
	Each vertex attribute is tight packed in its dedicated array
*/
class JAM_API TightVertexBuffer : public IVertexBuffer
{
public:
							TightVertexBuffer( U16 vertexCount = TightVertexBuffer::DefaultMaxVertexCount, U16 indexCount = 0 ) ;
							TightVertexBuffer( const TightVertexBuffer& other) ;
							~TightVertexBuffer() ;

	Vertex3f*				getVertexArray() ;
	Vertex3f*				getNormalArray() ;
	Color*					getColorArray() ;
	Vertex2f*				getUVArray() ;
	U16*					getIndexArray() ;

	U16						getNumOfVertices() const override ;
	U16						getNumOfIndices() const override ;

	U16						getMaxNumOfVertices() const ;
	U16						getMaxNumOfIndices() const ;

	void					useNormals( bool fNormals /*= true*/ ) ;
	void					useColors( bool fColors /*= true*/ ) ;
	void					useUVs( bool fUVs /*= true*/ ) ;

	int16_t					addVertex( float x, float y, float z, float nx, float ny, float nz, uint32_t c, float tu=0.0f, float tv=0.0f );
	void					addIndex( U16 v ) ;
	void					addTriIndices( U16 v0, U16 v1, U16 v2 );
	void					addQuadIndices( U16 v0, U16 v1, U16 v2, int16_t v3 );

	void					setNewBlock() ;
	void					reset() ;

	bool					isSpaceAvailable( U16 vertexCount, U16 indexCount ) const ;

	void					upload() override ;
	void					bindVao() override ;
	void					unbindVao() override ;

public:
	static const U16		DefaultMaxVertexCount ;

private:
	Vertex3f*				m_vertex ;
	Vertex3f*				m_normal ;
	Color*					m_colors ;
	Vertex2f*				m_UVs ;
	U16*					m_index ;

	U16						m_vertexCount ;
	U16						m_indexCount ;

	U16						m_startVertexCount ;
	U16						m_startIndexCount ;

	U16						m_maxVertexCount ;
	U16						m_maxIndexCount ;

	bool					m_useNormals ;
	bool					m_useColors ;
	bool					m_useUVs ;

	GLuint					m_vbo[4] ;
};

JAM_INLINE Vertex3f*		TightVertexBuffer::getVertexArray() { return &m_vertex[m_startVertexCount]; }
JAM_INLINE Vertex3f*		TightVertexBuffer::getNormalArray() { return &m_normal[m_startVertexCount]; }
JAM_INLINE Color*			TightVertexBuffer::getColorArray() { return &m_colors[m_startVertexCount]; }
JAM_INLINE Vertex2f*		TightVertexBuffer::getUVArray() { return &m_UVs[m_startVertexCount]; }
JAM_INLINE U16*				TightVertexBuffer::getIndexArray() { return &m_index[m_startIndexCount]; }

JAM_INLINE U16				TightVertexBuffer::getNumOfVertices() const { return m_vertexCount; }
JAM_INLINE U16				TightVertexBuffer::getNumOfIndices() const { return m_indexCount; }
 
JAM_INLINE U16				TightVertexBuffer::getMaxNumOfVertices() const { return m_maxVertexCount; } 
JAM_INLINE U16				TightVertexBuffer::getMaxNumOfIndices() const { return m_maxIndexCount; }

}

#endif // __JAM_TIGHTVERTEXBUFFER_H__ 
