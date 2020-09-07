/**********************************************************************************
* 
* Timer.cpp
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

#include "jam/Timer.h"

#include <jam/Application.h>

namespace jam
{

	int Timer::m_timersCount = 0 ;
	const float	Timer::m_timeScale = 0.001f ;
	const int Timer::m_timeSpeed = 1;

	Timer::Timer() :
		m_startTime(0),
		m_currentTime(0),
		m_lastTime(0),
		m_elapsedTime(0),
		m_elapsedTimeSecs(0),
		m_endTime(0),
		m_running(false),
		m_isSweep(false),
		m_sweepDelay(0),
		m_repeatSweep(1),
		m_repeatSweepTime(0),
		m_lastRunning(false),
		m_pausing(false),
		m_remain(0)
	{
		//JAM_TRACE( ("Timer created") ) ;
		m_timersCount++ ;
		reset() ;
	}

	Timer::~Timer()
	{
		m_timersCount-- ;
		//JAM_TRACE( ("Timer destroyed") ) ;
	}

	void Timer::start()
	{
		m_lastRunning = true;
		m_running = true ;
		m_pausing = false;
	}

	void Timer::startWithDelay(uint64_t delay)
	{
		m_running = true ;
		m_startTime += delay * m_timeSpeed ;
	}

	void Timer::pause()
	{
		if (m_pausing) return;
		m_lastRunning=m_running;
		m_running = false ;
		m_pausing = true;

		m_remain = m_endTime - m_lastTime;
		m_elapsedTime = m_currentTime - m_lastTime ;
		m_elapsedTimeSecs = m_elapsedTime*m_timeScale ;

		m_lastTime = GetAppMgr().getTotalElapsedMs() ;
		m_currentTime = m_lastTime ;

	}

	void Timer::resume()
	{
		if (!m_pausing) return;
		m_running = m_lastRunning;
		m_pausing = false;

		m_currentTime = GetAppMgr().getTotalElapsedMs() ;
		m_elapsedTime = 0;
		
		m_endTime = m_currentTime + m_remain;

		m_lastTime = m_currentTime;
		m_startTime = m_lastTime ;
		
		m_remain = m_endTime - m_lastTime;
	}

	void Timer::stop()
	{
		m_lastRunning = false;
		m_running = false ;
		m_pausing = true;
	}

	void Timer::reset()
	{
		//m_lastTime = s3eTimerGetMs() ;
		m_lastTime = GetAppMgr().getTotalElapsedMs() ;
		m_startTime = m_lastTime ;
		m_currentTime = m_startTime ;
		m_remain = m_endTime - m_lastTime;
	}

	void Timer::update()
	{
		m_isSweep = m_running && m_currentTime >= m_endTime ;

		if( isRunning() ) {
			m_currentTime = GetAppMgr().getTotalElapsedMs() ;			
			m_elapsedTime = m_currentTime - m_lastTime ;
			m_elapsedTimeSecs = m_elapsedTime*m_timeScale ;
			m_lastTime = m_currentTime ;

			checkSweep() ;
		}
	}

	void Timer::checkSweep()
	{
		if( m_isSweep && m_repeatSweep != 0 ) 
		{
			TimeExpiredEventArgs* evtArgs = TimeExpiredEventArgs::create() ;
			m_timeExpiredEvent.fire(evtArgs,this) ;
			if( m_repeatSweep > 0 ) { m_repeatSweep-- ; }
			setSweep(m_repeatSweepTime != 0 ? m_repeatSweepTime : m_sweepDelay ) ;
		}
	}

	float Timer::getTotalElapsed() const
	{
		return (m_currentTime - m_startTime ) * m_timeScale ;
	}

	uint64_t Timer::getTotalElapsedMs() const
	{
		return m_currentTime - m_startTime ;
	}

	bool Timer::isSweep() const
	{
		return m_isSweep ;
	}

	uint64_t Timer::sweepLeft() const
	{
		return m_endTime - m_currentTime;
	}

	void Timer::setSweep(uint64_t delay)
	{
		delay *= m_timeSpeed;
		m_sweepDelay = delay ;
		m_endTime = m_currentTime + delay;		
	}

	void Timer::setRepeatSweep( int32_t v )
	{
		m_repeatSweep = v ;
	}

	void Timer::setRepeatSweepTime( uint64_t t )
	{
		m_repeatSweepTime = t * m_timeSpeed ;
	}

	Timer* Timer::create()
	{
		return new (GC) Timer() ;
	}

	void TimerManager::update()
	{
		Timer* pTimer ;
		for( auto& n : getManagerMap() ) {
			pTimer = n.second ;

			if( !pTimer->isRunning() )
				continue ;

			pTimer->update();
		}
	}

	void TimerManager::pauseAll()
	{
		Timer* pTimer ;
		for( auto& n : getManagerMap() ) {
			pTimer = n.second ;
			pTimer->pause();
		}
	}

	void TimerManager::resumeAll()
	{
		Timer* pTimer ;
		for( auto& n : getManagerMap() ) {
			pTimer = n.second ;
			pTimer->resume();
		}
	}

	void TimerManager::stopAll()
	{
		Timer* pTimer ;
		for( auto& n : getManagerMap() ) {
			pTimer = n.second ;
			pTimer->stop();
		}
	}

	void TimerManager::startAll()
	{
		Timer* pTimer ;
		for( auto& n : getManagerMap() ) {
			pTimer = n.second ;
			pTimer->start();
		}
	}

	TimeExpiredEventArgs* TimeExpiredEventArgs::create()
	{
		return new (GC) TimeExpiredEventArgs() ;
	}
}
