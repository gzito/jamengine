/**********************************************************************************
* 
* AutoReleasePool.cpp
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

//#include <GL/glew.h>

#ifdef JAM_EXCLUDED_BLOCK

#include "jam/AutoReleasePool.h"
#include "jam/RefCountedObject.h"

#include <jam/Sprite.h>
#include <jam/TextNode.h>

#include <map>
#include <typeinfo>

using namespace std ;

namespace jam
{

const jam::time AutoReleasePool::DefaultCollectinterval = 2.0f;		// in seconds

#ifdef JAM_TRACE_AUTORELEASEPOOL
const jam::time AutoReleasePool::DefaultTraceInterval = 5.0f;
#endif

void AutoReleasePool::addObject( RefCountedObject* obj )
{
	JAM_ASSERT_MSG( (obj!=0), ("AutoReleasePool::addObject: null pointer") ) ;

	if( obj == 0 ) {
		return ;
	}

	//m_managedObjs.addObject(obj) ;
	m_managedObjs.insert(obj) ;
}

void AutoReleasePool::removeObject( RefCountedObject* obj )
{
	//m_managedObjs.removeObject(obj) ;
	m_managedObjs.erase(obj) ;
}

AutoReleasePool::AutoReleasePool() :
	m_collectInterval(DefaultCollectinterval),
	m_totalReleased(0),
	m_collectTimer(0)
#ifdef JAM_TRACE_AUTORELEASEPOOL
	, m_traceTimer(0),
	m_traceInterval(DefaultTraceInterval)
#endif
{
	m_collectTimer = Timer::create() ;
	m_collectTimer->setReserved() ;
	m_collectTimer->setName("AutoReleasePoolCollect") ;
	m_collectTimer->start();


#ifdef JAM_TRACE_AUTORELEASEPOOL
	m_traceTimer = Timer::create() ;
	m_traceTimer->setReserved() ;
	m_traceTimer->getTimeExpiredEvent() += JAM_EVT_HANDLER(AutoReleasePool,TimeExpiredEventArgs,this,&AutoReleasePool::traceObjects) ;
	m_traceTimer->setRepeatSweep(-1) ;
	m_traceTimer->setRepeatSweepTime((uint64)(m_traceInterval*1000)) ;
	m_traceTimer->start() ;
	GetTimerMgr().addByName(m_traceTimer,"AutoReleasePoolTrace") ;
#endif
}

AutoReleasePool::~AutoReleasePool()
{
	JAM_TRACE( "Terminating AutoReleasePool" ) ;

#ifdef JAM_TRACE_AUTORELEASEPOOL
	JAM_RELEASE_NULL(m_traceTimer) ;
#endif
	JAM_RELEASE_NULL(m_collectTimer) ;
//	for( MutableArray<>::Iterator it=m_managedObjs.begin(); it != m_managedObjs.end(); it++ ) {
	for( set<RefCountedObject*>::iterator it=m_managedObjs.begin(); it != m_managedObjs.end(); ) {
		RefCountedObject* obj = *it ;
		
#ifdef JAM_DEBUG
			if( obj->m_refCount > 1 ) 
			{
				//JAM_TRACE( ("WARNING - %s -> refCount = %d - possibile memory leak (block ID=%d).", typeid(*obj).name(), obj->m_refCount, IwMemBucketDebugGetID(obj)) ) ;
				//JAM_TRACE( ("Info[%s]",(*obj).getDebugInfo().c_str() )) ;
			}
#endif

			// prevents the object to erase itseft from the list when release() is invoked
			obj->m_managed = false ;
			obj->release() ;
			m_managedObjs.erase(it++) ;
	}
}

int AutoReleasePool::getObjectsCount() const
{
//	return m_managedObjs.count();
	return (int)(m_managedObjs.size());
}


void AutoReleasePool::setCollectInterval( jam::time t )
{
	m_collectInterval = t ;
}


void AutoReleasePool::collectOnIdle( uint64_t frameStartMs )
{
	m_collectTimer->update() ;

	uint32_t count = 0 ;
	//for( size_t i=0; i<m_managedObjs.size(); i++ ) {
	for( std::set<RefCountedObject*>::iterator it = m_managedObjs.begin(); it!=m_managedObjs.end(); ) {
		//RefCountedObject* obj = m_managedObjs.getObjectAt(i) ;
		RefCountedObject* obj = *it; ;
		if( obj && (obj->m_refCount == 1 ) )
		{
			// prevents the object to erase itseft from the list when release() is invoked
			obj->m_managed = false ;

			obj->release() ;
			m_managedObjs.erase(it++) ;
//			m_managedObjs.removeObjectAt(i) ;
			count++ ;
		}
		else {
			++it ;
		}

		// checks remaining frame time
		if( GetAppMgr().getRemainingFrameMs() == 0 ) {
			break ;
		}
	}
	
	//m_managedObjs.firstAvailableIndex();
	m_totalReleased = count ;

	if( m_managedObjs.size() == 0 ) {
		m_collectTimer->reset() ;
	}
}

void AutoReleasePool::collect( bool force /*= false */, size_t maxObjectsToRelease /*= UINT_MAX*/ )
{
	JAM_PROFILE("PoolMrg.clt") ;

	m_collectTimer->update() ;
	if (m_collectRequest) { 
		force = true; 
		m_collectRequest = false; 
		maxObjectsToRelease = UINT_MAX; 
	}

	if( !force && m_collectTimer->getTotalElapsed() < m_collectInterval ) {
		return ;
	}

	processDestroyList() ;

	uint32_t count = 0 ;
	//for( size_t i=0; i<m_managedObjs.size(); i++ ) {
	for( std::set<RefCountedObject*>::iterator it = m_managedObjs.begin(); it!=m_managedObjs.end(); ) {
		//RefCountedObject* obj = m_managedObjs.getObjectAt(i) ;
		RefCountedObject* obj = *it; ;

		if( obj && (obj->m_refCount == 1 ) )
		{
			 // prevents the object to erase itseft from the list when release() is invoked
			obj->m_managed = false ;

			obj->release() ;
			m_managedObjs.erase(it++) ;
			//m_managedObjs.removeObjectAt(i) ;
			count++ ;
			if( count >= maxObjectsToRelease ) {
				break ;
			}
		}
		else {
			++it ;
		}
	}

	//m_managedObjs.firstAvailableIndex();

	m_totalReleased = count ;

	m_collectTimer->reset() ;
}

#ifdef JAM_TRACE_AUTORELEASEPOOL
void AutoReleasePool::traceObjects(TimeExpiredEventArgs& args, IEventSource& source)
{
	JAM_TRACE( ("------------------------------")) ;
	JAM_TRACE( ("Total number of objects in the memory pool: %d", m_managedObjs.size()) ) ;
//	JAM_TRACE( ("Maximum size of autorelease pool: %d", m_managedObjs.size()) ) ;

#ifdef JAM_TRACE_AUTORELEASEPOOL_DETAILS
	RefCountedObject* obj = 0 ; 
	String typeName ;
	map<String,int> typesCount ;
	map<String,int> spriteFramesCount ;

	//for( MutableArray<RefCountedObject*>::Iterator it = m_managedObjs.begin(); it != m_managedObjs.end(); it++ ) {
	for( std::set<RefCountedObject*>::iterator it = m_managedObjs.begin(); it != m_managedObjs.end(); it++ ) {
		obj = *it ;
		if( obj ) {
//			if( dynamic_cast<Action*>(obj) != 0 ) {
				typeName = typeid(*obj).name() ;
				typesCount[typeName]++ ;
//			}

 #if JAM_TRACE_AUTORELEASEPOOL_DETAILS_SPRITES
			if( typeName == "class jam::Sprite") {
 				Sprite* s = (Sprite*)obj ;
				DrawItem* pItem = s->getFrame() ;
				spriteFramesCount[(pItem ? pItem->getName() : "<noname>")]++ ;
 			}
#endif
		}
	}

	for( map<String,int>::const_iterator it=typesCount.begin(); it != typesCount.end(); it++ ) {
 		JAM_TRACE( ("Number of objects of type '%s': %d", it->first.c_str(), it->second) ) ;

#if JAM_TRACE_AUTORELEASEPOOL_DETAILS_SPRITES
 		if( it->first == "class jam::Sprite") {
 			for( map<String,int>::const_iterator it=spriteFramesCount.begin(); it != spriteFramesCount.end(); it++ ) {
//				if( it->first.find( "back_" ) == String::npos ) {
 						JAM_TRACE( (">> Num of sprites with frame '%s': %d", it->first.c_str(), it->second) ) ;
// 				}
 			}
 		}
#endif
 	}
#endif
}

#endif

void AutoReleasePool::addToDestroyList( RefCountedObject* pObj )
{
	std::list<RefCountedObject*>::iterator it = std::find(m_markedForDestroyItems.begin(), m_markedForDestroyItems.end(), pObj ) ;
	JAM_ASSERT_MSG( it==m_markedForDestroyItems.end(),("Duplicated object/destroy") ) ;
	m_markedForDestroyItems.push_back(pObj) ;
}

void AutoReleasePool::removeFromDestroyList( RefCountedObject* pObj )
{
	m_markedForDestroyItems.remove(pObj) ;
}

void AutoReleasePool::processDestroyList()
{
	while( m_markedForDestroyItems.size() ) {
		RefCountedObject* pObj = m_markedForDestroyItems.front() ;
		m_markedForDestroyItems.pop_front() ;
		pObj->destroy_internal() ;
//		JAM_ASSERT_MSG( (pObj->getRefCount()==1), ("RefCount should be 1 (currently is %d)", pObj->getRefCount()) ) ;

		// prevent the object to erase itseft from the list (m_markedForDestroyItems) when release() is invoked
		pObj->m_markedAsDestroyed = false ;
		JAM_RELEASE(pObj) ;
	}
}

}

#endif
