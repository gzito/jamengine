/**********************************************************************************
* 
* Draw2d.cpp
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

#include "jam/Draw2d.h"
#include "jam/Polygon2f.h"
#include "jam/core/bmkextras.hpp"
#include "jam/core/geom.h"


namespace jam
{
	
AABB::AABB() : x1(0.0f), y1(0.0f), x2(0.0f), y2(0.0f), width(0.0f), height(0.0f), hfWidth(0.0f), hfHeight(0.0f)
{

}

AABB::AABB( float cx, float cy, float halfWidth, float halfHeight ) :
	x1(cx-halfWidth),
	y1(cy+halfHeight),
	x2(cx+halfWidth),
	y2(cy-halfHeight),
	hfWidth(halfWidth),
	hfHeight(halfHeight)
{
	width  = Abs(x2 - x1) ;
	height = Abs(y2 - y1) ;
}

void AABB::setBounds( float px1, float py1, float px2, float py2 )
{
	x1 = Min(px1,px2); y1 = Max(py1,py2) ;
	x2 = Max(px1,px2); y2 = Min(py1,py2) ;
	width  = Abs(x2 - x1) ;
	height = Abs(y2 - y1) ;
	hfWidth  = width / 2.0f ;
	hfHeight = height / 2.0f ;
}

bool AABB::contains( const Vector2& v ) const
{
	return (v.x >= x1 && v.x <= x2) && (v.y >= y2 && v.y <= y1) ; 
}

bool AABB::contains( const AABB& box ) const
{
	return (box.x1 >= x1 && box.x2 <= x2) && (box.y2 >= y2 && box.y1 <= y1) ;
}

bool AABB::intersects( const AABB& box ) const
{
	return !(x1 > box.x2 || x2 < box.x1 ||	y2 > box.y1 || y1 < box.y2);
}

bool AABB::intersects( const Polygon2f& poly ) const
{
	for( int i = 0; i<poly.getCount(); i++ ) {
		if( contains(poly.getVertex(i)) ) return true ; 
	}

	return false ;
}

bool AABB::contains( const Polygon2f& poly ) const
{
	for( int i = 0; i<poly.getCount(); i++ ) {
		if( !contains(poly.getVertex(i)) ) return false ; 
	}

	return true ;
}

void AABB::transform( const Matrix3& m )
{
	glm::vec2 lt(x1,y1) ;
	glm::vec2 rb(x2,y2) ;
	lt = jam::transform(m,lt) ;
	rb = jam::transform(m,rb) ;
	setBounds( lt.x, lt.y, rb.x, rb.y ) ;
}


//
// Rect
//

Rect::Rect() :
	left(0), top(0), right(0), bottom(0)
{
}

Rect::Rect( int32_t l, int32_t t, int32_t r, int32_t b ) :
	left(l), top(t), right(r), bottom(b)
{
}

int32_t Rect::getWidth() const
{
	return Abs(right - left) ;
}

int32_t Rect::getHeight() const 
{
	return Abs(bottom - top) ;
}

void Rect::setBounds(int32_t x, int32_t y, int32_t width, int32_t height)
{
	left = x ;
	top = y ;
	right = x+width ;
	bottom = y+height;
}

void Rect::grow(int32_t h,int32_t v)
{
	left = left - h ;
	top = top + v ;
	right = right + h ;
	bottom = bottom - v ;
}

bool Rect::isEmpty() const
{
	return (right-left == 0) && (bottom-top == 0) ;
}

bool Rect::hasSameSize( const Rect& other )
{
	return (right-left == other.right-other.left) && (bottom-top == other.bottom-other.top) ;
}

bool Rect::operator==( const Rect& other )
{
	return left==other.left && top==other.top && right == other.right && bottom == other.bottom ;
}

void Rect::scale( float s )
{
	left *= s ;
	top *= s ;
	right *= s ;
	bottom *= s ;
}

bool Rect::contains( const Point& point )
{
	return contains(point.x,point.y) ;
}

bool Rect::contains( int32_t x, int32_t y )
{
	// TODO to take in account of negative bottom
	return (x >= left && x <= right) && (y >= bottom && y <= top) ; 
}


//
// Point
// 

Point::Point() : x(0), y(0)
{
}

Point::Point(int32_t xp,int32_t yp ) : x(xp), y(yp)
{
}


Bounds::Bounds() :
	m_min(0,0,0),
	m_max(0,0,0),
	m_size(0,0,0),
	m_center(0,0,0),
	m_extents(0,0,0)
{
}

Bounds::Bounds( const Vector3& center, const Vector3& extents ) :
	Bounds()
{
	m_center = center ;
	m_extents = extents ;
	m_size = m_extents * 2.0f ;
	m_min = glm::min( center-extents, center+extents ) ;
	m_max = glm::max( center-extents, center+extents ) ;
}

void Bounds::setMinMax( const Vector3& min, const Vector3& max )
{
	m_min = min ;
	m_max = max ;
	m_size = glm::abs(max - min) ;
	m_extents  = m_size / 2.0f ;
	m_center = m_min + m_extents ;
}

bool Bounds::contains( const Vector3& v ) const
{
	return (v.x >= m_min.x && v.x <= m_max.y) &&
		(v.y >= m_min.y && v.y <= m_max.y) && 
		(v.z >= m_min.z && v.z <= m_max.z); 
}

bool Bounds::contains( const Bounds& box ) const
{
	return (box.m_min.x >= m_min.x && box.m_max.x <= m_max.x) &&
		(box.m_min.y >= m_min.y && box.m_max.y <= m_max.y) && 
		(box.m_min.z >= m_min.z && box.m_max.z <= m_max.z) ;
}

bool Bounds::intersects( const Bounds& box ) const
{
	return !(m_min.x > box.m_max.x || m_max.x < box.m_min.x ||
			 m_max.y > box.m_min.y || m_min.y < box.m_max.y ||
			 m_max.z > box.m_min.z || m_min.z < box.m_max.z );
}

void Bounds::transform( const Matrix4& m )
{
	Vector3 min = jam::transform(m,m_min) ;
	Vector3 max = jam::transform(m,m_max) ;
	setMinMax( min, max ) ;
}


/*
Polygon::Polygon()
	: m_points(0), m_numOfPoints(0)
{

}

Polygon::Polygon( int xpoint[], int ypoint[], int nCount )
	: m_points(0), m_numOfPoints(0)
{
	m_points = new Point[nCount] ;

	for(int i=0; i<nCount; i++) {
		m_points[i] = Point( xpoint[i], ypoint[i] ) ;
	}
	m_numOfPoints = nCount ;
}

Polygon::~Polygon()
{
	JAM_DELETEARRAY(m_points) ;
}

Polygon::Polygon( const Polygon& other )
	: m_points(0), m_numOfPoints( other.m_numOfPoints )
{
	m_points = new Point[m_numOfPoints] ;
	memcpy( m_points, other.m_points, sizeof(Point) * other.m_numOfPoints ) ;
}

Polygon& Polygon::operator=( const Polygon& other )
{
	if( this != &other ) {
		JAM_DELETEARRAY(m_points) ;
		
		m_numOfPoints = other.m_numOfPoints ;
		m_points = new Point[m_numOfPoints] ;
		memcpy( m_points, other.m_points, sizeof(Point) * other.m_numOfPoints ) ;

	}

	return *this ;
}

const Point* Polygon::getPoints()
{
	return (const Point*)m_points ;
}

int Polygon::getNumOfPoints()
{
	return m_numOfPoints;
}

void Polygon::translate(int deltaX,int deltaY)
{
	for(int i=0; i<m_numOfPoints; i++) {
		m_points[i].x += deltaX ;
		m_points[i].y += deltaY ;
	}
}
*/

}
