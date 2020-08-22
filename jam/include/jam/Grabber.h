/**********************************************************************************
* 
* Grabber.h
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

#ifndef __JAM_GRABBER_H__
#define __JAM_GRABBER_H__

#include <jam/RefCountedObject.h>
#include <jam/DrawItem.h>
#include <jam/Color.h>

namespace jam 
{

/**
	Class for internal use
*/
class Grabber
{
public:
/*
	Grabber(void);

	~Grabber(void);
*/
	// attach the texture to the framebuffer, so that it can be possible render on texture
	void grab(jam::DrawItem* pDrawitem);

	// clear the texture and prepare to render on it
	// called before rendering to texture begin
	void beforeRender();

	// make the default framebuffer as current (i.e. screen)
	// called after rendering to texture is over
	void afterRender();

	Texture2D* getTexture() ;
/*
protected:
	CIwGxSurface*	m_pSurface ;
*/
	jam::Color		m_clearColor ;
};

}

#endif // __JAM_GRABBER_H__
