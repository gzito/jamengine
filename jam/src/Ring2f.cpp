/**********************************************************************************
* 
* Ring2f.cpp
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

#include <jam/Ring2f.h>

namespace jam
{
	Ring2f::Ring2f() :
		m_innerRing(),
		m_outerRing()
	{
	}

	Ring2f::Ring2f( const Circle2f& innerRing, const Circle2f& outerRing ) :
		m_innerRing(innerRing), m_outerRing(outerRing)
	{
	}

	float Ring2f::isIn( const Vector2& point ) const
	{
		float retVal = 0.0f ;

		if( m_innerRing.isPointInside(point) ) {
			retVal = 1.0f ;
		}
		else if( m_outerRing.isPointInside(point) ) {
			float delta = m_outerRing.getRadius() - m_innerRing.getRadius() ;
			float r = point.length() - m_innerRing.getRadius() ;
			float ratio =  1.0f - (r / delta) ;
			return ratio ;
		}

		return retVal ;
	}

	bool Ring2f::isZero() const
	{
		return m_innerRing.getRadius() == m_outerRing.getRadius() ;
	}

}
