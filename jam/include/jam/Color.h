/**********************************************************************************
* 
* Color.h
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

#ifndef __JAM_COLOR_H__
#define __JAM_COLOR_H__

#include <jam/jam.h>
#include <jam/String.h>

#include <glm/vec4.hpp>

/** Constructs a color setting all 4 components from floating point values. */
#define COLOR_RGBA_F(r,g,b,a)	jam::Color::fMakeColorRgba((r),(g),(b),(a))

/** Constructs a color setting the rgb components from floating point values, and make alpha value fully opaque. */
#define COLOR_RGB_F(r,g,b)		jam::Color::fMakeColorRgba((r),(g),(b))

namespace jam
{

class JAM_API Color
{
public:
	static const			Color WHITE;		
	static const			Color YELLOW;			
	static const			Color MAGENTA;			
	static const			Color RED;				
	static const			Color SILVER;			
	static const			Color GRAY;				
	static const			Color OLIVE;			
	static const			Color PURPLE;			
	static const			Color MAROON;			
	static const			Color CYAN;				
	static const			Color LIME;				
	static const			Color TEAL;				
	static const			Color GREEN;			
	static const			Color BLUE;				
	static const			Color NAVY;				
	static const			Color BLACK;			

	static const			Color PINK;				
	static const			Color ORANGE;		

	static const			Color SLATEBLUE;		
	static const			Color TOMATO;			
	static const			Color DARKORCHID;		
	static const			Color PALEVIOLETRED;	
	static const			Color GAINSBORO;		
	static const			Color BEIGE;			
	static const			Color CORAL;			
	static const			Color GOLD;				
	static const			Color LAVENDER;		
	static const			Color BROWN;			
	static const			Color SIENNA;			
	static const			Color FORESTGREEN;	
	static const			Color SEAGREEN;		
	static const			Color MIDNIGHTBLUE;	

	static const			Color JAM_TRANSPARENT ; 
	static const			Color INVISIBLE ; 




	/** Constructs a color setting all 4 components from a single rgba 32-bit value. */
							Color( uint32_t rgba ) ;

	/** Constructs a color setting all 4 components.
		@param r 8-bit r value to set.
		@param g 8-bit g value to set.
		@param b 8-bit b value to set.
		@param a 8-bit a value to set.
	*/
							Color( uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255 ) ;

	/** Copy constructor. */
							Color( const Color& other ) ;

	/** Adds two color values together to create a new color value. */
	Color					operator+( const Color& other) const ;

	/** Subtracts two color values to create a new color value. */
	Color					operator-( const Color& other) const;

	/**
		Blends two colors.
	\remark This function blends together two colors by multiplying matching color components
	*/
	Color					operator*( const Color& other) const;

	/** Scales color components */
	Color					operator*( float scalar ) const;

	/** Creates the negative of a color value. */
	Color					operator-() const ; 

	/** Sets all 4 components from a single input, by specifying a grayscale value.	*/
	void					setGrey(uint8_t g) ;

	/** Sets all 3 components from a single input, and make alpha value fully opaque. */
	void					setRgbOpaque(uint32_t rgb) ;

	/** Sets all 4 components from a single input. */
	void					setRgba(uint32_t rgba) ;

	/** Sets all 4 components */
	void					setRgba( uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255 );

	/** Returns all 4 components as rgba 32-bit value. */
	uint32_t				getRgba() const ;

	/** Returns all 4 components as floating point values */
	glm::vec4				getFloatingComponents() const ;

	static Color			fMakeColorRgba(float r,float g,float b,float a=1.0f) ;
	static void				hextoRgb(String hex, Color& rgb) ;
	static void				hextoRgba(String hex, Color& rgba) ;
	//static Color hsvToRgb(int h, int s, int v);
	static Color			hsvfToRgb(float h, float s, float v);

	/** 
	Linearly interpolates between two colors.
	\remark This function linearly interpolates each component of a Color separately and returns a Color with the new component values
	*/
	static Color			lerp( const Color& value1, const Color& value2, float amount ) ;

	void					darken(float factor=0.1f);
	void					lighten(float factor=0.1f);

	void					toTransparent(float factor=0.1f);
	void					toUnTransparent(float factor=0.1f);

	void					toMatte(float factor=0.1f);

public:
	uint8_t					r ;
	uint8_t					g ;
	uint8_t					b ;
	uint8_t					a ;
};

	
JAM_INLINE glm::vec4 Color::getFloatingComponents() const
{
	return glm::vec4( r/255.0f, g/255.0f, b/255.0f, a/255.0f );
}

JAM_INLINE Color Color::fMakeColorRgba(float r,float g,float b,float a/*=1.0f*/)
{
	Color c = Color((uint8_t)(r*255),(uint8_t)(g*255),(uint8_t)(b*255),(uint8_t)(a*255)) ;
	return c ;
}

JAM_INLINE void Color::hextoRgb(String hex, Color& rgb)
{    
	rgb.setRgba(convertFromHex(hex.substr(0, 2)) , convertFromHex(hex.substr(2, 2)), convertFromHex(hex.substr(4, 2)) );
}

JAM_INLINE void Color::hextoRgba(String hex, Color& rgba)
{    
	rgba.setRgba(convertFromHex(hex.substr(0, 2)) , convertFromHex(hex.substr(2, 2)), convertFromHex(hex.substr(4, 2)), convertFromHex(hex.substr(6, 2)) );
} 

//Color Color::hsvToRgb(int h, int s, int v);
}

#endif // __JAM_COLOR_H__
