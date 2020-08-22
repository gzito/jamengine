/**********************************************************************************
* 
* Quadtree.h
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

#ifndef __JAM_QUADTREE_H__
#define __JAM_QUADTREE_H__

#include <jam/jam.h>
#include <jam/Node.h>

namespace jam
{



/**
	A QuadTree Object that provides fast and efficient storage of objects in a world space.
	Internal class used to optimize collision detection

	Quadtree maxLevel
	maxLevel - quadnodes
	--------------------
	1        -      1
	2        -      4
	3        -     16
	4        -     64
	5        -    256
	6        -   1024
*/
class JAM_API Quadtree
{
public:
	/** Creates a QuadTree for the specified area. */
							Quadtree(const AABB& rect) ;

	/**
		Creates a QuadTree for the specified area.

		\param x - The top-left position of the area rectangle
		\param y - The top-right position of the area reactangle
		\param width- - The width of the area rectangle
		\param height - The height of the area rectangle
	*/
							Quadtree(int x, int y, int width, int height) ;

							~Quadtree() ;

	/**  The area this QuadTree represents. */
	const AABB&				getQuadRect() const { return m_rect; }
	
	/**  The objects contained in this QuadTree at it's level (ie, excludes children) */
	const jam::NodesList&	getObjects() { return m_objects; }

	/**  The top left child for this QuadTree */
	Quadtree*				getTopLeftChild() const { return m_childTL; }
	
	/**  The top right child for this QuadTree */
	Quadtree*				getTopRightChild() const { return m_childTR; }

	/**  The bottom left child for this QuadTree */
	Quadtree*				getBottomLeftChild() const { return m_childBL; }
	
	/**  The bottom right child for this QuadTree */
	Quadtree*				getBottomRightChild() const { return m_childBR; }
	
	/**  How many total objects are contained within this QuadTree (ie, includes children) */
	size_t					getCount() const { return getObjectCount(); }

	/**  Clears the QuadTree of all objects, including any objects living in its children. */
	void					clear() ;
	
	/**
		Deletes an item from this QuadTree.
		If the object is removed causes this Quad to have no objects in its children, it's children will be removed as well.
	*/
	void					erase( Node* item ) ;

	/**  Insert an item into this QuadTree object. */
	void					insert( Node* item ) ;

	/**  Get the objects in this tree that intersect with the specified rectangle. */
	void					getObjects( const Polygon2f& rect, NodesList& results ) ;

	/**  Get all objects in this Quad, and it's children. */
	void					getAllObjects( NodesList& results ) ;

private:
	void					add( Node* item ) ;
//	void					remove( Node* item ) ;
	size_t					getObjectCount() const ;
	void					subdivide() ;
	Quadtree*				getDestinationTree( Node* item ) ;

	// How many objects can exist in a QuadTree before it sub divides itself
	static const uint32_t	MAX_OBJECTS_PER_NODE = 2;
	NodesList				m_objects ;		// The objects in this QuadTree
	AABB					m_rect ;		// The area this QuadTree represents

	Quadtree*				m_childTL ;		// Top Left Child
	Quadtree*				m_childTR ;		// Top Right Child
	Quadtree*				m_childBL ;		// Bottom Left Child
	Quadtree*				m_childBR ;		// Bottom Right Child

	// to prevent the use
							Quadtree( const Quadtree& ) = delete ;
	Quadtree&				operator=( const Quadtree& ) = delete ;
};

}


#endif // __JAM_QUADTREE_H__
