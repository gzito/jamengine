/**********************************************************************************
* 
* XmlResource.cpp
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

#include "jam/XmlResource.h"
#include "jam/Application.h"

#include <tinyxml.h>

namespace jam
{
/***************************************************************************
* XmlResourceExtraData
*/

String XmlResourceExtraData::toString()
{
	return "XmlResourceExtraData" ;
}

void XmlResourceExtraData::parseXml(char* pRawBuffer)
{
	m_pXmlDocument->Parse(pRawBuffer) ;
}

TiXmlElement* XmlResourceExtraData::getRoot()
{
	return m_pXmlDocument->RootElement();
}

/***************************************************************************
* XmlResourceLoader
*/

XmlResourceLoader::XmlResourceLoader()
{
	m_patterns.clear() ;
	m_patterns.push_back("*.xml") ;
}

bool XmlResourceLoader::loadResource(char* rawBuffer,size_t rawSize,ResHandle& handle)
{
	if (rawSize <= 0)
		return false;

	XmlResourceExtraData* pExtraData = new XmlResourceExtraData();
	pExtraData->parseXml(rawBuffer);

	handle.setExtra(pExtraData);

	return true;
}

TiXmlElement* XmlResourceLoader::loadAndReturnRootXmlElement(const char* resourceString)
{
	Resource resource(resourceString);
	ResHandle* pResourceHandle = GetAppMgr().getResourceManager().getHandle(&resource);  // this actually loads the XML file from the ResourceFile
	XmlResourceExtraData* pExtraData = dynamic_cast<XmlResourceExtraData*>(pResourceHandle->getExtra());
	return pExtraData->getRoot();
}

}
