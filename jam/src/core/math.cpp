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

#include "stdafx.h"

#include "jam/core/math.h"

namespace jam
{
const float JAM_PI = (const float)M_PI ;
const float JAM_TWOPI = (const float)(2 * M_PI) ;
const float JAM_PI_2 = (const float)M_PI_2 ;
const float JAM_PI_4 = (const float)M_PI_4 ;
const float RAD_DEG_RATIO = (float)M_PI / 180.0f ;
const float DEG_RAD_RATIO = 180.0f / (float)M_PI  ;

int greatestCommonDivisor( int a, int b )
{
	while (1)
	{
		a = a % b;
		if (a == 0)
			return b;
		b = b % a;

		if (b == 0)
			return a;
	}
}


int gcd (int a, int b)
{
	return (b == 0) ? a : gcd (b, a%b);
}

	
// Fast inverse square root 
// See Quake III John Carmack implementaton: http://www.codemaestro.com/reviews/9
float Q_rsqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
	//    y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

	return y;
}

float Q_sqrt(float number)
{
	return 1.0f / Q_rsqrt(number) ;
}


float catmullRom( float value1, float value2, float value3, float value4, float amount )
{
	// Using formula from http://www.mvps.org/directx/articles/catmull/

	float amountSquared = amount * amount;
	float amountCubed = amountSquared * amount;
	return (float)(0.5f * (2.0f * value2 +
		(value3 - value1) * amount +
		(2.0f * value1 - 5.0f * value2 + 4.0f * value3 - value4) * amountSquared +
		(3.0f * value2 - value1 - 3.0f * value3 + value4) * amountCubed));
}

bool isPowerOfTwo( int x )
{
	return (( x & ( x - 1 ) ) == 0) && (x > 0);
}

float wrapf(float value,float minInclusive,float maxEclusive)
{
	float x = fmodf(value,maxEclusive) ;
	if( x < minInclusive ) {
		x += maxEclusive ;
	}
	return x ;
}

}

