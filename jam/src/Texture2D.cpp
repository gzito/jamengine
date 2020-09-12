/**********************************************************************************
* 
* Texture2D.cpp
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

#include <jam/Texture2D.h>

#include <stb_image.h>

namespace jam
{
	std::atomic_int Texture2D::_lastSortingKey = 0 ;

	/*!
		\brief Create a Texture object
	*/
	Texture2D::Texture2D() :
		m_width(0), m_height(0),
		m_bitCount(0),
		m_GLid(0),
		m_data(nullptr),
		m_freeClientMemoryWithStbi(true)
	{
		_sortingKey = ++_lastSortingKey ;
	}
	
	
	//
	//	Destructor
	//
	Texture2D::~Texture2D()
	{
		destroy() ;
	}
	
	/*
	* data can be null
	*/
	void Texture2D::create(U32 width,U32 height,GLenum format,const void * data /* = 0 */ ,bool fUpload /*= true*/)
	{
		if( m_data && m_GLid ) { destroy(); }

		m_width = width ;
		m_height = height ;
		m_bitCount = bitcountFromFormat(format) ;
		m_data = (U8*)data ;
		m_freeClientMemoryWithStbi = false ;
		if( fUpload ) {
			initGL() ;
		}
	}

	void Texture2D::createDefaultEmpty( Color color /*= Color::WHITE*/, bool fUpload /*= true*/ )
	{
		if( m_data || m_GLid ) { destroy(); }

		GLulong data = color.getRgba() ;

		m_width = 1;
		m_height = 1 ;
		m_bitCount = 32 ;
		m_data = (U8*)new U32 ;
		memcpy(m_data,&data,sizeof(GLulong)) ;
		m_freeClientMemoryWithStbi = false ;

		if( fUpload ) {
			initGL() ;
		}
	}

	void Texture2D::createFromSDLSurface(SDL_Surface* pSurface)
	{
		if( m_data || m_GLid ) { destroy(); }

		m_width = pSurface->w ;
		m_height = pSurface->h ;
		m_bitCount = pSurface->format->BitsPerPixel ;
		m_data = 0 ;
		m_freeClientMemoryWithStbi = false ;

		GLenum texture_format ;
		Uint8 bytes_per_pixel = pSurface->format->BytesPerPixel;

		if( bytes_per_pixel == 4 ) {
			if( pSurface->format->Rmask == 0x000000ff )
				texture_format = GL_RGBA ;
			else
				texture_format = GL_BGRA ;
		}
		else { // no_alpha
			if (pSurface->format->Rmask == 0x000000ff)
				texture_format = GL_RGB;
			else
				texture_format = GL_BGR;
		}
		
		glGenTextures(1, &m_GLid);
		glBindTexture(GL_TEXTURE_2D, m_GLid);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pSurface->w, pSurface->h, 0, texture_format, GL_UNSIGNED_BYTE, pSurface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void Texture2D::load( const String& filename, bool flipV /*= true*/, bool fUpload /*= true*/ )
	{
		if( m_data && m_GLid ) { destroy(); }

		stbi_set_flip_vertically_on_load(flipV);
		int w, h, channels ;
		m_data = stbi_load( filename.c_str(), &w, &h, &channels, STBI_default ) ;
		if( !m_data ) {
			JAM_ERROR( "Failed to load %s", filename.c_str() ) ;
		}
		m_width = w ;
		m_height = h ;
		m_bitCount = channels * 8 ;

		if( fUpload ) {
			initGL() ;
		}
	}

	void Texture2D::freeData()
	{
		if( m_data != nullptr ) {
			if( m_freeClientMemoryWithStbi ) {
				stbi_image_free( m_data );
			}
			else {
				delete m_data ;
			}
		}
		m_data = 0 ;
	}

	void Texture2D::upload()
	{
		if( !m_GLid) {
			initGL() ;
		}
	}

	U8 Texture2D::bitcountFromFormat(GLenum fmt)
	{
		U8 bitcount = 0 ;
		switch( fmt ) {
		case GL_RED:
			bitcount = 8 ;
			break ;

		case GL_RG:
			bitcount = 16 ;
			break ;

		case GL_RGB:
		case GL_BGR:
			bitcount = 24 ;
			break ;

		case GL_RGBA:
		case GL_BGRA:
			bitcount = 32 ;
			break ;

		case GL_DEPTH_COMPONENT:
			bitcount = 8 ;
			break ;

		case GL_DEPTH_STENCIL:
			bitcount = 16 ;
			break ;

		default:
			break ;
		}

		return bitcount ;
	}

	int Texture2D::getSortingKey() const
	{
		return _sortingKey ;
	}

	void Texture2D::initGL()
	{
		glGenTextures(1, &m_GLid);
		glBindTexture( GL_TEXTURE_2D, m_GLid );

		GLenum format = ( m_bitCount == 24 ) ? GL_RGB : GL_RGBA ;

		glTexImage2D( GL_TEXTURE_2D,
			0,
			GL_RGBA,			// internal format
			(GLsizei)m_width, (GLsizei)m_height,
			0,
			format,				// format
			GL_UNSIGNED_BYTE,	// data type of the pixel data
			m_data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void Texture2D::destroy()
	{
		freeData() ;

		if( m_GLid ) {
			glDeleteTextures( 1, &m_GLid ) ;
		}
		m_GLid = 0 ;
	}


}
