/**********************************************************************************
* 
* SpritesDemo.h
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

#ifndef __Sprites_App_H__
#define __Sprites_App_H__

#include <jam/Application.h>
#include <jam/Draw3dManager.h>
#include <jam/BaseManager.hpp>
#include <jam/Pivot2d.h>
#include <jam/Layer.h>


struct SpriteManager : public jam::NamedTaggedObjectManager<jam::Sprite>
{
};

class SpritesDemo : public jam::Application
{
public:
	SpritesDemo() ;

protected:
	virtual bool					init() override ;
	virtual void					handleInput() override ;
	virtual void					beforeSceneUpdate() override ;
	virtual void					destroy() override ;

private:
	SpriteManager*					m_pSprMgr;

	float							m_angle ;
	float							m_initialSpeed ;

	jam::String						m_sonicName ;
	jam::String						m_gemSheetName ;
};

#endif
