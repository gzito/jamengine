/**********************************************************************************
* 
* ActionInterval.cpp
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

#include "jam/ActionInterval.h"
#include "jam/Anim2D.h"
#include "jam/Node.h"
#include "jam/Sprite.h"
//#include "jam/DrawItem.h"

#include <assert.h>
#include <math.h>

namespace jam
{

bool ActionInterval::initWithDuration( jam::time d )
{
	m_fDuration = d;

	// prevent division by 0
	// This comparison could be in step:, but it might decrease the performance
	// by 3% in heavy based action games.
	if (m_fDuration == 0)
	{
		m_fDuration = FLT_EPSILON;
	}

	m_elapsed = 0;
	m_bFirstTick = true;

	return true;
}

bool ActionInterval::isDone( void )
{
	return m_elapsed >= m_fDuration;
}

void ActionInterval::step( jam::time dt )
{
	if (m_bFirstTick)
	{
		m_bFirstTick = false;
		m_elapsed = 0;
	}
	else
	{
		m_elapsed += dt;
	}

	//	update(min(1, m_elapsed/m_fDuration));
	update(1 > m_elapsed/m_fDuration ? m_elapsed/m_fDuration : 1);
}

void ActionInterval::startWithTarget( Node* pTarget )
{
	FiniteTimeAction::startWithTarget(pTarget);
	m_elapsed = 0.0f;
	m_bFirstTick = true;
}

//TODO : implement!
ActionInterval* ActionInterval::reverse( void )
{
	return 0 ;
}

ActionInterval* ActionInterval::actionWithDuration( jam::time d )
{
	ActionInterval *pAction = new ActionInterval();
	pAction->initWithDuration(d);

	return pAction;
}

void ActionInterval::setAmplitudeRate(float amp)
{
	// Abstract class needs implementation
	assert(0);
}

float ActionInterval::getAmplitudeRate(void)
{
	// Abstract class needs implementation
	assert(0);
	return 0;
}


Sequence::Sequence()
{
	m_pActions[0] = nullptr ;
	m_pActions[1] = nullptr ;
}

Sequence::~Sequence()
{
	m_pActions[0] = nullptr ;
	m_pActions[1] = nullptr ;
}

bool Sequence::initOneTwo( FiniteTimeAction *pActionOne, FiniteTimeAction *pActionTwo )
{
	assert(pActionOne != nullptr);
	assert(pActionTwo != nullptr);

	jam::time d = pActionOne->getDuration() + pActionTwo->getDuration();
	ActionInterval::initWithDuration(d);

	m_pActions[0] = pActionOne;
	m_pActions[1] = pActionTwo;

	return true;
}

void Sequence::startWithTarget( Node *pTarget )
{
	ActionInterval::startWithTarget(pTarget);
	m_split = m_pActions[0]->getDuration() / m_fDuration;
	m_last = -1;
}

void Sequence::update( jam::time time )
{
	int found = 0;
	jam::time new_t = 0.0f;

	if (time >= m_split) {
		found = 1;
		if (m_split == 1) {
			new_t = 1;
		}
		else {
			new_t = (time - m_split) / (1 - m_split);
		}
	}
	else {
		found = 0;
		if (m_split != 0) {
			new_t = time / m_split;
		}
		else {
			new_t = 1;
		}
	}

	if (m_last == -1 && found == 1) {
		m_pActions[0]->startWithTarget(m_pTarget);
		m_pActions[0]->update(1.0f);
		m_pActions[0]->stop();
	}

	if (m_last != found) {
		if (m_last != -1)
		{
			m_pActions[m_last]->update(1.0f);
			m_pActions[m_last]->stop();
		}

		m_pActions[found]->startWithTarget(m_pTarget);
	}

	m_pActions[found]->update(new_t);
	m_last = found;
}

void Sequence::stop()
{
	m_pActions[0]->stop();
	m_pActions[1]->stop();
	ActionInterval::stop();
}

ActionInterval* Sequence::reverse()
{
	return Sequence::actionOneTwo(m_pActions[1]->reverse(), m_pActions[0]->reverse());
}

FiniteTimeAction* Sequence::actions( FiniteTimeAction *pAction1, ...)
{
	va_list params;
	va_start(params, pAction1);

	FiniteTimeAction *pNow;
	FiniteTimeAction *pPrev = pAction1;

	while (pAction1)
	{
		pNow = va_arg(params, FiniteTimeAction*);
		if (pNow)
		{
			pPrev = actionOneTwo(pPrev, pNow);
		}
		else
		{
			break;
		}
	}

	va_end(params);
	return pPrev;
}

Sequence* Sequence::actionOneTwo( FiniteTimeAction *pActionOne, FiniteTimeAction *pActionTwo )
{
	Sequence *pSequence = new Sequence();
	pSequence->initOneTwo(pActionOne, pActionTwo);
	return pSequence;
}

//
// Repeat
//
Repeat* Repeat::actionWithAction(FiniteTimeAction *pAction, unsigned int times)
{
	Repeat* pRepeat = new Repeat();
	pRepeat->initWithAction(pAction, times);
	return pRepeat;
}

bool Repeat::initWithAction(FiniteTimeAction *pAction, unsigned int times)
{
	jam::time d = pAction->getDuration() * times;

	if (ActionInterval::initWithDuration(d))
	{
		m_uTimes = times;
		m_pOther = pAction;
		m_uTotal = 0;
		return true;
	}

	return false;
}

Repeat::~Repeat(void)
{
	m_pOther = nullptr ;
}

void Repeat::startWithTarget(Node *pTarget)
{
	m_uTotal = 0;
	ActionInterval::startWithTarget(pTarget);
	m_pOther->startWithTarget(pTarget);
}

void Repeat::stop(void)
{
	m_pOther->stop();
	ActionInterval::stop();
}

// issue #80. Instead of hooking step:, hook update: since it can be called by any 
// container action like Repeat, Sequence, AccelDeccel, etc..
void Repeat::update(jam::time time)
{
	jam::time t = time * m_uTimes;
	if (t > m_uTotal + 1)
	{
		m_pOther->update(1.0f);
		m_uTotal++;
		m_pOther->stop();
		m_pOther->startWithTarget(m_pTarget);

		// repeat is over?
		if (m_uTotal == m_uTimes)
		{
			// so, set it in the original position
			m_pOther->update(0);
		}
		else
		{
			// no ? start next repeat with the right update
			// to prevent jerk (issue #390)
			m_pOther->update(t - m_uTotal);
		}
	}
	else
	{
		float r = fmodf(t, 1.0f);

		// fix last repeat position
		// else it could be 0.
		if (time == 1.0f)
		{
			r = 1.0f;
			m_uTotal++; // this is the added line
		}

		//		m_pOther->update(min(r, 1));
		m_pOther->update(r > 1 ? 1 : r);
	}
}

bool Repeat::isDone(void)
{
	return m_uTotal == m_uTimes;
}

ActionInterval* Repeat::reverse(void)
{
	return Repeat::actionWithAction(m_pOther->reverse(), m_uTimes);
}

Repeat::Repeat()
{
}

//
// RepeatForever
//
RepeatForever::~RepeatForever()
{
	m_pOther = nullptr ;
}

RepeatForever *RepeatForever::actionWithAction(ActionInterval *pAction)
{
	RepeatForever *pRet = new RepeatForever();
	if (pRet ) {
		if( pRet->initWithAction(pAction) ) {
		}
		else {
			pRet=nullptr;
		}
	}
	return pRet;
}

bool RepeatForever::initWithAction(ActionInterval *pAction)
{
	assert(pAction != nullptr);
	m_pOther = pAction;
	return true;
}

void RepeatForever::startWithTarget(Node* pTarget)
{
	ActionInterval::startWithTarget(pTarget);
	m_pOther->startWithTarget(pTarget);
}

void RepeatForever::step(jam::time dt)
{
	m_pOther->step(dt);
	if (m_pOther->isDone())
	{
		jam::time diff = dt + m_pOther->getDuration() - m_pOther->getElapsed();
		m_pOther->startWithTarget(m_pTarget);
		// to prevent jerk. issue #390
		m_pOther->step(diff);
	}
}

bool RepeatForever::isDone()
{
	return false;
}

ActionInterval *RepeatForever::reverse()
{
	return (ActionInterval*)(RepeatForever::actionWithAction(m_pOther->reverse()));
}

RepeatForever::RepeatForever()
{
}

Spawn::~Spawn( void )
{
	m_pOne = nullptr ;
	m_pTwo = nullptr ;
}

bool Spawn::initOneTwo( FiniteTimeAction *pAction1, FiniteTimeAction *pAction2 )
{
	assert(pAction1 != nullptr);
	assert(pAction2 != nullptr);

	jam::time d1 = pAction1->getDuration();
	jam::time d2 = pAction2->getDuration();

	// __super::initWithDuration(fmaxf(d1, d2));
	float maxd = (d1 >= d2 || isnan(d2)) ? d1 : d2;
	ActionInterval::initWithDuration(maxd);

	m_pOne = pAction1;
	m_pTwo = pAction2;

	if (d1 > d2)
	{
		m_pTwo = Sequence::actionOneTwo(pAction2, DelayTime::actionWithDuration(d1 - d2));
	}
	else if (d1 < d2)
	{
		m_pOne = Sequence::actionOneTwo(pAction1, DelayTime::actionWithDuration(d2 - d1));
	}

	return true;
}

void Spawn::startWithTarget( Node *pTarget )
{
	ActionInterval::startWithTarget(pTarget);
	m_pOne->startWithTarget(pTarget);
	m_pTwo->startWithTarget(pTarget);
}

void Spawn::stop( void )
{
	m_pOne->stop();
	m_pTwo->stop();
	ActionInterval::stop();
}

void Spawn::update( jam::time time )
{
	if (m_pOne)
	{
		m_pOne->update(time);
	}
	if (m_pTwo)
	{
		m_pTwo->update(time);
	}
}

ActionInterval* Spawn::reverse( void )
{
	return Spawn::actionOneTwo(m_pOne->reverse(), m_pTwo->reverse());
}

FiniteTimeAction* Spawn::actions( FiniteTimeAction *pAction1, ... )
{
	va_list params;
	va_start(params, pAction1);

	FiniteTimeAction *pNow;
	FiniteTimeAction *pPrev = pAction1;

	while (pAction1)
	{
		pNow = va_arg(params, FiniteTimeAction*);
		if (pNow)
		{
			pPrev = actionOneTwo(pPrev, pNow);
		}
		else
		{
			break;
		}
	}

	va_end(params);
	return pPrev;
}

Spawn* Spawn::actionOneTwo( FiniteTimeAction *pAction1, FiniteTimeAction *pAction2 )
{
	Spawn *pSpawn = new Spawn();
	pSpawn->initOneTwo(pAction1, pAction2);
	return pSpawn;
}

Spawn::Spawn()
{
}

bool MoveTo::initWithDuration( jam::time duration, const Vector2& position )
{
	if (ActionInterval::initWithDuration(duration))
	{
		m_endPosition = position;
		return true;
	}

	return false;
}

void MoveTo::startWithTarget( Node *pTarget )
{
	ActionInterval::startWithTarget(pTarget);
	m_startPosition = pTarget->getPos();
	m_delta = m_endPosition - m_startPosition;
}

void MoveTo::stop()
{
	m_pTarget->clearActionFlags( Node::ActionFlags::MOVING ) ;
	ActionInterval::stop() ;
}

void MoveTo::update( jam::time time )
{
	if (m_pTarget)
	{
		m_pTarget->setPos(m_startPosition + m_delta * time);
		m_pTarget->setActionFlags( Node::ActionFlags::MOVING ) ;
	}
}

MoveTo* MoveTo::actionWithDuration( jam::time duration, const Vector2& position )
{
	MoveTo *pMoveTo = new MoveTo();
	pMoveTo->initWithDuration(duration, position);
	return pMoveTo;
}

MoveTo::MoveTo()
{
}

MoveTo::~MoveTo()
{
}

bool MoveBy::initWithDuration( jam::time duration, const Vector2& position )
{
	if (ActionInterval::initWithDuration(duration))
	{
		m_delta = position;
		return true;
	}

	return false;
}

void MoveBy::startWithTarget( Node *pTarget )
{
	Vector2 dTmp = m_delta;
	MoveTo::startWithTarget(pTarget);
	m_delta = dTmp;
}

ActionInterval* MoveBy::reverse()
{
	return MoveBy::actionWithDuration(m_fDuration, -m_delta);
}

MoveBy* MoveBy::actionWithDuration( jam::time duration, const Vector2& position )
{
	MoveBy *pMoveBy = new MoveBy();
	pMoveBy->initWithDuration(duration, position);
	return pMoveBy;
}

MoveBy::MoveBy()
{
}

MoveBy::~MoveBy()
{
}

//
// JumpBy
//
JumpBy* JumpBy::actionWithDuration(jam::time duration, const Vector2& position, jam::time height, int jumps)
{
	JumpBy *pJumpBy = new JumpBy();
	pJumpBy->initWithDuration(duration, position, height, jumps);
	return pJumpBy;
}

bool JumpBy::initWithDuration(jam::time duration, const Vector2& position, jam::time height, int jumps)
{
	if (ActionInterval::initWithDuration(duration))
	{
		m_delta = position;
		m_height = height;
		m_nJumps = jumps;

		return true;
	}

	return false;
}

void JumpBy::startWithTarget(Node *pTarget)
{
	ActionInterval::startWithTarget(pTarget);
	m_startPosition = pTarget->getPos();
}

void JumpBy::update(jam::time time)
{
	// parabolic jump (since v0.8.2)
	if (m_pTarget)
	{
		jam::time frac = fmodf(time * m_nJumps, 1.0f);
		jam::time y = m_height * 4 * frac * (1 - frac);
		y += m_delta.y * time;
		jam::time x = m_delta.x * time;
		m_pTarget->setPos(Vector2(m_startPosition.x + x, m_startPosition.y + y));
	}
}

ActionInterval* JumpBy::reverse(void)
{
	return JumpBy::actionWithDuration(m_fDuration, Vector2(-m_delta.x, -m_delta.y),
		m_height, m_nJumps);
}

JumpBy::~JumpBy()
{
}

JumpBy::JumpBy()
{
}

//
// JumpTo
//
JumpTo* JumpTo::actionWithDuration(jam::time duration, const Vector2& position, jam::time height, int jumps)
{
	JumpTo *pJumpTo = new JumpTo();
	pJumpTo->initWithDuration(duration, position, height, jumps);
	return pJumpTo;
}

void JumpTo::startWithTarget(Node *pTarget)
{
	JumpBy::startWithTarget(pTarget);
	m_delta = Vector2(m_delta.x - m_startPosition.x, m_delta.y - m_startPosition.y);
}

JumpTo::~JumpTo()
{
}

JumpTo::JumpTo()
{
}


RotateTo::RotateTo() :
	m_fDstAngle(0),
	m_fStartAngle(0),
	m_fDiffAngle(0)
{
}

bool RotateTo::initWithDuration( jam::time duration, float fDeltaAngle )
{
	if (ActionInterval::initWithDuration(duration))
	{
		m_fDstAngle = fDeltaAngle;
		return true;
	}

	return false;
}

void RotateTo::startWithTarget( Node *pTarget )
{
	ActionInterval::startWithTarget(pTarget);

	m_fStartAngle = pTarget->getRotationAngle();

	if (m_fStartAngle > 0)
	{
		m_fStartAngle = fmodf(m_fStartAngle, 360.0f);
	}
	else
	{
		m_fStartAngle = fmodf(m_fStartAngle, -360.0f);
	}

	m_fDiffAngle = m_fDstAngle - m_fStartAngle;
	if (m_fDiffAngle > 180)
	{
		m_fDiffAngle -= 360;
	}

	if (m_fDiffAngle < -180)
	{
		m_fDiffAngle += 360;
	}
}

void RotateTo::stop() 
{
	m_pTarget->clearActionFlags( Node::ActionFlags::ROTATING ) ;
	ActionInterval::stop() ;
}

void RotateTo::update( jam::time time )
{
	if (m_pTarget)
	{
		m_pTarget->setRotationAngle(m_fStartAngle + m_fDiffAngle * time);
		m_pTarget->setActionFlags( Node::ActionFlags::ROTATING ) ;
	}
}

RotateTo* RotateTo::actionWithDuration( jam::time duration, float fDeltaAngle )
{
	RotateTo* pRotateTo = new RotateTo();
	pRotateTo->initWithDuration(duration, fDeltaAngle);

	return pRotateTo;
}

RotateTo::~RotateTo()
{
}

RotateBy::RotateBy() :
	m_fAngle(0),
	m_fStartAngle(0)
{
}

bool RotateBy::initWithDuration( jam::time duration, float fDeltaAngle )
{
	if (ActionInterval::initWithDuration(duration))
	{
		m_fAngle = fDeltaAngle;
		return true;
	}

	return false;
}

void RotateBy::startWithTarget( Node *pTarget )
{
	ActionInterval::startWithTarget(pTarget);
	m_fStartAngle = pTarget->getRotationAngle();
}

void RotateBy::stop() 
{
	m_pTarget->clearActionFlags( Node::ActionFlags::ROTATING ) ;
	ActionInterval::stop() ;
}

void RotateBy::update( jam::time time )
{
	// XXX: shall I add % 360
	if (m_pTarget)
	{
		m_pTarget->setRotationAngle(m_fStartAngle + m_fAngle * time);
//		JAM_TRACE( "RotateBy::update: %f\n", m_fStartAngle + m_fAngle * time ) ;
		m_pTarget->setActionFlags( Node::ActionFlags::ROTATING ) ;
	}
}

ActionInterval* RotateBy::reverse()
{
	return RotateBy::actionWithDuration(m_fDuration, -m_fAngle);
}

RotateBy* RotateBy::actionWithDuration( jam::time duration, float fDeltaAngle )
{
	RotateBy *pRotateBy = new RotateBy();
	pRotateBy->initWithDuration(duration, fDeltaAngle);
	return pRotateBy;
}

RotateBy::~RotateBy()
{
}

// Bezier cubic formula:
//	((1 - t) + t)3 = 1 
// Expands to¡­ 
//   (1 - t)3 + 3t(1-t)2 + 3t2(1 - t) + t3 = 1 
static JAM_INLINE float bezierat( float a, float b, float c, float d, jam::time t )
{
	return (powf(1-t,3) * a + 
		3*t*(powf(1-t,2))*b + 
		3*powf(t,2)*(1-t)*c +
		powf(t,3)*d );
}

//
// BezierBy
//
BezierBy* BezierBy::actionWithDuration(jam::time t, BezierConfig c)
{
	BezierBy *pBezierBy = new BezierBy();
	pBezierBy->initWithDuration(t, c);
	return pBezierBy;
}

bool BezierBy::initWithDuration(jam::time t, BezierConfig c)
{
	if (ActionInterval::initWithDuration(t))
	{
		m_sConfig = c;
		return true;
	}

	return false;
}

void BezierBy::startWithTarget(Node *pTarget)
{
	ActionInterval::startWithTarget(pTarget);
	m_startPosition = pTarget->getPos();
}

void BezierBy::stop() 
{
	m_pTarget->clearActionFlags( Node::ActionFlags::MOVING ) ;
	ActionInterval::stop() ;
}

void BezierBy::update(jam::time time)
{
	if (m_pTarget)
	{
		float xa = 0;
		float xb = m_sConfig.controlPoint_1.x;
		float xc = m_sConfig.controlPoint_2.x;
		float xd = m_sConfig.endPosition.x;

		float ya = 0;
		float yb = m_sConfig.controlPoint_1.y;
		float yc = m_sConfig.controlPoint_2.y;
		float yd = m_sConfig.endPosition.y;

		float x = bezierat(xa, xb, xc, xd, time);
		float y = bezierat(ya, yb, yc, yd, time);
		m_pTarget->setPos( m_startPosition + Vector2(x, y));
		m_pTarget->setActionFlags( Node::ActionFlags::MOVING) ;
	}
}

ActionInterval* BezierBy::reverse(void)
{
	BezierConfig r;

	r.endPosition = -m_sConfig.endPosition;
	r.controlPoint_1 = m_sConfig.controlPoint_2 - m_sConfig.endPosition;
	r.controlPoint_2 = m_sConfig.controlPoint_1 - m_sConfig.endPosition;

	BezierBy *pAction = BezierBy::actionWithDuration(m_fDuration, r);
	return pAction;
}

BezierBy::~BezierBy()
{

}

BezierBy::BezierBy()
{
}

//
// BezierTo
//
BezierTo* BezierTo::actionWithDuration(jam::time t, BezierConfig c)
{
	BezierTo *pBezierTo = new BezierTo();
	pBezierTo->initWithDuration(t, c);
	return pBezierTo;
}

void BezierTo::startWithTarget(Node *pTarget)
{
	BezierBy::startWithTarget(pTarget);
	m_sConfig.controlPoint_1 = m_sConfig.controlPoint_1 - m_startPosition;
	m_sConfig.controlPoint_2 = m_sConfig.controlPoint_2 - m_startPosition;
	m_sConfig.endPosition = m_sConfig.endPosition - m_startPosition;
}

BezierTo::~BezierTo()
{
}

BezierTo::BezierTo()
{
}

//
// ScaleTo
//
ScaleTo* ScaleTo::actionWithDuration(jam::time duration, float s)
{
	ScaleTo *pScaleTo = new ScaleTo();
	pScaleTo->initWithDuration(duration, s);
	return pScaleTo;
}

bool ScaleTo::initWithDuration(jam::time duration, float s)
{
	assert(s!=0) ;
	if (ActionInterval::initWithDuration(duration))
	{
		m_fEndScaleX = s;
		m_fEndScaleY = s;

		return true;
	}

	return false;
}

ScaleTo* ScaleTo::actionWithDuration(jam::time duration, float sx, float sy)
{
	ScaleTo *pScaleTo = new ScaleTo();
	pScaleTo->initWithDuration(duration, sx, sy);
	return pScaleTo;
}

bool ScaleTo::initWithDuration(jam::time duration, float sx, float sy)
{
	assert(sx!=0.0 && sy!=0.0) ;
	if (ActionInterval::initWithDuration(duration))
	{
		m_fEndScaleX = sx;
		m_fEndScaleY = sy;

		return true;
	}

	return false;
}

void ScaleTo::startWithTarget(Node *pTarget)
{
	ActionInterval::startWithTarget(pTarget);
	m_fStartScaleX = pTarget->getScaleX();
	m_fStartScaleY = pTarget->getScaleY();
	m_fDeltaX = m_fEndScaleX - m_fStartScaleX;
	m_fDeltaY = m_fEndScaleY - m_fStartScaleY;
}

void ScaleTo::update(jam::time time)
{
	if (m_pTarget)
	{
		m_pTarget->setScaleX(m_fStartScaleX + m_fDeltaX * time);
		m_pTarget->setScaleY(m_fStartScaleY + m_fDeltaY * time);
	}
}

ScaleTo::~ScaleTo()
{

}

ScaleTo::ScaleTo()
{

}

//
// ScaleBy
//
ScaleBy* ScaleBy::actionWithDuration(jam::time duration, float s)
{
	ScaleBy *pScaleBy = new ScaleBy();
	pScaleBy->initWithDuration(duration, s);
	return pScaleBy;
}

ScaleBy* ScaleBy::actionWithDuration(jam::time duration, float sx, float sy)
{
	ScaleBy *pScaleBy = new ScaleBy();
	pScaleBy->initWithDuration(duration, sx, sy);
	return pScaleBy;
}

void ScaleBy::startWithTarget(Node *pTarget)
{
	ScaleTo::startWithTarget(pTarget);
	m_fDeltaX = m_fStartScaleX * m_fEndScaleX - m_fStartScaleX;
	m_fDeltaY = m_fStartScaleY * m_fEndScaleY - m_fStartScaleY;
}

ActionInterval* ScaleBy::reverse(void)
{
	return ScaleBy::actionWithDuration(m_fDuration, 1 / m_fEndScaleX, 1 / m_fEndScaleY);
}

ScaleBy::~ScaleBy()
{
}

ScaleBy::ScaleBy()
{
}

//
// Blink
//
Blink* Blink::actionWithDuration(jam::time duration, unsigned int uBlinks)
{
	Blink *pBlink = new Blink();
	pBlink->initWithDuration(duration, uBlinks);
	return pBlink;
}

bool Blink::initWithDuration(jam::time duration, unsigned int uBlinks)
{
	if (ActionInterval::initWithDuration(duration))
	{
		m_nTimes = uBlinks;
		return true;
	}

	return false;
}

void Blink::update(jam::time time)
{
	if (m_pTarget)
	{
		jam::time slice = 1.0f / m_nTimes;
		jam::time m = fmodf(time, slice);
		m_pTarget->setVisible(m > slice / 2 ? true : false);
		if (time>=1)
			m_pTarget->setVisible(true);
	}
}

ActionInterval* Blink::reverse(void)
{
	// return 'self'
	return Blink::actionWithDuration(m_fDuration, m_nTimes);
}

Blink::~Blink()
{
}

Blink::Blink()
{
}

//
// FadeIn
//
FadeIn* FadeIn::actionWithDuration(jam::time d)
{
	FadeIn* pAction = new FadeIn();
	pAction->initWithDuration(d);
	return pAction;
}

void FadeIn::update(jam::time time)
{
	Color tmpCol = m_pTarget->getColor() ;
	tmpCol.a = (uint8_t)(255 * time) ;
	m_pTarget->setColor(tmpCol) ;
}

ActionInterval* FadeIn::reverse(void)
{
	return FadeOut::actionWithDuration(m_fDuration);
}

FadeIn::~FadeIn()
{
}

FadeIn::FadeIn()
{
}

//
// FadeOut
//
FadeOut* FadeOut::actionWithDuration(jam::time d)
{
	FadeOut* pAction = new FadeOut();
	pAction->initWithDuration(d);
	return pAction;
}

void FadeOut::update(jam::time time)
{
	Color tmpCol = m_pTarget->getColor() ;
	tmpCol.a =(uint8_t)(255 * (1 - time)) ;
	m_pTarget->setColor(tmpCol) ;
}

ActionInterval* FadeOut::reverse(void)
{
	return FadeIn::actionWithDuration(m_fDuration);
}

FadeOut::~FadeOut()
{
}

FadeOut::FadeOut()
{
}

//
// FadeTo
//
FadeTo* FadeTo::actionWithDuration( jam::time duration, uint8_t opacity )
{
	FadeTo *pFadeTo = new FadeTo();
	pFadeTo->initWithDuration(duration, opacity);
	return pFadeTo;
}

bool FadeTo::initWithDuration( jam::time duration, uint8_t opacity )
{
	if (ActionInterval::initWithDuration(duration))
	{
		m_toOpacity = opacity;
		return true;
	}

	return false;
}

void FadeTo::startWithTarget(Node *pTarget)
{
	ActionInterval::startWithTarget(pTarget);

	m_fromOpacity = m_pTarget->getColor().a;
}

void FadeTo::update(jam::time time)
{
	Color tmpCol = m_pTarget->getColor();
	tmpCol.a = (uint8_t)(m_fromOpacity + (m_toOpacity - m_fromOpacity) * time) ;
	m_pTarget->setColor(tmpCol) ;
}

FadeTo::~FadeTo()
{

}

FadeTo::FadeTo()
{
}

//
// TintTo
//
TintTo* TintTo::actionWithDuration(jam::time duration, uint8_t red, uint8_t green, uint8_t blue)
{
	TintTo *pTintTo = new TintTo();
	pTintTo->initWithDuration(duration, red, green, blue);
	return pTintTo;
}

bool TintTo::initWithDuration(jam::time duration, uint8_t red, uint8_t green, uint8_t blue)
{
	if (ActionInterval::initWithDuration(duration))
	{
		m_to = Color(red, green, blue);
		return true;
	}

	return false;
}

void TintTo::startWithTarget(Node *pTarget)
{
	ActionInterval::startWithTarget(pTarget);
	m_from = m_pTarget->getColor();
}

void TintTo::update(jam::time time)
{
	Color tmpCol = m_pTarget->getColor();
	tmpCol.r = (uint8_t)(m_from.r + (m_to.r - m_from.r) * time) ;
	tmpCol.g = (uint8_t)(m_from.g + (m_to.g - m_from.g) * time) ;
	tmpCol.b = (uint8_t)(m_from.b + (m_to.b - m_from.b) * time) ;
	m_pTarget->setColor(tmpCol) ;

}

TintTo::~TintTo()
{
}

TintTo::TintTo()
{
}

//
// TintBy
//
TintBy* TintBy::actionWithDuration(jam::time duration, uint16_t deltaRed, uint16_t deltaGreen, uint16_t deltaBlue)
{
	TintBy *pTintBy = new TintBy();
	pTintBy->initWithDuration(duration, deltaRed, deltaGreen, deltaBlue);
	return pTintBy;
}

bool TintBy::initWithDuration(jam::time duration, uint16_t deltaRed, uint16_t deltaGreen, uint16_t deltaBlue)
{
	if (ActionInterval::initWithDuration(duration))
	{
		m_deltaR = deltaRed;
		m_deltaG = deltaGreen;
		m_deltaB = deltaBlue;

		return true;
	}

	return false;
}

void TintBy::startWithTarget(Node *pTarget)
{
	ActionInterval::startWithTarget(pTarget);

	Color tmpCol = m_pTarget->getColor();
	m_fromR = tmpCol.r;
	m_fromG = tmpCol.g;
	m_fromB = tmpCol.b;

}

void TintBy::update(jam::time time)
{
	Color tmpCol = m_pTarget->getColor();
	tmpCol.r = (uint8_t)(m_fromR + m_deltaR * time) ;
	tmpCol.g = (uint8_t)(m_fromG + m_deltaG * time) ;
	tmpCol.b = (uint8_t)(m_fromB + m_deltaB * time);
	m_pTarget->setColor(tmpCol) ;
}

ActionInterval* TintBy::reverse(void)
{
	return TintBy::actionWithDuration(m_fDuration, -m_deltaR, -m_deltaG, -m_deltaB);
}

TintBy::~TintBy()
{
}

TintBy::TintBy()
{
}

void DelayTime::update( jam::time time )
{
}

ActionInterval* DelayTime::reverse()
{
	return DelayTime::actionWithDuration(m_fDuration);
}

DelayTime* DelayTime::actionWithDuration( jam::time d )
{
	DelayTime* pAction = new DelayTime();
	pAction->initWithDuration(d);
	return pAction;
}

DelayTime::~DelayTime()
{
}

DelayTime::DelayTime()
{
}


//
// ReverseTime
//
ReverseTime* ReverseTime::actionWithAction(FiniteTimeAction *pAction)
{
	// casting to prevent warnings
	ReverseTime *pReverseTime = new ReverseTime();
	pReverseTime->initWithAction(pAction);
	return pReverseTime;
}

bool ReverseTime::initWithAction(FiniteTimeAction *pAction)
{
	if (ActionInterval::initWithDuration(pAction->getDuration())) {
		m_pOther = pAction;
		return true;
	}

	return false;
}

ReverseTime::~ReverseTime(void)
{
	m_pOther = nullptr ;
}

void ReverseTime::startWithTarget(Node *pTarget)
{
	ActionInterval::startWithTarget(pTarget);
	m_pOther->startWithTarget(pTarget);
}

void ReverseTime::stop(void)
{
	m_pOther->stop();
	ActionInterval::stop();
}

void ReverseTime::update(jam::time time)
{
	if (m_pOther) {
		m_pOther->update(1 - time);
	}
}

ReverseTime::ReverseTime()
{
}

/*	IntervalAction* ReverseTime::reverse(void)
{
return (IntervalAction*)(m_pOther->copy());
}*/

//
// Animate
//
Animate* Animate::actionWithAnimation(Animation2D *pAnimation)
{
	Animate *pAnimate = new Animate();
	pAnimate->initWithAnimation(pAnimation, true);
	return pAnimate;
}

Animate* Animate::actionWithAnimation(Animation2D *pAnimation, bool bRestoreOriginalFrame)
{
	Animate *pAnimate = new Animate();
	pAnimate->initWithAnimation(pAnimation, bRestoreOriginalFrame);
	return pAnimate;
}

Animate* Animate::actionWithDuration(jam::time duration, Animation2D *pAnimation, bool bRestoreOriginalFrame)
{
	Animate *pAnimate = new Animate();
	pAnimate->initWithDuration(duration, pAnimation, bRestoreOriginalFrame);
	return pAnimate;
}

bool Animate::initWithAnimation(Animation2D *pAnimation)
{
	return initWithAnimation(pAnimation, true);
}

bool Animate::initWithAnimation(Animation2D *pAnimation, bool bRestoreOriginalFrame)
{
	assert(pAnimation);

	if( ActionInterval::initWithDuration(pAnimation->getTotalTime()) ) {
		m_bRestoreOriginalFrame = bRestoreOriginalFrame;
		m_pAnimation = pAnimation;
		m_pOrigFrame = NULL;

		return true;
	}

	return false;
}

bool Animate::initWithDuration(jam::time duration, Animation2D *pAnimation, bool bRestoreOriginalFrame)
{
	assert(pAnimation != NULL);

	if (ActionInterval::initWithDuration(duration)) {
		m_bRestoreOriginalFrame = bRestoreOriginalFrame;
		m_pAnimation = pAnimation;
		m_pOrigFrame = NULL;

		return true;
	}

	return false;
}

void Animate::startWithTarget(Node *pTarget)
{
	ActionInterval::startWithTarget(pTarget);
	m_pSprite = (Sprite*)(pTarget);

	if (m_bRestoreOriginalFrame) {
		m_pOrigFrame = m_pSprite->getFrame();
	}

	m_pSprite->getAnimator().setAnimation(m_pAnimation) ;
	m_pSprite->getAnimator().startAnimation();
}

void Animate::stop()
{
	if (m_bRestoreOriginalFrame) {
		((Sprite*)(m_pTarget))->setFrame(m_pOrigFrame);
	}

	m_pSprite->getAnimator().stopAnimation();
	m_pSprite->clearActionFlags( Node::ActionFlags::ANIMATING) ;
	ActionInterval::stop();
}

void Animate::update(jam::time time)
{
	m_pSprite->getAnimator().update();
	m_pTarget->setActionFlags( Node::ActionFlags::ANIMATING ) ;
}

Animate::~Animate()
{
}

Animate::Animate()
{
}

/*	CCIntervalAction* CCAnimate::reverse()
{
NSMutableArray<CCSpriteFrame*> *pOldArray = m_pAnimation->getFrames();
NSMutableArray<CCSpriteFrame*> *pNewArray = new NSMutableArray<CCSpriteFrame*>(pOldArray->count());

if (pOldArray->count() > 0)
{
CCSpriteFrame *pElement;
NSMutableArray<CCSpriteFrame*>::NSMutableArrayRevIterator iter;
for (iter = pOldArray->rbegin(); iter != pOldArray->rend(); iter++)
{
pElement = *iter;
if (! pElement)
{
break;
}

pNewArray->addObject((CCSpriteFrame*)(pElement->copy()->autorelease()));
}
}

CCAnimation *pNewAnim = CCAnimation::animationWithName(m_pAnimation->getName(),
m_pAnimation->getDelay(), pNewArray);

pNewArray->release();

return CCAnimate::actionWithDuration(m_fDuration, pNewAnim, m_bRestoreOriginalFrame);
}*/

}
