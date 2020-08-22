/**********************************************************************************
* 
* StateMachine.cpp
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

#include "jam/StateMachine.h"
#include "jam/State.h"

namespace jam
{
namespace game
{

void StateMachine::start( State* initialState )
{
	m_pNewState = m_pInitialState = initialState;
}

void StateMachine::checkNewState()
{
	if( m_pNewState ) {
		bool stateChanged = m_pNewState != m_pCurrentState;
		m_pCurrentState = m_pNewState ;
		if( stateChanged ) m_pCurrentState->init() ;
		m_pCurrentState->begin() ;
		m_pNewState = 0 ;
	}
}

void StateMachine::update()
{
	const TransitionList& transitions = m_pCurrentState->getTransitions() ;
	if( !transitions.empty() ) {

		Transition* triggeredTransition = 0 ;

		for( TransitionList::const_iterator it = transitions.begin(); it != transitions.end(); it++ ) {
			Transition* t = *it ;
			if( t->isTriggered() ) {
				triggeredTransition = t ;
				break ;
			}
		}

		if( triggeredTransition ) {
			State* targetState = triggeredTransition->getTargetState() ;
			bool stateChanged = targetState != m_pCurrentState;
			m_pCurrentState->end() ;
			if(stateChanged) m_pCurrentState->destroy() ;
			m_pNewState = targetState ;
		}
	}
}


StateMachine::StateMachine() :
	m_pInitialState(0),
	m_pCurrentState(0),
	m_pNewState(0)
{

}

StateMachine::~StateMachine()
{

}

}
}
