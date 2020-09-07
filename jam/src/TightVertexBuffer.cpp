/**********************************************************************************
* 
* TightVertexBuffer.cpp
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

#include "jam/TightVertexBuffer.h"
#include "jam/Gfx.h"
#include "jam/Shader.h"

using namespace std ;


namespace jam
{

//
// Draw3DStream
//

const U16	TightVertexBuffer::DefaultMaxVertexCount = 4096 ;

TightVertexBuffer::TightVertexBuffer( U16 vertexCount /*= Draw3DStream::DefaultMaxVertexCount*/, U16 indexCount /*= 0*/ ) :
	m_vertex(0),
	m_normal(0),
	m_colors(0),
	m_UVs(0),
	m_index(0),
	m_vertexCount(0),
	m_indexCount(0),
	m_startVertexCount(0),
	m_startIndexCount(0),
	m_maxVertexCount(vertexCount),
	m_maxIndexCount(0),
	m_useNormals(true),
	m_useColors(true),
	m_useUVs(true)
{
	if( indexCount == 0 ) {
		// n. of indices for triangles list
		m_maxIndexCount = m_maxVertexCount + (m_maxVertexCount/2) ;
	}
	else {
		m_maxIndexCount = indexCount ;
	}

	m_vertex = new Vertex3f[m_maxVertexCount] ;
	m_normal = new Vertex3f[m_maxVertexCount] ;
	m_colors = new Color[m_maxVertexCount] ;
	m_UVs = new Vertex2f[m_maxVertexCount] ;
	m_index = new U16[m_maxIndexCount] ;

	for( uint8_t i=0; i<sizeof(m_vbo)/sizeof(GLuint); i++ ) { m_vbo[i] = 0; }
}

	TightVertexBuffer::TightVertexBuffer( const TightVertexBuffer& other ) :
		m_vertex(0),
		m_normal(0),
		m_colors(0),
		m_UVs(0),
		m_index(0),
		m_vertexCount(other.m_vertexCount),
		m_indexCount(other.m_indexCount),
		m_startVertexCount(other.m_startVertexCount),
		m_startIndexCount(other.m_startIndexCount),
		m_maxVertexCount(other.m_maxVertexCount),
		m_maxIndexCount(other.m_maxIndexCount),
		m_useNormals(other.m_useNormals),
		m_useColors(other.m_useColors),
		m_useUVs(other.m_useUVs),
		IVertexBuffer()
{
	m_vertex = new Vertex3f[m_maxVertexCount] ;
	m_normal = new Vertex3f[m_maxVertexCount] ;
	m_index = new U16[m_maxIndexCount] ;
	m_colors = new Color[m_maxVertexCount] ;
	m_UVs = new Vertex2f[m_maxVertexCount] ;

	memcpy( m_vertex, other.m_vertex, m_maxVertexCount*sizeof(Vertex3f) ) ;
	memcpy( m_normal, other.m_normal, m_maxVertexCount*sizeof(Vertex3f) ) ;
	memcpy( m_colors, other.m_vertex, m_maxVertexCount*sizeof(Color) ) ;
	memcpy( m_UVs, other.m_vertex, m_maxVertexCount*sizeof(Vertex2f) ) ;
	memcpy( m_index, other.m_index, m_maxIndexCount*sizeof(U16) ) ;

	for( uint8_t i=0; i<sizeof(m_vbo)/sizeof(GLuint); i++ ) { m_vbo[i] = 0; }
}

TightVertexBuffer::~TightVertexBuffer()
{
	if( m_ebo ) {
		glDeleteBuffers(1, &m_ebo);
		m_ebo = 0 ;
	}

	glDeleteBuffers(4, m_vbo);

	if( m_vao ) {
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0 ;
	}

	JAM_DELETE(m_vertex) ;
	JAM_DELETE(m_normal) ;
	JAM_DELETE(m_colors) ;
	JAM_DELETE(m_UVs) ;
	JAM_DELETE(m_index) ;
}


void TightVertexBuffer::useNormals(bool fNormals)
{
	m_useNormals = fNormals ;
}

void TightVertexBuffer::useColors(bool fColors)
{
	m_useColors = fColors ;
}

void TightVertexBuffer::useUVs(bool fUVs)
{
	m_useUVs = fUVs ;
}

int16_t TightVertexBuffer::addVertex( float x, float y, float z, float nx, float ny, float nz, uint32_t c, float tu/*=0.0f*/, float tv/*=0.0f */ )
{
	U16 idx = m_startVertexCount + m_vertexCount ;
	m_vertex[idx].x = x ;
	m_vertex[idx].y = y ;
	m_vertex[idx].z = z ;
	m_normal[idx].x = nx ;
	m_normal[idx].y = ny ;
	m_normal[idx].z = nz ;
	m_colors[idx] = c ;
	m_UVs[idx].x = tu ;
	m_UVs[idx].y = tv ;
	return m_vertexCount++ ;
}

void TightVertexBuffer::addIndex(U16 v)
{
	U16 idx = m_startIndexCount + m_indexCount ;
	m_index[idx++] = v ;
	m_indexCount += 1 ;
}

void TightVertexBuffer::addTriIndices( U16 v0, U16 v1, U16 v2 )
{
	U16 idx = m_startIndexCount + m_indexCount ;
	m_index[idx++] = v0 ;
	m_index[idx++] = v1 ;
	m_index[idx++] = v2 ;
	m_indexCount += 3 ;
}

void TightVertexBuffer::addQuadIndices( U16 v0, U16 v1, U16 v2, int16_t v3 )
{
	U16 idx = m_startIndexCount + m_indexCount ;
	m_index[idx++] = v0 ;
	m_index[idx++] = v1 ;
	m_index[idx++] = v2 ;
	m_index[idx++] = v0 ;
	m_index[idx++] = v2 ;
	m_index[idx++] = v3 ;
	m_indexCount += 6 ;
}

void TightVertexBuffer::reset()
{
	m_indexCount = 0 ;
	m_vertexCount = 0 ;
	m_startVertexCount = 0 ;
	m_startIndexCount = 0 ;
}

void TightVertexBuffer::setNewBlock()
{
	m_startVertexCount += m_vertexCount ;
	m_startIndexCount += m_indexCount ;
	m_vertexCount = 0 ;
	m_indexCount = 0 ;
}

bool TightVertexBuffer::isSpaceAvailable( U16 vertexCount, U16 indexCount ) const
{
	return ((m_startVertexCount + vertexCount) <= m_maxVertexCount) && ((m_startIndexCount + indexCount) <= m_maxIndexCount) ;
}


void TightVertexBuffer::upload()
{
	Shader* p = GetShaderMgr().getCurrent() ;

	// create VAO
	glGenVertexArrays(1,&m_vao) ;
	// create VBOs
	glGenBuffers(4,m_vbo) ;
	// create EBO
	glGenBuffers(1,&m_ebo) ;

	// bind vao
	// all glBindBuffer, glVertexAttribPointer, and glEnableVertexAttribArray calls are tracked and stored in the vao
 	glBindVertexArray(m_vao) ;

	// upload vertices
	glBindBuffer( GL_ARRAY_BUFFER, m_vbo[0] ) ;
	glBufferData( GL_ARRAY_BUFFER, getNumOfVertices() * sizeof(Vertex3f), getVertexArray(), GL_STATIC_DRAW ) ;
	glVertexAttribPointer( p->attrib(JAM_PROGRAM_ATTRIB_POSITION), 3, GL_FLOAT,	GL_FALSE, sizeof(Vertex3f), (void*)0 ) ;
	glEnableVertexAttribArray( p->attrib(JAM_PROGRAM_ATTRIB_POSITION) ) ;

	if( m_useNormals ) {
		// upload normals
		glBindBuffer( GL_ARRAY_BUFFER, m_vbo[1] ) ;
		glBufferData( GL_ARRAY_BUFFER, getNumOfVertices() * sizeof(Vertex3f), getNormalArray(), GL_STATIC_DRAW ) ;
		glVertexAttribPointer( p->attrib(JAM_PROGRAM_ATTRIB_NORMAL), 3, GL_FLOAT,	GL_FALSE, sizeof(Vertex3f), (void*)0 ) ;
		glEnableVertexAttribArray( p->attrib(JAM_PROGRAM_ATTRIB_NORMAL) ) ;
	}

	if( m_useColors ) {
		// upload colors
		glBindBuffer( GL_ARRAY_BUFFER, m_vbo[2] ) ;
		glBufferData( GL_ARRAY_BUFFER, getNumOfVertices() * sizeof(Color), getColorArray(), GL_STATIC_DRAW ) ;
		glVertexAttribPointer( p->attrib(JAM_PROGRAM_ATTRIB_COLOR), 4, GL_UNSIGNED_BYTE, GL_TRUE,  sizeof(Color), (void*)0  ) ;
		glEnableVertexAttribArray( p->attrib(JAM_PROGRAM_ATTRIB_COLOR) ) ;
	}

	if( m_useUVs ) {
		// upload texture coordinates
		glBindBuffer( GL_ARRAY_BUFFER, m_vbo[3] ) ;
		glBufferData( GL_ARRAY_BUFFER, getNumOfVertices() * sizeof(Vertex2f), getUVArray(), GL_STATIC_DRAW ) ;
		glVertexAttribPointer(  p->attrib(JAM_PROGRAM_ATTRIB_TEXCOORDS), 2, GL_FLOAT,	GL_FALSE, sizeof(Vertex2f), (void*)0 ) ;
		glEnableVertexAttribArray( p->attrib(JAM_PROGRAM_ATTRIB_TEXCOORDS) ) ;
	}

	// upload indices
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ebo ) ;
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, getNumOfIndices() * sizeof(GLushort), getIndexArray(), GL_STATIC_DRAW ) ;

	// unbind vao
 	glBindVertexArray(0) ;

	// unbind vbo and ebo
	glBindBuffer(GL_ARRAY_BUFFER,0) ;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0) ;

	m_uploaded = true ;
}

	void TightVertexBuffer::bindVao()
	{
		if( !m_uploaded ) upload() ;
		JAM_ASSERT_MSG( m_vao!=0, "TightVertexBuffer is not uploaded" ) ;
		glBindVertexArray(m_vao) ;
	}
	
	void TightVertexBuffer::unbindVao()
	{
		glBindVertexArray(0) ;
	}

}
