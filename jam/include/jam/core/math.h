/**********************************************************************************
* 
* math.h
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

#ifndef __JAM_MATH_H__
#define __JAM_MATH_H__

#include <jam/jam.h>

#define ToRadian( degree )		((degree) * jam::RAD_DEG_RATIO)
#define ToDegree( radian )		((radian) * jam::DEG_RAD_RATIO)

namespace jam
{

extern JAM_API const float	JAM_PI ;		// PI
extern JAM_API const float	JAM_TWOPI ;		// 2 * PI 
extern JAM_API const float	JAM_PI_2 ;		// PI / 2
extern JAM_API const float	JAM_PI_4 ;		// PI / 4
extern JAM_API const float	RAD_DEG_RATIO ;
extern JAM_API const float	DEG_RAD_RATIO ;

/// Greatest Common Divisor (GCD), iterative algorithm
JAM_API int					greatestCommonDivisor(int a, int b);

/// Greatest Common Divisor (GCD), recursive algorithm:
JAM_API int					gcd (int a, int b) ;

/** Performs a quick 1/sqrt */
JAM_API float				Q_rsqrt( float number ) ;

/** Performs a quick sqrt */
JAM_API float				Q_sqrt(float number) ;

/** Performs a Catmull-Rom interpolation using the specified positions */
JAM_API float				catmullRom( float value1, float value2, float value3, float value4, float amount) ;

JAM_API bool				isPowerOfTwo( int x ) ;

JAM_API float				wrapf( float value, float minInclusive, float maxEclusive ) ;

// use with integral types
template<typename T> T wrap(T value,T minInclusive,T maxEclusive)
{
	T x = value % maxEclusive ;
	if( x < minInclusive ) { x += maxEclusive; }
	return x ;
}

}

#endif	// __JAM_MATH_H__
