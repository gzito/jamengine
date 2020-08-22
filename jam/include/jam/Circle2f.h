/**********************************************************************************
* 
* Circle2f.h
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

#ifndef __JAM_CIRCLE2F_H__
#define __JAM_CIRCLE2F_H__

#include <jam/jam.h>
#include <jam/Polygon2f.h>
#include <jam/Draw2d.h>


namespace jam
{

/**
	This class represents a (bounding) circle in 2D
*/
class JAM_API Circle2f
{
public:
	/** Constructs a circle	with radius 0 and center (0,0). */
							Circle2f() ;

	/** Constructs a circle	with the given radius and center. */
							Circle2f( const Vector2& center, float radius ) ;

	/** Constructs a circle	enclosing the given polygon. */
	explicit				Circle2f( const Polygon2f& poly ) ;

	/** Constructs a circle	enclosing the given axis-aligned bounding box. */
	explicit				Circle2f( const AABB& aabb ) ;

	/** Returns the circle's center. */
	const Vector2&			getCenter() const { return m_center; }

	/** Sets the circle's center. */
	void					setCenter(const Vector2& val) { m_center = val; }

	/** Returns the circle's radius. */
	float					getRadius() const { return m_radius; }

	/** Returns the circle's squared radius, useful to compare it with squared distances. */
	float					getRadiusSquared() const { return m_radius*m_radius; }

	/** Sets the circle's radius. */
	void					setRadius(float val) { m_radius = val; }

	/** Transform the circle by the given matrix. */
	void					transform(const Matrix3& mat) ;

	/** Scales the circle */
	void					scale( float scaleFactor );

	/** Returns true if this circle intersects the given circle. */
	bool					intersects( const Circle2f& other ) const ;

	/** Returns true if the given point in inside this circle. */
	bool					isPointInside(const Vector2& p) const ;
	
private:
	Vector2					m_center ;
	float					m_radius ;
};
	
}


#endif // __JAM_CIRCLE2F_H__
