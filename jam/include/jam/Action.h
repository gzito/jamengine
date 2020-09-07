/**********************************************************************************
* 
* Action.h
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

#ifndef __JAM_ACTION_H__
#define __JAM_ACTION_H__

#include <jam/jam.h>
#include <jam/Object.h>

#include <vector>
#include <utility>

namespace jam
{

class Node;

/** 
@brief Base class for Action objects.
*/
class JAM_API Action : public NamedTaggedObject
{
public:
#ifdef JAM_DEBUG
	// store the total number of allocated nodes
	static int32_t			m_totCount ;
#endif

	virtual					~Action();

	//! return true if the action has finished
	virtual bool			isDone();

	//! called before the action start. It will also set the target.
	virtual void			startWithTarget(Node* pTarget) ;

	/** 
	called after the action has finished. It will set the 'target' to null.
    IMPORTANT: You should never call "[action stop]" manually. Instead, use: "target->stopAction(action);"
	*/
	virtual void			stop();

	//! called every frame with it's delta time. DON'T override unless you know what you are doing.
	virtual void			step(jam::time deltaTime) ;

	/** 
	called once per frame. time a value between 0 and 1

	For example: 
	- 0 means that the action just started
	- 0.5 means that the action is in the middle
	- 1 means that the action is over
	*/
	virtual void			update(jam::time time) ;

	Node*					getTarget() { return m_pTarget; }
	/** The action will modify the target properties. */
	void					setTarget(Node* val) { m_pTarget = val; }

	/**
	Set the original target, since target can be null.
	Is the target that were used to run the action. Unless you are doing something complex, like ActionManager, you should NOT call this method.
	*/
	Node*					getOriginalTarget() { return m_pOriginalTarget; }

	void					setAttributeOnStart(const String& key, const String& val);
	void					setAttributeOnEnd(const String& key, const String& val) ;
	
protected:
							Action();

	Node*					m_pOriginalTarget ;
	Node*					m_pTarget ;

private:
	std::vector<std::pair<String,String> >	m_attributesOnStart ;
	std::vector<std::pair<String,String> >	m_attributesOnEnd ;
};


/** 
@brief 
 Base class actions that do have a finite time duration.
 Possible actions:
   - An action with a duration of 0 seconds
   - An action with a duration of 35.5 seconds

 Infinite time actions are valid
 */
class JAM_API FiniteTimeAction : public Action
{
public:
	virtual					~FiniteTimeAction() {}
    //! get duration in seconds of the action
	JAM_INLINE jam::time	getDuration(void) { return m_fDuration; }
	//! set duration in seconds of the action
	JAM_INLINE void			setDuration(jam::time duration) { m_fDuration = duration; }

	//! returns a reversed action
	virtual FiniteTimeAction* reverse(void);

protected:
							FiniteTimeAction() : m_fDuration(0) {}

	//! duration in seconds
	jam::time				m_fDuration;
};

// fwd reference
class ActionInterval;

/** 
 @brief Changes the speed of an action, making it take longer (speed>1)
 or less (speed<1) time.
 Useful to simulate 'slow motion' or 'fast forward' effect.
 @warning This action can't be Sequenceable because it is not an IntervalAction
 */
class JAM_API Speed : public Action
{
public:
	virtual					~Speed();

	JAM_INLINE float		getSpeed() { return m_fSpeed; }
	/** alter the speed of the inner function in runtime */
	JAM_INLINE void			setSpeed(float fSpeed) { m_fSpeed = fSpeed; }

	/** initializes the action */
	bool					initWithAction(ActionInterval* pAction, float fRate);

	//virtual RefCountedObject* clone() ;
	virtual void			startWithTarget(Node* pTarget);
	virtual void			stop();
	virtual void			step(jam::time dt);
	virtual bool			isDone();
	virtual ActionInterval*	reverse();

public:
	/** creates the action */
	static Speed*			actionWithAction(ActionInterval* pAction, float fRate);
    
protected:
							Speed();

	float					m_fSpeed;
	ActionInterval*			m_pOther;
};

}

#endif // __JAM_ACTION_H__
