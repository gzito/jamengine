/**********************************************************************************
* 
* RefCountedObject.cpp
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

#include "jam/jam.h"
#include "jam/RefCountedObject.h"
#include <sstream>
#include <climits>


namespace jam
{


RefCountedObject::RefCountedObject() : 
	m_refCount(1)
#ifdef JAM_DEBUG
	, m_file(), m_line(0)
#endif
{
}

RefCountedObject::~RefCountedObject()
{
}

void RefCountedObject::addRef()
{
	JAM_ASSERT_MSG(m_refCount>0, "m_refCount should be greater than 0");
	m_refCount++;
}

void RefCountedObject::release()
{
	// if something goes wrong with memory, m_refCount become garbagled and generally a very high value (absolute value)
	JAM_ASSERT_MSG(m_refCount>0 && m_refCount<INT_MAX, "m_refCount=%d (should be > 0)", (int32_t)m_refCount );
	if( --m_refCount == 0 ) {
		delete this ;
	}
}

int32_t RefCountedObject::getRefCount() const
{
	return m_refCount ;
}

#ifdef JAM_DEBUG
String RefCountedObject::getDebugInfo(bool typeInfo/*=true*/)
{
	std::stringstream oss ;
 	if( typeInfo ) {
		oss << "type=" << typeid(*this).name() << ", " ;
	}
	oss << "refCount=" << (int32_t)m_refCount << ", "  ;
	return oss.str().c_str() ;
}
#endif

}
