/**********************************************************************************
* 
* Quad2D.cpp
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

#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <jam/Gfx.h>

#include "Quad2D.h"

using namespace jam ;

Vector2 quad_vertices[] = {
	{ -1.0f,  1.0f },
	{  1.0f,  1.0f },
	{  1.0f, -1.0f },
	{ -1.0f, -1.0f },
};

Color quad_colors[] = {
	Color::WHITE,
	Color::WHITE,
	Color::WHITE,
	Color::WHITE,
};

Vector2 quad_tex_coords[] = {
	// front
	{ 0.0f, 1.0f },
	{ 1.0f, 1.0f },
	{ 1.0f, 0.0f },
	{ 0.0f, 0.0f },
};

U16 quad_elements[] = {
	0, 1, 2,
	2, 3, 0,
};

Quad2D::Quad2D() : m_pTexture(), m_pProg(), m_uploaded(false)
{
	m_vertices.create(4) ;
	m_texCoords.create(4) ;
	m_colors.create(4) ;
	m_elements.create(6) ;

	m_vertices.copyFrom( quad_vertices ) ;
	m_texCoords.copyFrom( quad_tex_coords ) ;
	m_colors.copyFrom( quad_colors ) ;
	m_elements.copyFrom( quad_elements ) ;

	for( size_t i=0; i<m_vbos.length()-1; i++ ) {
		m_vbos[i].setTarget(GL_ARRAY_BUFFER) ;
	}
	m_vboElements.setTarget(GL_ELEMENT_ARRAY_BUFFER) ;

	m_pProg = ShaderManager::getSingleton().getScreen() ;
}

Quad2D::~Quad2D()
{

}

void Quad2D::upload()
{
	if( m_uploaded ) {
		return ;
	}

	m_pProg->use();

	for( size_t i=0; i<m_vbos.length(); i++ ) {
		m_vbos[i].create() ;
	}
	m_vboElements.create() ;
	m_vao.create() ;

	m_vao.bind() ;

	int vboIdx = 0 ;

	// upload vertices
	m_vbos[vboIdx].bind() ;
	m_vbos[vboIdx].bufferData( m_vertices.byteSize(), m_vertices.data() ) ;
	m_pProg->setVertexAttribPointer(JAM_PROGRAM_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0) ;
	m_pProg->enableVertexAttribArray(JAM_PROGRAM_ATTRIB_POSITION) ;

	// upload texture coordinates
	vboIdx++;
	m_vbos[vboIdx].bind() ;
	m_vbos[vboIdx].bufferData( m_texCoords.byteSize(), m_texCoords.data() ) ;
	m_pProg->setVertexAttribPointer(JAM_PROGRAM_ATTRIB_TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Vector2), 0) ;
	m_pProg->enableVertexAttribArray(JAM_PROGRAM_ATTRIB_TEXCOORDS) ;

	// upload colors
/*
	vboIdx++;
	m_vbos[vboIdx].bind() ;
	m_vbos[vboIdx].bufferData( m_colors.byteSize(), m_colors.data() ) ;
	pProg->setVertexAttribPointer(JAM_PROGRAM_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Color), 0) ;
	pProg->enableVertexAttribArray(JAM_PROGRAM_ATTRIB_COLOR) ;
*/
	// upload indices
	m_vboElements.bind() ;
	m_vboElements.bufferData( m_elements.byteSize(), m_elements.data() ) ;

	// unbind vao
 	m_vao.unbind() ;

	// unbind ARRAY_BUFFER and ELEMENT_ARRAY_BUFFER
	m_vbos[vboIdx].unbind() ;
	m_vboElements.unbind() ;

	m_uploaded = true ;
}

void Quad2D::draw()
{
	m_pProg->use() ;

	if( !m_uploaded ) {
		upload() ;
	}
	
	m_vao.bind() ;
	GetGfx().setDepthTest(false) ;

	if( m_pTexture ) {
		glActiveTexture( GL_TEXTURE0 );	// select active texture unit
		glBindTexture( GL_TEXTURE_2D, m_pTexture->getId() );
		m_pProg->setUniformSafe( "material_diffuse", 0 ) ;	// <--- idx is textureUnit
	}
	glDrawElements( GL_TRIANGLES, (GLsizei)m_elements.length(), GL_UNSIGNED_SHORT, 0 );

	glActiveTexture( GL_TEXTURE0 );	// select active texture unit
	glBindTexture( GL_TEXTURE_2D, 0 );

	GetGfx().setDepthTest(true) ;
	m_vao.unbind() ;
}
