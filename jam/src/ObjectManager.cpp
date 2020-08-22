/**********************************************************************************
* 
* ObjectManager.cpp
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

#ifdef JAM_USE_MEMORY_MANAGER	
#include "jam/ObjectManager.h"

#include <typeinfo>


using namespace std ;

namespace jam
{

ObjectManager::ObjectManager()
{

}

ObjectManager::~ObjectManager()
{

}

RefCountedObject* ObjectManager::insert( RefCountedObject* obj )
{
	JAM_ASSERT_MSG( obj != 0, ("obj is NULL") ) ;
	m_objectsTable[typeid(*obj).name()].insert( obj ) ;
	return obj ;
}

RefCountedObject* ObjectManager::erase( RefCountedObject* obj )
{
	if( obj ) {
	//	JAM_TRACE( ("ObjectManager::erase [%s]", typeid(*obj).name()) ) ;
		m_objectsTable[typeid(*obj).name()].erase( obj ) ;
	}
	return obj ;
}

void ObjectManager::dump()
{
	TObjectMapIterator itMap = m_objectsTable.begin() ;
	while( itMap != m_objectsTable.end() ) {
		const char* objectType = itMap->first ; 

		set<RefCountedObject*>& theSet = itMap->second ;
		JAM_TRACE( ("%s: %d",objectType, theSet.size()) ) ;

		set<RefCountedObject*>::iterator itSet = theSet.begin() ;
		while ( itSet!=theSet.end() ) {
			RefCountedObject* theObj = *itSet ;
			JAM_TRACE( ("\t%s", theObj->getDebugInfo(false).c_str()) ) ;
			itSet++ ;
		}

		itMap++ ;
	} 
}

}
#endif
