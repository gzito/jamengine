/**********************************************************************************
* 
* Poligon2f.h
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

#ifndef __JAM_POLYGON2F_H__
#define __JAM_POLYGON2F_H__

#include <jam/jam.h>
#include <jam/Draw2d.h>
#include <jam/core/geom.h>

#include <vector>
#include <algorithm>

#define JAM_POLYGON2D_MAX_VERTICES		32
		

namespace jam
{

/**
This class represents a polygon in 2D
*/
class JAM_API Polygon2f
{
public:
	/** Constructs an empty polygon */
							Polygon2f();

	/**
		Constructs a polygon using the given vertices.
		\remark Vertices have to be clockwise ordered
	*/
							Polygon2f(Vector2* points, int count);

	/**
		Constructs a polygon using the given vector of vertices.
		\remark Vertices have to be clockwise ordered
	*/
							Polygon2f(const std::vector<Vector2>& points);

	/** Constructs an equilateral polygon given the enclosing circle centered at (0,0). */
							Polygon2f(float radius, int count);

	/** Constructs a polygon from axis-aligned bounding box.  */
	explicit				Polygon2f( const AABB& aabb ) ;

	/** Copy constructor. */
							Polygon2f( const Polygon2f& other ) ;

	/** Assignment operator. */
	Polygon2f&				operator=( const Polygon2f& other ) ;

	/** Returns the number of vertices. */
	int						getCount() const { return m_count; }
	
	/**
		Returns the vertex at given index
		\remark Index starts from 0 to the number of vertex-1
	*/
	const Vector2&			getVertex(int32_t idx) const;

	/** Transform the polygon by the given matrix. */
	void					transform(const Matrix3& m);

	/** Scales the polygon uniformly */
	void					scale( float m_collisionScaleFactor );

	/** Returns true if this polygon intersects the given polygon. */
	bool					intersects(const Polygon2f& poly) const;

	/** Returns true if this polygon intersects the given segment. */
	bool					intersects(const Vector2& p0, const Vector2& p1) ;

	/**
		Returns true if this polygon entirely contains the given polygon.
		\remark This method if slower than intersects
	*/
	bool					contains(const Polygon2f& poly) const;

	/** Returns true if the given point in inside this polygon. */
	bool					isPointInside(const Vector2& v) const ;

	/**
		Returns true if the given point in outside this polygon, also returning the nearest edge number.
		\remark Edge number starts from 1 to the number of edges
	*/
	bool					isPointOutside( const Vector2& v, uint32_t& edgeNumber ) const ;

	/**
		Returns the inward normal of given polygon edge number
		\sa isPointOutside
	*/
	Vector2					getInwardEdgeNormal( uint32_t edgeNumber ) const ;

	/**
		Returns the outward normal of given polygon edge number
		\sa isPointOutside
	*/
	Vector2					getOutwardEdgeNormal( uint32_t edgeNumber ) const ;

	/**
		Returns true if the given point in inside this polygon.
		\remark This is an optimized version of isPointInside, to be used only if the polygon has 4 vertices
	*/
	bool					isPointInsideQuad(const Vector2& v) const ;


	double					getArea();
	Vector2					getCenter();

protected:
	Vector2					m_vertices[JAM_POLYGON2D_MAX_VERTICES];
	int						m_count;

private:
	// collision functions
	void					calculateInterval(const Vector2& axis, float& min, float& max) const;
	bool					intervalsSeparated(float mina, float maxa, float minb, float maxb) const;
	bool					separatedByAxis(const Vector2& axis, const Polygon2f& poly) const;
};



// 2D cross product of OA and OB vectors, i.e. z-component of their 3D cross product.
// Returns true for clockwise turn or the points are collinear, false another.
bool clockwise (const Vector2 &O, const Vector2 &A, const Vector2 &B) ;

// Returns a list of points on the convex hull in counter-clockwise order.
// NOTE: the last point in the returned list is the same as the first one.
std::vector<Vector2> convex_hull (std::vector <Vector2> P, bool clockwiseOrder=true) ;
std::vector<Vector2> convex_hull_findTopMost( std::vector <Vector2> P);

}
#endif // __JAM_POLYGON2F_H__
