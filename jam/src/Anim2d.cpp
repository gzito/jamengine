/**********************************************************************************
* 
* Anim2d.cpp
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

#include "jam/Anim2d.h"

#include <math.h>

namespace jam
{

AnimFrame2D::AnimFrame2D() :
	m_handleImg(0), m_time(0), m_flipX(false), m_flipY(false), m_partialTime(0.0f)
{
}

AnimFrame2D* AnimFrame2D::create()
{
	return new AnimFrame2D() ; 
}

AnimFrame2D::~AnimFrame2D()
{
	JAM_RELEASE(m_handleImg) ;
}

void AnimFrame2D::setDrawItem( DrawItem* val )
{
	m_handleImg = val;
	m_handleImg->addRef() ;
}

//
// class Animation2D
//
Animation2D::Animation2D() :
	m_totalTime(0.0f),
	m_bLoop(false),
	m_flipAllX(0),
	m_flipAllY(0)
{
}

Animation2D::~Animation2D()
{
	for(size_t i=0; i<m_frames.size(); i++ ) {
		JAM_RELEASE(m_frames[i]);
	}
	m_frames.clear() ;
	JAM_DELETE(m_flipAllX) ;
	JAM_DELETE(m_flipAllY) ;
}

void Animation2D::addFrame(AnimFrame2D* frame)
{
#ifdef JAM_ANIMFRAME2D_AUTORELEASED
	frame->addRef() ;
#endif
	m_frames.push_back(frame) ;
	m_totalTime += frame->getTime();
	m_frames[m_frames.size()-1]->m_partialTime = m_totalTime ;
}

void Animation2D::addFrame( const Ref<Texture2D>& pTexture, const jam::Rect& rect, float time, bool flipX/*=false*/, bool flipY/*=false*/ )
{
	DrawItem* drawItem = DrawItem::create(pTexture,rect) ;
#ifdef JAM_ANIMFRAME2D_AUTORELEASED
	drawItem->autorelease();
#endif
	addFrame(drawItem,time,flipX,flipY) ;
}

void Animation2D::addFrame( DrawItem* elem, float time, bool flipX/*=false*/, bool flipY/*=false*/ )
{
	AnimFrame2D* frame = AnimFrame2D::create();
#ifdef JAM_ANIMFRAME2D_AUTORELEASED
	frame->autorelease() ;
#endif
	frame->setDrawItem(elem) ;
	frame->setTime(time) ;		
	frame->setFlipX(flipX);
	frame->setFlipY(flipY);
	addFrame(frame) ;
}

void Animation2D::update(const jam::Timer& timer, int& currentFrameIndex, float speed/*=1.0f*/, int lastFrame/*=0*/)
{

	float timeCount = 0.0f ;

	float elapsed = speed * timer.getTotalElapsed();
	
	// in the case of startWithDelay elapsed is less than 0
	if( elapsed >= 0.0f ) {

		// loop the animation
		if( m_bLoop && elapsed >= m_totalTime ) {
			elapsed = fmodf(elapsed,m_totalTime) ;
			lastFrame=0;
		}

		for( currentFrameIndex=lastFrame; currentFrameIndex < (int)m_frames.size(); currentFrameIndex++ ) {
			timeCount = m_frames[currentFrameIndex]->m_partialTime ;
			if( elapsed <= timeCount ) break ;
		}

		if( /*!m_bLoop &&*/ currentFrameIndex>=(int)m_frames.size() ) {
			currentFrameIndex = m_frames.size()-1 ;
		}

	}
	else {
		// no frame to render
		currentFrameIndex = -1 ;
	}
}

AnimFrame2D* Animation2D::getFrame(int index) const
{
	AnimFrame2D* pAnimFrame = 0 ;
	if( index < 0 || index >= (int)m_frames.size() ) {
		index = 0;
	}
	pAnimFrame = m_frames[index] ;
	if( m_flipAllX != 0 ) {
		pAnimFrame->setFlipX(*m_flipAllX) ;
	}
	if( m_flipAllY != 0 ) {
		pAnimFrame->setFlipY(*m_flipAllY) ;
	}
	return pAnimFrame;
}

float Animation2D::getTotalTime()
{
	return m_totalTime ;
}

void Animation2D::setLoop(bool bLoop)
{
	m_bLoop = bLoop ;
}

bool Animation2D::isLooping()
{
	return m_bLoop ;
}

void Animation2D::setFlipAllX( bool flipX )
{
	if( m_flipAllX ) {
		JAM_DELETE(m_flipAllX) ;
	}
	m_flipAllX = new bool(flipX); 
}

void Animation2D::setFlipAllY( bool flipY )
{ 
	if( m_flipAllY ) {
		JAM_DELETE(m_flipAllY) ;
	}
	m_flipAllY = new bool(flipY); 
}

bool Animation2D::getFlipAllX() const
{
	return m_flipAllX == 0 ? false : *m_flipAllX ;
}

bool Animation2D::getFlipAllY() const
{
	return m_flipAllY == 0 ? false : *m_flipAllY ;
}

Animation2D* Animation2D::create()
{
	return new Animation2D() ;
}

}
