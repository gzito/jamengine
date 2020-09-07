/**********************************************************************************
* 
* Texture2DResource.h
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

#ifndef __JAM_TEXTURE2DRESOURCE_H__
#define __JAM_TEXTURE2DRESOURCE_H__

#include <jam/jam.h>
#include <jam/ResourceManager.h>
#include <jam/Texture2D.h>


namespace jam
{

class JAM_API Texture2DResourceExtraData : public IResourceExtraData, public Collectible
{
public:
							Texture2DResourceExtraData() ;
	virtual					~Texture2DResourceExtraData() ;

	virtual String			toString() override ;
	void					createTextureFromMemory( char* pRawBuffer, size_t len ) ;
	Texture2D*				getTexture2D() ;

private:
	Texture2D*				m_pTexture ;
};

JAM_INLINE Texture2D* Texture2DResourceExtraData::getTexture2D() { return m_pTexture; }


class JAM_API Texture2DResourceLoader : public IResourceLoader, public Collectible
{
public:
							
							Texture2DResourceLoader() ;

	// Inherited via IResourceLoader
	virtual bool			useRawFile() const override;
	virtual size_t			getLoadedResourceSize(char* rawBuffer,size_t rawSize) override;
	virtual bool			loadResource(char* rawBuffer,size_t rawSize, ResHandle& handle) override;
	virtual bool			discardRawBufferAfterLoad() const override;
	virtual bool			addNullZero() const override;

	// convenience function
    static Texture2D*		loadAndReturnTexture2D(const char* resourceString);
};

JAM_INLINE	bool Texture2DResourceLoader::useRawFile() const { return false; }
JAM_INLINE	size_t Texture2DResourceLoader::getLoadedResourceSize(char* rawBuffer,size_t rawSize) { return rawSize; }
JAM_INLINE	bool Texture2DResourceLoader::discardRawBufferAfterLoad() const { return true; }
JAM_INLINE	bool Texture2DResourceLoader::addNullZero() const { return false; }

}

#endif // __JAM_TEXTURE2DRESOURCE_H__
