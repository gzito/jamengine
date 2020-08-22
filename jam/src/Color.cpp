/**********************************************************************************
* 
* Color.cpp
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

#include "jam/Color.h"
#include "jam/core/bmkextras.hpp"

#include <math.h>

namespace jam
{
	const Color Color::WHITE			= Color(255,255,255,255) ;
	const Color Color::YELLOW			= Color(255,255,0,255) ;
	const Color Color::MAGENTA			= Color(255,0,255,255) ;	//FUCHSIA
	const Color Color::RED				= Color(255,0,0,255);
	const Color Color::SILVER			= Color(192,192,192,255) ;
	const Color Color::GRAY				= Color(128,128,128,255) ;
	const Color Color::OLIVE			= Color(128,128,0,255) ;
	const Color Color::PURPLE			= Color(128,0,128,255) ;
	const Color Color::MAROON			= Color(128,0,0,255) ;
	const Color Color::CYAN				= Color(0,255,255,255) ;	//AQUA
	const Color Color::LIME				= Color(0,255,0,255) ;
	const Color Color::TEAL				= Color(0,128,128,255) ;
	const Color Color::GREEN			= Color(0,128,0,255) ;
	const Color Color::BLUE				= Color(0,0,255,255) ;
	const Color Color::NAVY				= Color(0,0,128,255) ;
	const Color Color::BLACK			= Color(0,0,0,255) ;
	

	const Color Color::PINK				= Color(255,128,192,255) ;
	const Color Color::ORANGE			= Color(255,165,0,255) ;

	
	// Added from http://www.htmlcolorname.com/ or http://www.febooti.com/products/iezoom/online-help/html-color-names-x11-color-chart.html

	const Color Color::SLATEBLUE		= Color(106,90,205,255) ;
	const Color Color::TOMATO			= Color(255,99,71,255) ;
	const Color Color::DARKORCHID		= Color(153,50,204,255) ;
	const Color Color::PALEVIOLETRED	= Color(219,112,147,255) ;
	const Color Color::GAINSBORO		= Color(220,220,220,255) ;
	const Color Color::BEIGE			= Color(245,245,220,255) ;
	const Color Color::CORAL			= Color(255,127,80,255) ;
	const Color Color::GOLD				= Color(255,215,0,255) ;
	const Color Color::LAVENDER		= Color(230,230,250,255) ;
	const Color Color::BROWN			= Color(165,42,42,255) ;
	const Color Color::SIENNA			= Color(160,82,45,255) ;
	const Color Color::FORESTGREEN	= Color(34,139,34,255) ;
	const Color Color::SEAGREEN		= Color(46,139,87,255) ;
	const Color Color::MIDNIGHTBLUE	= Color(25,25,112,255) ;

	const Color Color::INVISIBLE		= Color(255,255,255,0) ;
	const Color Color::JAM_TRANSPARENT	= Color(255,255,255,128) ;

	Color::Color( uint8_t r /*= 255*/, uint8_t g /*= 255*/, uint8_t b /*= 255*/, uint8_t a /*= 255 */ )
	{
		this->r = r ;
		this->g = g ;
		this->b = b ;
		this->a = a ;
	}

	Color::Color( const Color& other )
	{
		this->r = other.r ;
		this->g = other.g ;
		this->b = other.b ;
		this->a = other.a ;
	}

	Color::Color( uint32_t rgba )
	{
		setRgba(rgba) ;
	}

	Color Color::operator+( const Color& other ) const
	{
		Color c(*this) ;
		c.r += other.r ;
		c.g += other.g ;
		c.b += other.b ;
		c.a += other.a ;
		return c ;
	}

	Color Color::operator-( const Color& other ) const
	{
		Color c(*this) ;
		c.r -= other.r ;
		c.g -= other.g ;
		c.b -= other.b ;
		c.a -= other.a ;
		return c ;
	}

	Color Color::operator*( const Color& other ) const
	{
		Color c(*this) ;
		c.r *= other.r ;
		c.g *= other.g ;
		c.b *= other.b ;
		c.a *= other.a ;
		return c ;
	}

	Color Color::operator*( float factor ) const
	{
		Color c(*this) ;

		if(factor>1.0f)	factor=1.0f;
		if(factor<0.0f)	factor=0.0f;

		c.r *= factor ;
		c.g *= factor ;
		c.b *= factor ;
		c.a *= factor ;
		return c ;
	}

	Color Color::operator-() const
	{
		return Color(255-r, 255-g, 255-b, 255-a) ;
	}

	void Color::setGrey( uint8_t g )
	{
		r = g ;
		g = g ;
		b = g ;
	}

	void Color::setRgbOpaque( uint32_t rgb )
	{
		r = (rgb >> 16)  & 0xff ;
		g = (rgb >> 8) & 0xff ;
		b = rgb & 0xff ;
	}

	void Color::setRgba( uint32_t rgba )
	{
		r = (rgba >> 24)  & 0xff ;
		g = (rgba >> 16) & 0xff ;
		b = (rgba >> 8)  & 0xff ;
		a = rgba & 0xff ;
	}

	uint32_t Color::getRgba() const
	{
		return (r << 24) | (g << 16) | (b << 8) | a ;
	}

	void Color::setRgba( uint8_t r, uint8_t g, uint8_t b, uint8_t a )
	{
		this->r = r ;
		this->g = g ;
		this->b = b ;
		this->a = a ;
	}

	void Color::darken( float factor/*=0.1f*/ )
	{
		if( factor>1.0f ) {
			factor = 1.0f;
		}
		
		if( factor<0.0f ) {
			factor = 0.0f;
		}

		factor = 1 - factor;

		r *= factor;
		b *= factor;
		g *= factor;
	}

	void Color::lighten( float factor/*=0.1f*/ )
	{
		if( factor>1.0f ) {
			factor=1.0f;
		}

		if( factor<0.0f ) {
			factor=0.0f;
		}

		r += (255 - r) * factor;
		b += (255 - b) * factor;
		g += (255 - g) * factor;
	}

	void Color::toTransparent( float factor/*=0.1f*/ )
	{
		if(factor>1.0f)	factor=1.0f;
		if(factor<0.0f)	factor=0.0f;

		a *= 1 - factor;
	}

	void Color::toUnTransparent( float factor/*=0.1f*/ )
	{
		if(factor>1.0f)	factor=1.0f;
		if(factor<0.0f)	factor=0.0f;

		a *= factor;
	}

	void Color::toMatte( float factor/*=0.1f*/ )
	{
		if(factor>1.0f)	factor=1.0f;
		if(factor<0.0f)	factor=0.0f;

		a += (255 - a) * factor;
	}
	/*
	float Color::getBrightness()
	{
	uint8_t max=0;

	if(m_nativeObj.r>max)
	max=m_nativeObj.r;
	if(m_nativeObj.g>max)
	max=m_nativeObj.g;
	if(m_nativeObj.b>max)
	max=m_nativeObj.b;
	max/=255;
	return max;
	}*/
	// ************************************************************************

/*
	GZ to be tested, it seems not working. h,s,v are float or int?

	Color Color::hsvToRgb(int h, int s, int v)
	{
		float r;
		float g;
		float b;

		int i = (int)(h * 6);
		float f = h * 6 - i;
		float p = v * (1 - s);
		float q = v * (1 - f * s);
		float t = v * (1 - (1 - f) * s);

		switch(i % 6){
		case 0: r = v, g = t, b = p; break;
		case 1: r = q, g = v, b = p; break;
		case 2: r = p, g = v, b = t; break;
		case 3: r = p, g = q, b = v; break;
		case 4: r = t, g = p, b = v; break;
		case 5: r = v, g = p, b = q; break;
		}

		return Color(r * 255, g * 255, b * 255);
	}
*/

	Color Color::hsvfToRgb(float h, float s, float v)
	{
		if (h == 0 && s == 0)
			return Color::fMakeColorRgba(v, v, v);

		float c = s * v;
		float x = c * (1 - Abs(fmodf(h,2) - 1));
		float m = v - c;

		if (h < 1) return Color::fMakeColorRgba(c + m, x + m, m);
		else if (h < 2) return Color::fMakeColorRgba(x + m, c + m, m);
		else if (h < 3) return Color::fMakeColorRgba(m, c + m, x + m);
		else if (h < 4) return Color::fMakeColorRgba(m, x + m, c + m);
		else if (h < 5) return Color::fMakeColorRgba(x + m, m, c + m);
		else return Color::fMakeColorRgba(c + m, m, x + m);
	}

	Color Color::lerp( const Color& value1, const Color& value2, float amount )
	{
		Color col ;
		col.r = (uint8_t)LERP(value1.r, value2.r, amount ) ;
		col.g = (uint8_t)LERP(value1.g, value2.g, amount ) ;
		col.b = (uint8_t)LERP(value1.b, value2.b, amount ) ;
		col.a = (uint8_t)LERP(value1.a, value2.a, amount ) ;

		return  col ;
	}

	
}