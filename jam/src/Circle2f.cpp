/**********************************************************************************
* 
* Circle2f.cpp
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

#include <jam/Circle2f.h>
#include <jam/core/bmkextras.hpp>
#include <jam/core/math.h>
#include <jam/core/geom.h>

#include <glm/gtx/norm.hpp>

#include <math.h>

namespace jam
{

Circle2f::Circle2f() :
m_center(0.0f), m_radius(0.0f)
{

}

Circle2f::Circle2f( const Vector2& center, float radius ) :
	m_center(center), m_radius(radius)
{

}

Circle2f::Circle2f( const Polygon2f& poly ) :
	m_center(0.0f,0.0f), m_radius(0.0f)
{
	float squaredRadius = 0.0f ;
	float distSquared = 0.0f ;
	for( int i=0; i<poly.getCount(); i++ ) {
		distSquared = glm::length2(poly.getVertex(i) - m_center) ;
		if( distSquared > m_radius*m_radius ) {
			squaredRadius = distSquared ;
		}
	}

	m_radius = Q_SQRT(squaredRadius) ;
}

Circle2f::Circle2f( const AABB& aabb ) : 
	m_center(0.0f,0.0f), m_radius( glm::length(Vector2(aabb.hfWidth,aabb.hfHeight)) )
{
}

void Circle2f::transform( const Matrix3& mat )
{
	m_center = Vector2(mat * Vector3(m_center,1.0f)) ;
	Vector2 scale = jam::getScale(mat) ;
	m_radius *= Max(scale.x, scale.y) ;
}

void Circle2f::scale( float scaleFactor )
{
	m_radius *= scaleFactor ;
}

bool Circle2f::intersects( const Circle2f& other ) const
{
	float distanceSquared = glm::length2(getCenter() - other.getCenter()) ;
	float rr = getRadius() + other.getRadius() ;
	return distanceSquared <= (rr * rr) ;
}

bool Circle2f::isPointInside( const Vector2& p ) const
{
	Vector2 distFromCenter = p - m_center ;
	return glm::length2(distFromCenter) <= (m_radius*m_radius) ;
}

}
