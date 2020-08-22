/**********************************************************************************
* 
* Ring2f.h
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

#ifndef __JAM_RING2F_H__
#define __JAM_RING2F_H__

#include <jam/jam.h>
#include <jam/Circle2f.h>


namespace jam
{

/**
	This class represents a (bounding) circle in 2D
*/
class JAM_API Ring2f
{
public:

							Ring2f() ;
							Ring2f( const Circle2f& innerRing, const Circle2f& outerRing );

	const Circle2f&			getInnerRing() const { return m_innerRing; }
	void					setInnerRing( const Circle2f& circle ) { m_innerRing = circle; }

	const Circle2f&			getOuterRing() const { return m_outerRing; }
	void					setOuterRing( const Circle2f& circle ) { m_outerRing = circle; }

	float					isIn( const Vector2& point ) const ;

	bool					isZero() const ;

private:
	Circle2f				m_innerRing ;
	Circle2f				m_outerRing ;
};
	
}


#endif // __JAM_RING2F_H__
