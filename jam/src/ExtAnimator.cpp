/**********************************************************************************
* 
* ExtAnimator.cpp
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
#include "jam/ExtAnimator.h"
#include "jam/core/bmkextras.hpp"
#include "jam/SysTimer.h"

#include <math.h>

using namespace std ;

namespace jam
{

	ExtAnimator::ExtAnimator(IDrawable2D* pIDrawable) :
		m_pAnimation(0), m_defaultAnimation(0), m_timer(0), m_pIDrawable(pIDrawable), m_lastFrameIndex(0), m_speed(1.0f), m_completeFrameIndex(-100)
	{
		m_timer = Timer::create() ;
		String timeMs = to_string( GetSysTimer().getTime() ) ;
		m_timer->setName(String("ExtAnimator_")+timeMs);
		m_defaultAnimation = Animation2D::create() ;
		m_pAnimation = m_defaultAnimation;
	}


	ExtAnimator::~ExtAnimator()
	{
		m_timer = nullptr ;
		m_pAnimation = nullptr ;
		m_defaultAnimation = nullptr ;
	}
		
	
	void ExtAnimator::setAnimation( Animation2D* pAnimation, bool autoStart/*=false*/, int firstFrame/*=0*/ )
	{
		assert(pAnimation) ;
		if(m_pAnimation!=pAnimation) {
			m_pAnimation = pAnimation ;
			if( m_timer->isRunning() ) {
				m_timer->stop();
			}
		}
		if( !m_timer->isRunning() && autoStart ) {
			startAnimation();
		}

		if( autoStart == false ) {
			m_lastFrameIndex = firstFrame ;
			AnimFrame2D* pAnimFrame = m_pAnimation->getFrame(m_lastFrameIndex) ;
			/*pAnimFrame->setFlipX(pAnimFrame->getFlipX() & m_pAnimation->getFlipAllX());
			pAnimFrame->setFlipY(pAnimFrame->getFlipY() & m_pAnimation->getFlipAllY());*/
			DrawItem* pFrame = pAnimFrame->getDrawItem() ;
			m_pIDrawable->setFrame(pFrame) ;
		}
	}

	void ExtAnimator::update()
	{
		if( m_timer->isRunning() && m_pAnimation ) {
			m_timer->update();
			int idx = 0 ;
			m_pAnimation->update(*m_timer, idx, m_speed, m_lastFrameIndex);
			
			if( idx >= 0 ) {
				AnimFrame2D* pAnimFrame = m_pAnimation->getFrame(idx) ;
				m_pIDrawable->setFrame( pAnimFrame->getDrawItem() ) ;
				m_lastFrameIndex = idx ;
			}
			else {
				m_lastFrameIndex = 0 ;
			}

			if( m_completeFrameIndex >= 0 && m_lastFrameIndex == m_completeFrameIndex ) {
				stopAnimation() ;
			}
		}
	}

	void ExtAnimator::startAnimation()
	{
		if( !m_timer->isRunning() ) {
			m_timer->start() ;
			m_lastFrameIndex = 0 ;
			m_completeFrameIndex = -100 ;
		}
	}
	void ExtAnimator::completeAnimation( int frame/*=-1*/ )
	{
		if( frame == -1 ) frame=m_pAnimation->getNumOfFrames()-1;

		frame = Limit(frame, 0,m_pAnimation->getNumOfFrames()-1);
		m_completeFrameIndex = frame ;
	}

	void ExtAnimator::stopAnimation()
	{
		m_timer->stop() ;
		m_lastFrameIndex = 0 ;
	}


	void ExtAnimator::resetAnimation()
	{
		m_timer->reset() ;
		m_lastFrameIndex = 0 ;
	}


	bool ExtAnimator::isPlaying()
	{
		return m_timer->isRunning() ;
	}

}
