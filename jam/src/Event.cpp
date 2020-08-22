/**********************************************************************************
* 
* Event.cpp
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

#include "StdAfx.h"

#include "jam/Event.h"
#include "jam/ObjectManager.h"

namespace jam
{
EventArgs::EventArgs() : m_isConsumed(false), m_timestamp(0.0f)
{
}

EventDispatcher::EventDispatcher()
{
}

EventDispatcher::~EventDispatcher()
{
}

EventDispatcher::EventEventArgsPair::EventEventArgsPair(IEvent* ev,EventArgs* args,IEventSource* evSrc) :
	m_event(ev), m_eventArgs(args), m_eventSource(evSrc)
{
}

void EventDispatcher::enqueue( IEvent* evt, EventArgs* evtArgs, IEventSource* evtSrc )
{
	evtArgs->addRef() ;
	evtSrc->addRef() ;
	EventEventArgsPair p(evt,evtArgs,evtSrc) ;
	m_queue.push(p) ;
}

void EventDispatcher::dispatch()
{
	while( !m_queue.empty() ) {
		EventEventArgsPair p = m_queue.front() ;
		p.m_event->fire( p.m_eventArgs, p.m_eventSource ) ;
		p.m_eventArgs->release() ;
		p.m_eventSource->release() ;
		m_queue.pop();
	}
}

void EventDispatcher::removeAllEnqueuedEvents()
{
	while( !m_queue.empty() ) {
		EventEventArgsPair p = m_queue.front() ;
		p.m_eventArgs->release() ;
		p.m_eventSource->release() ;
		m_queue.pop();
	}
}
}