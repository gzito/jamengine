/**********************************************************************************
* 
* ActionEase.cpp
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

#include "jam/ActionEase.h"
#include "jam/core/math.h"

#include <math.h>
#include <assert.h>

namespace jam
{

//
// EaseAction
//
ActionEase* ActionEase::actionWithAction(ActionInterval *pAction)
{
	ActionEase *pRet = JAM_ALLOC(ActionEase);
	if (pRet ) {
		if( pRet->initWithAction(pAction)) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet;
}

bool ActionEase::initWithAction(ActionInterval *pAction)
{
	assert(pAction != NULL);

	if (ActionInterval::initWithDuration(pAction->getDuration()))
	{
		m_pOther = pAction;
		pAction->addRef();

		return true;
	}

	return false;
}

ActionEase::~ActionEase()
{
	JAM_RELEASE_NULL(m_pOther) ;
}

void ActionEase::startWithTarget(Node *pTarget)
{
	ActionInterval::startWithTarget(pTarget);
	m_pOther->startWithTarget(m_pTarget);
}

void ActionEase::stop(void)
{
	m_pOther->stop();
	ActionInterval::stop();
}

void ActionEase::update(jam::time time)
{
	m_pOther->update(time);
}

ActionInterval* ActionEase::reverse(void)
{
	return ActionEase::actionWithAction(m_pOther->reverse());
}

ActionEase::ActionEase()
{

}

//
// EaseRateAction
//
EaseRateAction* EaseRateAction::actionWithAction(ActionInterval *pAction, float fRate)
{
	EaseRateAction *pRet = JAM_ALLOC(EaseRateAction);
	if (pRet ) {
		if( pRet->initWithAction(pAction,fRate) ) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet;
}

bool EaseRateAction::initWithAction(ActionInterval *pAction, float fRate)
{
	if (ActionEase::initWithAction(pAction))
	{
		m_fRate = fRate;
		return true;
	}

	return false;
}

ActionInterval* EaseRateAction::reverse(void)
{
	return EaseRateAction::actionWithAction(m_pOther->reverse(), 1 / m_fRate);
}

EaseRateAction::EaseRateAction()
{

}

//
// EeseIn
//
EaseIn* EaseIn::actionWithAction(ActionInterval *pAction, float fRate)
{
	EaseIn *pRet = JAM_ALLOC(EaseIn);
	if (pRet ) {
		if( pRet->initWithAction(pAction,fRate)) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet;
}

void EaseIn::update(jam::time time)
{
	m_pOther->update(powf(time, m_fRate));
}

EaseIn::EaseIn()
{

}

//
// EaseOut
//
EaseOut* EaseOut::actionWithAction(ActionInterval *pAction, float fRate)
{
	EaseOut *pRet = JAM_ALLOC(EaseOut);
	if (pRet ) {
		if( pRet->initWithAction(pAction,fRate)) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet;      
}

void EaseOut::update(jam::time time)
{
	m_pOther->update(powf(time, 1 / m_fRate));
}

EaseOut::EaseOut()
{

}

//
// EaseInOut
//
EaseInOut* EaseInOut::actionWithAction(ActionInterval *pAction, float fRate)
{
	EaseInOut *pRet = JAM_ALLOC(EaseInOut);
	if (pRet ) {
		if( pRet->initWithAction(pAction,fRate)) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet;        
}

void EaseInOut::update(jam::time time)
{
	int sign = 1;
	int r = (int) m_fRate;

	if (r % 2 == 0) {
		sign = -1;
	}

	time *= 2;
	if (time < 1) {
		m_pOther->update(0.5f * powf(time, m_fRate));
	}
	else {
		m_pOther->update(sign * 0.5f * (powf(time - 2, m_fRate) + sign * 2));
	}
}

// InOut and OutIn are symmetrical
ActionInterval* EaseInOut::reverse(void)
{
	return EaseInOut::actionWithAction(m_pOther->reverse(), m_fRate);
}

EaseInOut::EaseInOut()
{

}


//
// EaseExponentialIn
//
EaseExponentialIn* EaseExponentialIn::actionWithAction(ActionInterval* pAction)
{
	EaseExponentialIn *pRet = JAM_ALLOC(EaseExponentialIn);
	if (pRet ) {
		if( pRet->initWithAction(pAction)) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet;      
}

void EaseExponentialIn::update(jam::time time)
{
	m_pOther->update(time == 0 ? 0 : powf(2, 10 * (time/1 - 1)) - 1 * 0.001f);
}

ActionInterval* EaseExponentialIn::reverse(void)
{
	return EaseExponentialOut::actionWithAction(m_pOther->reverse());
}

EaseExponentialIn::EaseExponentialIn()
{

}

//
// EaseExponentialOut
//
EaseExponentialOut* EaseExponentialOut::actionWithAction(ActionInterval* pAction)
{
	EaseExponentialOut *pRet = JAM_ALLOC(EaseExponentialOut);
	if (pRet ) {
		if( pRet->initWithAction(pAction)) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet; 
}

void EaseExponentialOut::update(jam::time time)
{
	m_pOther->update(time == 1 ? 1 : (-powf(2, -10 * time / 1) + 1));
}

ActionInterval* EaseExponentialOut::reverse(void)
{
	return EaseExponentialIn::actionWithAction(m_pOther->reverse());
}

EaseExponentialOut::EaseExponentialOut()
{

}

//
// EaseExponentialInOut
//
EaseExponentialInOut* EaseExponentialInOut::actionWithAction(ActionInterval *pAction)
{
	EaseExponentialInOut *pRet = JAM_ALLOC(EaseExponentialInOut);
	if (pRet ) {
		if( pRet->initWithAction(pAction)) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet; 
}

void EaseExponentialInOut::update(jam::time time)
{
	time /= 0.5f;
	if (time < 1) {
		time = 0.5f * powf(2, 10 * (time - 1));
	}
	else {
		time = 0.5f * (-powf(2, 10 * (time - 1)) + 2);
	}

	m_pOther->update(time);
}

EaseExponentialInOut::EaseExponentialInOut()
{

}

//
// EaseElastic
//
EaseElastic* EaseElastic::actionWithAction(ActionInterval *pAction)
{
	EaseElastic *pRet = JAM_ALLOC(EaseElastic);
	if (pRet ) {
		if( pRet->initWithAction(pAction)) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet; 
}

EaseElastic* EaseElastic::actionWithAction(ActionInterval *pAction, float fPeriod)
{
	EaseElastic *pRet = JAM_ALLOC(EaseElastic);
	if (pRet ) {
		if( pRet->initWithAction(pAction, fPeriod)) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}


	return pRet; 
}

bool EaseElastic::initWithAction(ActionInterval *pAction)
{
	return initWithAction(pAction, 0.3f);
}

bool EaseElastic::initWithAction(ActionInterval *pAction, float fPeriod)
{
	if (ActionEase::initWithAction(pAction))
	{
		m_fPeriod = fPeriod;
		return true;
	}

	return false;
}

ActionInterval* EaseElastic::reverse(void)
{
	assert(0);
	return 0;
}

EaseElastic::EaseElastic()
{

}

//
// EaseElasticIn
//
EaseElasticIn* EaseElasticIn::actionWithAction(ActionInterval *pAction, float fPeriod)
{
	EaseElasticIn *pRet = JAM_ALLOC(EaseElasticIn);
	if (pRet ) {
		if( pRet->initWithAction(pAction, fPeriod)) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet; 
}

EaseElasticIn* EaseElasticIn::actionWithAction(ActionInterval *pAction)
{
	EaseElasticIn *pRet = JAM_ALLOC(EaseElasticIn);
	if( pRet ) {
		if( pRet->initWithAction(pAction) ) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet; 
}

void EaseElasticIn::update(jam::time time)
{
	jam::time newT = 0;
	if (time == 0 || time == 1)
	{
		newT = time;
	}
	else
	{
		float s = m_fPeriod / 4;
		time = time - 1;
		newT = -powf(2, 10 * time) * sinf((time - s) * ((float)(JAM_PI*2.0f)) / m_fPeriod);
	}

	m_pOther->update(newT);
}

ActionInterval* EaseElasticIn::reverse(void)
{
	return EaseElasticOut::actionWithAction(m_pOther->reverse(), m_fPeriod);
}

EaseElasticIn::EaseElasticIn()
{

}

//
// EaseElasticOut
//
EaseElasticOut* EaseElasticOut::actionWithAction(ActionInterval *pAction)
{
	EaseElasticOut *pRet = JAM_ALLOC(EaseElasticOut);
	if (pRet ) {
		if( pRet->initWithAction(pAction)) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet; 
}

EaseElasticOut* EaseElasticOut::actionWithAction(ActionInterval *pAction, float fPeriod)
{
	EaseElasticOut *pRet = JAM_ALLOC(EaseElasticOut);
	if( pRet ) {
		if( pRet->initWithAction(pAction, fPeriod) ) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet; 
}

void EaseElasticOut::update(jam::time time)
{
	jam::time newT = 0;
	if (time == 0 || time == 1)
	{
		newT = time;
	}
	else
	{
		float s = m_fPeriod / 4;
		newT = powf(2, -10 * time) * sinf((time - s) * ((float)(JAM_PI*2.0f)) / m_fPeriod) + 1;
	}

	m_pOther->update(newT);
}

ActionInterval* EaseElasticOut::reverse(void)
{
	return EaseElasticIn::actionWithAction(m_pOther->reverse(), m_fPeriod);
}

EaseElasticOut::EaseElasticOut()
{

}

//
// EaseElasticInOut
//
EaseElasticInOut* EaseElasticInOut::actionWithAction(ActionInterval *pAction)
{
	EaseElasticInOut *pRet = JAM_ALLOC(EaseElasticInOut);
	if (pRet ) {
		if( pRet->initWithAction(pAction)) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet; 
}

EaseElasticInOut* EaseElasticInOut::actionWithAction(ActionInterval *pAction, float fPeriod)
{
	EaseElasticInOut *pRet = JAM_ALLOC(EaseElasticInOut);
	if (pRet ) {
		if( pRet->initWithAction(pAction,fPeriod)) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet; 
}

void EaseElasticInOut::update(jam::time time)
{
	jam::time newT = 0;
	if (time == 0 || time == 1)
	{
		newT = time;
	}
	else
	{
		time = time * 2;
		if (! m_fPeriod)
		{
			m_fPeriod = 0.3f * 1.5f;
		}

		jam::time s = m_fPeriod / 4;

		time = time - 1;
		if (time < 0)
		{
			newT = -0.5f * powf(2, 10 * time) * sinf((time -s) * ((float)(JAM_PI*2.0f)) / m_fPeriod);
		}
		else
		{
			newT = powf(2, -10 * time) * sinf((time - s) * ((float)(JAM_PI*2.0f)) / m_fPeriod) * 0.5f + 1;
		}
	}

	m_pOther->update(newT);
}

ActionInterval* EaseElasticInOut::reverse(void)
{
	return EaseInOut::actionWithAction(m_pOther->reverse(), m_fPeriod);
}

EaseElasticInOut::EaseElasticInOut()
{

}

//
// EaseBounce
//
EaseBounce* EaseBounce::actionWithAction( ActionInterval* pAction )
{
	EaseBounce *pRet = JAM_ALLOC(EaseBounce);
	if (pRet ) {
		if( pRet->initWithAction(pAction)) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet; 
}

jam::time EaseBounce::bounceTime( jam::time time )
{
	if (time < 1 / 2.75)
	{
		return 7.5625f * time * time;
	} else 
		if (time < 2 / 2.75)
		{
			time -= 1.5f / 2.75f;
			return 7.5625f * time * time + 0.75f;
		} else
			if(time < 2.5 / 2.75)
			{
				time -= 2.25f / 2.75f;
				return 7.5625f * time * time + 0.9375f;
			}

			time -= 2.625f / 2.75f;
			return 7.5625f * time * time + 0.984375f;
}

EaseBounce::EaseBounce()
{

}

//
// EaseBounceIn
//
EaseBounceIn* EaseBounceIn::actionWithAction(ActionInterval* pAction)
{
	EaseBounceIn *pRet = JAM_ALLOC(EaseBounceIn);
	if (pRet ) {
		if( pRet->initWithAction(pAction)) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet; 
}

void EaseBounceIn::update( jam::time time )
{
	jam::time newT = 1 - bounceTime(1 - time);
	m_pOther->update(newT);
}

ActionInterval* EaseBounceIn::reverse( void )
{
	return EaseBounceOut::actionWithAction(m_pOther->reverse());
}

EaseBounceIn::EaseBounceIn()
{

}

//
// EaseBounceOut
//
EaseBounceOut* EaseBounceOut::actionWithAction(ActionInterval* pAction)
{
	EaseBounceOut *pRet = JAM_ALLOC(EaseBounceOut);
	if (pRet ) {
		if( pRet->initWithAction(pAction)) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet; 
}

void EaseBounceOut::update( jam::time t )
{
	jam::time newT = bounceTime(t);
	m_pOther->update(newT);
}

ActionInterval* EaseBounceOut::reverse( void )
{
	return EaseBounceIn::actionWithAction(m_pOther->reverse());
}

EaseBounceOut::EaseBounceOut()
{

}

//
// EaseBounceInOut
//
EaseBounceInOut	* EaseBounceInOut::actionWithAction(ActionInterval* pAction)
{
	EaseBounceInOut *pRet = JAM_ALLOC(EaseBounceInOut);
	if (pRet ) {
		if( pRet->initWithAction(pAction)) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet; 
}

void EaseBounceInOut::update( jam::time t )
{
	jam::time newT = 0;
	if (t < 0.5f)
	{
		t = t * 2;
		newT = (1 - bounceTime(1 - t)) * 0.5f;
	}
	else
	{
		newT = bounceTime(t * 2 - 1) * 0.5f + 0.5f;
	}

	m_pOther->update(newT);
}

EaseBounceInOut::EaseBounceInOut()
{

}

//
// EaseBackIn
//
EaseBackIn* EaseBackIn::actionWithAction(ActionInterval *pAction)
{
	EaseBackIn *pRet = JAM_ALLOC(EaseBackIn);
	if (pRet ) {
		if( pRet->initWithAction(pAction)) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet;
}

void EaseBackIn::update(jam::time t)
{
	jam::time overshoot = 1.70158f;
	m_pOther->update(t * t * ((overshoot + 1) * t - overshoot));
}

ActionInterval* EaseBackIn::reverse(void)
{
	return EaseBackOut::actionWithAction(m_pOther->reverse());
}

EaseBackIn::EaseBackIn()
{

}

//
// EaseBackOut
//
EaseBackOut* EaseBackOut::actionWithAction(ActionInterval* pAction)
{
	EaseBackOut *pRet = JAM_ALLOC(EaseBackOut);
	if (pRet ) {
		if( pRet->initWithAction(pAction)) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet;
}

void EaseBackOut::update(jam::time t)
{
	jam::time overshoot = 1.70158f;

	t = t - 1;
	m_pOther->update(t * t * ((overshoot + 1) * t + overshoot) + 1);
}

ActionInterval* EaseBackOut::reverse(void)
{
	return EaseBackIn::actionWithAction(m_pOther->reverse());
}

EaseBackOut::EaseBackOut()
{

}

//
// EaseBackInOut
//
EaseBackInOut* EaseBackInOut::actionWithAction(ActionInterval* pAction)
{
	EaseBackInOut *pRet = JAM_ALLOC(EaseBackInOut);
	if (pRet ) {
		if( pRet->initWithAction(pAction)) {
			pRet->autorelease() ;
		}
		else {
			JAM_RELEASE_NULL(pRet);
		}
	}

	return pRet;
}

void EaseBackInOut::update(jam::time t)
{
	jam::time overshoot = 1.70158f * 1.525f;

	t = t * 2;
	if (t < 1) {
		m_pOther->update((t * t * ((overshoot + 1) * t - overshoot)) / 2);
	}
	else {
		t = t - 2;
		m_pOther->update((t * t * ((overshoot + 1) + overshoot)) / 2 + 1);
	}
}

EaseBackInOut::EaseBackInOut()
{

}

}
