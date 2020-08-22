/**********************************************************************************
* 
* Draw2d.h
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

#ifndef __JAM_DRAW2D_H__
#define __JAM_DRAW2D_H__

#include <jam/jam.h>
#include <jam/Color.h>
#include <jam/core/bmkextras.hpp>
#include <jam/core/geom.h>

namespace jam
{

// forward declarations
class Polygon2f ;

/** Axis-aligned bounding box */
class JAM_API AABB
{
public:

	/** Default constructor, returns an empty AABB */
							AABB();

	/** Constructs an AABB starting from its center and its extent */
							AABB( float cx, float cy, float halfWidth, float halfHeight );

	/**
		Constructs an AABB given its main diagonal coordinates.
		\remark The main diagonal have to be defined to start from top/left and ending to bottom/right
	*/
	void					setBounds( float px1, float py1, float px2, float py2 );

	/** Returns true if the specified point is inside this AABB. */
	bool					contains( const Vector2& v ) const ;

	/** Returns true if this AABB entirely contains the specified AABB. */
	bool					contains( const AABB& box ) const ;

	/** Returns true if this AABB intersects the specified AABB. */
	bool					intersects( const AABB& box ) const ;

	/** Returns true if this AABB intersects the specified polygon. */
	bool					intersects( const Polygon2f& poly ) const ;

	/** Returns true if this AABB entirely contains the specified polygon. */
	bool					contains( const Polygon2f& poly ) const ;

	/** Transform the AABB by the given matrix. */
	void					transform(const Matrix3& m);

	float					x1, y1, x2, y2 ;
	float					width, height ;
	float					hfWidth, hfHeight ;
};



/**	This class represent a 2D point (pixel) */
class JAM_API Point
{
public:
	/** Default constructor */
							Point();

	/**
		Constructor.
		\param xp Specifies the x-coordinate of the point
		\param yp Specifies the y-coordinate of the point
	*/
							Point(int32_t xp,int32_t yp ) ;

	int32_t					x ;
	int32_t					y ;
};


/**	This class represents a 2D rectangle */
class JAM_API Rect
{
public:
	/** Default constructor */
							Rect() ;

	/*!
		Constructor
		\param l - Specifies the x-coordinate of the upper-left corner of the rectangle
		\param t - Specifies the y-coordinate of the upper-left corner of the rectangle
		\param r - Specifies the x-coordinate of the lower-right corner of the rectangle
		\param b - Specifies the y-coordinate of the lower-right corner of the rectangle
	*/
							Rect( int32_t l, int32_t t, int32_t r, int32_t b ) ;

	int32_t					getWidth() const ;
	int32_t					getHeight() const ; 
	void					setBounds(int32_t x, int32_t y, int32_t width, int32_t height) ;
	void					grow(int32_t h,int32_t v) ;
	bool					isEmpty() const ;
	bool					hasSameSize( const Rect& other ) ;
	void					scale(float s) ;
	bool					contains(const Point& point) ;
	bool					contains(int32_t x, int32_t y) ;

	bool					operator==( const Rect& other ) ;
	
	int32_t					left ;
	int32_t					top ;
	int32_t					right ;
	int32_t					bottom ;
};


struct GridSize
{
							GridSize() : x(0), y(0) {}
							GridSize(int i,int j) : x(i), y(j) {}

	int32_t					x ;
	int32_t					y ;
};

struct JAM_API Vertex2f
{
							Vertex2f() : x(0.0f), y(0.0f) {}
							Vertex2f(float i,float j) : x(i), y(j) {}

	float					x ;
	float					y ;
};

struct JAM_API Vertex3f
{
							Vertex3f() : x(0.0f), y(0.0f), z(0.0f) {}
							Vertex3f(float i,float j, float k) : x(i), y(j), z(k) {}

	float					x ;
	float					y ;
	float					z ;
};

//!	A 3D Quad. 4 * 3 floats
struct JAM_API Quad3f
{
	Vertex3f				bl;
	Vertex3f				br;
	Vertex3f				tl;
	Vertex3f				tr;
};


struct JAM_API V3F_C4B_T2F
{
	Vertex3f				vertex ;
	Color					color ;
	Vertex2f				texCoords ;
};

struct JAM_API V3F_C4B_T2F_Quad
{
	V3F_C4B_T2F				tl ;
	V3F_C4B_T2F				bl ;
	V3F_C4B_T2F				tr ;
	V3F_C4B_T2F				br ;
};

struct JAM_API V3F_N3F_T2F
{
	Vector3					vertex;
	Vector3					normal ;
	Vector2					texCoords ;
};

struct JAM_API V3F_N3F_C4B_T2F
{
	Vector3					vertex;
	Vector3					normal ;
	Color					color ;
	Vector2					texCoords ;
};

/** Axis-aligned bounding box */
class JAM_API Bounds
{
public:

	/** Default constructor, returns an empty AABB */
							Bounds();

	/** Constructs an AABB starting from its center and its extent */
							Bounds( const Vector3& center, const Vector3& extents );

	/**
		Constructs an AABB given its min/max coordinates.
	*/
	void					setMinMax( const Vector3& min, const Vector3& max );

	/** Returns true if the specified point is inside this AABB. */
	bool					contains( const Vector3& v ) const ;

	/** Returns true if this AABB entirely contains the specified AABB. */
	bool					contains( const Bounds& box ) const ;

	/** Returns true if this AABB intersects the specified AABB. */
	bool					intersects( const Bounds& box ) const ;

	/** Transform the AABB by the given matrix. */
	void					transform(const Matrix4& m);

	Vector3					getCenter() const ;
	Vector3					getExtents() const ;
	Vector3					getMin() const ;
	Vector3					getMax() const ;
	Vector3					getSize() const ;

private:
	Vector3					m_min ;
	Vector3					m_max ;
	Vector3					m_size ;
	Vector3					m_center ;
	Vector3					m_extents ;
};
JAM_INLINE Vector3 Bounds::getCenter() const {
	return m_center ;
}

JAM_INLINE Vector3 Bounds::getExtents() const {
	return m_extents ;
}

JAM_INLINE Vector3 Bounds::getMin() const {
	return m_min ;
}

JAM_INLINE Vector3 Bounds::getMax() const {
	return m_max ;
}

JAM_INLINE Vector3 Bounds::getSize() const {
	return m_size ;
}


/*
class JAM_API Polygon
{
public:
							Polygon();
							Polygon( int xpoint[], int ypoint[], int nCount ) ;

							Polygon( const Polygon& ) ;
	Polygon&				operator=( const Polygon& ) ;

							~Polygon() ;

	const Point*			getPoints();
	int						getNumOfPoints();
	void					translate(int deltaX,int deltaY);

private:
	Point*					m_points ;
	int						m_numOfPoints;
};
*/

}	

#endif // __JAM_DRAW2D_H__
