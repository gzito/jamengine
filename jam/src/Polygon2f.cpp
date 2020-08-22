/**********************************************************************************
* 
* Polygon2f.cpp
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
#include "jam/Polygon2f.h"

#include <jam/core/bmkextras.hpp>
#include <jam/core/math.h>
#include <jam/core/geom.h>

#include <math.h>

#define JAM_POLYGON2D_ORDERCW

namespace jam
{

	Polygon2f::Polygon2f() : m_count(0)
	{
	}


	// have to specify points in clockwise order
	Polygon2f::Polygon2f( Vector2* points, int count ) : m_count(count)
	{
		for(int i = 0; i < m_count; i++) {
			m_vertices[i] = points[i] ;
		}
	}

	// some of the above but with std::vector
	Polygon2f::Polygon2f( const std::vector<Vector2>& points ) : m_count((int)points.size())
	{
		for(int i = 0; i < m_count; i++) {
			m_vertices[i] = points[i] ;
		}
	}

	Polygon2f::Polygon2f( float radius, int count ) : m_count(count)
	{
		m_count = count;

		for(int i = 0; i < m_count; i++)
		{
#ifdef JAM_POLYGON2D_ORDERCW
			float a = -(2.0f * ((float)JAM_PI) * (i / (float) m_count));
#else
			float a = 2.0f * ((float)JAM_PI) * (i / (float) m_count);
#endif
			m_vertices[i] = Vector2(cosf(a), sinf(a)) * radius;
		}
	}

	Polygon2f::Polygon2f( const AABB& aabb )
	{
		m_count = 4 ;
		m_vertices[0] = Vector2( aabb.x1, aabb.y1 ) ;
		m_vertices[1] = Vector2( aabb.x2, aabb.y1 ) ;
		m_vertices[2] = Vector2( aabb.x2, aabb.y2 ) ;
		m_vertices[3] = Vector2( aabb.x1, aabb.y2 ) ;
	}

	Polygon2f::Polygon2f( const Polygon2f& other )
	{
		*this = other ;
	}

	Polygon2f& Polygon2f::operator=( const Polygon2f& other )
	{
		if( this != &other ) {
			m_count = other.m_count ;
			for(int i=0; i<m_count; i++) {
				m_vertices[i] = other.m_vertices[i] ;
			}
		}

		return *this ;
	}

	const Vector2& Polygon2f::getVertex( int32_t idx ) const
	{
		JAM_ASSERT_MSG( (idx>=0 && idx<m_count || m_count==0), "Index out of bounds: %d", idx ) ;
		return m_vertices[idx];
	}

	void Polygon2f::transform(const Matrix3& m)
	{
		for(int i = 0; i < m_count; i++) {
			m_vertices[i] = jam::transform(m,m_vertices[i]);
		}
	}

	void Polygon2f::scale( float scaleFactor )
	{
		Matrix3 scaleMat = jam::createScaleMatrix2D( Vector2(scaleFactor) ) ;
		transform(scaleMat);
	}

	bool Polygon2f::intersects(const Polygon2f& poly) const
	{
		if( m_count==0 || poly.m_count==0 ) {
			return false ;
		}

		// test separation axes of current polygon
		for(int j = m_count-1, i = 0; i < m_count; j = i, i ++)
		{
			Vector2 v0 = m_vertices[j];
			Vector2 v1 = m_vertices[i];

			Vector2 edge = v1 - v0; // edge
			Vector2 axis = Vector2(-edge.y, edge.x) ;	// sep axis is perpendicular at the edge
															// the normal of a vector (x,y) in 2D is (-y,x) or (y,-x).

			if(separatedByAxis(axis, poly))
				return false;
		}

		// test separation axes of other polygon
		for(int j = poly.m_count-1, i = 0; i < poly.m_count; j = i, i ++)
		{
			Vector2 v0 = poly.m_vertices[j];
			Vector2 v1 = poly.m_vertices[i];

			Vector2 edge = v1 - v0; // edge
			Vector2 axis = Vector2(-edge.y, edge.x) ;	// sep axis is perpendicular at the edge
															// the normal of a vector (x,y) in 2D is (-y,x) or (y,-x).

			if(separatedByAxis(axis, poly))
				return false;
		}
		return true;
	}

	bool Polygon2f::intersects( const Vector2& p0, const Vector2& p1 )
	{
		// for algorithm see: http://softsurfer.com/algorithm_archive.htm

		float tE = 0.0f ;		// the maximum entering segment parameter
		float tL = 1.0f ;		// the minimum leaving segment parameter

		Vector2 dS = p1 - p0 ;	// the segment direction vector

		for( int i=0; i<getCount(); i++) {
			Vector2 n = getOutwardEdgeNormal(i+1) ;
			float N = -(glm::dot( (p0-m_vertices[i]), n ) ) ;
			float D = glm::dot( dS, n ) ;
			if( D == 0 ) {
				if( N < 0 ) {
					return false ;
				}
				else {
					continue;
				}
			}

			float t = N / D ;

			if( D < 0 ) {
				tE = Max(t,tE) ;
				if( tE > tL ) return false ;
			}
			else {
				tL = Min(t,tL) ;
				if( tL < tE ) return false ;
			}
		}

		// tE <= tL implies that there is a valid intersection subsegment

		// entering point: P(tE) = P0 + tE * dS
		// leaving point:  P(tL) = P0 + tL * dS

		return true ;
	}


	bool Polygon2f::contains( const Polygon2f& poly ) const
	{
		for( int i=0; i<poly.getCount(); i++ ) {
			if( !isPointInside( poly.getVertex(i)) ) return false ;
		}

		return true ;
	}

	bool Polygon2f::isPointInsideQuad( const Vector2& v ) const
	{
		return  ( (m_vertices[1].y-m_vertices[0].y)*(v.x-m_vertices[1].x) - (m_vertices[1].x-m_vertices[0].x)*(v.y-m_vertices[1].y) >=0 ) &&
			( (m_vertices[2].y-m_vertices[1].y)*(v.x-m_vertices[2].x) - (m_vertices[2].x-m_vertices[1].x)*(v.y-m_vertices[2].y) >=0 ) &&
			( (m_vertices[3].y-m_vertices[2].y)*(v.x-m_vertices[3].x) - (m_vertices[3].x-m_vertices[2].x)*(v.y-m_vertices[3].y) >=0 ) &&
			( (m_vertices[0].y-m_vertices[3].y)*(v.x-m_vertices[0].x) - (m_vertices[0].x-m_vertices[3].x)*(v.y-m_vertices[0].y) >=0 ) ;

	}


	bool Polygon2f::isPointInside( const Vector2& v ) const
	{
		for(int i = 0; i < m_count-1; i++)	{
			if( (m_vertices[i+1].y-m_vertices[i].y)*(v.x-m_vertices[i+1].x) - (m_vertices[i+1].x-m_vertices[i].x)*(v.y-m_vertices[i+1].y) < 0 ) {
				return false ;
			}
		}

		return	(m_vertices[0].y-m_vertices[m_count-1].y)*(v.x-m_vertices[0].x) - (m_vertices[0].x-m_vertices[m_count-1].x)*(v.y-m_vertices[0].y) >= 0 ;
	}


	bool Polygon2f::isPointOutside( const Vector2& v, uint32_t& edgeNumber ) const
	{
		for(int i = 0; i < m_count-1; i++)	{
			if( (m_vertices[i+1].y-m_vertices[i].y)*(v.x-m_vertices[i+1].x) - (m_vertices[i+1].x-m_vertices[i].x)*(v.y-m_vertices[i+1].y) < 0 ) {
				edgeNumber = i+1 ;
				return true ;
			}
		}

		if( (m_vertices[0].y-m_vertices[m_count-1].y)*(v.x-m_vertices[0].x) - (m_vertices[0].x-m_vertices[m_count-1].x)*(v.y-m_vertices[0].y) < 0 ) {
			edgeNumber = m_count ;
			return true ;
		}

		edgeNumber = 0 ;
		return false ;
	}


	Vector2 Polygon2f::getInwardEdgeNormal( uint32_t edgeNumber ) const
	{
		JAM_ASSERT_MSG( (edgeNumber>=1 && edgeNumber<=(uint32_t)m_count), "Index out of bounds: %d", edgeNumber ) ;

		uint32_t startVertex = edgeNumber-1 ;
		uint32_t endVertex = (edgeNumber != (uint32_t)m_count) ? edgeNumber : 0;

		Vector2 side = m_vertices[endVertex]-m_vertices[startVertex] ;

		// Polygon CW  (x,y) -> inward normal -> ( y, -x)
		// Polygon CWW (x.y) -> inward normal -> (-y,  x)
#ifdef JAM_POLYGON2D_ORDERCW
		return glm::normalize( Vector2( side.y, -side.x ) ) ;
#else
		return Vector2( -side.y, side.x ).getNormalized() ;
#endif
	}


	Vector2 Polygon2f::getOutwardEdgeNormal( uint32_t edgeNumber ) const
	{
		JAM_ASSERT_MSG( (edgeNumber>=1 && edgeNumber<=(uint32_t)m_count), "Index out of bounds: %d", edgeNumber ) ;

		uint32_t startVertex = edgeNumber-1 ;
		uint32_t endVertex = (edgeNumber != (uint32_t)m_count) ? edgeNumber : 0;

		Vector2 side = m_vertices[endVertex]-m_vertices[startVertex] ;

		// Polygon CW  (x,y) -> outward normal -> (-y,  x)
		// Polygon CWW (x.y) -> outward normal -> ( y, -x)
#ifdef JAM_POLYGON2D_ORDERCW
		return glm::normalize(Vector2( -side.y, side.x )) ;
#else
		return glm::normalize(Vector2( side.y, -side.x )) ;
#endif
	}

	void Polygon2f::calculateInterval(const Vector2& axis, float& min, float& max) const
	{
		min = max = glm::dot(m_vertices[0],axis);

		for(int i = 1; i < m_count; i ++)
		{
			float d = glm::dot(m_vertices[i],axis);
			if (d < min) 
				min = d; 
			else if (d > max) 
				max = d;
		}
	}

	bool Polygon2f::intervalsSeparated(float mina, float maxa, float minb, float maxb) const
	{
		return (mina > maxb) || (minb > maxa);
	}

	bool Polygon2f::separatedByAxis(const Vector2& axis, const Polygon2f& poly) const
	{
		float mina, maxa;
		float minb, maxb;

		calculateInterval(axis, mina, maxa);

		poly.calculateInterval(axis, minb, maxb);

		return intervalsSeparated(mina, maxa, minb, maxb);
	}

	bool clockwise( const Vector2 &O, const Vector2 &A, const Vector2 &B )
	{
		return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x) <= 0; 
	}

	bool glm_vec2_less( const Vector2& v1, const Vector2& v2 )
	{
		return glm::all(glm::lessThan(v1,v2)) ;
	}

	std::vector<Vector2> convex_hull( std::vector <Vector2> P, bool clockwiseOrder/*=true*/ )
	{
		int n = (int)P.size (), // size of P
			k = 0; // practically size of K
		std::vector <Vector2> H (2 * n);

		// Sort points lexicographically
		std::sort (P.begin (), P.end (), glm_vec2_less);

		// Build lower hull
		for (int i = 0; i < n; i ++) {
			while (k >= 2 && clockwise (H [k - 2], H [k - 1], P [i])) k --;
			H [k ++] = P [i];
		};

		// Build upper hull
		for (int i = n-2, t = k + 1; i >= 0; i --) {
			while (k >= t && clockwise (H [k - 2], H [k - 1], P [i])) k --;
			H [k ++] = P [i];
		};

		H.resize (k);
		if (clockwiseOrder)  std::reverse(H.begin(), H.end());		
		return H;
	}

std::vector<Vector2> convex_hull_findTopMost( std::vector <Vector2> P)
{
 		int n = (int)P.size ();			
 		std::vector <Vector2> H(n);
		Vector2 max=P[0];
		int iMax=0;
		for (int i = 1; i < n; i ++) 
		{
			if ( glm::all( glm::lessThan(P[i], max) ) ) {max=P[i]; iMax=i;}
		} 		
		 int j=0;
		for (int i=iMax; i<n; i++, j++)
			H[j]=P[i];

		for (int i=0; i<iMax; i++, j++)
			H[j]=P[i];

 		return H;
 	}


 
/** METHODS TO CALCULATE THE AREA AND CENTROID OF A POLYGON
    INSERT THEM INTO THE CORRESPONDING CLASS **/
double  Polygon2f::getArea()
{	
	int i,j;
	double area = 0;

	for (i=0;i<m_count;i++) {
		j = (i + 1) % m_count;
		area += m_vertices[i].x * m_vertices[j].y;
		area -= m_vertices[i].y * m_vertices[j].x;
	}
	area /= 2.0;

   return(area);
	//return(area < 0 ? -area : area); for unsigned
}
 
/* CENTROID */

Vector2  Polygon2f::getCenter()
{
	float cx=0,cy=0;
	float A=(float)getArea();
	Vector2 res;
	int i,j;

	float factor=0;
	for (i=0;i<m_count;i++) 
	{
		j = (i + 1) % m_count;
		factor=(m_vertices[i].x*m_vertices[j].y-m_vertices[j].x*m_vertices[i].y);
		cx+=(m_vertices[i].x+m_vertices[j].x)*factor;
		cy+=(m_vertices[i].y+m_vertices[j].y)*factor;
	}
	//A*=6.0f; 
	A*=m_count;
	factor=1/A;
	cx*=factor;	cy*=factor;
	res.x=cx; res.y=cy;

	return res;
}


/*
double PolygonArea()
{
	int i,j;
	double area = 0;

	for (i=0;i<m_count;i++) {
		j = (i + 1) % m_count;
		area += m_vertices[i].x * m_vertices[j].y;
		area -= m_vertices[i].y * m_vertices[j].x;
	}

	area /= 2;
	return(area < 0 ? -area : area);
}
*/



}