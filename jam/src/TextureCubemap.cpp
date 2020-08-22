/**********************************************************************************
* 
* TextureCubemap.cpp
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

#pragma warning( disable: 4786 )

#include "stdafx.h"

#include <jam/TextureCubemap.h>

#include <stb_image.h>

namespace jam
{
	
	/*!
		\brief Create a Texture object
	*/
	TextureCubemap::TextureCubemap() :
		m_width(0), m_height(0),
		m_bitCount(0),
		m_GLid(0),
		m_data(),
		m_freeClientMemoryWithStbi(true)
	{
	}
	
	
	//
	//	Destructor
	//
	TextureCubemap::~TextureCubemap()
	{
		destroy() ;
	}
	

	void TextureCubemap::createDefaultEmpty( Color color /*= Color::WHITE*/, bool fUpload /*= true*/ )
	{
		GLulong data = color.getRgba() ;

		m_width = 1;
		m_height = 1 ;
		m_bitCount = 32 ;
		for( size_t i = 0; i < 6; i++ ) {
			m_data[i] = (uint8_t*)new uint32_t ;
			memcpy(m_data[i],&data,sizeof(GLulong)) ;
		}
		m_freeClientMemoryWithStbi = false ;

		if( fUpload ) {
			initGL() ;
		}
	}

	void TextureCubemap::load( const std::vector<String>& filenames, bool flipV /*= true*/, bool fUpload /*= true*/ )
	{
		int w, h, channels ;

		if( filenames.size() != 6 ) {
			JAM_ERROR( "filenames.size() <> 6 when loading cubemap!" ) ;
		}

		stbi_set_flip_vertically_on_load(flipV);

		for( unsigned int i = 0; i < 6; i++ ) {
			m_data[i] = stbi_load( filenames[i].c_str(), &w, &h, &channels, 0 ) ;
			if( !m_data[i] ) {
				JAM_ERROR( "Failed to load %s", filenames[i].c_str() ) ;
			}
		}

		m_width = w ;
		m_height = h ;
		m_bitCount = channels * 8 ;

		if( fUpload ) {
			initGL() ;
		}
	}

	void TextureCubemap::freeData()
	{
		for( unsigned int i = 0; i < 6; i++ ) {
			if( m_data[i] ) {
				if( m_freeClientMemoryWithStbi ) {
					stbi_image_free( m_data[i] );
				}
				else {
					delete m_data[i] ;
				}
			}
			m_data[i] = 0 ;
		}
	}

	void TextureCubemap::upload()
	{
		if( !m_GLid) {
			initGL() ;
		}
	}

	void TextureCubemap::initGL()
	{
		glGenTextures(1, &m_GLid);
		glBindTexture( GL_TEXTURE_CUBE_MAP, m_GLid );

		GLenum format = ( m_bitCount == 24 ) ? GL_RGB : GL_RGBA ;

		for( unsigned int i = 0; i < 6; i++ ) {
			glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				format,
				(GLsizei)m_width, (GLsizei)m_height,
				0,
				format,
				GL_UNSIGNED_BYTE,
				m_data[i]);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	void TextureCubemap::destroy()
	{
		freeData() ;

		if( m_GLid ) {
			glDeleteTextures( 1, &m_GLid ) ;
		}
		m_GLid = 0 ;
	}


}
