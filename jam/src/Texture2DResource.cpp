/**********************************************************************************
* 
* Texture2DResource.cpp
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

#include "jam/Texture2DResource.h"
#include "jam/Application.h"

#include <stb_image.h>


namespace jam
{
	/***************************************************************************
	* XmlResourceExtraData
	*/

	Texture2DResourceExtraData::Texture2DResourceExtraData() : m_pTexture(nullptr)
	{
	}

	Texture2DResourceExtraData::~Texture2DResourceExtraData()
	{
		JAM_DELETE( m_pTexture ) ;
	}

	String Texture2DResourceExtraData::toString()
	{
		return "Texture2DResourceExtraData" ;
	}

	void Texture2DResourceExtraData::createTextureFromMemory(char* pRawBuffer,size_t len)
	{
		int x, y, channelsInFile ;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* pData = stbi_load_from_memory( (const stbi_uc*)pRawBuffer, len, &x, &y, &channelsInFile, STBI_default) ;
		m_pTexture = new Texture2D() ;
		m_pTexture->create( x, y, GL_RGBA, pData, true ) ;
	}


	/***************************************************************************
	* XmlResourceLoader
	*/

	Texture2DResourceLoader::Texture2DResourceLoader()
	{
		m_patterns.clear() ;
		m_patterns.emplace_back("*.png") ;
		m_patterns.emplace_back("*.jpg") ;
		m_patterns.emplace_back("*.bmp") ;
		m_patterns.emplace_back("*.tga") ;
		m_patterns.emplace_back("*.psd") ;
	}

	bool Texture2DResourceLoader::loadResource(char* rawBuffer,size_t rawSize, ResHandle& handle)
	{
		if (rawSize <= 0)
			return false;

		Texture2DResourceExtraData* pExtraData = new Texture2DResourceExtraData();
		pExtraData->createTextureFromMemory(rawBuffer,rawSize);

		handle.setExtra(pExtraData);

		return true;
	}

	Texture2D* Texture2DResourceLoader::loadAndReturnTexture2D(const char* resourceString)
	{
		Resource resource(resourceString);
		ResHandle* pResourceHandle = GetAppMgr().getResourceManager().getHandle(&resource);  // this actually loads the texture file from the ResourceFile
		Texture2DResourceExtraData* pExtraData = dynamic_cast<Texture2DResourceExtraData*>(pResourceHandle->getExtra());
		return pExtraData->getTexture2D();
	}
}
