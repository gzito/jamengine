/**********************************************************************************
* 
* Action.cpp
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
#include "jam/Action.h"
#include "jam/Node.h"
#include "jam/ActionInterval.h"

using namespace std ;

namespace jam
{
#ifdef JAM_DEBUG
	int32_t Action::m_totCount = 0 ;
#endif

Action::Action() :
	m_pOriginalTarget(0),
	m_pTarget(0)
{
#ifdef JAM_DEBUG
	m_totCount++ ;
#endif
}

Action::~Action()
{
#ifdef JAM_DEBUG
	m_totCount-- ;
#endif
}

bool Action::isDone()
{
	return true ;
}

void Action::startWithTarget( Node* pTarget )
{
	m_pOriginalTarget = m_pTarget = pTarget ;

	if( m_pTarget ) {
		vector<pair<String,String> >::const_iterator it = m_attributesOnStart.begin() ;
		for( ; it!=m_attributesOnStart.end(); it++ ) {
			m_pTarget->setAttribute(it->first,it->second) ;								
		}
	}
}

void Action::stop()
{
	if( m_pTarget ) {

		vector<pair<String,String> >::const_iterator it = m_attributesOnEnd.begin() ;
		for( ; it!=m_attributesOnEnd.end(); it++ ) {
			m_pTarget->setAttribute(it->first,it->second) ;								
		}

		m_pTarget = 0 ;
	}
}

// overrides
void Action::step( jam::time deltaTime )
{
	
}

// overrides
void Action::update( jam::time time )
{

}


void Action::setAttributeOnStart( const String& key, const String& val )
{
	m_attributesOnStart.push_back( pair<String,String>(key,val) ) ;
}

void Action::setAttributeOnEnd( const String& key, const String& val )
{
	m_attributesOnEnd.push_back( pair<String,String>(key,val) ) ;
}

// implemented in derived classes
FiniteTimeAction* FiniteTimeAction::reverse( void )
{
	return nullptr ;
}

//
// Speed
//
Speed::~Speed()
{
}

Speed* Speed::actionWithAction(ActionInterval* pAction, float fRate)
{
	Speed* pRet = new Speed() ;
	if( pRet ) {
		pRet->initWithAction(pAction, fRate) ;
	}
	return pRet;
}

bool Speed::initWithAction(ActionInterval* pAction, float fRate)
{
	assert(pAction != 0);
	m_pOther = pAction;
	m_fSpeed = fRate;	
	return true;
}

void Speed::startWithTarget(Node* pTarget)
{
	Action::startWithTarget(pTarget);
	m_pOther->startWithTarget(pTarget);
}

void Speed::stop()
{
	m_pOther->stop();
	Action::stop();
}

void Speed::step(jam::time dt)
{
	m_pOther->step(dt * m_fSpeed);
}

bool Speed::isDone()
{
	return m_pOther->isDone();
}

ActionInterval* Speed::reverse()
{
	return dynamic_cast<ActionInterval*>(Speed::actionWithAction(m_pOther->reverse(), m_fSpeed)) ;
}

Speed::Speed()
{
}

}
