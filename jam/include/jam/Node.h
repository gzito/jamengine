/**********************************************************************************
* 
* Node.h
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

#ifndef __JAM_NODE_H__
#define __JAM_NODE_H__

#include <jam/jam.h>
#include <jam/Object.h>
#include <jam/Polygon2f.h>
#include <jam/Circle2f.h>
#include <jam/Action.h>
#include <jam/Color.h>
#include <jam/CollisionManager.h>
#include <jam/Event.h>
#include <jam/InputManager.h>

#include <list>
#include <map>

#include <glm/vec2.hpp>
#include <glm/mat3x3.hpp>

namespace jam {

// forward references
class Draw3DImageManager ;
class CollisionEventArgs ;
class GridBase ;
class Camera ;
class Timer ;
class TimeExpiredEventArgs;
typedef	Event<CollisionEventArgs>	CollisionEvent ;


// queued
class JAM_API TouchEventArgs : public EventArgs
{
public:
	/** Creates a new TouchEventArgs and calls autorelease() on it */
	static TouchEventArgs* create(uint32_t touchId, float x, float y, int status) ;

	int32_t					m_touchId ;
	float					m_x ;
	float					m_y ;
	int						m_status ;

private:
	TouchEventArgs(uint32_t touchId, float x, float y, int status) ;
};

typedef	Event<TouchEventArgs>	TouchEvent ;


struct JAM_API ObjCollision {
	Node *with;
//	Vector coords;
//	Collision collision;
};

typedef std::list<Node*>					NodesList ;

/**
	This is the core class Node

	\remark This is an abstract class
*/
class JAM_API Node : public NamedTaggedObject
{
	friend class CollisionManager ;
	friend class ActionManager ;
	friend class MoveTo ;
	friend class BezierBy ;
	friend class RotateTo ;
	friend class RotateBy ;
	friend class Animate ;
	friend class Scene ;

public:
	typedef std::map<String,String>				AttributesList ;
	typedef std::vector<const ObjCollision*>	CollisionsList ;

#ifdef JAM_DEBUG
#ifdef JAM_TRACE_ACTIVE_NODES
	// total number of nodes
	static int32			m_totCount ;
	static int32			m_totInView ;

	// store the total number of allocated nodes
	struct DebugNodeInfo
	{
		uint32 count ;
	};
	static std::map<String,DebugNodeInfo> m_debugNodeInfoTypes ;

	static void				addDebugNodeInfo(Node* n) ;
	static void				dumpDebugNodeInfo() ;
	static void				clearDebugNodeInfo() ;
#endif

	static void				dumpNodeHierarchy( Node* node, int level = 0 ) ;
	virtual String			getDebugInfo(bool typeInfo/*=true*/);
#endif

	static bool				VisitInProgress ;

	Node();
	virtual ~Node();

	/**
		Marks the object as destroyed.
		Overrides RefCountedObject::destroy()
		\remark Object destruction will be delayed at the end of current frame
	*/
	virtual void			destroy() ;

	virtual void			destroy_internal() ;

	/** Returns the list of children nodes. */
	const NodesList&		getChildren() const { return m_children; } ;

	/** Returns the parent node or 0 if this node has no parent. */
	Node*					getParent() const { return m_parent; }

	/**
		Adds a child to the container using 0 as z-order.
	
		\param child	The child node to be added
		\param global	true for the child to retain its global position and orientation. Defaults to true.
		\remark It returns this, so you can chain several addChilds. Also this method increment the reference count of the child node
	*/
	virtual Node*			addChild(Node* child, bool global = true);

	/**
		Adds a child to the container with a z-order.

		\param child	The child node to be added
		\param zOrder	The z-order of the node relative to it's "brothers", i.e. children of the same parent.
						Negative values will be drawn first, positive values after.
		\param global	true for the child to retain its global position and orientation. Defaults to true.
		\remark  It returns this, so you can chain several addChilds. Also this method increment the reference count of the child node.
	*/
	virtual Node*			addChild(Node* child, int zOrder, bool global = true);

	/** Gets a child from the container given its tag */
	Node*					getFirstChildByTag(const TagType& tag) const ;

	/** Gets a child from the container given its name */
	Node*					getChildByName(const String& name) const ;

	/**
		Remove itself from its parent node. If cleanup is YES, then also remove all actions. If the node orphan, then nothing happens
	
		\remark This method decrement the reference count of the child node
	*/
	void					removeFromParentAndCleanup(bool cleanup);

	/**
		Assing a new parent		
	*/
	void					newParent(Node* newparent, bool cleanup=false );

	/**
		Removes a child from the container. It will also cleanup all running actions depending on the cleanup parameter. 
	
		\remark This method decrement the reference count of the child node
	*/
	virtual void			removeChild(Node* node, bool cleanup);

	/**
		Removes a child from the container by its tag. It will also cleanup all running actions depending on the cleanup parameter 
	
		\remark This method decrement the reference count of the child node
	*/
	void					removeFirstChildByTag(const TagType& tag, bool cleanup);

	
	/**
		Removes a child from the container by its name. It will also cleanup all running actions depending on the cleanup parameter 
	
		\remark This method decrement the reference count of the child node
	*/
	void					removeChildByName(const String& name, bool cleanup);

	/**
		Removes all children from the container and do a cleanup all running actions depending on the cleanup parameter.
	
		\remark This method decrement the reference count of the children nodes
	*/
	virtual void			removeAllChildrenWithCleanup(bool cleanup);

	/** Returns the z order of the node relative to it's "brothers", i.e. children of the same parent */
	int						getZOrder() const { return m_ZOrder; }

	/** Reorders a child according to a new z value. The child MUST be already added. */
	virtual void			reorderChild(Node* child, int zOrder);

	/** Returns the node's hotspot */
	Vector2				getHspot() const { return m_hspot; }

	/** Sets the node's hotspot. Default to (0,0), i.e. node center */
	void					setHspot(const Vector2& val) { m_hspot = val; }

	/**
		Returns the node's local trasformation matrix
	
		\remark The term 'local' means relative to its parent
	*/
	Matrix3				getLocalTform() const ;

	
	/**
		Sets the node's local trasformation matrix
	
		\remark The term 'local' means relative to its parent
	*/
	void					setLocalTform(const Matrix3& m) ;

	/**
		Returns the node's local x-coordinate
	
		\remark The term 'local' means relative to its parent
	*/
	virtual float			getX() const { return m_local_pos.x; }
	
	/**
		Returns the node's local y-coordinate
	
		\remark The term 'local' means relative to its parent
	*/
	virtual float			getY() const { return m_local_pos.y; }

	/**
		Returns the node's local position
	
		\remark The term 'local' means relative to its parent
	*/
	virtual Vector2		getPos() const { return m_local_pos; }

	/**
		Sets the node's local x-coordinate
	
		\remark The term 'local' means relative to its parent
	*/
	virtual void			setX(float x);
 
	/**
		Sets the node's local y-coordinate
	
		\remark The term 'local' means relative to its parent
	*/
	virtual void			setY(float y);

	/**
		Sets the node's local position
	
		\remark The term 'local' means relative to its parent
	*/
	virtual void			setPos(const Vector2& val);
	void					setPos(float x, float y);

	/** Adds a delta value to the node's position */
	void					move( float dx, float dy );

	/** Adds a delta value to the node's position */
	void					move( const Vector2& delta );

	/** Adds a delta value to the node's x-coordinate */
	void					moveX( float dx );

	/** Adds a delta value to the node's y-coordinate */
	void					moveY( float dy );

	/**
		Returns the node's scale (scalar) value 
	
		\remarks Only callable if scale is the same for both x and y dimensions, otherwise produces an assert (in debug mode)
	*/
	float					getUniformScale() const ;

	/** Returns the node's scale values for both dimensions */
	Vector2				getScale() const { return m_local_scl; }

	/** Returns the node's scale value for x dimension */
	float					getScaleX() const { return m_local_scl.x; }

	/** Returns the node's scale value for y dimension */
	float					getScaleY() const { return m_local_scl.y; }

	/** Set the node's scale value for both dimensions */
	void					setScale(float val);

	/** Set the node's scale value for both dimensions */
	void					setScale(const Vector2& v);

	/** Set the node's scale value for x dimension */
	void					setScaleX(float val);

	/** Set the node's scale value for y dimension */
	void					setScaleY(float val);

	/**
		Returns the node's local rotation matrix
	
		\remark The term 'local' means relative to its parent
	*/
	Matrix3				getRotationMat() const { return m_local_rot; }

	/**
		Sets the node's local rotation matrix
	
		\remark The term 'local' means relative to its parent
	*/
	void					setRotationMat(const Matrix3& m);

	/**
		Returns the node's world transformation matrix
	
		\remark The term 'world' means relative to the absolute coordinate system
	*/
	Matrix3				getWorldTform() const ;

	/**
		Sets the node's world transformation matrix
	
		\remark The term 'world' means relative to the absolute coordinate system
	*/
	void					setWorldTform(const Matrix3& m) ;

	/**
		Returns the node's world x-coordinate
	
		\remark The term 'world' means relative to the absolute coordinate system
	*/
	float					getWorldX() const;

	/**
		Returns the node's world y-coordinate
	
		\remark The term 'world', means relative to the absolute coordinate system
	*/
	float					getWorldY() const;

	/**
		Returns the node's world position
	
		\remark The term 'world' means relative to the absolute coordinate system
	*/
	Vector2				getWorldPos() const;


	/**
		Sets the node's world position
	
		\remark The term 'world' means relative to the absolute coordinate system
	*/
	void					setWorldPos(const Vector2& val);

	/**
		Sets the node's world position
	
		\remark The term 'world' means relative to the absolute coordinate system
	*/
	void					setWorldPos(float x, float y);

	/** Returns the node's world scale value for both dimensions */
	Vector2				getWorldScale() const ;

	/** Set the node's world scale value for both dimensions */
	void					setWorldScale( const Vector2& v ) ;

	/**
		Returns the node's world rotation matrix
	
		\remark The term 'world' means relative to the absolute coordinate system
	*/
	Matrix3				getWorldRotationMat() const ;

	/**
		Sets the node's world rotation matrix
	
		\remark The term 'world' means relative to the absolute coordinate system
	*/
	void					setWorldRotationMat( const Matrix3& m ) ;

	/** Returns the node's local rotation angle in degrees */
	float					getRotationAngle() const ;

	/** Returns the node's world rotation angle in degrees */
	float					getWorldRotationAngle() const ;
	
	/**
		Sets the node's local rotation angle in degrees
		\remark Reference axis for orientation is y-axis
				Positive angles rotate clockwise, negative counter-clockwise
	*/
	void					setRotationAngle(float a) ;

	/**
		Sets the node's world rotation angle in degrees
		\remark reference axis for orientation is y-axis
				Positive angles rotate clockwise, negative counter-clockwise
	*/
	void					setWorldRotationAngle(float a) ;

	/** Returns the distance from a given node */
	float					getDistance(const jam::Node* other) const ;
	
	/** Returns the squared distance from a given node */
	float					getDistanceSquared(const jam::Node* other) const ;

	/**
		Sets node orientation towards the given node

		\remark If you wish to point the node at a certain position rather than another node,
				simply create a Pivot2d at your desired position, face the node at this and then free the pivot. 
	*/
	void					faceTo(const Node& n, bool global = true);

	/**
		Returns the axis-aligned bounding box for this node
		
		\remark AABB is relative to the untransformed local coordinate system of this node
	*/
	const AABB&				getAABB() const ;

	/**
		Returns the oriented bounding box for this node
	
		\remark This is obtained by transforming AABB with the node world matrix. The OBB position, rotation and scale are relative to the absolute coordinate system
	*/
	const Polygon2f&		getTransformedAABB() const ;

	/**
		Returns the bounding circle for this node
		
		\remark The bounding circle center position and radius length is relative to the untransformed local coordinate system of this node
	*/
	const Circle2f&			getBoundingCircle() const ;

	/**
		Returns the transformed bounding circle for this node
		
		\remark This is obtained by transforming bounding circle with the node world matrix. The center position and radius length are relative to the absolute coordinate system
	*/
	const Circle2f&			getTransformedBoundingCircle() const ;
	
	GridBase*				getGrid() { return m_pGrid; }

	void					setGrid(GridBase* val);

	/** Returns whether this node is enabled or disable, e.g. if it is taken in account in collisions */
	bool					isEnabled() const { return m_enabled; }

	/** Sets the node as enabled or disable */
	void					setEnabled(bool val) { m_enabled = val; }

	/** Returns whether this node is visible, i.e. rendered */
	bool					isVisible() const { return m_visible; }

	/** Sets the node as visible or invisible, i.e. rendered/unrendered	*/
	void					setVisible(bool val) { m_visible = val; }

	/** Returns 'true' if the node is inside the 2D viewport, i.e. visible to the camera */
	bool					isInView();
	bool					isInViewActive(bool forceVerify = true);

	/** Returns the list of this node (if enabled) and all its enabled children */
	void					enumEnabled( std::vector<Node*> &out ) const ;

	/** Returns the list of this node (if visibile) and all its visible children */
	void					enumVisible( std::vector<Node*> &out ) const ;

	/**
		Returns whether the node is running or not, i.e. it was added to scene and we are in the main loop
	
		\sa onEnter, onExit
	*/
	bool					isRunning() const { return m_running; }

	void					visit() ;

	/**
		Updates the node and its data, i.e. its matrices, AABB, etc.

		\remark When overriding this method, you have to call it from derived classes (e.g. see Sprite class). This method is called every frame if the node is enabled
	*/
	virtual void			update();

	/**
		Renders the node
	
		\remark You must override this method in derived classes (e.g. see Sprite class). This method is called every frame if the node is enabled and visible
	*/
	virtual void			render();

	/**
		Called when this node is added to the scene and before entering the main loop

		\remark Overridable.
	*/	 
	virtual void			onEnter();

	/**
		Called when this node is removed from the scene and after exited from the main loop
		
		\remark Overridable
	*/
	virtual void			onExit();

	/** Returns the node's status */
	virtual int				getStatus() const { return m_status; }

	/** Sets the node's status */
	virtual void			setStatus(int val) { m_status = val; }

	/** Returns the time interval since this instance was created, in milliseconds */
	uint64_t				getLifeTime() const;


	/** 
		Executes an action, and returns the action that is executed.
		The node becomes the action's target.

		\return An Action pointer
	*/
	Action*					runAction(Action* action) ;
	Action*					runAction( Action* action,  const TagType& tag);

	void					stopActionsByTag(const TagType& tag);

	/** Removes all actions from the running action list */
	void					stopAllActions(void);

	/** Removes an action from the running action list */
	void					stopAction(Action* action);

	/** Removes an action from the running action list given its name */
	void					stopActionByName(const String& name);

	/**
		Gets an action from the running action list given its name
		\return The Action the with the given name
	*/
	Action*					getActionByName(const String& name);

	/**
		Returns the numbers of actions that are running plus the ones that are schedule to run.
		Composable actions are counted as 1 action. Example:
		If you are running 1 Sequence of 7 actions, it will return 1.
		If you are running 7 Sequences of 2 actions, it will return 7.
	*/
	int						numberOfRunningActions(void);
	
	/**
	Sets the node's and children color

	\remarks Pure virtual method, must to be implemented in concrete, derived classes
	\sa Sprite & TextNode classes
	*/
	virtual void			setColorGlobal( const Color& c ) ;
	/**
		Sets the node's color

		\remarks Pure virtual method, must to be implemented in concrete, derived classes
		\sa Sprite & TextNode classes
	*/
	virtual void			setColor( const Color& c ) = 0 ;

	/**
		Returns the node's color
		
		\remarks Pure virtual method, must to be implemented in concrete, derived classes
		\sa Sprite, TextNode
	*/
	virtual const Color&	getColor() const = 0 ;

	/** Returns whether this node is moving or not, i.e. there is a running action changing its position */
	bool					isMoving() const { return (m_actionFlags & MOVING) != 0 ; }

	/** Returns whether this node is rotating or not, i.e. there is a running action changing its rotation angle */
	bool					isRotating() const { return (m_actionFlags & ROTATING) != 0 ; }

	/**
		Returns whether this node is animating or not, i.e. it is running an action changing its frames
	
		\sa Sprite
	*/
	bool					isAnimating() const { return (m_actionFlags & ANIMATING) != 0; }

	/**
		Returns 'true' this node isn't moving, rotating nor animating; it returns 'false' otherwise
	
		\sa Sprite
	*/
	bool					isCompleted() const { return m_actionFlags == 0; }

	/**
		Sets a node attribute, given its key and its value
	
		\remark The "status" and "visible" keys call respectively setStatus and setVisible methods
	*/
	void					setAttribute(const String& key, const String& val) ;

	/**
		Returns a node attibute value given its key
	
		\remark The "status" and "visible" keys call respectively getStatus and getVisible methods
	*/
	String					getAttribute(const String& key) const;


	//
	// pause/resume handling
	//

	/**
		Pause this node and all its children.
		Actions, updates and collisions are suspended.
	*/
	void					pause() ;

	/**
		Resume this node and all its children.
		Actions, updates and collisions are resumed.
	*/
	void					resume() ;

	/**
		Pause or resume this node and all its children
		Actions, updates and collisions are paused or resumed.
	*/
	void					togglePause() ;

	/** Called when this node is paused. Can be overriden */
	virtual void			onPause() ;

	/** Called when this node is resumed. Can be overriden */
	virtual void			onResume() ;


	//
	// collisions
	//

	float					getCollisionScaleFactor() const { return m_collisionScaleFactor; }

	void					setCollisionScaleFactor(float val) { m_collisionScaleFactor = val; }

	const Polygon2f&		getCollisionOBB() const ;
	const Circle2f&			getCollisionBoundingCircle() const ;

	/** Returns true if this node is collisions-enabled */
	bool					canCollide() const		{	return m_collType!=0;	}	

	/**
		Disables collisions detection for this node
		\remark This method clears the node collision type to 0, "remembering" the old collision type. Collisions detection can be re-enabled later with resumeCollisions()
		\sa resumeCollisions
	*/
	void					pauseCollisions();

	/**
		Re-enables collisions detection for this node.
		\remark This method reset the collision type value the node had before pauseCollisions() was called
		\sa pauseCollisions
	*/
	void					resumeCollisions();

	/**
	\brief Sets the collision type for a node.

	A collision_type value of 0 indicates that no collision checking will occur with that node.
	A collision value of 1-999 will mean collision checking will occur.

	\param collType Collision type of node. Must be in the range 0-999.
	\param recursive Optional - true to apply collision type to entity's children. Defaults to false.
	*/
	void					setCollisionType( int collType, bool recursive=false ) ;

	/** Returns the collision type of a node */
	int						getCollisionType() const { return m_collType; }

	/** Returns how many collisions this node was involved in during the last update. */
	size_t					countCollisions() const ;

	/** Returns a pointer to the node of the specified type that collided with this node. */
	Node*					collided(int type) const ;

	/**
		Returns the other node involved in a particular collision.
		\param index Should be in the range 1...countCollisions( entity ), inclusive.
	*/
	Node*					collisionNode( int index ) ;

	/**
		Check for collision between this and another node
	
		\remark This is simply implemented as returning false. Derived classes must override it
		\sa Sprite
	*/
	virtual bool			collide(Node* other, CollisionManager::Method m) const ;


	//
	// touch handling
	//

	/**
		Sets whether or not this node is sensible to touches
		\remarks Parent node has to be also touchable. Exception to this rule is when the parent node is the scene
	*/
	void					setTouchable(bool val = true);
	/** Returns whether or not this node is sensible to touches */
	bool					isTouchable() const { return m_touchable; }

	/** Returns true if this node has been touched in the current frame and wasn't touched in the previous frame */
	bool					isPressed(int touchId) const { return m_pressed[touchId]; }
	/** Returns true if this node is currently touched */
	bool					isDown(int touchId) const { return m_down[touchId]; }
	/** Returns true if this node has been released in the current frame and was touched in the previous frame */
	bool					isReleased(int touchId) const { return m_released[touchId]; }

	bool					isInsideTouch(int touchId) const { return m_inside[touchId]; }

	/** Starts a drag operation on this node */
	virtual void			startDrag(int32_t touchId) ;

	/** Ends a drag operation on this node */
	virtual void			endDrag() ;

	/** Returns whether or not this node is currently being dragged */
	bool					isDragging() const { return m_isDragging; }


	/**
		Returns the event fired when the node is pressed
	
		\sa isPressed
	*/
	TouchEvent&				getTouchPressedEvent() { return m_touchPressedEvent; }

	/**
		Returns the event fired when the node is down
	
		\sa isDown
	*/
	TouchEvent&				getTouchDownEvent() { return m_touchDownEvent; }

	/**
		Returns the event fired when the node is released
	
		\sa isReleased
	*/
	TouchEvent&				getTouchReleasedEvent() { return m_touchReleasedEvent; }
		/**
		Returns the event fired when the node is leaved
	
		\sa isLeaved
	*/
	TouchEvent&				getTouchLeaveEvent() { return m_touchLeaveEvent; }

	
	/**
		Returns the event fired when the node collides with another node
	
		\sa isPressed
	*/
	CollisionEvent&			getCollisionEvent() { return m_collisionEvent; }

	
	/**
		Transforms between coordinate systems.
	
		\param v coordinates of a point
		\param src pointer to source node, or 0 for the world's coordinate system
		\param dest pointer to destination node, or 0 for the world's coordinate system. Default to 0.
		\remark TransformVector doesn't take position into account, while TransformPoint does.
				TransformNormal is the same as TransformVector except it normalizes the vector, so it always has a length of 1 unit.
				An example would be, you want to know where a vertex in a mesh is in your world/game, so you use:
				TransformPoint( VertexX,VertexY,VertexZ,pMyMesh,0 )
				You might also want to know which direction the vertex's normal is pointing in the world/game, so you use:
				TransformPoint( VertexN,pMyMesh,0 ) (You could also have used TransformNormal for that)
				So TransformPoint is for finding out stuff about a location, while TransformVector is for finding out stuff about a direction. 
	*/
	static Vector2		TransformPoint( const Vector2& v, Node *src, Node *dest=0 ) ;

	/**
		Transforms between coordinate systems. Similar to TransformPoint(), but operates on a vector. A vector can be thought of 
		as 'displacement relative to current location'. 
	
		\param v component of a vector in 2d space
		\param src pointer to source node, or 0 for world's coordinate system
		\param dest pointer to destination node, or 0 for world's coordinate system. Default to 0.
		\remark TransformVector doesn't take position into account, while TransformPoint does.
				TransformNormal is the same as TransformVector except it normalizes the vector, so it always has a length of 1 unit.
				An example would be, you want to know where a vertex in a mesh is in your world/game, so you use:
				TransformPoint( VertexX,VertexY,VertexZ,pMyMesh,0 )
				You might also want to know which direction the vertex's normal is pointing in the world/game, so you use:
				TransformPoint( VertexN,pMyMesh,0 ) (You could also have used TransformNormal for that)
				So TransformPoint is for finding out stuff about a location, while TransformVector is for finding out stuff about a direction. 
	*/
	static Vector2		TransformVector( const Vector2& v, Node *src, Node *dest=0 ) ;

	/**
		Transforms between coordinate systems. This is exactly the same as TransformVector but in addition 
		after the transformation the new vector is 'normalized', meaning it is scaled to have length 1. 
	
		\param v component of a vector in 2d space
		\param src pointer to source node, or 0 for world's coordinate system
		\param dest pointer to destination node, or 0 for world's coordinate system. Default to 0.
	*/
	static Vector2		TransformNormal( const Vector2& v, Node *src, Node *dest=0) ;
	

	// camera
	static Camera*			getCurrentCamera() ;
	static void				setCurrentCamera( Camera* pCamera ) ;

	void					setCamera( Camera* pCamera ) ;
	Camera*					getCamera() ;

	Camera*					getAncestorCamera() ;


	/*** Actions Speed **/
	float getActionSpeed() const { return m_actionSpeed; }
	void setActionSpeed(float val=1.0f);

protected:
	const CollisionsList&	getCollisions() const { return m_colls; }

	void					updateLocalTForm();
	void					updateWorldTForm();
	void					updateOBB();
	void					updateTouches();

	void					invalidateLocal();
	void					invalidateWorld();
	void					invalidateOBB();

	bool					isLocalTformInvalid() const { return m_local_tform_dirty; }
	bool					isWorldTformInvalid() const { return m_world_tform_dirty; }
	bool					isOBBInvalid() const { return m_obbDirty; }

	void					setAABB( const AABB& aabb );

private:
	// to prevent the use	
							Node(const Node&) = delete ;
	Node&					operator=(const Node&) = delete ;

	// called by CollisionMananger
	void					addCollision( const ObjCollision* n ) ;
	// called by CollisionMananger
	void					clearCollisions();

	void					setActionFlags( U8 flags ) ;	// called by varius Actions, see ActionFlags enum
	void					clearActionFlags( U8 flags ) ;	// called by ActionManager

	void					setParent(Node* node) { m_parent=node; }
	void					insertChild(Node* child, int z);

	/** Used internally to alter the zOrder variable. DON'T call this method manually */
	void					setZOrder(int z);

	void					detachChild(Node *child, bool doCleanup);

protected:
	bool					m_local_tform_dirty ;
	Matrix3				m_local_tform ;
	Vector2				m_local_pos ;
	Matrix3				m_local_rot ;
	Vector2				m_local_scl ;

	bool					m_world_tform_dirty ;
	Matrix3				m_world_tform ;
	Vector2				m_world_pos ;
	Matrix3				m_world_rot ;
	Vector2				m_world_scl ;

	Vector2				m_hspot ;
	bool					m_enabled ;
	bool					m_visible ;
	jam::Node*				m_parent ;
	NodesList				m_children ;
	int						m_status ;
	uint64_t				m_lifeTime;

	float					m_collisionScaleFactor ;

	// axis-aligned and oriented bounding box
	bool					m_obbDirty ;
	AABB					m_aabb ;
	Polygon2f				m_obb ;
	Polygon2f				m_collisionObb ;			// used only when m_collisionScaleFactor != 1.0f

	// bounding circle
	Circle2f				m_boundingCircle ;
	Circle2f				m_transformedBoundingCircle ;
	Circle2f				m_collisionBoundingCircle ;

	// touch handling
	bool					m_touchable;
	bool					m_wasDownLastFrame[JAM_MAX_TOUCHES] ;
	bool					m_wasInsideLastFrame[JAM_MAX_TOUCHES] ;
	bool					m_down[JAM_MAX_TOUCHES]; 
	bool					m_released[JAM_MAX_TOUCHES];
	bool					m_pressed[JAM_MAX_TOUCHES];
	bool					m_inside[JAM_MAX_TOUCHES];

	TouchEvent				m_touchPressedEvent ;
	TouchEvent				m_touchDownEvent ;
	TouchEvent				m_touchReleasedEvent ;
	TouchEvent				m_touchLeaveEvent;
	CollisionEvent			m_collisionEvent ;

	bool					m_isDragging ;
	int32_t					m_draggingTouchId ;

	// grid
	GridBase*				m_pGrid ;

	// camera attached to this node
	Camera*					m_pCamera ;
	static Camera*			m_pCurrentCamera ;

#ifdef JAM_CHECK_SINGLE_UPDATE_CALL
	uint64_t				m_lastUpdate ;
	uint64_t				m_lastRender ;
#endif

private:
	enum ActionFlags
	{
		MOVING = 1,		// MoveTo & BezierBy
		ROTATING = 2,	// RotateTo & RotateBy
		ANIMATING = 4	// Animate
	};

	// The z order of the node relative to it's "brothers": children of the same parent
	// Negative values will be drawn first, positive values after
	int						m_ZOrder ;
	bool					m_running ;
	U8						m_actionFlags ;

	AttributesList			m_attributes ;

	// collisions
	int						m_collType ;
	CollisionsList			m_colls ;
	int						m_savedCollType;			// used to pause collision detection for this node
	bool					m_justCollisionPaused;		// tells if collisions detection is paused for the node

	// Speed of actions
	float					m_actionSpeed;
	bool					m_is_in_view;
	bool					m_isInViewCalculated;
};

// queued
class CollisionEventArgs : public EventArgs
{
public:
	/** Creates a new CollisionEventArgs and calls autorelease() on it */
	static CollisionEventArgs* create( Node* src, Node* dst ) ;
	Node*					getSrcNode() const { return m_src; }
	Node*					getDstNode() const { return m_dst; }

private:
	CollisionEventArgs( Node* src, Node* dst ) : m_src(src), m_dst(dst) {}

	Node*					m_src ;
	Node*					m_dst ;
};

}

#endif // __JAM_NODE_H__
