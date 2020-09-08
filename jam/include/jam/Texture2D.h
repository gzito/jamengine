/**********************************************************************************
* 
* Texture2D.h
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
 
#ifndef __JAM_TEXTURE2D_H__
#define __JAM_TEXTURE2D_H__

#include <jam/jam.h>

#include <jam/Color.h>
#include <jam/Singleton.h>
#include <jam/String.h>
#include <jam/Object.h>

// GLenum definition
#include <GL/glew.h>

namespace jam
{

/*!
	\class Texture2D
	\brief This class represents a texture
*/
class JAM_API Texture2D : public NamedObject
{
public:
	// ctors/dtor
							Texture2D() ;
	virtual					~Texture2D();

	// load methods
	void					create( U32 width, U32 height, GLenum format, const void* data = 0, bool fUpload = true ) ;
	void					createDefaultEmpty( Color color = Color::WHITE, bool fUpload = true ) ;
	void					load( const String& filename, bool flipV = true, bool fUpload = true );

	/// Returns the width of texture
	U32						getWidth() const { return m_width; }
	/// Returns the height of texture
	U32						getHeight() const { return m_height; }
	U8*						getData() const { return m_data; }
	U8						getBitCount() const { return m_bitCount; } 
	unsigned int			getId() const { return m_GLid; }

	void					freeData() ;
	void					upload() ;

	U8						bitcountFromFormat( GLenum fmt ) ;

private:
	void					initGL() ;
	void					destroy() ;

protected:
	U32						m_width ;
	U32						m_height ;
	U8						m_bitCount ;
	U8*						m_data ;
	unsigned int			m_GLid ;

private:
	bool					m_freeClientMemoryWithStbi ;

// to prevent the use
private:
							Texture2D( const Texture2D& ) = default ;
	Texture2D&				operator=( const Texture2D& ) = default ;
};

}	// end namespace

#endif // __JAM_TEXTURE2D_H__
