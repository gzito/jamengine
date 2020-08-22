/**********************************************************************************
* 
* Quadtree.cpp
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

#include <jam/Quadtree.h>
#include <algorithm>

namespace jam
{

Quadtree::Quadtree( const AABB& rect ) :
	m_objects(),
	m_rect(rect),
	m_childTL(0),
	m_childTR(0),
	m_childBL(0),
	m_childBR(0)
{

}

Quadtree::Quadtree( int x, int y, int width, int height ) :
	m_objects(),
	m_rect(),
	m_childTL(0),
	m_childTR(0),
	m_childBL(0),
	m_childBR(0)
{
	m_rect.setBounds( (float)x,(float)y,(float)(x+width),(float)(y-height) ) ;
}


Quadtree::~Quadtree()
{
	JAM_DELETE(m_childTL);
	JAM_DELETE(m_childTR);
	JAM_DELETE(m_childBL);
	JAM_DELETE(m_childBR);
}

void Quadtree::clear()
{
	// Clear out the children, if we have any
	if (m_childTL != 0)
	{
		m_childTL->clear();
		m_childTR->clear();
		m_childBL->clear();
		m_childBR->clear();
	}

	// Clear any objects at this level
	if (m_objects.size())
	{
		m_objects.clear();
	}

	// Set the children to null
	JAM_DELETE(m_childTL) ;
	JAM_DELETE(m_childTR) ;
	JAM_DELETE(m_childBL) ;
	JAM_DELETE(m_childBR) ;
}

void Quadtree::erase( Node* item )
{
	// If this level contains the object, remove it
	bool objectRemoved = false;
	if (m_objects.size() )
	{
		NodesList::iterator it = std::find(m_objects.begin(),m_objects.end(),item) ;
		if( it!=m_objects.end() )
		{
			m_objects.erase(it);
			objectRemoved = true;
		}
	}

	// If we didn't find the object in this tree, try to delete from its children
	if (m_childTL != 0 && !objectRemoved)
	{
		m_childTL->erase(item);
		m_childTR->erase(item);
		m_childBL->erase(item);
		m_childBR->erase(item);
	}

	if (m_childTL != 0)
	{
		// If all the children are empty, delete all the children
		if (m_childTL->getCount() == 0 &&
			m_childTR->getCount() == 0 &&
			m_childBL->getCount() == 0 &&
			m_childBR->getCount() == 0)
		{
			JAM_DELETE(m_childTL);
			JAM_DELETE(m_childTR);
			JAM_DELETE(m_childBL);
			JAM_DELETE(m_childBR);
		}
	}

}

void Quadtree::insert( Node* item )
{
	// If this quad doesn't intersect the items rectangle, do nothing
	const Polygon2f& poly = item->getCollisionOBB() ;

	if (!m_rect.intersects(poly)) {
		return;
	}

	if (m_objects.empty() || 
		(m_childTL == 0 && m_objects.size() + 1 <= MAX_OBJECTS_PER_NODE))
	{
		// If there's room to add the object, just add it
		add(item);
	}
	else
	{
		// No quads, create them and bump objects down where appropriate
		if (m_childTL == 0) {
			subdivide();
		}

		// Find out which tree this object should go in and add it there
		Quadtree* destTree = getDestinationTree(item);
		if (destTree == this) {
			add(item);
		}
		else {
			destTree->insert(item);
		}
	}

}

void Quadtree::getObjects( const Polygon2f& rect, NodesList& results )
{
	if( rect.contains(Polygon2f(m_rect)) ) {
		// If the search area completely contains this quad, just get every object this quad and all it's children have
		getAllObjects(results);
	}
	else if (rect.intersects(Polygon2f(m_rect))) {
		// Otherwise, if the quad isn't fully contained, only add objects that intersect with the search rectangle
		if (m_objects.size()) {
			for (NodesList::iterator it=m_objects.begin(); it!=m_objects.end(); it++ ) {
				const Polygon2f& poly = (*it)->getCollisionOBB() ;

				if (rect.intersects(poly)) {
					results.push_back(*it);
				}
			}
		}

		// Get the objects for the search rectangle from the children
		if (m_childTL != 0) {
			m_childTL->getObjects(rect, results);
			m_childTR->getObjects(rect, results);
			m_childBL->getObjects(rect, results);
			m_childBR->getObjects(rect, results);
		}
	}
}

void Quadtree::getAllObjects( NodesList& results )
{
	// If this Quad has objects, add them
	if( m_objects.size() ) {
		results.insert( results.end(), m_objects.begin(), m_objects.end() ) ;
	}

	// If we have children, get their objects too
	if (m_childTL != 0)	{
		m_childTL->getAllObjects(results);
		m_childTR->getAllObjects(results);
		m_childBL->getAllObjects(results);
		m_childBR->getAllObjects(results);
	}
}

void Quadtree::add( Node* item )
{
	m_objects.push_back(item) ;
}

// void jam::Quadtree::remove( Node* item )
// {
// 	if( m_objects.size() ) {
// 		NodesList::iterator it = std::find(m_objects.begin(), m_objects.end(), item ) ;
// 		if( it != m_objects.end() ) m_objects.erase(it) ;
// 	}
// }

size_t Quadtree::getObjectCount() const
{
	size_t count = 0 ;
	count = m_objects.size() ;

	if( m_childTL != 0 ) {
		count += m_childTL->getObjectCount() ;
		count += m_childTR->getObjectCount() ;
		count += m_childBL->getObjectCount() ;
		count += m_childBR->getObjectCount() ;
	}

	return count ;
}

void Quadtree::subdivide()
{
	Vertex2f size( m_rect.hfWidth, m_rect.hfHeight ) ;
	Vertex2f mid( m_rect.x1 + size.x, m_rect.y1 + size.y ) ;
	
	m_childTL = new Quadtree(AABB(m_rect.x1, m_rect.y1, size.x, size.y));
	m_childTR = new Quadtree(AABB(mid.x, m_rect.y1, size.x, size.y));
	m_childBL = new Quadtree(AABB(m_rect.x1, mid.y, size.x, size.y));
	m_childBR = new Quadtree(AABB(mid.x, mid.y, size.x, size.y));

	// If they're completely contained by the quad, bump objects down
	for( NodesList::iterator it = m_objects.begin(); it != m_objects.end(); it++ )
	{
		Quadtree* destTree = getDestinationTree(*it);

		if (destTree != this) {
			// Insert to the appropriate tree, remove the object, and back up one in the loop
			destTree->insert(*it);
			it = m_objects.erase(it) ;
		}
	}
}

Quadtree* Quadtree::getDestinationTree( Node* item )
{
	// If a child can't contain an object, it will live in this Quad
	Quadtree* destTree = this;

	const Polygon2f& poly = item->getCollisionOBB() ;

	if( m_childTL->getQuadRect().contains(poly) ) {
		destTree = m_childTL;
	}
	else if( m_childTR->getQuadRect().contains(poly) ) {
		destTree = m_childTR;
	}
	else if( m_childBL->getQuadRect().contains(poly) ) {
		destTree = m_childBL;
	}
	else if( m_childBR->getQuadRect().contains(poly) ) {
		destTree = m_childBR;
	}

	return destTree;
}

} // namespace jam
