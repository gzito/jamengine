/**********************************************************************************
* 
* Timer.h
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

#ifndef __JAM_TIMER_H__
#define __JAM_TIMER_H__

#include <jam/jam.h>
#include <jam/BaseManager.hpp>
#include <jam/Event.h>

#include <jam/Object.h>

namespace jam
{
// fired
class TimeExpiredEventArgs : public EventArgs
{
public:
	/** Creates a new TimeExpiredEventArgs */
	static TimeExpiredEventArgs*	create() ;

private:
	TimeExpiredEventArgs() = default ;
};

/**
* This class supplies a way for measuring elapsed time
*/
class JAM_API Timer : public NamedObject
{
public:
	friend class TimerManager ;
	typedef Event<TimeExpiredEventArgs>		TimeExpiredEvent ;

	// dtor
	virtual					~Timer() ;

	/** Create a timer */
	static Timer*			create() ;

	/** Turns the timer on. */
	void					start() ;

	/** Turns the timer on, after waiting for the specified milliseconds. */
	void					startWithDelay(uint64_t delay) ;

	/** Turns the timer off. */
	void					stop() ;

	/** Resets the timer without changing its state. */
	void					reset() ;

	/** This method must be called periodically to update the timer progress. */
	void					update() ;

	/** Gets the time elapsed from 2 update call, in seconds */
	float					getElapsed() const { return m_elapsedTimeSecs ; }

	/** Gets the time elapsed since the timer was started, in seconds. */
	float					getTotalElapsed() const ;

	/** Gets the time elapsed from 2 update call, in milliseconds. */
	uint64_t				getElapsedMs() const { return m_elapsedTime ; }

	/** Gets the time elapsed since the timer was started, in milliseconds. */
	uint64_t				getTotalElapsedMs() const ;

	/** Returns true if timer is on. */
	bool					isRunning() const { return m_running ; }

	/** Returns true if sweep interval is expired. */
	bool					isSweep() const;

	/** Sets a sweep interval, in milliseconds */
	void					setSweep(uint64_t delay);

	/** Returns how many milliseconds remain before to sweep. */
	uint64_t				sweepLeft() const;

	/** Pause the timer changing its state. */

	void					pause();
	/** Resume the timer changing its state. */
	void					resume();

	/**
	Sets the sweep repeat count.
	\param v Set to -1 to repeat forever, otherwise a value greater than 0.
	\remark Default value is 1
	*/
	void					setRepeatSweep( int32_t v ) ;

	/**
	Sets the sweep time to set after the first sweeep.
	\param t The sweep time in milliseconds
	\remark Default value is the one setted with setSweep.
	*/
	void					setRepeatSweepTime( uint64_t t ) ;

	/** Returns the event object associated to this timer */
	TimeExpiredEvent&		getTimeExpiredEvent() { return m_timeExpiredEvent; }

	uint64_t				getRemain() const { return m_remain; }

private:
	// Default constructor.
							Timer() ;

	void					checkSweep() ;

	uint64_t				m_startTime ;
	uint64_t				m_currentTime ;
	uint64_t				m_elapsedTime ;
	uint64_t				m_lastTime ;
	uint64_t				m_endTime;
	uint64_t				m_remain;
		
		
	float					m_elapsedTimeSecs ;
	bool					m_running ;
	bool					m_isSweep;
	bool					m_pausing;
	bool					m_lastRunning;

	uint64_t				m_sweepDelay ;

	int32_t					m_repeatSweep ;
	uint64_t				m_repeatSweepTime ;

	TimeExpiredEvent		m_timeExpiredEvent ;

	static const float		m_timeScale ;
	static const int		m_timeSpeed ;

	static int				m_timersCount ;
};

class JAM_API TimerManager : public NamedObjectManager<Timer>
{

public:
							TimerManager() = default ;

	void					update() ;
	void					pauseAll();
	void					resumeAll();
	void					stopAll();
	void					startAll();
};

}

#endif // __JAM_TIMER_H__
