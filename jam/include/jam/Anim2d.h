/**********************************************************************************
* 
* Amin2d.h
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

#ifndef __JAM_ANIM2D_H__
#define __JAM_ANIM2D_H__

#include <jam/jam.h>

//#include <jam/BankItem.h>
#include <jam/DrawItem.h>
//#include <jam/Draw2d.h>
#include <jam/Timer.h>
//#include <jam/Texture2D.h>

#include <vector>

namespace jam
{

/**
	This class represents a single frame of a 2D animation
*/
class JAM_API AnimFrame2D : public BankItem
{
	friend class Animation2D ;

public:
	virtual					~AnimFrame2D();

	DrawItem*				getDrawItem() { return m_handleImg; }
	void					setDrawItem(DrawItem* val);

	void					setTime(float fTime) { m_time = fTime; } ;
	float					getTime() const { return m_time; } ;

	void					setFlipX( bool flipX ) { m_flipX = flipX; }
	bool					getFlipX() const { return m_flipX; }

	void					setFlipY( bool flipY ) { m_flipY = flipY; }
	bool					getFlipY() const { return m_flipY; }

	static AnimFrame2D*		create() ;

private:
							AnimFrame2D();

	DrawItem*				m_handleImg;
	float					m_time ;
	bool					m_flipX ;
	bool					m_flipY ;
	float					m_partialTime ;
};

// *************************************************************************

/**
	This class represents a 2D animation
*/
class JAM_API Animation2D : public BankItem
{
public:
	virtual					~Animation2D();

	void					addFrame(AnimFrame2D* pFrame);
	void					addFrame(const Ref<Texture2D>& pTexture, const jam::Rect& rect, float time, bool flipX=false, bool flipY=false);
	void					addFrame(DrawItem* pItem, float time, bool flipX=false, bool flipY=false);
	
	AnimFrame2D*			getFrame(int index) const ;
	size_t					getNumOfFrames() const { return m_frames.size(); } ;

	float					getTotalTime();

	void					setFlipAllX( bool flipX ) ;
	bool					getFlipAllX() const;

	void					setFlipAllY( bool flipY ) ;
	bool					getFlipAllY() const;


	void					setLoop(bool bLoop) ;
	bool					isLooping() ;

	void					update(const jam::Timer&, int& currentFrameIndex, float speed=1.0f, int lastFrame=0) ;

	static Animation2D*		create() ;

private:
							Animation2D();

	std::vector<AnimFrame2D*>	m_frames ;
	float					m_totalTime ;
	bool					m_bLoop ;
	bool*					m_flipAllX;
	bool*					m_flipAllY;
};

}

#endif // __JAM_ANIM2D_H__
