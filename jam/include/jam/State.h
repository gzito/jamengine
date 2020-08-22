/**********************************************************************************
* 
* State.h
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

#ifndef __JAM_STATE_H__
#define __JAM_STATE_H__

#include <list>
#include <jam/BankItem.h>

namespace jam
{
namespace game
{

// fw reference
class State ;

/**
	Define the condition to be tested in order to trigger transition from the current state to a new state
*/
class ICondition : public RefCountedObject
{
public:
	virtual					~ICondition() {}
	virtual bool			test() = 0 ;
};

class AndCondition : public ICondition
{
public:
							AndCondition( ICondition* condA, ICondition* condB ) ;
	virtual					~AndCondition() ;

	virtual bool			test() override ;

protected:
	ICondition*				m_pConditionA ;
	ICondition*				m_pConditionB ;
};

class OrCondition : public ICondition
{
public:
							OrCondition( ICondition* condA, ICondition* condB ) ;
	virtual					~OrCondition() ;

	virtual bool			test() override ;

protected:
	ICondition*				m_pConditionA ;
	ICondition*				m_pConditionB ;
};

class NotCondition : public ICondition
{
public:
							NotCondition( ICondition* cond ) ;
	virtual					~NotCondition() ;

	virtual bool			test() ;

protected:
	ICondition*				m_pCondition ;
};

/**
	Represents a Transition linking two states
*/
class Transition : public RefCountedObject
{
public:
							Transition( ICondition* pCondition = 0, State* targetState = 0 ) ;
	virtual					~Transition() ;

	/** Returns true if the condition linked to this transition is met (in this case the transition is said "triggered"). */
	virtual bool			isTriggered() ;

	/** Returns the Condition to be tested in order to be triggered */
	ICondition*				getCondition() { return m_pCondition; }

	/** Sets the Condition to be tested in order to be triggered */
	void					setCondition(ICondition* pCondition) { m_pCondition = pCondition; }
	
	/** Returns which state to transition to */
	State*					getTargetState() ;

	/** Sets which state to transition to */
	void					setTargetState( State* targetState ) ;

private:
	State*					m_pTargetState ;
	ICondition*				m_pCondition ;
};

typedef std::list<Transition*>		TransitionList ;


/**
	This class represents a state inside a FSM
*/
class State : public BankItem
{
public:
	virtual					~State() ;

	/**
		Called when this state is entered and only if the previous state is different by the new one
		\remark You can use this method to initialize resources and to load media and grafix.
				The engine will call the corresponding method defined in the Application class before,
				then it will call this method.
	*/
	virtual void			init() {}
	
	/**
		Called when this state is entered.
		\remark You can use this method to initialize variables (e.g. at the start of a round)
				There is not a corresponding method defined in the Application class.
	*/
	virtual void			begin() {}

	/**
		Called when the current frame is entered.

		\remark The engine will call the corresponding method defined in the Application class before,
				then it will call this method.
	*/
	virtual void			enterFrame() {}

	/**
		Called just after entering the frame.

		\remark The engine will call the corresponding method defined in the Application class before,
				then it will call this method.
	*/
	virtual void			handleInput() {}

	/**
		Called before the scene is updated.

		\remark The engine will call the corresponding method defined in the Application class before,
				then it will call this method.
	*/
	virtual void			beforeSceneUpdate() {}

	/**
		Called after the scene is updated.

		\remark The engine will call the corresponding method defined in the Application class before,
				then it will call this method.
	*/
	virtual void			afterSceneUpdate() {}

	/**
		Called before the scene is rendered.

		\remark The engine will call the corresponding method defined in the Application class before,
				then it will call this method.
	*/
	virtual void			render() {}


	/**
		Called after the scene is rendered, using the same batch of the scene

		\remark The engine will call this method just after the scene was rendered in the same batch.
	*/
	virtual void			extraRender() {}

	/**
		Called just before the current frame ends.

		\remark The engine will call the corresponding method defined in the Application class before,
				then it will call this method.
	*/
	virtual void			exitFrame() {}

	/** 
		Called when this state is exited.

		\remark You can use this method the cleanup variables (e.g. at the end of a round)
				There is not a corresponding method defined in the Application class.
	*/
	virtual void			end() {}

	/** 
		Called when this state is exited and only if the new state is different by this one.
		
		\remark You can use this method as last chance to cleanup, e.g. to release resources and unload media
	*/
	virtual void			destroy() {}

	/** Returns the list of transitions that are outgoing from this state */
	const TransitionList&	getTransitions();

	/**
		Links this state to another state, using the given Transition.

		\remark You may optional supply a ICondition instance, or passing 0 to use the one inside the given Transition
	*/
	void					linkState(State* targetState, Transition* transition, ICondition* cond = 0 ) ;

protected:
							State() = default ;
	void					addTransition(Transition* transition) ;

	TransitionList			m_transitions ;
};

}
}

#endif // __JAM_STATE_H__
