/**********************************************************************************
* 
* State.cpp
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

#include "jam/State.h"
#include "jam/Object.h"

namespace jam
{
namespace game
{

//
// Conditions
//
bool AndCondition::test()
{
	return m_pConditionA->test() && m_pConditionB->test() ; 
}

AndCondition::AndCondition( ICondition* condA, ICondition* condB ) :
	m_pConditionA(condA), m_pConditionB(condB)
{
}

AndCondition::~AndCondition()
{
	m_pConditionA = nullptr ;
	m_pConditionB = nullptr ;
}


bool OrCondition::test()
{
	return m_pConditionA->test() || m_pConditionB->test() ; 
}

OrCondition::OrCondition( ICondition* condA, ICondition* condB ) :
	m_pConditionA(condA), m_pConditionB(condB)
{

}

OrCondition::~OrCondition()
{
	m_pConditionA = nullptr ;
	m_pConditionB = nullptr ;
}


bool NotCondition::test()
{
	return !m_pCondition->test() ; 
}

NotCondition::NotCondition( ICondition* cond ) :
	m_pCondition(cond)
{

}

NotCondition::~NotCondition()
{
	m_pCondition = nullptr ;
}


//
// Transition
//
State* Transition::getTargetState()
{
	return m_pTargetState ;
}

void Transition::setTargetState( State* targetState )
{
	m_pTargetState = targetState ;
}

bool Transition::isTriggered()
{
	return m_pCondition->test() ;
}

Transition::Transition( ICondition* pCondition /*= 0 */, State* targetState /*= 0 */ ) :
	m_pTargetState(targetState), m_pCondition(pCondition)
{

}

Transition::~Transition()
{
	m_pCondition = nullptr ;
}


//
// State
//
const TransitionList& State::getTransitions()
{
	return m_transitions ;
}

void State::linkState( State* targetState, Transition* transition, ICondition* cond /*= 0*/ )
{
	transition->setTargetState(targetState) ;
	if( cond ) {
		transition->setCondition(cond) ;
	}
	addTransition( transition ) ;
}

void State::addTransition( Transition* transition )
{
	m_transitions.push_back(transition) ;
}

}
}
