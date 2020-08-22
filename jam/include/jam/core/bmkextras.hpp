/**********************************************************************************
* 
* bmkextras.hpp
* 
* This file is part of Jam
* 
* Copyright (c) 2014-2019 Giovanni Zito, Gianluca Sclano.
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

#ifndef __JAM_BMKEXTRAS_H__
#define __JAM_BMKEXTRAS_H__

#include <jam/thirdparty/random.h>
#include <jam/core/Math.h>


//
// Macros from BMK Extras
//

// Calcolo condizionale
 
#ifndef Min
#define Min(a,b)				(((a)<(b)) ? (a) : (b))
#endif

#ifndef Max
#define Max(a,b)				(((a)>(b)) ? (a) : (b))
#endif

#define Abs(i)					((i)<0 ? -(i) : (i))
#define Limit(x,low,up)			(((x) < (low)) ? (low) : (((x) > (up)) ? (up) : (x)))
#define LimitMin(x,low)			(((x) < (low)) ? (low) : (x))
#define LimitMax(x,up)			(((x) > (up)) ? (up) : (x))
#define Wrap(x,low,up)			jam::wrap((x),(low),(up))
#define Wrapf(x,low,up)			jam::wrapf((x),(low),(up))
#define WrapAngle(x)			jam::wrapf((x),0.0f,360.0f)
#define WrapAngleRadian(x)		jam::wrapf((x),0.0f,JAM_TWOPI)
#define Near(x,low,up)			((Abs((x)-(low)) > Abs((up)-(x))) ? (up) : (low))
#define Far(x,low,up)			((Abs((x)-(low)) < Abs((up)-(x))) ? (up) : (low))
#define Same(x,y,p)				((Abs((x)-(y)))<=(p) ? (1) : (0))

// Computazioni
#define AbsMax(a,b)				((Abs(a))>(Abs(b))) ? (Abs(a)) : (Abs(b))
#define AbsMin(a,b)				((Abs(a))<(Abs(b))) ? (Abs(a)) : (Abs(b))
#define Max3(a,b,c)				(((a)>(b)) ? (((a)>(c)) ? (a) : (((c)>(b)) ? (c) : (b))): (((c)>(b)) ? (c) : (b)))
#define Min3(a,b,c)				(((a)<(b)) ? (((a)<(c)) ? (a) : (((c)<(b)) ? (c) : (b))): (((c)<(b)) ? (c) : (b)))
#define Middle(a,b,c)			(( (a)>(b) ) ? (( (a)<(c) ) ? (a) : (( (b)<(c) ) ? (b) : (c) )) : (( (b)<(c) ) ? (b) : (c) ))
#define Delta(a,b)				(Abs((a)-(b)))
#define Ratio(a,b)				((a)/(b))
#define MaxRatio(a,b)			((AbsMax((a),(b)))/(AbsMin((a),(b))))
#define MinRatio(a,b)			((AbsMin((a),(b)))/(AbsMax((a),(b))))
#define Sgn(a)					(((a)<(0)) ? (-1) : (((a)>(0)) ? (1) : (0)))
#define Sqr(a)					((a)*(a))

// Quick sqrtf
#define Q_SQRT(a)				(jam::Q_sqrt(a))

// Statistica
// Percent(10,20) = 10+20%  = 10+2=12 -> 10+((20/100)*10)
#define addperc(v,p)			(v)+ (((p)*(v))/100.0f)
#define Percent(a,p)			(((a)*(p))/(100.0))
#define Rapporto(a,b)			(((a)*100).0/(b))
#define Average(low,up)			(((low)+(up))/2.0)
#define Average3(a,b,c)			(((a)+(b)+(c))/3.0)

// Calcolo Geometrico
#define Modulo(a,b)				(sqrt((a)*(a)+(b)*(b)))
#define Modulof(a,b)			(sqrtf((a)*(a)+(b)*(b)))
#define De(x1,x2)				((x2)-(x1))
#define CoefAngle(x1,y1,x2,y2)	((De((x1),(x2)))/(De((y1)/(y2)))
#define Distanza(x1,y1,x2,y2)	(Modulo((De((x1),(x2))),(De((y1),(y2)))))

// Calcolo Direzionale
#define GoDir(x1,x2)			(Sgn(De(x1,x2)))
#define AwayDir(x1,x2)			(Sgn(De(x2,x1)))
#define GoRange(x1,x2,r)		( (abs((x2)-(x1))>(r)) ? (Sgn((x2)-(x1))) : (0) )
#define AwayRange(x1,x2,r)		( (abs((x2)-(x1))>(r)) ? (Sgn((x1)-(x2))) : (0) )
//#define StepX(x1,y1,x2,y2)	(De((y1),(y2))) ? (De((x1),(x2))/MaxDe((x1),(y1),(x2),(y2))) : (0))
#define StepX(x1,y1,x2,y2)		((De((x1),(x2)))/(MaxDe((x1),(y1),(x2),(y2))))
#define StepY(x1,y1,x2,y2)		((De((y1),(y2)))/(MaxDe((x1),(y1),(x2),(y2))))

#define MaxDe(x1,y1,x2,y2)		AbsMax((De((x1),(x2))),(De((y1),(y2))))
#define INT(x)					(floor((x)))
#define DeRange(a,b,r)			((INT(De((a),(b))/(r))))
#define DeRangePerc(a,b,p)		(Percent((INT(De((a),(b)))),(p)))

// Calcolo Casuale

/// Seeds the random number generator
/// deprecated
#define RNDSEED(seed)			srand((seed))

/// Generates random int32 between [0,max), max esclusive
#define RND(n)					jam::Random::get<uint32_t>(0,(n)-1)

/// Generates random int32 between [0,max], max inclusive
#define RAND(n)					RND((n)+1)

/// Generates random int32 between [a,b], a and b inclusive
#define RANGERAND(a,b)			jam::Random::get<int32_t>((a),(b))

/// Returns a random number that is a or b
//	e.g. : 4+8=0..11-> 0..3,4..11
#define RNDCHOOSE(a,b)			((RND((a)+(b)))<(a)) ? (a) : (b)


/// Generates random float between [0.0f,1.0f]
#define RANDF()					jam::Random::get<float>(0.0f,1.0f)

/// Returns a random float in the indicated interval [Mi,Ma], max inclusive
#define RANGERANDF(Mi, Ma)		jam::Random::get<float>((float)(Mi),(float)(Ma))

/// deprecated
#define RandomBinomial(v)		((v*(rand()-rand()) / (float)RAND_MAX))

// Calcolo Trigonometrico
#define AbsDegree(degree)		(degree<0) ? -degree : degree
#define PosDegree(degree)		(degree<0) ? 180-degree : degree
//DEL = PI - ABS(PI - ABS(A - B)) in radians.
//	DEL = 180 - ABS(180- ABS(A - B)) in degrees.
#define DeltaAngles(a1,a2)		(180.0f - Abs(180.0f- Abs(a2 - a1)))

#define AbsIDegree(degree)		(degree ^ (degree >> 31)) – (degree >> 31)

// XOR SWAP ALGORITHM, http://en.wikipedia.org/wiki/XOR_swap_algorithm
#define SWAP(a,b)				{(a^=(b), (b)^=(a), (a)^=(b);}

#define LIMIT					Limit

// This method performs the linear interpolation based on the following formula.
// amount is a value between 0 and 1 indicating the weight of value2.
// Passing amount a value of 0 will cause value1 to be returned, a value of 1 will cause value2 to be returned.
#define LERP(value1,value2,amount)		(value1 + (value2 - value1) * amount)

#endif	__JAM_BMKEXTRAS_H__
