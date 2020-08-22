/**********************************************************************************
* 
* SkyBox.cpp
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

#include <jam/SkyBox.h>
#include <jam/Gfx.h>
#include <jam/Shader.h>
#include <jam/Camera.h>
#include <jam/Scene.h>
#include <jam/Application.h>

namespace jam
{

// positions
static float skyboxVertices[] = {
	// back
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	// left
	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	// right			
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	// front
	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	// top
	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	// bottom
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

//*******************
//
// Class Mesh
//
//*******************

SkyBox::SkyBox( GameObject* gameObject ) :
	Component(*gameObject),
	m_pTexture3D(0), 
	m_vertices(0),
//		m_elements(0),
	m_verticesVbo(),
//		m_elementsVbo(),
	m_vao(),
	m_uploaded(false)
{
	m_pTexture3D = new TextureCubemap() ;
	m_verticesVbo.setTarget(GL_ARRAY_BUFFER) ;
//		m_elementsVbo.setTarget(GL_ELEMENT_ARRAY_BUFFER) ;
}

SkyBox::~SkyBox()
{
	destroy() ;

	JAM_RELEASE(m_pTexture3D) ;

	m_verticesVbo.destroy() ;
//		m_elementsVbo.destroy() ;
	m_vao.destroy() ;
}

void SkyBox::setTexture3D(TextureCubemap * pTexture3D)
{
	if( m_pTexture3D ) {
		JAM_RELEASE_NULL(m_pTexture3D) ;
	}
	m_pTexture3D = pTexture3D ;
	if( m_pTexture3D ) {
		m_pTexture3D->addRef() ;
	}
}

void SkyBox::create()
{
	m_vertices.create( sizeof(skyboxVertices) / sizeof(Vector3) ) ;
	memcpy( m_vertices.data(), skyboxVertices, m_vertices.byteSize() ) ;
//		m_elements.create(numOfElements) ;
//		memcpy( m_elements.data(), skyboxElements, m_elements.byteSize() ) ;
}

void SkyBox::destroy()
{
	m_vertices.destroy() ;
//		m_elements.destroy() ;
}

void SkyBox::upload()
{
	if( m_uploaded ) {
		return ;
	}

	Shader* pProg = ShaderManager::getSingleton().getCurrent() ;

	m_verticesVbo.create() ;
//		m_elementsVbo.create() ;

	m_vao.create() ;

	// bind vao
	// all glBindBuffer, glVertexAttribPointer, and glEnableVertexAttribArray calls are tracked and stored in the vao
	m_vao.bind() ;

	// upload vertices
	m_verticesVbo.bind() ;
	m_verticesVbo.bufferData( m_vertices.byteSize(), m_vertices.data() ) ;
	pProg->setVertexAttribPointer(JAM_PROGRAM_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), 0) ;
	pProg->enableVertexAttribArray(JAM_PROGRAM_ATTRIB_POSITION) ;

	// upload indices
//		m_elementsVbo.bind() ;
//		m_elementsVbo.bufferData( m_elements.byteSize(), m_elements.data() ) ;

	// unbind vao
 	m_vao.unbind() ;

	// unbind all vbos
	m_verticesVbo.unbind() ;
//		m_elementsVbo.unbind() ;

	m_uploaded = true ;
}

void SkyBox::draw()
{
	if( !isUploaded() ) {
		upload() ;
	}

	GetGfx().setDepthMask(false);
	GetGfx().setDepthFunc(GL_LEQUAL) ;

	Shader* pShader = ShaderManager::getSingleton().getSkyBox().get() ;
	pShader->use();

	// sets view and projection matrix
	Camera* pCam = GetAppMgr().getScene()->getCamera() ;
	pShader->setViewMatrix( Matrix4(Matrix3(pCam->getViewMatrix())) ) ;
	pShader->setProjectionMatrix(pCam->getProjectionMatrix()) ;

	m_vao.bind() ;

	// bind cubemap texture
	glActiveTexture( GL_TEXTURE0 );	// select active texture unit
	glBindTexture( GL_TEXTURE_CUBE_MAP, getTexture3D()->getId() );
	pShader->setUniform( JAM_PROGRAM_UNIFORM_MATERIAL_DIFFUSE, 0 ) ;

	glDrawArrays( GL_TRIANGLES, 0, getVerticesArray().length() );

	// unbind cubemap texture
	glActiveTexture( GL_TEXTURE0 );	// select active texture unit
	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 ); // no texture

	m_vao.unbind() ;

	GetGfx().setDepthMask(true);
	GetGfx().setDepthFunc(GL_LESS) ;
}

}
