/**********************************************************************************
* 
* CollisionManager.h
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

#ifndef __JAM_COLLISIONMANAGER_H__
#define __JAM_COLLISIONMANAGER_H__

#include <jam/jam.h>

#include <jam/Singleton.h>
#include <jam/RefCountedObject.h>

#include <vector>
#include <map>
#include <set>
#define COLLISION_MANAGER_DEFAULT_SCALE_FACTOR			1.0f
#define COLLISION_MANAGER_MAX_COLLISIONS					10


namespace jam
{

	// fw reference 
	class Node ;
	class Quadtree ;
	class AABB ;
	class Timer ;
	struct ObjCollision ;

/**
* Collision types are just numbers you assign to a node using setCollisionType.
* This class then uses the collision types to check for collisions between
* all the nodes that have those collision types.
*/
class JAM_API CollisionManager : public Singleton<CollisionManager>, public RefCountedObject
{
	friend class Singleton<CollisionManager> ;

public:
	enum class Method {
		BoundingSphere,
		BoundingBox,
		PerPixel
	};

	/**
	* Clears the collision information list
	*
	* Whenever you use the setCollisions command to enable collisions between two different collision types,
	* Information is added to the collision list. This command clears that list, so that no collisions
	* will be detected until the setCollisions command is used again.
	*/
	void					clearCollisions();

	/// Enables collisions between two different collision types.
	void					setCollisions( int src_type,int dest_type,Method method,int response );
	
	/// Called by Application class. Normally you shouldn't call this method
	void					update( float elapsed );
	
	bool					getOptimized() const { return m_isOptimized; }
	void					setOptimized(bool val) { m_isOptimized = val; }

	void					setRegionBounds( const AABB& aabb) ;

	Timer&					getUpdateTimer() { return *m_pUpdateTimer; }
	const Timer&			getUpdateTimer() const { return *m_pUpdateTimer; }

	int						getMaxSimultaneousColls() const { return m_maxSimultaneousColls; }
	void					setMaxSimultaneousColls(int val=COLLISION_MANAGER_MAX_COLLISIONS) { m_maxSimultaneousColls = val; }
	int						getMaxCollsType() const { return m_maxCollsType; }
	void					setMaxCollsType(int val=JAM_CM_MAX_COLL_TYPES) { m_maxCollsType = val; }
#ifdef JAM_TRACE_COLLISIONS
	int						getNumOfCollidedPairs() const { return m_numOfCollidedPairs; }
	int						getNumOfCheckedPairs() const { return m_numOfCheckedPairs; }
	int						getNumOfObjects() const { return m_numOfObjects; } 
#endif

private:
#ifdef JAM_TRACE_COLLISIONS
	// coppie confrontate in un frame
	int						m_numOfCollidedPairs ;
	// coppie che hanno colliso in un frame
	int						m_numOfCheckedPairs ;
	// numero di oggetti attivi in un frame per cui sono attivate le collisioni
	int						m_numOfObjects ;
#endif

	struct CollInfo {
		int dst_type ;
		Method method ;
		int response;
	};

	// number of simoultaneous collisions detected
	int						m_maxSimultaneousColls;
	
	// number of detectable collisions type, must be <= JAM_CM_MAX_COLL_TYPES
	int						m_maxCollsType;
	
	// for each coll_type there are one or more CollInfo
	std::vector<CollInfo>	m_collInfo[JAM_CM_MAX_COLL_TYPES];

	// for each coll_type there are one or more nodes
	std::vector<Node*>		m_objsByType[JAM_CM_MAX_COLL_TYPES];

	// list of enabled nodes
	std::vector<Node*>		m_enabled ;

	// temporary collections that keep objects allocated by allocObjColl
	std::vector<ObjCollision*>	m_freeColls,m_usedColls ;

	// matrix containg already checked Nodes (avoid to check a pair already checked)
	std::map<Node*,std::set<Node*> > m_checked ;

	bool					m_isOptimized;

#ifndef JAM_CM_QUADTREE_DISABLED
	Quadtree*				m_quadTree ;
#endif

	Timer*					m_pUpdateTimer ;
	
	// Performs hit test 
	bool					hitTest(Node* src, Node* dst, Method method);

	// called when src and dest Objects have collided
	// updates the list of src and also the list of dest
	// queue a collision event
	void					collided(Node* src, Node* dest) ;

	// the main collision detection (and response) method
	void					checkCollisions(Node* src) ;

	// helper method to allocate an ObjCollision
	ObjCollision*			allocObjColl( Node* with ) ;

	// to prevent the use
							CollisionManager();
	virtual					~CollisionManager() ;
};


/** Returns the singleton instance */
JAM_INLINE CollisionManager& GetCollMgr() { return CollisionManager::getSingleton(); }

}

#endif // __JAM_COLLISIONMANAGER_H__
