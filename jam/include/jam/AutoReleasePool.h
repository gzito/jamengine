/**********************************************************************************
* 
* AutoReleasePool.h
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

#ifndef __JAM_AUTORELEASEPOOL_H__
#define __JAM_AUTORELEASEPOOL_H__

#include <jam/jam.h>

#ifdef JAM_EXCLUDED_BLOCK

#include <jam/Timer.h>
#include <jam/Singleton.h>
//#include <jam/MutableArray.hpp>

#include <set>
#include <list>
#include <limits.h>

namespace jam
{

class RefCountedObject ;

/**
	A singleton class managing RefCountedObject instances
*/
class JAM_API AutoReleasePool : public Singleton<AutoReleasePool>
{
	friend class Singleton<AutoReleasePool> ;

public:
	void					addObject(RefCountedObject* obj) ;
	
	void					removeObject(RefCountedObject* obj) ;
	
	int						getObjectsCount() const ;
	uint32_t				getTotalReleased() const { return m_totalReleased; }

	void					setCollectInterval(jam::time t) ;
	jam::time				getCollectInterval() const { return m_collectInterval; } 

	void					collect( bool force = false, size_t maxObjectsToRelease = UINT_MAX );
	void					collectOnIdle( uint64_t frameStartMs ) ;
	void					collectRequest() { m_collectRequest = true; }

#ifdef JAM_TRACE_AUTORELEASEPOOL
	void					traceObjects(TimeExpiredEventArgs& args, IEventSource& source) ;
	jam::time				getTraceInterval() const { return m_traceInterval; }
	void					setTraceInterval(jam::time val) { m_traceInterval = val; }
#endif

	void					addToDestroyList(RefCountedObject* pObj) ;
	void					removeFromDestroyList(RefCountedObject* pObj) ;

private:
							AutoReleasePool() ;
							~AutoReleasePool() ;

	void					processDestroyList() ;

	std::set<RefCountedObject*>	m_managedObjs ;

	uint32_t				m_totalReleased;
	Timer*					m_collectTimer ;
	jam::time				m_collectInterval ;
	static const jam::time	DefaultCollectinterval ;
	bool					m_collectRequest;

#ifdef JAM_TRACE_AUTORELEASEPOOL
	Timer*					m_traceTimer ;
	jam::time				m_traceInterval ;
	static const jam::time	DefaultTraceInterval ;
#endif

	std::list<RefCountedObject*>	m_markedForDestroyItems ;
	
};

/** Returns the singleton instance */
JAM_INLINE AutoReleasePool& GetAutoReleasePoolMgr() { return AutoReleasePool::getSingleton(); }

}

#endif // JAM_EXCLUDED_BLOCK

#endif // __JAM_AUTORELEASEPOOL_H__
