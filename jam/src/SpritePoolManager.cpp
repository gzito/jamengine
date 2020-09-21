/**********************************************************************************
* 
* SpritePoolManager.cpp
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

#include "jam/SpritePoolManager.h"

namespace jam
{

Sprite* SpritePoolManager::getNew( const String& tag )
{
	size_t howMuch= countObjectsByTag(tag) ;
	Sprite* res=0;
	if( howMuch ) {
		Sprite* res = (Sprite*)findObjectByTag(tag);
		res->setEnabled(true);
	}
	else {
		res = new Sprite();
		res->setTag(tag);
		addObject(res) ;
	}
	return res;
}

void SpritePoolManager::dispose( Sprite* spr,const String& tag )
{
	size_t howMuch = countObjectsByTag(tag) ;
	if( howMuch > 100 )	{
		spr->destroy();
	}
	else {
		spr->stopAllActions();
		spr->setEnabled(false);
		addObject(spr);
	}
}

void SpritePoolManager::disposeAll()
{

}

}
