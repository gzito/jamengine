/**********************************************************************************
* 
* ActionInstant.h
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

#ifndef __JAM_ACTIONINSTANT_H__
#define __JAM_ACTIONINSTANT_H__

#include <jam/jam.h>
#include <jam/Action.h>
#include <jam/core/geom.h>

namespace jam
{
class Node; 

/** 
@brief Instant actions are immediate actions. They don't have a duration like
the IntervalAction actions.
*/ 
class JAM_API ActionInstant : public FiniteTimeAction
{
public:
	// overrides action methods
	//virtual RefCountedObject* clone() ;
	virtual void step(jam::time dt);
	virtual FiniteTimeAction* reverse();

protected:
	ActionInstant() {}
};

/** @brief Show the node
*/
class JAM_API Show : public ActionInstant
{
public:
	virtual ~Show();
	//super methods
	virtual void startWithTarget(Node *pTarget);
	virtual FiniteTimeAction * reverse(void);
public:
	//override static method
	/** Allocates and initializes the action */
	static Show * action();

protected:
	Show();
};

/** 
@brief Hide the node
*/
class JAM_API Hide : public ActionInstant
{
public:
	virtual ~Hide();
	//super methods
	virtual void startWithTarget(Node *pTarget);
	virtual FiniteTimeAction * reverse(void);
public:
	//override static method
	/** Allocates and initializes the action */
	static Hide * action();

protected:
	Hide();

};

/** @brief Toggles the visibility of a node
*/
class JAM_API ToggleVisibility : public ActionInstant
{
public:
	virtual ~ToggleVisibility(){}
	//super method
	virtual void startWithTarget(Node *pTarget);
public:
	//override static method
	/** Allocates and initializes the action */
	static ToggleVisibility * action();

protected:
	ToggleVisibility(){}
};

/** @brief Places the node in a certain position
*/
class JAM_API Place : public ActionInstant //<NSCopying>
{
public:
	virtual ~Place(){}
	/** creates a Place action with a position */
	static Place* actionWithPosition(const Vector2& pos);
	/** Initializes a Place action with a position */
	bool initWithPosition(const Vector2& pos);
	//super methods
	virtual void startWithTarget(Node *pTarget);

protected:
	Place(){}

	Vector2 m_tPosition;
};

/** @brief Calls a 'callback'
*/
template <class T>
class CallInstanceFunc : public ActionInstant
{
public:
	typedef void (T::*CallbackMethod)();

	/** creates the action with the callback */
	static CallInstanceFunc* actionWithTarget(T* pTargetObject, CallbackMethod method);
	
	/** initializes the action with the callback */
	virtual bool initWithTarget(T* pTargetObject, CallbackMethod method);
	
	/** executes the callback */
	virtual void execute();
	
	//super methods
	virtual void startWithTarget(Node *pTarget);

protected:
	CallInstanceFunc() ;

	T*				m_pTargetObject ;
	CallbackMethod	m_pMethod ;
};

/** @brief Calls a 'callback'
*/
class JAM_API CallStaticFunc : public ActionInstant
{
public:
	typedef void (*CallbackFunc)();

	/** creates the action with the callback */
	static CallStaticFunc* actionWithTarget(CallbackFunc staticFunc);
	
	/** initializes the action with the callback */
	virtual bool initWithTarget(CallbackFunc staticFunc);
	
	/** executes the callback */
	virtual void execute();
	
	//super methods
	virtual void startWithTarget(Node *pTarget);

protected:
	CallStaticFunc();

	CallbackFunc	m_pFunc ;
};

class JAM_API DestroyTarget : public ActionInstant
{
public:
	virtual ~DestroyTarget();

	//super methods
	virtual void startWithTarget(Node *pTarget) ;

public:
	//override static method
	/** Allocates and initializes the action */
	static DestroyTarget * action(Node *pTarget);

protected:
	DestroyTarget();
};



/** 
@brief Enable the node
*/
class JAM_API Enable : public ActionInstant
{
public:
	virtual ~Enable();
	//super methods
	virtual void startWithTarget(Node *pTarget);
	virtual FiniteTimeAction * reverse(void);
public:
	//override static method
	/** Allocates and initializes the action */
	static Enable * action();

protected:
	Enable();

};


/** 
@brief Disable the node
*/
class JAM_API Disable : public ActionInstant
{
public:
	virtual ~Disable();
	//super methods
	virtual void startWithTarget(Node *pTarget);
	virtual FiniteTimeAction * reverse(void);
public:
	//override static method
	/** Allocates and initializes the action */
	static Disable * action();

protected:
	Disable();

};




// CallFuncT implementation
template <class T>
CallInstanceFunc<T>::CallInstanceFunc() : ActionInstant(), m_pMethod(0), m_pTargetObject(0)
{
}

template <class T>
CallInstanceFunc<T>* CallInstanceFunc<T>::actionWithTarget( T* pTargetObject, CallbackMethod method )
{
	CallInstanceFunc<T>* pCallFunc = new CallInstanceFunc<T>();
	if( pCallFunc->initWithTarget(pTargetObject,method) ) {
		pCallFunc->autorelease() ;
		return pCallFunc;
	}

	JAM_DELETE(pCallFunc);
	return 0;
}

template <class T>
bool CallInstanceFunc<T>::initWithTarget(T* pTargetObject, CallbackMethod method)
{
	m_pTargetObject = pTargetObject ;
	m_pMethod = method;
	return true;
}

template <class T>
void CallInstanceFunc<T>::execute()
{
	if( m_pTargetObject && m_pMethod) {
		(m_pTargetObject->*m_pMethod)();
	}
}

template <class T>
void CallInstanceFunc<T>::startWithTarget( Node *pTarget )
{
	ActionInstant::startWithTarget(pTarget);
	this->execute();
}

}

#endif // __JAM_ACTIONINSTANT_H__
