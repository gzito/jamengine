/**********************************************************************************
* 
* TextureCubemap.h
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
 
#ifndef __JAM_TEXTURE_CUBEMAP_H__
#define __JAM_TEXTURE_CUBEMAP_H__

#include <jam/jam.h>

#include <jam/Color.h>
#include <jam/Singleton.h>
#include <jam/String.h>
#include <jam/Object.h>
#include <jam/BaseManager.hpp>
#include <jam/Ref.hpp>

namespace jam
{

/*!
	\class TextureCubemap
	\brief This class represents a cubemap texture
*/
class JAM_API TextureCubemap : public NamedObject
{
public:
							// ctors/dtor
							TextureCubemap() ;
	virtual					~TextureCubemap();

	// load methods
	void					createDefaultEmpty( Color color = Color::WHITE, bool fUpload = true ) ;
	void					load( const std::vector<String>& filenames, bool flipV = true, bool fUpload = true );

	/// Returns the width of texture
	uint32_t				getWidth() const { return m_width; }
	/// Returns the height of texture
	uint32_t				getHeight() const { return m_height; }
	uint8_t*				getData(size_t i) const { return m_data[i]; }
	uint8_t					getBitCount() const { return m_bitCount; } 
	unsigned int			getId() const { return m_GLid; }

	void					freeData() ;
	void					upload() ;

private:
	void					initGL() ;
	void					destroy() ;

protected:
	uint32_t				m_width ;
	uint32_t				m_height ;
	uint8_t					m_bitCount ;
	uint8_t*				m_data[6] ;
	unsigned int			m_GLid ;

private:
	bool					m_freeClientMemoryWithStbi ;

// to prevent the use
private:
							TextureCubemap( const TextureCubemap& ) = delete ;
	TextureCubemap&			operator=( const TextureCubemap& ) = delete ;
};

class JAM_API TextureCubemapManager : public NamedObjectManager<TextureCubemap>, public jam::Singleton<TextureCubemapManager>
{
	friend class jam::Singleton<TextureCubemapManager> ;

public:

protected:
	TextureCubemapManager() = default ;
private:
};

JAM_INLINE TextureCubemapManager&	GetTextureCubemapMgr() { return (TextureCubemapManager&) TextureCubemapManager::getSingleton(); }

}	// end namespace

#endif // __JAM_TEXTURE3D_H__
