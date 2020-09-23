/**********************************************************************************
* 
* Mesh.cpp
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

#include <jam/Mesh.h>
#include <jam/Gfx.h>
#include <jam/Shader.h>

namespace jam
{

//*******************
//
// Class Mesh
//
//*******************

Mesh::Mesh() :
	m_pMaterial(0),
	m_vertices(0),
	m_normals(0), 
	m_texCoords(0),
	m_tangents(0),
	m_bitangents(0),
	m_elements(0),
	m_vbos(),
	m_elementsVbo(),
	m_vao(),
	m_uploaded(false),
	m_tangentsDisabled(true),
	m_needsCalculateTangents(true)
{
	m_pMaterial = new Material() ;
	for( size_t i=0; i<m_vbos.length()-1; i++ ) {
		m_vbos[i].setTarget(GL_ARRAY_BUFFER) ;
	}
	m_elementsVbo.setTarget(GL_ELEMENT_ARRAY_BUFFER) ;
}

Mesh::~Mesh()
{
}

void Mesh::setMaterial( Material* pMaterial )
{
	m_pMaterial.assign( pMaterial, true ) ;
}
	
void Mesh::create(int numOfVertices, int numOfElements)
{
	m_vertices.create( numOfVertices ) ;
	m_normals.create( numOfVertices ) ;
	m_texCoords.create( numOfVertices ) ;
	m_tangents.create( numOfVertices ) ;
	m_bitangents.create( numOfVertices ) ;
	m_elements.create( numOfElements ) ;
}

void Mesh::destroy()
{
	for( size_t i=0; i<m_vbos.length(); i++ ) {
		m_vbos[i].destroy() ;
	}
	m_elementsVbo.destroy() ;
	m_vao.destroy() ;

	m_vertices.destroy() ;
	m_normals.destroy() ;
	m_texCoords.destroy() ;
	m_tangents.destroy() ;
	m_bitangents.destroy() ;
	m_elements.destroy() ;
}

void Mesh::upload()
{
	if( m_uploaded ) {
		return ;
	}

	if( !m_tangentsDisabled && m_needsCalculateTangents ) {
		calculateTangents() ;
	}
	
	Shader* pShader = m_pMaterial->getShader() ;
	pShader->use();

	for( size_t i=0; i<m_vbos.length(); i++ ) {
		m_vbos[i].create() ;
	}

	m_vao.create() ;

	// bind vao
	// all glBindBuffer, glVertexAttribPointer, and glEnableVertexAttribArray calls are tracked and stored in the vao
	m_vao.bind() ;

	int vboIdx = 0 ;

	// upload vertices
	m_vbos[vboIdx].bind() ;
	m_vbos[vboIdx].bufferData( m_vertices.byteSize(), m_vertices.data() ) ;
	pShader->setVertexAttribPointer(JAM_PROGRAM_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0) ;
	pShader->enableVertexAttribArray(JAM_PROGRAM_ATTRIB_POSITION) ;

	// upload normals
	vboIdx++;
	m_vbos[vboIdx].bind() ;
	m_vbos[vboIdx].bufferData( m_normals.byteSize(), m_normals.data() ) ;
	pShader->setVertexAttribPointer(JAM_PROGRAM_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0) ;
	pShader->enableVertexAttribArray(JAM_PROGRAM_ATTRIB_NORMAL) ;

	// upload texture coordinates
	vboIdx++;
	m_vbos[vboIdx].bind() ;
	m_vbos[vboIdx].bufferData( m_texCoords.byteSize(), m_texCoords.data() ) ;
	pShader->setVertexAttribPointer(JAM_PROGRAM_ATTRIB_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0) ;
	pShader->enableVertexAttribArray(JAM_PROGRAM_ATTRIB_TEXCOORDS) ;

	// upload tangents
	if( !m_tangentsDisabled ) {
		vboIdx++;
		m_vbos[vboIdx].bind() ;
		m_vbos[vboIdx].bufferData( m_tangents.byteSize(), m_tangents.data() ) ;
		pShader->setVertexAttribPointer(JAM_PROGRAM_ATTRIB_TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0) ;
		pShader->enableVertexAttribArray(JAM_PROGRAM_ATTRIB_TANGENT) ;
	}

	// upload indices
	m_elementsVbo.bind() ;
	m_elementsVbo.bufferData( m_elements.byteSize(), m_elements.data() ) ;

	// unbind vao
 	m_vao.unbind() ;

	// unbind ARRAY_BUFFER and ELEMENT_ARRAY_BUFFER
	m_vbos[vboIdx].unbind() ;
	m_elementsVbo.unbind() ;

	m_uploaded = true ;
}

void Mesh::draw()
{
	if( !isUploaded() ) {
		upload() ;
	}
	GetGfx().drawIndexedPrimitive( &m_vao, getElementsArray().length(), m_pMaterial ) ;
}

void jam::Mesh::disableTangents( bool value )
{
	m_tangentsDisabled = value ;
}

void Mesh::calculateTangents()
{
	m_tangents.setZero() ;
	m_bitangents.setZero() ;

	U16* triangle = m_elements.data() ;
	Vector3* vertex = m_vertices.data() ;
	Vector2* texcoord = m_texCoords.data() ;
	for (U16 a = 0; a < m_elements.length(); a += 3)
	{
		U16 i1 = triangle[a];
		U16 i2 = triangle[a+1];
		U16 i3 = triangle[a+2];

		const Vector3& v1 = vertex[i1];
		const Vector3& v2 = vertex[i2];
		const Vector3& v3 = vertex[i3];

		const Vector2& w1 = texcoord[i1];
		const Vector2& w2 = texcoord[i2];
		const Vector2& w3 = texcoord[i3];

		F32 x1 = v2.x - v1.x;
		F32 x2 = v3.x - v1.x;
		F32 y1 = v2.y - v1.y;
		F32 y2 = v3.y - v1.y;
		F32 z1 = v2.z - v1.z;
		F32 z2 = v3.z - v1.z;

		F32 s1 = w2.x - w1.x;
		F32 s2 = w3.x - w1.x;
		F32 t1 = w2.y - w1.y;
		F32 t2 = w3.y - w1.y;

		F32 r = 1.0f / (s1 * t2 - s2 * t1);
		Vector3 sdir( (t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r ) ;
		Vector3 tdir( (s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r ) ;

		m_tangents[i1] += sdir;
		m_tangents[i2] += sdir;
		m_tangents[i3] += sdir;

		m_bitangents[i1] += tdir;
		m_bitangents[i2] += tdir;
		m_bitangents[i3] += tdir;
	}

	m_needsCalculateTangents = false ;
}

void Mesh::doNotCalculateTangents()
{
	m_needsCalculateTangents = false ;
}

}
