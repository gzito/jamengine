/**********************************************************************************
* 
* SysTimer.cpp
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
#include <ctime>

#include "jam/SysTimer.h"

namespace jam
{

	// SysTimer

	SysTimer::SysTimer()
	{
		// GZ TODO:
		// Here the engine should determine the appropriate timescale (ms, usec or nanosecs) for the platform
		// For now default to milliseconds

		// get timer resolution, see: http://www.cs.rutgers.edu/~pxk/416/notes/c-tutorials/gettime.html
//		timespec ts = {0} ;
//		clock_getres(CLOCK_MONOTONIC, &ts);

		// sets the timescale as nanoseconds 
//		fpGetTime = &SysTimer::getTimeNanoSecs ;
//		m_timerResolution = 1000000000UL ;

		// sets the timescale as microseconds 
//		fpGetTime = &SysTimer::getTimeUSecs ;
//		m_timerResolution = 1000000UL ;

		// sets the timescale as milliseconds
		fpGetTime = &SysTimer::getTimeMs ;
		m_unitsPerSecond = 1000UL ;
	}

	uint64_t SysTimer::getTimeMs()
	{
		// CLOCKS_PER_SEC EQU 1000 on Windows
		return clock() ;
	}

/*
	uint64_t SysTimer::getTimeUSecs()
	{
		timeval tv = {0} ;
		gettimeofday(&tv,0) ;
		return ((uint64_t)tv.tv_sec)*1000000UL + tv.tv_usec ;
	}

	//
	// C Tutorial: Use Linux's high resolution clock
	// See: http://www.cs.rutgers.edu/~pxk/416/notes/c-tutorials/gettime.html
	//
	uint64_t SysTimer::getTimeNanoSecs()
	{
		timespec ts = { 0 } ;
		clock_gettime(CLOCK_MONOTONIC,&ts) ;
		return ((uint64_t)ts.tv_sec)*1000000000UL + ts.tv_nsec ;
	}
*/
}
