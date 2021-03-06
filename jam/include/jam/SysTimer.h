/**********************************************************************************
* 
* SysTimer.h
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

#ifndef __JAM_SYSTIMER_H__
#define __JAM_SYSTIMER_H__


#include <jam/jam.h>
#include <jam/Singleton.h>

namespace jam
{

class JAM_API SysTimer : public jam::Singleton<SysTimer>
{
	friend class jam::Singleton<SysTimer> ;

public:
	SysTimer() ;

	uint64_t				getTime() { return this->getTimeMs(); }
	uint64_t				getUnitsPerSecond() { return m_unitsPerSecond; }

private:
	// get the time in milliseconds
	uint64_t				getTimeMs() ;

private:
	uint64_t				m_unitsPerSecond ;
};

/** Returns the singleton instance */
JAM_INLINE jam::SysTimer&	GetSysTimer() { return (SysTimer&) SysTimer::getSingleton(); }
}

#endif // __JAM_SYSTIMER_H__
