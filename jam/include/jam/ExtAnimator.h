/**********************************************************************************
* 
* ExtAnimator.h
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

#ifndef __JAM_EXTANIMATOR_H__
#define __JAM_EXTANIMATOR_H__


#include <jam/jam.h>
#include <jam/Timer.h>
#include <jam/Anim2d.h>

namespace jam
{

	class JAM_API ExtAnimator : public RefCountedObject
	{
	public:
		ExtAnimator(IDrawable2D* pIDrawable);
		virtual ~ExtAnimator();

		// animations
		void			setAnimation( Animation2D* pAnimation, bool autoStart=false, int firstFrame=0 );
		Animation2D*	getAnimation()	const { return m_pAnimation ; }
		
		float			getSpeed() const { return m_speed; }
		void			setSpeed(float val) { m_speed = val; }

		void			startAnimation();
		void			stopAnimation();
		void			resetAnimation();
		void			completeAnimation(int frame=-1);
		bool			isPlaying();

		int				getLastFrameIndex() const { return m_lastFrameIndex; }

		virtual void	update() ;
	

	private:
		int					m_lastFrameIndex ;
		int					m_completeFrameIndex;
		float				m_speed ;
		jam::Timer*		m_timer ;
		jam::Animation2D*	m_pAnimation ;
		jam::IDrawable2D*	m_pIDrawable ;
		jam::Animation2D*	m_defaultAnimation ;
	};

}

#endif // __JAM_EXTANIMATOR_H__
