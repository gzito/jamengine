/**********************************************************************************
* 
* Event.h
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

#ifndef __JAM_EVENT_H__
#define __JAM_EVENT_H__

#include <jam/jam.h>
#include <jam/RefCountedObject.h>
#include <jam/Singleton.h>
#include <jam/Application.h>		// for Eventargs timestamp
#include <jam/thirdparty/MultiCastDelegate.h>

#include <queue>

namespace jam {

/**
* This interface represent a source of event
* Classes firing events have to implement this interface
*/
using IEventSource = RefCountedObject ;
	
/**
* This class represent parameters attached to an Event instance
* You may want to specialize this class to enrich your own event parameters.
*/
class JAM_API EventArgs : public RefCountedObject
{
public:    
	virtual					~EventArgs() = default ;

	/**
	* Event handlers may set this attribute to true to avoid the event is passed down the event handlers chain
	*/
	bool					isConsumed() const { return m_isConsumed; }
	void					setConsumed(bool val=true) { m_isConsumed = val; }

	/**
	* Return the time when this event was fired or queued, in seconds
	*
	* \remark The time is relative to the start of application
	*/
	float					getTimestamp() const { return m_timestamp; }
	void					setTimestamp(float val) { m_timestamp = val; }

protected:
							EventArgs() : m_isConsumed(false), m_timestamp(0.0f) {}

	bool					m_isConsumed ;
	float					m_timestamp ;
};

/**
* Interface IEvent
* Event class implement it
*/
class JAM_API IEvent : public RefCountedObject
{
public:
							IEvent() = default ;
	virtual					~IEvent() = default ;

	virtual void			fire( EventArgs* eventArgs, IEventSource* s ) = 0 ;
	virtual void			enqueue( EventArgs* eventArgs, IEventSource* s ) = 0 ;
};

/**
* EventDispatcher
*/
class JAM_API EventDispatcher : public Singleton<EventDispatcher>
{
	friend class Singleton<EventDispatcher> ;

public:
	void					enqueue(IEvent* evt, EventArgs* args, IEventSource* evSrc) ;
	void					dispatch();
	void					removeAllEnqueuedEvents() ;

private:

	struct EventEventArgsPair
	{
	public:
		EventEventArgsPair(IEvent* evt, EventArgs* args, IEventSource* evSrc) ;
		IEvent*				m_event ;
		EventArgs*			m_eventArgs ;
		IEventSource*		m_eventSource ;
	} ;

	using EventEventArgsQueue = std::queue<EventEventArgsPair> ;

	EventEventArgsQueue		m_queue ;

							EventDispatcher() = default ;
	virtual					~EventDispatcher() = default ;
};


template <class T> using EventHandler = SA::delegate<void(T&, IEventSource&)> ;

/**
* class Event<T>
* T is the EventArgs specific class name (i.e. TouchEventArgs)
*/
template <class T>
class Event : public IEvent
{
public:
	/** Constructor */
							Event() = default ;

	/** Destructor */
	virtual					~Event() ;

	Event&					operator+=( const EventHandler<T>& handler ) ; 
	Event&					operator-=( const EventHandler<T>& handler ) ; 

	/** Immediatly fires the given event */
	virtual void			fire( EventArgs* eventArgs, IEventSource* s ) override ;

	/** Queues an event which will be fired at the next update() invocation */
	virtual void			enqueue( EventArgs* eventArgs, IEventSource* s ) override ;

	/** Unregisters all the event handlers.
	 \remark It is automatically called by the destructor
	*/
	void					removeAllHandlers() ;

private:
	template <class T=EventArgs> using EventHandlers = SA::multicast_delegate<void(T&, IEventSource&)> ;

	EventHandlers<T>		m_eventHandlers ;
};

template <class T>
Event<T>::~Event()
{
	removeAllHandlers() ;
}


template <class T>
Event<T>& Event<T>::operator+=( const EventHandler<T>& handler )
{
	m_eventHandlers += handler ;
	return *this ;
}

template <class T>
Event<T>& Event<T>::operator-=( const EventHandler<T>& handler )
{
	m_eventHandlers -= handler ;
	return *this ;
}


template <class T>
void Event<T>::fire( EventArgs* eventArgs, IEventSource* s )
{
	eventArgs->setTimestamp( GetAppMgr().getTotalElapsed() ) ;
	std::function<bool (void)> fp = std::bind(&EventArgs::isConsumed,eventArgs) ;
	m_eventHandlers.invoke_if( fp, dynamic_cast<T&>(*eventArgs), *s ) ;
}


template <class T>
void Event<T>::enqueue( EventArgs* eventArgs, IEventSource* s )
{
	EventDispatcher::getSingleton().enqueue(this,eventArgs,s) ;
}

template <class T>
void Event<T>::removeAllHandlers()
{
	m_eventHandlers.clear() ;
}

JAM_INLINE EventDispatcher& GetEventDispatcher() { return EventDispatcher::getSingleton(); }

} // namespace jam

#endif // __JAM_EVENT_H__
