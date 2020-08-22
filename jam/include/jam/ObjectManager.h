/**********************************************************************************
* 
* ObjectManager.h
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

#ifndef __JAM_OBJECTMANAGER_H__
#define __JAM_OBJECTMANAGER_H__

#ifdef JAM_USE_MEMORY_MANAGER
#include <jam/jam.h>

#include <jam/Singleton.h>
#include <jam/RefCountedObject.h>

#include <map>
#include <set>

#define JAM_ALLOC(t)				(t*)jam::GetObjectMgr().insert(new t())
#define JAM_ALLOC_1(t,x)			(t*)jam::GetObjectMgr().insert(new t((x)))
#define JAM_ALLOC_2(t,x,y)			(t*)jam::GetObjectMgr().insert(new t((x),(y)))
#define JAM_ALLOC_3(t,x,y,w)		(t*)jam::GetObjectMgr().insert(new t((x),(y),(w)))
#define JAM_ALLOC_4(t,x,y,w,z)		(t*)jam::GetObjectMgr().insert(new t((x),(y),(w),(z)))
#define JAM_ALLOC_5(t,u,x,y,w,z)	(t*)jam::GetObjectMgr().insert(new t((u),(x),(y),(w),(z)))
#define JAM_FREE(t)					if( t ) delete (jam::GetObjectMgr().erase((t)))


namespace jam
{

/**
	A singleton class tracking RefCountedObject instances
*/
class JAM_API ObjectManager : public Singleton<ObjectManager>
{
	friend class Singleton<ObjectManager> ;

	using TObjectMap = std::map< const char*, std::set<RefCountedObject*> > ;
	using TObjectMapIterator = TObjectMap::iterator ;

public:
	RefCountedObject*				insert( RefCountedObject* obj ) ;
	RefCountedObject*				erase( RefCountedObject* obj ) ;
	
	void							dump() ;
	
private:
									ObjectManager() ;
	virtual							~ObjectManager() ;

	TObjectMap						m_objectsTable ;
};
/** Returns the singleton instance */
JAM_INLINE ObjectManager& GetObjectMgr() { return ObjectManager::getSingleton(); }

}

#else

#define JAM_ALLOC(t)				new t()
#define JAM_ALLOC_1(t,x)			new t((x))
#define JAM_ALLOC_2(t,x,y)			new t((x),(y))
#define JAM_ALLOC_3(t,x,y,w)		new t((x),(y),(w))
#define JAM_ALLOC_4(t,x,y,w,z)		new t((x),(y),(w),(z))
#define JAM_ALLOC_5(t,u,x,y,w,z)	new t((u),(x),(y),(w),(z))
#define JAM_FREE(t)					JAM_DELETE(t)

#endif // JAM_USE_MEMORY_MANAGER

#endif // __JAM_OBJECTMANAGER_H__
