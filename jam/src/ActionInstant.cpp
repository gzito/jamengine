/**********************************************************************************
* 
* ActionInstant.cpp
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

#include "jam/ActionInstant.h"
#include "jam/Node.h"

namespace jam
{


void ActionInstant::step( jam::time dt )
{
	update(1) ;
}

FiniteTimeAction* ActionInstant::reverse()
{
	return 0 ;
}

//
// Show
//
Show* Show::action()
{
	return new (GC) Show();
}
void Show::startWithTarget(Node *pTarget)
{
	ActionInstant::startWithTarget(pTarget);
	pTarget->setVisible(true);
}

FiniteTimeAction *Show::reverse()
{
	return (FiniteTimeAction*)(Hide::action());
}

Show::Show()
{
}

Show::~Show()
{
}

//
// Hide
//
Hide * Hide::action()
{
	return new (GC) Hide();
}
void Hide::startWithTarget(Node *pTarget)
{
	ActionInstant::startWithTarget(pTarget);
	pTarget->setVisible(false);
}

FiniteTimeAction *Hide::reverse()
{
	return (FiniteTimeAction*)(Show::action());
}

Hide::~Hide()
{
}

Hide::Hide()
{
}

//
// ToggleVisibility
//
ToggleVisibility * ToggleVisibility::action()
{
	return new (GC) ToggleVisibility();
}
void ToggleVisibility::startWithTarget(Node *pTarget)
{
	ActionInstant::startWithTarget(pTarget);
	pTarget->setVisible(! pTarget->isVisible());
}

//
// Place
//
Place * Place::actionWithPosition(const Vector2& pos)
{
	Place *pRet = new (GC) Place();
	pRet->initWithPosition(pos);
	return pRet;
}
bool Place::initWithPosition(const Vector2& pos)
{
	m_tPosition = pos;
	return true;
}

void Place::startWithTarget(Node *pTarget)
{
	ActionInstant::startWithTarget(pTarget);
	m_pTarget->setPos(m_tPosition);
}


CallStaticFunc::CallStaticFunc() : ActionInstant(), m_pFunc(0)
{
}

CallStaticFunc* CallStaticFunc::actionWithTarget( CallbackFunc staticFunc )
{
	CallStaticFunc* pCallFunc = new (GC) CallStaticFunc();
	if( pCallFunc && pCallFunc->initWithTarget(staticFunc) ) {
		return pCallFunc ;
	}

	return nullptr;
}

bool CallStaticFunc::initWithTarget( CallbackFunc staticFunc )
{
	m_pFunc = staticFunc;
	return true;
}

void CallStaticFunc::execute()
{
	if( m_pFunc ) {
		(*m_pFunc)();
	}
}

void CallStaticFunc::startWithTarget( Node *pTarget )
{
	ActionInstant::startWithTarget(pTarget);
	this->execute();
}

void DestroyTarget::startWithTarget( Node *pTarget )
{
	ActionInstant::startWithTarget(pTarget);
	pTarget->destroy() ;
}

DestroyTarget * DestroyTarget::action( Node *pTarget )
{
	return new (GC) DestroyTarget() ;
}

DestroyTarget::~DestroyTarget()
{
}

DestroyTarget::DestroyTarget()
{
}

//
// Enable
//
Enable* Enable::action()
{
	return new (GC) Enable() ;
}
void Enable::startWithTarget(Node *pTarget)
{
	ActionInstant::startWithTarget(pTarget);
	pTarget->setVisible(false);
}

FiniteTimeAction *Enable::reverse()
{
	return (FiniteTimeAction*)(Disable::action());
}

Enable::~Enable()
{
}

Enable::Enable()
{
}

//
// Disable
//
Disable * Disable::action()
{
	return new (GC) Disable() ;
}
void Disable::startWithTarget(Node *pTarget)
{
	ActionInstant::startWithTarget(pTarget);
	pTarget->setVisible(false);
}

FiniteTimeAction *Disable::reverse()
{
	return (FiniteTimeAction*)(Enable::action());
}

Disable::~Disable()
{
}

Disable::Disable()
{
}

}
