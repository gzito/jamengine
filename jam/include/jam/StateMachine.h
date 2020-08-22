/**********************************************************************************
* 
* StateMachine.h
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

#ifndef __JAM_STATEMACHINE_H__
#define __JAM_STATEMACHINE_H__

#include <jam/jam.h>
#include <jam/State.h>
#include <jam/Singleton.h>
#include <jam/Bank.h>

#include <vector>


namespace jam
{
namespace game
{

/**
	This class implement a simple finite state machine (FSM).

	You are responsible to create and add states to it (via Bank interface)
*/
class JAM_API StateMachine : public Bank<State>, public Singleton<StateMachine>
{
	friend class Singleton<StateMachine> ;

public:
	/** Starts the state machine, given the starting state. */
	void					start( State* initialState );

	/**
		Update the state machine.
		\remark This method is called by Application class, normally you don't need to explicitly call it
	*/
	void					update();

	/**
		Check if machine entered a new state.
		\remark This method is called by Application class, normally you don't need to explicitly call it
	*/
	void					checkNewState() ;

	/** Returns if the machine is stated */
	bool					isStarted() const { return m_pInitialState!=0; }

	/** Returns the current state */
	State*					getCurrentState() const { return m_pCurrentState; }

	bool					stateChanged() const { return m_pNewState!=0; }

private:
							StateMachine();	
	virtual					~StateMachine();

	State*					m_pCurrentState ;
	State*					m_pNewState ;
	State*					m_pInitialState ;
};

/** Returns the singleton instance */
JAM_INLINE StateMachine&	GetStateMachine() { return StateMachine::getSingleton(); }

}
}

#endif // __JAM_STATEMACHINE_H__
