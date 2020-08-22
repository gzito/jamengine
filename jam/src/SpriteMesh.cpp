/**********************************************************************************
* 
* SpriteMesh.cpp
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

#include "jam/SpriteMesh.h"

using namespace std ;


namespace jam
{

SpriteMesh::SpriteMesh() :
	m_pMaterial(0),
	m_vertices(0),
	m_texCoords(0),
	m_elements(0),
	m_verticesVbo(),
	m_texCoordsVbo(),
	m_elementsVbo(),
	m_vao(),
	m_uploaded(false)
{
	m_pMaterial = Ref<Material>(new Material()) ;
	m_verticesVbo.setTarget(GL_ARRAY_BUFFER) ;
	m_texCoordsVbo.setTarget(GL_ARRAY_BUFFER) ;
	m_elementsVbo.setTarget(GL_ELEMENT_ARRAY_BUFFER) ;
}

SpriteMesh::~SpriteMesh()
{
}

void SpriteMesh::setMaterial( const Ref<Material>& pMaterial )
{
	m_pMaterial = pMaterial ;
}
	
void SpriteMesh::create(int numOfVertices, int numOfElements)
{
	m_vertices.create( numOfVertices ) ;
	m_texCoords.create( numOfVertices ) ;
	m_elements.create( numOfElements ) ;
}

void SpriteMesh::destroy()
{
	m_vertices.destroy() ;
	m_texCoords.destroy() ;
	m_elements.destroy() ;
	m_verticesVbo.destroy() ;
	m_texCoordsVbo.destroy() ;
	m_elementsVbo.destroy() ;
	m_vao.destroy() ;
	m_uploaded = false ;
}

void SpriteMesh::upload()
{
	if( m_uploaded ) {
		return ;
	}

	Ref<Shader> pShader = m_pMaterial->getShader() ;
	pShader->use();

	// bind vao
	// all glBindBuffer, glVertexAttribPointer, and glEnableVertexAttribArray calls are tracked and stored in the vao
	m_vao.bind() ;

	// upload vertices
	m_verticesVbo.bind() ;
	m_verticesVbo.bufferData( m_vertices.byteSize(), m_vertices.data() ) ;
	pShader->setVertexAttribPointer(JAM_PROGRAM_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0) ;
	pShader->enableVertexAttribArray(JAM_PROGRAM_ATTRIB_POSITION) ;

	// upload texture coordinates
	m_texCoordsVbo.bind() ;
	m_texCoordsVbo.bufferData( m_texCoords.byteSize(), m_texCoords.data() ) ;
	pShader->setVertexAttribPointer(JAM_PROGRAM_ATTRIB_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0) ;
	pShader->enableVertexAttribArray(JAM_PROGRAM_ATTRIB_TEXCOORDS) ;

	// upload indices
	m_elementsVbo.bind() ;
	m_elementsVbo.bufferData( m_elements.byteSize(), m_elements.data() ) ;

	// unbind vao
 	m_vao.unbind() ;

	// unbind ARRAY_BUFFER and ELEMENT_ARRAY_BUFFER
	m_verticesVbo.unbind() ;	// also unbind m_texCoordsVbo
	m_elementsVbo.unbind() ;

	m_uploaded = true ;
}

void SpriteMesh::set( float hw, float hh )
{
	//   3 -------- 2
    //   |       /  |
    //   |     /    |
    //   |   /      |
    //   | /        |
	//   0 -------- 1
	//
	//   triangles are in counter-clockwise order

	m_vertices[0] = Vector2( -hw, -hh ) ;
	m_vertices[1] = Vector2(  hw, -hh ) ;
	m_vertices[2] = Vector2(  hw,  hh ) ;
	m_vertices[3] = Vector2( -hw,  hh ) ;

	m_texCoords[0] = Vector2( 0, 0 ) ;
	m_texCoords[1] = Vector2( 1, 0 ) ;
	m_texCoords[2] = Vector2( 1, 1 ) ;
	m_texCoords[3] = Vector2( 0, 1 ) ;

	m_elements[0] = 0 ;
	m_elements[1] = 1 ;
	m_elements[2] = 2 ;
	m_elements[3] = 0 ;
	m_elements[4] = 2 ;
	m_elements[5] = 3 ;
}

}
