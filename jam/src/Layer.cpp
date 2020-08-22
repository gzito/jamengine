/**********************************************************************************
* 
* Layer.cpp
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

#include "jam/Layer.h"
#include "jam/Draw3dManager.h"
#include "jam/Gfx.h"

namespace jam
{
	
Layer::Layer()
{
	init() ;
}

void Layer::init()
{
	float LDrawHalfXSize = Draw3DManager::Original3DWidth / 2.0f;
	float LDrawHalfYSize = Draw3DManager::Original3DHeight / 2.0f;

	AABB aabb( 0,0, LDrawHalfXSize,LDrawHalfYSize ) ;
	setAABB( aabb ) ;
}


ColorLayer::ColorLayer() :
	Layer(), m_color(Color::BLACK)
{
	init( (float)Draw3DManager::Original3DWidth,(float)Draw3DManager::Original3DHeight );
}


ColorLayer::ColorLayer( const Color& color ) :
	Layer(), m_color()
{
	setColor( color ) ;
	init( (float)Draw3DManager::Original3DWidth,(float)Draw3DManager::Original3DHeight );
}

ColorLayer::ColorLayer( const Color& color, float width, float height ) :
	Layer(), m_color()
{
	setColor(color) ;
	init( width, height ) ;
}

void ColorLayer::setWidth( float w )
{
	float LDrawXSize = w / 2;

	m_aabb = AABB(0,0,LDrawXSize,m_aabb.hfHeight) ;
	invalidateOBB() ;
}

void ColorLayer::setHeight( float h )
{
	float LDrawYSize = h / 2;

	m_aabb = AABB(0,0,m_aabb.hfWidth,LDrawYSize) ;
	invalidateOBB() ;
}

void ColorLayer::setWidthAndHeight( float w, float h )
{
	init( w, h ) ;
}

void ColorLayer::setColor( const Color& c )
{
	m_color = c ;
}

void ColorLayer::init( float w, float h )
{
	float LDrawHalfXSize = w / 2;
	float LDrawHalfYSize = h / 2;

	AABB aabb( 0,0, LDrawHalfXSize,LDrawHalfYSize ) ;
	setAABB( aabb ) ;
}


void ColorLayer::render()
{
	if( isEnabled() && isVisible() ) {
		GetGfx().setRenderLevel( getZOrder() ) ;
		GetDraw3DMgr().DrawQuad3D( m_obb, &m_color ) ;
	}
}

MultiplexLayer::MultiplexLayer()
{

}

}
