/**********************************************************************************
* 
* CollisionManager.cpp
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

#include "jam/CollisionManager.h"
#include "jam/Application.h"
#include "jam/Scene.h"
#include "jam/Timer.h"
#include "jam/core/bmkextras.hpp"

#ifndef JAM_CM_QUADTREE_DISABLED
#include <jam/Quadtree.h>
#include <jam/Draw3dManager.h>
#endif

using namespace std;

namespace jam
{

CollisionManager::CollisionManager() :
	m_maxSimultaneousColls(COLLISION_MANAGER_MAX_COLLISIONS),
	m_maxCollsType(JAM_CM_MAX_COLL_TYPES),
	m_isOptimized(false)
#ifndef JAM_CM_QUADTREE_DISABLED
	, m_quadTree(0)
#endif
#ifdef JAM_TRACE_COLLISIONS
	,m_numOfObjects(0)
	,m_numOfCollidedPairs(0)
	,m_numOfCheckedPairs(0)
#endif

{
#ifndef JAM_CM_QUADTREE_DISABLED
	m_quadTree = new Quadtree( -Draw3DManager::HalfOriginal3DWidth, Draw3DManager::HalfOriginal3DHeight, Draw3DManager::Original3DWidth, Draw3DManager::Original3DHeight ) ;
#endif

	m_pUpdateTimer = Timer::create() ;
	m_pUpdateTimer->setReserved() ;
}


CollisionManager::~CollisionManager()
{
	for( unsigned int i=0; i < m_usedColls.size(); i++ ){
		JAM_DELETE( m_usedColls[i] ) ;
	}
	for( unsigned int i=0; i < m_freeColls.size(); i++ ){
		JAM_DELETE( m_freeColls[i] ) ;
	}

#ifndef JAM_CM_QUADTREE_DISABLED
	JAM_DELETE(m_quadTree) ;
#endif

	JAM_RELEASE_NULL(m_pUpdateTimer) ;
}


void CollisionManager::clearCollisions()
{
	for( int k=0; k<JAM_CM_MAX_COLL_TYPES; k++ ){
		m_collInfo[k].clear();
	}
}


void CollisionManager::setCollisions( int src_type,int dest_type,Method method,int response )
{
	JAM_ASSERT_MSG( src_type<Min(JAM_CM_MAX_COLL_TYPES,m_maxCollsType), "src_type must be less than %d",Min(JAM_CM_MAX_COLL_TYPES,m_maxCollsType) ) ;
	JAM_ASSERT_MSG( dest_type<Min(JAM_CM_MAX_COLL_TYPES,m_maxCollsType), "dest_type must be less than %d",Min(JAM_CM_MAX_COLL_TYPES,m_maxCollsType) ) ;

	vector<CollInfo> &info=m_collInfo[src_type];

	// already present?
	for( unsigned int k=0 ;k<info.size(); k++ ) {
		const CollInfo& t = info[k];
		if( dest_type == t.dst_type ) return;
	}

	CollInfo co={dest_type,method,response};
	info.push_back(co);
}


void CollisionManager::update( float elapsed )
{
	// collect previusly allocated ObjCollision from used_colls and store them in free_colls for later use
	for( ;m_usedColls.size();m_usedColls.pop_back() ){
		m_freeColls.push_back( m_usedColls.back() );
	}

	// get enabled nodes
	m_enabled.clear();
	const NodesList& children = GetAppMgr().getScene()->getChildren() ;
	for( NodesList::const_iterator childIt=children.begin(); childIt!=children.end(); childIt++ ){
		(*childIt)->enumEnabled( m_enabled );
	}

	// select nodes to be tested for collision detection
	vector<Node*>::const_iterator it;

	int collType = 0 ;
#ifdef JAM_TRACE_COLLISIONS
	m_numOfCheckedPairs = 0 ;
	m_numOfCollidedPairs = 0 ;
	m_numOfObjects = 0 ;
#endif
	for( it = m_enabled.begin(); it != m_enabled.end(); it++ ){
		Node *n = *it;
		collType = n->getCollisionType() ;
		if( collType != 0 ) {
			n->clearCollisions();
			m_objsByType[collType].push_back(n);
#ifndef JAM_CM_QUADTREE_DISABLED
			m_quadTree->insert(n) ;			// insert the node in the quadtree
#endif
		}
	}

	// for each enabled object check if it collide
	for( it = m_enabled.begin(); it != m_enabled.end(); it++ ) {
		Node *n = *it;
		if( n->getCollisionType() ) {
#ifdef JAM_TRACE_COLLISIONS
			m_numOfObjects++ ;
#endif
			checkCollisions( n );
		}
	}

	for( int k=0; k<JAM_CM_MAX_COLL_TYPES; k++ ){
		m_objsByType[k].clear();
	}

	m_checked.clear() ;

#ifndef JAM_CM_QUADTREE_DISABLED
	m_quadTree->clear() ;	// clear quadtree
#endif

#ifdef JAM_TRACE_COLLISIONS
	JAM_TRACE( ("m_numOfObjects: %d",m_numOfObjects) ) ;
	JAM_TRACE( ("m_numOfCheckedPairs: %d",m_numOfCheckedPairs) ) ;
	JAM_TRACE( ("m_numOfCollidedPairs: %d",m_numOfCollidedPairs) ) ;
#endif
}


bool CollisionManager::hitTest( Node* src, Node* dst, Method method )
{
	// virtual call, collide() can be overriden
	return src->collide( dst ,method ) ;
}


void CollisionManager::collided( Node* src, Node* dest )
{
#ifdef JAM_TRACE_COLLISIONS
	m_numOfCollidedPairs++ ;
#endif

	ObjCollision *c = 0 ;

	c=allocObjColl(dest) ;
	src->addCollision( c );

	c=allocObjColl(src) ;
	dest->addCollision( c );

	CollisionEventArgs* evtArgs = CollisionEventArgs::create(src,dest) ;
	src->getCollisionEvent().enqueue(evtArgs,this) ;
}


ObjCollision* CollisionManager::allocObjColl( Node* with )
{
	ObjCollision *c;
	// check if there is some ObjCollision in free_colls and use it, otherwise allocates a new object
	if( m_freeColls.size() ) {
		c=m_freeColls.back();
		m_freeColls.pop_back();
	}
	else {
		c=new ObjCollision();
	}
	m_usedColls.push_back( c );
	c->with=with;
	return c;
}


void CollisionManager::checkCollisions( Node* src )
{
	//if (!src->canCollide()) return;	// ***GS: src Collisions are in pause

#ifndef JAM_CM_QUADTREE_DISABLED
	NodesList objLst ;
	m_quadTree->getObjects( src->getCollisionOBB(),objLst) ;
	if( objLst.empty() ) {
		return ;
	}
#endif

	// gets the list of dest entity-types to be checked with source entity 
	const vector<CollInfo>& collinfos=m_collInfo[src->getCollisionType()];

	Node* coll_obj = 0;
	vector<CollInfo>::const_iterator coll_it,coll_info;

	int numOfColls = 0; 

	// for each dest coll-types
	for( coll_it = collinfos.begin(); coll_it!=collinfos.end(); coll_it++ ){

		vector<Node*>::const_iterator dst_it;

		// get the list of objects of coll-type "dest_type"
		const vector<Node*>& dst_objs = m_objsByType[coll_it->dst_type];

		// perform hit-test for all objects (of coll-type "dest_type") in the list
		for( dst_it = dst_objs.begin(); dst_it != dst_objs.end(); dst_it++ ) {

			Node* dst = *dst_it;

#ifndef JAM_CM_QUADTREE_DISABLED
			NodesList::iterator it = std::find(objLst.begin(),objLst.end(),dst) ;
			if( it == objLst.end() ) {
				continue;
			}
#endif

//			IwDebugTraceLinePrintf( "Checking %s with %s",src->getName().c_str(), dst->getName().c_str() );

			if( src == dst ) { 
//				IwDebugTraceLinePrintf( "Skip" );
				continue;
			}

		//	if (!dst->canCollide()) { continue; } // ***GS: dst Collisions are in pause

			m_checked[src].insert(dst) ;

			set<Node*>& checkedWithDst = m_checked[dst] ;
			if( checkedWithDst.find(src) == checkedWithDst.end() ) {
#ifdef JAM_TRACE_COLLISIONS
				m_numOfCheckedPairs++ ;
#endif
				if( hitTest(src,dst,coll_it->method) ) {
					//					IwDebugTraceLinePrintf("collision detected") ;
					coll_obj=dst;
					coll_info=coll_it;
					numOfColls++ ;
					if( !m_isOptimized ) {
						if( numOfColls < m_maxSimultaneousColls )
							collided(src,coll_obj);
						else
							break ;
					}
				}
				//else {
				//	IwDebugTraceLinePrintf("no collision detected") ;
				//}
			}
			//else {
			//	IwDebugTraceLinePrintf( "Already checked" );
			//}
		}
	}

	if( numOfColls > 0 && m_isOptimized ) {
		collided(src,coll_obj);
	}

}

void CollisionManager::setRegionBounds( const AABB& aabb )
{
#ifndef JAM_CM_QUADTREE_DISABLED
	JAM_DELETE(m_quadTree) ;
	m_quadTree = new Quadtree( aabb ) ;
#endif
}

} // namespace jam
