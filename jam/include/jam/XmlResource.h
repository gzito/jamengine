/**********************************************************************************
* 
* XmlResource.h
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

#ifndef __JAM_XMLRESOURCE_H__
#define __JAM_XMLRESOURCE_H__

#include <jam/jam.h>
#include <jam/ResourceManager.h>

class TiXmlElement ;
class TiXmlDocument ;

namespace jam
{

/***************************************************************************
* XmlResourceExtraData
*/
class JAM_API XmlResourceExtraData : public IResourceExtraData
{
public:
	virtual String			toString() override ;
	void					parseXml( char* pRawBuffer ) ;
	TiXmlElement*			getRoot() ;

private:
	TiXmlDocument*			m_pXmlDocument ;
};


/***************************************************************************
* XmlResourceLoader
*/
class JAM_API XmlResourceLoader : public IResourceLoader
{
public:
							XmlResourceLoader() ;

	// Inherited via IResourceLoader
	virtual bool			useRawFile() const override;
	virtual size_t			getLoadedResourceSize(char* rawBuffer,size_t rawSize) override;
	virtual bool			loadResource(char* rawBuffer,size_t rawSize,ResHandle& handle) override;
	virtual bool			discardRawBufferAfterLoad() const override;
	virtual bool			addNullZero() const override;

	// convenience function
    static TiXmlElement*	loadAndReturnRootXmlElement(const char* resourceString);
};

JAM_INLINE	bool XmlResourceLoader::useRawFile() const { return false; }
JAM_INLINE	size_t XmlResourceLoader::getLoadedResourceSize(char* rawBuffer,size_t rawSize) { return rawSize; }
JAM_INLINE	bool XmlResourceLoader::discardRawBufferAfterLoad() const { return true; }
JAM_INLINE	bool XmlResourceLoader::addNullZero() const { return false; }

}

#endif // __JAM_XMLRESOURCE_H__
