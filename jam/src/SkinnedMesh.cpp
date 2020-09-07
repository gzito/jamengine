/**********************************************************************************
* 
* SkinnedMesh.cpp
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

#include <jam/SkinnedMesh.h>
#include <jam/Gfx.h>
#include <jam/Shader.h>


namespace jam
{

	//*******************
	//
	// Class SkinnedMesh
	//
	//*******************

	SkinnedMesh::SkinnedMesh() : 
		m_pMaterial(0), 
		m_vertices(0),
		m_normals(0), 
		m_texCoords(0),
		m_bonesId(0),
		m_weights(0),
		m_elements(0),
		m_vbos(),
		m_vao(),
		m_uploaded(false)
	{
		m_pMaterial = new Material() ;
		for( size_t i=0; i<m_vbos.length()-1; i++ ) { m_vbos[i].setTarget(GL_ARRAY_BUFFER) ; }
		m_vbos[m_vbos.length()-1].setTarget(GL_ELEMENT_ARRAY_BUFFER) ;
	}

	SkinnedMesh::~SkinnedMesh()
	{
		destroy() ;

		m_pMaterial = nullptr ;

		for( size_t i=0; i<m_vbos.length(); i++ ) {
			m_vbos[i].destroy() ;
		}
		m_vao.destroy() ;
	}

	void SkinnedMesh::create(size_t numOfVertices, size_t numOfElements)
	{
		m_vertices.create(numOfVertices) ;
		m_normals.create(numOfVertices) ;
		m_texCoords.create(numOfVertices) ;
		m_bonesId.create(numOfVertices) ;
		m_bonesId.setZero() ;
		m_weights.create(numOfVertices) ;
		m_weights.setZero() ;
		m_elements.create(numOfElements) ;
	}

	void SkinnedMesh::destroy()
	{
		m_vertices.destroy() ;
		m_normals.destroy() ;
		m_texCoords.destroy() ;
		m_bonesId.destroy() ;
		m_weights.destroy() ;
		m_elements.destroy() ;
	}

	void SkinnedMesh::upload()
	{
		if( m_uploaded ) {
			return ;
		}

		Shader* pProg = GetShaderMgr().getCurrent() ;

		for( size_t i=0; i<m_vbos.length(); i++ ) {
			m_vbos[i].create() ;
		}

		m_vao.create() ;

		// bind vao
		// all glBindBuffer, glVertexAttribPointer, and glEnableVertexAttribArray calls are tracked and stored in the vao
		m_vao.bind() ;

		// upload vertices
		m_vbos[0].bind() ;
		m_vbos[0].bufferData( m_vertices.byteSize(), m_vertices.data() ) ;
		pProg->setVertexAttribPointer(JAM_PROGRAM_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0) ;
		pProg->enableVertexAttribArray(JAM_PROGRAM_ATTRIB_POSITION) ;

		// upload normals
		m_vbos[1].bind() ;
		m_vbos[1].bufferData( m_normals.byteSize(), m_normals.data() ) ;
		pProg->setVertexAttribPointer(JAM_PROGRAM_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0) ;
		pProg->enableVertexAttribArray(JAM_PROGRAM_ATTRIB_NORMAL) ;

		// upload texture coordinates
		m_vbos[2].bind() ;
		m_vbos[2].bufferData( m_texCoords.byteSize(), m_texCoords.data() ) ;
		pProg->setVertexAttribPointer(JAM_PROGRAM_ATTRIB_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0) ;
		pProg->enableVertexAttribArray(JAM_PROGRAM_ATTRIB_TEXCOORDS) ;

		// upload bones ids
		m_vbos[3].bind() ;
		m_vbos[3].bufferData( m_bonesId.byteSize(), m_bonesId.data() ) ;
		pProg->setVertexAttribIntegerPointer(JAM_PROGRAM_ATTRIB_BONESID, 4, GL_INT, sizeof(glm::ivec4), 0) ;
		pProg->enableVertexAttribArray(JAM_PROGRAM_ATTRIB_BONESID) ;

		// upload weights
		m_vbos[4].bind() ;
		m_vbos[4].bufferData( m_weights.byteSize(), m_weights.data() ) ;
		pProg->setVertexAttribPointer(JAM_PROGRAM_ATTRIB_WEIGHTS, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), 0) ;
		pProg->enableVertexAttribArray(JAM_PROGRAM_ATTRIB_WEIGHTS) ;

		// upload indices
		m_vbos[5].bind() ;
		m_vbos[5].bufferData( m_elements.byteSize(), m_elements.data() ) ;

		// unbind vao
 		m_vao.unbind() ;

		// unbind all vbos
		m_vbos[4].unbind() ;
		m_vbos[5].unbind() ;

		m_uploaded = true ;
	}

	void SkinnedMesh::draw()
	{
		if( !isUploaded() ) {
			upload() ;
		}
		GetGfx().drawIndexedPrimitive( &m_vao, getElementsArray().length(), m_pMaterial ) ;
	}

}
