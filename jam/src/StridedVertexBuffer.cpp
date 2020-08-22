/**********************************************************************************
* 
* StridedVertexBuffer.cpp
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

#include <jam/StridedVertexBuffer.h>
#include <jam/Draw3dManager.h>
#include "jam/Gfx.h"

using namespace std ;


namespace jam
{

//
// Draw3DVertexBuffer
//

const U16	StridedVertexBuffer::DefaultMaxVertexCount = 32768;		// 8192 quads

StridedVertexBuffer::StridedVertexBuffer( U16 vertexCount /*= Draw3DStream::DefaultMaxVertexCount*/, U16 indexCount /*= 0*/ ) :
	m_vertices(0),
	m_index(0),
	m_vertexCount(0),
	m_indexCount(0),
	m_startVertexCount(0),
	m_startIndexCount(0),
	m_maxVertexCount(vertexCount),
	m_maxIndexCount(0),
	m_vbo(0),
	IVertexBuffer()
{
	if( indexCount == 0 ) {
		// default to triangles list
		m_maxIndexCount = m_maxVertexCount + (m_maxVertexCount/2) ;
	}
	else {
		m_maxIndexCount = indexCount ;
	}

	m_vertices = new V3F_C4B_T2F[m_maxVertexCount] ;
	m_index = new U16[m_maxIndexCount] ;

#ifdef JAM_TRACE_BATCH
	JAM_TRACE( "Allocing new vertex buffer(%d,%d)",m_maxVertexCount,m_maxIndexCount ) ;
#endif
}


StridedVertexBuffer::~StridedVertexBuffer()
{
	if( m_ebo ) {
		glDeleteBuffers(1, &m_ebo);
		m_ebo = 0 ;
	}

	if( m_vbo ) {
	    glDeleteBuffers(1, &m_vbo);
		m_vbo = 0 ;
	}

	if( m_vao ) {
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0 ;
	}

	JAM_DELETE(m_vertices) ;
	JAM_DELETE(m_index) ;
}


void StridedVertexBuffer::addQuad(float x1,float y1,float x2,float y2,uint32_t diffuse,float u1,float v1,float u2,float v2)
{
	int16_t vert0 = addVertex( x1, y1, diffuse, u1, v1 ) ;
	int16_t vert1 = addVertex( x2, y1, diffuse, u2, v1 ) ;
	int16_t vert2 = addVertex( x2, y2, diffuse, u2, v2 ) ;
	int16_t vert3 = addVertex( x1, y2, diffuse, u1, v2 ) ;
	addQuadIndices( vert0, vert1, vert2, vert3 ) ;
}


void StridedVertexBuffer::addQuad( const Polygon2f& poly, uint32_t diffuse, float u1, float v1, float u2, float v2 )
{
	U16 vert0 = addVertex( poly.getVertex(0).x, poly.getVertex(0).y, diffuse, u1, v1 ) ;
	U16 vert1 = addVertex( poly.getVertex(1).x, poly.getVertex(1).y, diffuse, u2, v1 ) ;
	U16 vert2 = addVertex( poly.getVertex(2).x, poly.getVertex(2).y, diffuse, u2, v2 ) ;
	U16 vert3 = addVertex( poly.getVertex(3).x, poly.getVertex(3).y, diffuse, u1, v2 ) ;
	addQuadIndices( vert0, vert1, vert2, vert3 ) ;
}

U16 StridedVertexBuffer::addVertex( float x, float y, uint32_t c, float tu/*=0.0f*/, float tv/*=0.0f */ )
{
	U16 idx = m_startVertexCount + m_vertexCount ;
	JAM_ASSERT_MSG( (idx<m_maxVertexCount), "StridedVertexBuffer overflow: vertex count: %d", idx+1 ) ;

	m_vertices[idx].vertex.x = x ;
	m_vertices[idx].vertex.y = y ;
	m_vertices[idx].vertex.z = 0 ;
	m_vertices[idx].color = c ;
	m_vertices[idx].texCoords.x = tu ;
	m_vertices[idx].texCoords.y = tv ;
	m_vertexCount++ ;
	return idx++ ;
}

void StridedVertexBuffer::addIndex( U16 v )
{
	U16 idx = m_startIndexCount + m_indexCount ;
	JAM_ASSERT_MSG( ((idx)<m_maxIndexCount), "StridedVertexBuffer overflow: index count: %d", idx+1 ) ;
	m_index[idx] = v ;
	m_indexCount ++ ;
}


void StridedVertexBuffer::addTriIndices( U16 v0, U16 v1, U16 v2 )
{
	U16 idx = m_startIndexCount + m_indexCount ;
	JAM_ASSERT_MSG( ((idx+2)<m_maxIndexCount), "StridedVertexBuffer overflow: index count: %d", idx+3 ) ;
	m_index[idx] = v0 ;
	m_index[idx+1] = v2 ;
	m_index[idx+2] = v1 ;
	m_indexCount += 3 ;
}


void StridedVertexBuffer::addQuadIndices( U16 v0, U16 v1, U16 v2, int16_t v3 )
{
	U16 idx = m_startIndexCount + m_indexCount ;
	JAM_ASSERT_MSG( ((idx+5)<m_maxIndexCount), "StridedVertexBuffer overflow: index count: %d", idx+6 ) ;
	m_index[idx] = v0 ;
	m_index[idx+1] = v1 ;
	m_index[idx+2] = v2 ;
	m_index[idx+3] = v0 ;
	m_index[idx+4] = v2 ;
	m_index[idx+5] = v3 ;
	m_indexCount += 6 ;
}

void StridedVertexBuffer::reset()
{
	m_indexCount = 0 ;
	m_vertexCount = 0 ;
	m_startVertexCount = 0 ;
	m_startIndexCount = 0 ;
}

void StridedVertexBuffer::setNewBlock()
{
	if( ((m_startVertexCount+m_vertexCount)>=m_maxVertexCount) || ((m_startIndexCount+m_indexCount)>=m_maxIndexCount) ) {
		JAM_ERROR( "Buffer overflow in StridedVertexBuffer::setNewBlock()" ) ;
	}

	m_startVertexCount += m_vertexCount ;
	m_startIndexCount += m_indexCount ;
	m_vertexCount = 0 ;
	m_indexCount = 0 ;
}

bool StridedVertexBuffer::isSpaceAvailable( U16 vertexCount, U16 indexCount ) const
{
	return ((m_startVertexCount + vertexCount) < m_maxVertexCount) && ((m_startIndexCount + indexCount) < m_maxIndexCount) ;
}

void StridedVertexBuffer::upload()
{
	JAM_ASSERT(!m_uploaded) ;

	Shader* p = ShaderManager::getSingleton().getCurrent() ;

	// create VAO
	glGenVertexArrays( 1, &m_vao ) ;
	// create VBO
	glGenBuffers( 1, &m_vbo ) ;
	// create EBO
	glGenBuffers( 1, &m_ebo ) ;

	// bind vao
	// all glBindBuffer, glVertexAttribPointer, and glEnableVertexAttribArray calls are tracked and stored in the vao
 	glBindVertexArray( m_vao ) ;
	// upload vertices
	glBindBuffer( GL_ARRAY_BUFFER, m_vbo ) ;
	glBufferData( GL_ARRAY_BUFFER, getMaxNumOfVertices() * sizeof(V3F_C4B_T2F), getVertexArray(), GL_DYNAMIC_DRAW ) ;

	glVertexAttribPointer( p->attrib(JAM_PROGRAM_ATTRIB_POSITION), 3, GL_FLOAT,	GL_FALSE, sizeof(V3F_C4B_T2F), (void*)0 ) ;
	glEnableVertexAttribArray( p->attrib(JAM_PROGRAM_ATTRIB_POSITION) ) ;

	glVertexAttribPointer( p->attrib(JAM_PROGRAM_ATTRIB_COLOR), 4, GL_UNSIGNED_BYTE, GL_TRUE,  sizeof(V3F_C4B_T2F), (void*)sizeof(Vertex3f) ) ;
	glEnableVertexAttribArray( p->attrib(JAM_PROGRAM_ATTRIB_COLOR) ) ;

	glVertexAttribPointer( p->attrib(JAM_PROGRAM_ATTRIB_TEXCOORDS), 2, GL_FLOAT,	GL_FALSE, sizeof(V3F_C4B_T2F), (void*)(sizeof(Vertex3f) + sizeof(Color)) ) ;
	glEnableVertexAttribArray( p->attrib(JAM_PROGRAM_ATTRIB_TEXCOORDS) ) ;

	// upload indices
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ebo ) ;
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, getMaxNumOfIndices() * sizeof(U16), getIndexArray(), GL_DYNAMIC_DRAW ) ;

	// unbind vao
 	glBindVertexArray( 0 ) ;

	// unbind vbo and ebo
	glBindBuffer( GL_ARRAY_BUFFER, 0 ) ;
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ) ;

	m_uploaded = true ;
}

	void StridedVertexBuffer::bindVao()
	{
		if( !m_uploaded ) {
			upload() ;
		}
		JAM_ASSERT_MSG( m_vao!=0, "StridedVertexBuffer is not uploaded" ) ;
		glBindVertexArray(m_vao) ;
	}
	
	void StridedVertexBuffer::unbindVao()
	{
		glBindVertexArray(0) ;
	}

	void StridedVertexBuffer::update()
	{
		glBindBuffer( GL_ARRAY_BUFFER, m_vbo ) ;
		glBufferSubData( GL_ARRAY_BUFFER, (GLintptr)(m_startVertexCount * sizeof(V3F_C4B_T2F)), getNumOfVertices() * sizeof(V3F_C4B_T2F), getVertexArray() ) ;
		glBindBuffer( GL_ARRAY_BUFFER, 0 ) ;

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ebo ) ;
		glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, (GLintptr)(m_startIndexCount * sizeof(U16)), getNumOfIndices() * sizeof(U16), getIndexArray() ) ;
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ) ;
	}

}
