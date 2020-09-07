/**********************************************************************************
* 
* Node.cpp
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

#include <gl/glew.h> 

#include "jam/Node.h"
#include "jam/ActionManager.h"
#include "jam/InputManager.h"
#include "jam/Draw3dManager.h"
#include "jam/Application.h"
#include "jam/Scene.h"
#include "jam/Layer.h"
#include "jam/Event.h"
#include "jam/Grid.h"
#include "jam/Timer.h"
#include "jam/Gfx.h"
#include "jam/Camera.h"
#include "jam/core/geom.h"

#ifdef JAM_DEBUG
#include "jam/Sprite.h"
#include "jam/TextNode.h"
#endif

#include <glm/vec3.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>

#include <typeinfo>
#include <sstream>
#include <math.h>

using namespace std ;

namespace jam {

TouchEventArgs::TouchEventArgs(uint32_t touchId, float x, float y, int status) :
	EventArgs(), m_touchId(touchId), m_x(x), m_y(y), m_status(status)
{
}

TouchEventArgs* TouchEventArgs::create( uint32_t touchId, float x, float y, int status )
{
	TouchEventArgs* touchEvtArgs = new (GC) TouchEventArgs(touchId,x,y,status) ;
	return touchEvtArgs ;
}


// static member initialization

#ifdef JAM_TRACE_ACTIVE_NODES
int32 Node::m_totCount = 0 ;
int32 Node::m_totInView = 0 ;
std::map<String,Node::DebugNodeInfo> Node::m_debugNodeInfoTypes;
#endif

Camera* Node::m_pCurrentCamera = 0 ;

bool Node::VisitInProgress = false ;

// ctor
Node::Node() :
	m_local_tform_dirty(true),
	m_local_tform(1.0f),
	m_local_pos(0,0),
	m_local_rot(1.0f),
	m_local_scl(1,1),
	m_world_tform_dirty(true),
	m_world_tform(1.0f),
	m_world_pos(0,0),
	m_world_rot(1.0f),
	m_world_scl(1,1),
	m_hspot(0,0),
	m_enabled(true),
	m_visible(true),
	m_parent(0),
	m_children(),
	m_status(0),
	m_lifeTime(0),
	m_ZOrder(0),
	m_running(false),
	m_actionFlags(0),
	m_attributes(),
	m_collType(0),
	m_savedCollType(0),
	m_justCollisionPaused(false),
	m_colls(),
	m_aabb(),
	m_obb(),
	m_collisionScaleFactor(COLLISION_MANAGER_DEFAULT_SCALE_FACTOR),
	m_boundingCircle(),
	m_obbDirty(true),
	m_touchable(false),
	m_touchPressedEvent(),
	m_touchDownEvent(),
	m_touchReleasedEvent(),
	m_touchLeaveEvent(),
	m_isDragging(false),
	m_pGrid(0),
	m_actionSpeed(1.0f),
	m_is_in_view(false), m_isInViewCalculated(false)
	,m_pCamera(0)
#ifdef JAM_CHECK_SINGLE_UPDATE_CALL
	,m_lastUpdate(0)
	,m_lastRender(0)
#endif
{
	m_lifeTime = GetAppMgr().getTotalElapsedMs() ;

	for(int i=0; i<JAM_MAX_TOUCHES; i++) {
		m_wasDownLastFrame[i] = false ;
		m_wasInsideLastFrame[i] = true ;
		m_down[i] = false ;
		m_released[i] = false ;
		m_pressed[i] = false ;
		m_inside[i] = false ;
	}

#ifdef JAM_TRACE_ACTIVE_NODES
	m_totCount++ ;
#endif
}

// dtor
Node::~Node()
{
	m_pGrid = nullptr ;
	m_pCamera = nullptr ;

	Node* child = 0 ;
	while( !m_children.empty() ) {
		child = m_children.front() ;
		m_children.pop_front() ;
		child->m_parent = 0 ;
		child->stopAllActions() ;
	}

	// check for no parent is done inside the called method
	removeFromParentAndCleanup(true) ;
		
#ifdef JAM_TRACE_ACTIVE_NODES
	m_totCount-- ;
	if( m_totCount == 0 ) {
		m_debugNodeInfoTypes.clear() ;
	}
#endif
}

/** Sets sprite's color */
void Node::setColorGlobal(const Color& c ) 
{
	// color also children
	if( m_children.size() > 0 ) {
		for( NodesList::iterator it = m_children.begin(); it != m_children.end(); it++ ) 
		{
			Node* child =(*it);
			child->setColorGlobal(c) ;		// child is removed from parent
		}
	}
	setColor(c);
}

void Node::destroy()
{
		this->setEnabled(false) ;
		this->stopAllActions() ;

		// destroy also children
 		if( m_children.size() != 0 ) {
			NodesList::iterator it = m_children.begin(); 
 			while( !m_children.empty() ) {
 				Node* child = *it++ ;
 				child->destroy() ;
 			}
 		}

		removeFromParentAndCleanup(true) ;
}

void Node::destroy_internal()
{
	this->removeFromParentAndCleanup(true) ;		// cleanup also actions
}
	
Node* Node::addChild( Node* child, bool global /*=true*/ )
{
	return addChild(child,child->m_ZOrder,global);
}

Node* Node::addChild( Node* child, int zOrder, bool global /*=true*/ )
{
	JAM_ASSERT_MSG(child!=0, ("addChild() : null 'child' parameter"));

#ifdef JAM_DEBUG
	// adding a layer as child ?
	if( typeid(Layer) == typeid(*child) ) {
		JAM_ASSERT_MSG( (zOrder >= -4 && zOrder <= 31), ("The zOrder for a Layer should be in the range [-4,31])") ) ;
	}
#endif

	Matrix3 t ;
	if( global ) {
		t = child->getWorldTform();
	}

	if( child->m_parent) 
	{
		if ( child->m_parent != this )
		{
				child->m_parent->removeChild(child,true) ;
				this->insertChild(child,zOrder);
		}
		else
		{
			int x=0;
		}
	}
	else
	{
		this->insertChild(child,zOrder);
	}
		
	child->setParent(this);
	child->invalidateWorld();

	// recalculate world transform
	if( global ) {
		child->setWorldTform( t );
	}
	else {
		child->getWorldTform() ;
	}

	if( m_running ) {
		child->onEnter() ;
	}

	return child ;
}

Node* Node::getFirstChildByTag(const TagType& tag) const
{
	Node* n = 0 ;

	NodesList::const_iterator it = m_children.begin() ;
	while( it != m_children.end() ) {
		if( (*it)->getTag() == tag ) {
			n = *it ;
			break ;
		}
		it++ ;
	}

	return n ;
}

Node* Node::getChildByName(const String& name) const
{
	Node* n = 0 ;

	NodesList::const_iterator it = m_children.begin() ;
	while( it != m_children.end() ) {
		if( (*it)->getName() == name ) {
			n = *it ;
			break ;
		}
		it++ ;
	}

	JAM_ASSERT_MSG(n!=0, ("getChildByName() : child with name \"%s\" not found",name.c_str()));

	return n ;
}

void Node::removeFromParentAndCleanup( bool cleanup )
{
	if( m_parent ) {
		m_parent->removeChild(this, cleanup) ;
	}
}

void Node::removeChild( Node* node, bool cleanup )
{
	detachChild(node,cleanup) ;
}

void Node::removeChildByName( const String& name, bool cleanup )
{
	Node* node = getChildByName(name) ;
	if( node ) {
		removeChild(node,cleanup) ;
	}
}

void Node::removeFirstChildByTag( const TagType& tag, bool cleanup )
{
	Node* node = getFirstChildByTag(tag) ;
	if( node ) {
		removeChild(node,cleanup) ;
	}
}

void Node::removeAllChildrenWithCleanup( bool cleanup )
{
	// not using detachChild improves speed here
	while( !m_children.empty() )
	{
		Node * pNode = m_children.front();
		m_children.pop_front() ;
		if (pNode)
		{
			// IMPORTANT:
			//  -1st do onExit
			//  -2nd cleanup
			if(m_running)
			{
				pNode->onExit();
			}

			if (cleanup)
			{
				pNode->stopAllActions();
			}
			// set parent nil at the end
			pNode->setParent(nullptr);
		}
	}
}

void Node::newParent(Node* newparent, bool cleanup )
{
	if (!newparent) return;
	removeFromParentAndCleanup( cleanup );
	newparent->addChild(this);
}

void Node::reorderChild( Node* child, int zOrder )
{
	JAM_ASSERT_MSG(child->m_parent==this, ("reorderChild() : not a child, only a child can be reordered"));

	m_children.remove(child);
	insertChild(child, zOrder);
}

Matrix3 Node::getLocalTform() const
{
	if( isLocalTformInvalid() ) {
		const_cast<Node*>(this)->updateLocalTForm() ;
	}
	return m_local_tform ;
}

void Node::setLocalTform( const Matrix3& mat )
{
	m_local_pos = jam::getTranslate(mat) ;	// get translation part
	m_local_scl = jam::getScale(mat) ;
	m_local_rot = jam::getRotate(mat);
	invalidateLocal() ;
}

void Node::setX( float x )
{
	m_local_pos.x = x;
	invalidateLocal() ;
}

void Node::setY( float y )
{
	m_local_pos.y = y;
	invalidateLocal() ;
}

void Node::setPos( const Vector2& val )
{
	m_local_pos = val;
	invalidateLocal() ;
}

void Node::setPos(float x, float y)
{
	m_local_pos = Vector2(x,y);
	invalidateLocal() ;
}

void Node::move( float dx, float dy )
{
	if( dx != 0 || dy != 0 ) {
		m_local_pos.x += dx;
		m_local_pos.y += dy;
		invalidateLocal() ;
	}
}

void Node::move( const Vector2& delta )
{
	move(delta.x,delta.y) ;
}

void Node::moveX( float dx )
{
	if( dx != 0 ) {
		m_local_pos.x += dx;
		invalidateLocal() ;
	}
}

void Node::moveY( float dy )
{
	if( dy != 0 ) {
		m_local_pos.y += dy;
		invalidateLocal() ;
	}
}

float Node::getUniformScale() const
{
	JAM_ASSERT_MSG(m_local_scl.x == m_local_scl.y, ("getUniformScale() : m_local_scl.x is not equal to m_local_scl.y"));
	return m_local_scl.x ;
}

void Node::setScale( float val )
{
	JAM_ASSERT_MSG(val!=0.0, ("setScale() : scale must be not equal to 0"));
	m_local_scl = Vector2(val);
	invalidateLocal();
}

void Node::setScale( const Vector2& v )
{
	JAM_ASSERT_MSG(v.x!=0.0 && v.y!=0, ("setScale() : scale must be not equal to 0"));
	m_local_scl = v;
	invalidateLocal();
}

void Node::setScaleX( float val )
{
	JAM_ASSERT_MSG(val!=0.0, ("setScale() : scale must be not equal to 0"));
	m_local_scl.x = val;
	invalidateLocal();
}

void Node::setScaleY( float val )
{
	JAM_ASSERT_MSG(val!=0.0, ("setScale() : scale must be not equal to 0"));
	m_local_scl.y = val;
	invalidateLocal();
}

void Node::setRotationMat( const Matrix3& m )
{
	m_local_rot = m ;
	invalidateLocal();
}

Matrix3 Node::getWorldTform() const
{
	if( isWorldTformInvalid() ) {
		const_cast<Node*>(this)->updateWorldTForm() ;
	}
	return m_world_tform;
}

void Node::setWorldTform( const Matrix3& m )
{
	setLocalTform( m_parent ? m * glm::inverse(m_parent->getWorldTform()) : m );
}

float Node::getWorldX() const
{
	return getTranslate(getWorldTform()).x ;
}

float Node::getWorldY() const
{
	return getTranslate(getWorldTform()).y;
}

Vector2 Node::getWorldPos() const
{
	return getTranslate(getWorldTform());
}

void Node::setWorldPos( const Vector2& val )
{
	setPos( m_parent ? jam::transform(glm::inverse(m_parent->getWorldTform()),val) : val ) ;
}

void Node::setWorldPos( float x, float y )
{
	setWorldPos(Vector2(x,y)) ;
}

Vector2 Node::getWorldScale() const
{
	const_cast<Node*>(this)->m_world_scl = m_parent ? m_local_scl * m_parent->getWorldScale() : m_local_scl;
	return m_world_scl;
}

void Node::setWorldScale( const Vector2& v )
{
	setScale( m_parent ? m_parent->getWorldScale() / v : v );
}

Matrix3 Node::getWorldRotationMat() const
{
	// inverted mul order
	const_cast<Node*>(this)->m_world_rot = m_parent ? m_parent->getWorldRotationMat() * m_local_rot : m_local_rot;
	return m_world_rot;
}

void Node::setWorldRotationMat( const Matrix3& m )
{
	// inverted mul order
	setRotationMat( m_parent ? glm::inverse(m_parent->getWorldRotationMat()) * m : m );
}

float Node::getRotationAngle() const
{
	Matrix3 mat = getRotationMat() ;
	return ToDegree( atan2f(mat[1][0],mat[1][1]) ) ;
}

float Node::getWorldRotationAngle() const
{
	Matrix3 mat = getWorldRotationMat();
	return ToDegree( atan2f(mat[1][0],mat[1][1]) ) ;
}

void Node::setRotationAngle( float a )
{
	JAM_ASSERT_MSG( !std::isnan(a), "isnan: %f", a ) ;
	// should I wrap angle?
	Matrix3 m = createRotationMatrix2D(-a) ;
//		Matrix3 m = glm::rotate( m, -a ) ;
	setRotationMat(m) ;
}

void Node::setWorldRotationAngle( float a )
{
	JAM_ASSERT_MSG( !std::isnan(a), "isnan: %f", a ) ;

	// should I wrap angle?
	Matrix3 m = createRotationMatrix2D(-a) ;
//		Matrix3 m = glm::rotate( m, -a ) ;
	setWorldRotationMat(m) ;
}

const AABB& Node::getAABB() const
{
	return m_aabb ;
}

const Polygon2f& Node::getTransformedAABB() const
{
	if( m_obbDirty ) {
		const_cast<Node*>(this)->updateOBB() ;
	}
	return m_obb ;
}

const Circle2f& Node::getBoundingCircle() const
{
	return m_boundingCircle ;
}

const Circle2f& Node::getTransformedBoundingCircle() const
{
	if( m_obbDirty ) {
		const_cast<Node*>(this)->updateOBB() ;
	}
	return m_transformedBoundingCircle ;
}

void Node::enumEnabled( std::vector<Node*> &out ) const
{
	if( m_enabled && m_running ) {
		out.push_back(const_cast<Node*>(this));
		for( NodesList::const_iterator it = m_children.begin(); it!=m_children.end(); it++ ){
			(*it)->enumEnabled( out );
		}
	}
}

void Node::enumVisible( std::vector<Node*> &out ) const
{
	if( m_visible ) {
		out.push_back(const_cast<Node*>(this));
		for( NodesList::const_iterator it = m_children.begin(); it!=m_children.end(); it++ ){
			(*it)->enumVisible( out );
		}
	}
}

void Node::visit()
{
	m_isInViewCalculated=false;
	m_is_in_view=false;

	JAM_PROFILE("Node.visit") ;

	// quick return if not enabled (if the node is marked for destroy it is also not enabled)
	//2GZ: Non ho trovato il punto in cui lo se è marked allora è !enabled
	if( !m_enabled || !m_running ) {
		return;
	}

	if (m_pGrid && m_pGrid->isActive())	{
		m_pGrid->beforeDraw();
	}

	// self update (virtual call)
	this->update();

	if( isTouchable() ) {
		GetAppMgr().getScene()->addTouchableNode(this) ;
	}

	Node* pNode;
	NodesList::const_iterator it;

	bool isLayerOrScene = typeid(Layer) == typeid(*this) || typeid(ColorLayer) == typeid(*this) || typeid(Scene) == typeid(*this) ;
	if( isLayerOrScene ) {
		GetGfx().setRenderLevel( getZOrder() ) ;
	}

	// The children list could change and the iterator could become invalid, so we copy it
	NodesList appo( m_children.size() ) ;
	std::copy( m_children.begin(),m_children.end(), appo.begin() );

	if( appo.size() > 0) {
		// draw children zOrder < 0 (the ones behind the current node)
		for( it = appo.begin(); it != appo.end(); it++) {
			pNode = (*it);

			if ( pNode && pNode->m_ZOrder < 0 ) {
				pNode->visit();
			}
			else {
				break;
			}
		}
	}

	if( m_enabled && m_running ) {
		Camera* pCamera = this->getCamera() ;
		if( pCamera == 0) {
			pCamera = this->getAncestorCamera() ;
			JAM_ASSERT_MSG( (pCamera!=0), ("Camera not found") ) ;
		}


		if( pCamera != Node::getCurrentCamera() ) {
			Draw3DBatch* pBatch = GetGfx().getBatch() ;
	  
			bool batchInProgress = GetGfx().isBatchingInProgress() ;
			if( pBatch && batchInProgress) { pBatch->end() ; }
	  
			glFlush() ;

			pCamera->setActive() ;
			Node::setCurrentCamera(pCamera) ;
			GetShaderMgr().getCurrent()->setModelMatrix( Matrix4(1.0f) ) ;
	  
			if(pBatch && batchInProgress) { pBatch->begin() ; }
		}
	}

	// self draw
	if( m_enabled && m_running && m_visible ) this->render();

	if( isLayerOrScene ) {
		GetGfx().setRenderLevel( getZOrder() ) ;
	}

	// draw children zOrder >= 0 (the ones in front of the current node)
	if ( appo.size() > 0) {
		for ( ; it!=appo.end(); it++ ) {
			pNode = (*it);
			if (pNode) {
				pNode->visit();
			}
		}
	}

	if (m_pGrid && m_pGrid->isActive())
	{
		m_pGrid->afterDraw(this);
	}
}

void Node::update()
{
	JAM_PROFILE("Node.upd") ;

#ifdef JAM_CHECK_SINGLE_UPDATE_CALL
	JAM_ASSERT_MSG( m_lastUpdate != GetAppMgr().getTotalElapsedMs(), ("Update called twice") ) ;
	m_lastUpdate = GetAppMgr().getTotalElapsedMs() ;
#endif

#ifdef JAM_TRACE_ACTIVE_NODES
	addDebugNodeInfo(this) ;
#endif

	if( m_isDragging ) {
		Vector2 mousePos = Vector2( GetInputMgr().getTouchX(m_draggingTouchId), GetInputMgr().getTouchY(m_draggingTouchId) ) ;
		setWorldPos( mousePos ) ;
	}

	if( isLocalTformInvalid() ) {
		updateLocalTForm() ;
	}

	if( isWorldTformInvalid() ) {
		updateWorldTForm() ;
	}

	if( isOBBInvalid() ) {
		updateOBB() ;
	}

}

void Node::updateTouches()
{
	if( m_touchable )
	{
		for(uint32_t i=0; i<GetInputMgr().getMaxNumberOfTouches(); i++) {
			m_released[i] = false ;
			m_down[i] = false ;
			m_pressed[i] = false ;
		}

		for(uint32_t touchId=0; touchId<GetInputMgr().getMaxNumberOfTouches(); touchId++)
		{
			// un touch non viene propagato ai nodi sottostanti
			if( GetAppMgr().getScene()->getTouches(touchId) == true ) {
				continue ;
			}

			if( GetInputMgr().isTouchUp(touchId) ) {
				continue ;
			}

			if( GetInputMgr().isTouchReleased(touchId)) {
				m_wasDownLastFrame[touchId] = false ;
				m_released[touchId] = true ;
			}

			Vector2 mousePos = Vector2( GetInputMgr().getTouchX(touchId), GetInputMgr().getTouchY(touchId) ) ;
			m_inside[touchId] = getTransformedAABB().isPointInsideQuad(mousePos) ;

			if( m_inside[touchId] )
			{
				GetAppMgr().getScene()->setTouches(touchId, true) ;
				GetAppMgr().getScene()->setTouchedNode(touchId, this) ;

				m_wasInsideLastFrame[touchId] = true ;

				m_pressed[touchId] = GetInputMgr().isTouchPressed(touchId) ;
				m_down[touchId] = GetInputMgr().isTouchDown(touchId) || m_pressed[touchId] ;
				m_wasDownLastFrame[touchId] = m_down[touchId] ;

				// workaround
				Draw3DManager::MouseOver3DEventDetected = true ;
				Draw3DManager::MouseDown3DEventDetected = m_down[0] ;

				if( m_pressed[touchId] ) {
					TouchEventArgs* evtArgs = TouchEventArgs::create(touchId,mousePos.x,mousePos.y,GetInputMgr().getTouchState(touchId)) ;
					m_touchPressedEvent.enqueue(evtArgs,this) ;
				}
				else if( m_down[touchId] ) {
					TouchEventArgs* evtArgs = TouchEventArgs::create(touchId,mousePos.x,mousePos.y,GetInputMgr().getTouchState(touchId)) ;
					m_touchDownEvent.enqueue(evtArgs,this) ;
				}
				else if( m_released[touchId] ) {
					TouchEventArgs* evtArgs = TouchEventArgs::create(touchId,mousePos.x,mousePos.y,GetInputMgr().getTouchState(touchId)) ;
					m_touchReleasedEvent.enqueue(evtArgs,this) ;
				}
			}
			else // not inside quad
			{
				if ( m_wasDownLastFrame[touchId] && m_wasInsideLastFrame[touchId] )
				{
					TouchEventArgs* evtArgs = TouchEventArgs::create(touchId,mousePos.x,mousePos.y,GetInputMgr().getTouchState(touchId)) ;
					m_touchLeaveEvent.enqueue(evtArgs,this) ;
				}

				m_wasInsideLastFrame[touchId] = false ;
			}
		}   // for

	}
}

/// Override this method in children classes
void Node::render()
{
}

Action* Node::runAction( Action* action )
{
	JAM_ASSERT_MSG(action, ("%s : null 'action' parameter",JAM_FUNCTION_NAME));
	GetActionMgr().addAction(action,this,!m_running) ;
	return action ;
}
Action* Node::runAction( Action* action,  const TagType& tag)
{
	JAM_ASSERT_MSG(action, ("%s : null 'action' parameter",JAM_FUNCTION_NAME));
	action->setTag(tag);
	GetActionMgr().addAction(action,this,!m_running) ;
	return action;	
	//return runAction(action) ;
}

void Node::stopActionsByTag(const TagType& tag)
{
 	std::vector<Action*> actions ;
	auto range = GetActionMgr().findObjectsByTag(tag) ;
 	for( auto k = range.first; k!=range.second; k++ )
 		stopAction((*k).second);
}


void Node::stopAllActions( void )
{
	GetActionMgr().removeAllActionsFromTarget(this) ;
	clearActionFlags( Node::ActionFlags::MOVING | Node::ActionFlags::ROTATING | Node::ActionFlags::ANIMATING ) ;
}

void Node::stopAction( Action* action )
{
	GetActionMgr().removeAction(action) ;
}

void Node::stopActionByName( const String& name )
{
	GetActionMgr().removeActionByName(name,this) ;
}

Action* Node::getActionByName( const String& name )
{
	return GetActionMgr().getActionByName(name,this) ;
}

int Node::numberOfRunningActions( void )
{
	return GetActionMgr().getNumberOfRunningActionsInTarget(this) ;
}

void Node::setAttribute( const String& key, const String& val )
{
	if( key == "status" ) {
		setStatus( std::stoi(val) ) ;
	}
	else if( key == "visible" ) {
		setVisible( jam::stob(val) ) ;
	}
	else {
		m_attributes[key] = val ;
	}
}

String Node::getAttribute( const String& key )const
{
	if( key == "status" ) {
		return to_string(m_status) ;
	}
	else if( key == "visible" ) {
		return jam::btos(m_visible) ;
	}
	else {
		String val ;
		map<String,String>::const_iterator it = m_attributes.find(key) ;
		if( it!=m_attributes.end() ) {
			val = it->second ;
		}
		return val ;
	}
}

void Node::setCollisionType( int collType, bool recursive/*=false*/ )
{
	m_savedCollType = collType;	// avoid resume before pause issue
	m_justCollisionPaused=false;

	m_collType = collType;
	clearCollisions();
	if( recursive ) {
		NodesList::iterator it;
		for( it = m_children.begin(); it != m_children.end(); it++) {
			(*it)->setCollisionType(collType,recursive) ;
		}
	}
}

void Node::addCollision( const ObjCollision* n )
{
	//IwDebugTraceLinePrintf("Starting %s.m_colls.size()=%d", getName().c_str(), m_colls.size());
	//IwDebugTraceLinePrintf("Checking %s with %s", getName().c_str(), n->with->getName().c_str() );
	if( this != n->with ) {
		for( size_t i=0; i<m_colls.size(); i++ ) {
			if( m_colls[i]->with == n->with ) {
				return ;
			}
		}

		//		IwDebugTraceLinePrintf( "Adding %s for %s",n->with->getName().c_str(), getName().c_str() );
		m_colls.push_back(n);
		//		IwDebugTraceLinePrintf("Ending %s.m_colls.size()=%d", getName().c_str(), m_colls.size());
	}
}

void Node::clearCollisions()
{
	m_colls.clear() ;
}

void Node::setActionFlags( U8 flags )
{
	m_actionFlags = m_actionFlags | flags ;
//		JAM_TRACE("set action flag: %d\n", m_actionFlags) ;
}
	
void Node::clearActionFlags( U8 flags )
{
	m_actionFlags = m_actionFlags & (~flags) ;
//		JAM_TRACE("clear action flag: %d\n", m_actionFlags) ;
}

size_t Node::countCollisions() const
{
	return m_colls.size();
}

Node* Node::collided( int type ) const
{
	Node* n =  0;
	CollisionsList::const_iterator it;
	const CollisionsList& c = getCollisions();
	for( it=c.begin();it!=c.end();++it ){
		const ObjCollision *c=*it;
		if( c->with->getCollisionType()==type ) {
			n = c->with;
			break ;
		}
	}
	return n;
}

Node* Node::collisionNode( int index )
{
	return m_colls[index]->with;
}

bool Node::collide( Node* other, CollisionManager::Method m ) const
{
	return false ;
}

void Node::updateLocalTForm()
{
	m_local_tform = jam::createScaleMatrix2D(m_local_scl) ;
	m_local_tform *= m_local_rot ;
	jam::setTranslate(m_local_tform,m_local_pos) ;
	m_local_tform_dirty = false ;
}

void Node::updateWorldTForm()
{
	// inverted mul order
	m_world_tform = m_parent ? m_parent->getWorldTform() * getLocalTform() : getLocalTform() ;
	m_world_tform_dirty = false ;
}

void Node::updateOBB()
{
	m_obb = Polygon2f(m_aabb) ;
	m_obb.transform( getWorldTform() ) ;

	m_transformedBoundingCircle = m_boundingCircle ;
	m_transformedBoundingCircle.transform( getWorldTform() ) ;

	if( canCollide() && m_collisionScaleFactor != COLLISION_MANAGER_DEFAULT_SCALE_FACTOR ) {
		m_collisionBoundingCircle = m_transformedBoundingCircle ;
		m_collisionBoundingCircle.scale( m_collisionScaleFactor ) ;

		m_collisionObb = Polygon2f(m_aabb) ;
		Matrix3 mat = jam::createScaleMatrix2D( Vector2(m_collisionScaleFactor) ) ;
		m_collisionObb.transform( mat*getWorldTform() ) ;
	}

	m_obbDirty = false ;
}

void Node::invalidateLocal()
{
	m_local_tform_dirty = true ;
	invalidateWorld() ;
	invalidateOBB() ;
}

void Node::invalidateWorld()
{
	if( !m_world_tform_dirty ) {
		m_world_tform_dirty = true ;
		for( NodesList::iterator it = m_children.begin(); it != m_children.end(); it++ ) {
			(*it)->invalidateWorld() ;
		}
	}
}

void Node::invalidateOBB()
{
	if( !m_obbDirty ) {
		m_obbDirty = true ;
		for( NodesList::iterator it = m_children.begin(); it != m_children.end(); it++ ) {
			(*it)->invalidateOBB() ;
		}
	}
}

void Node::setAABB( const AABB& aabb )
{
	m_aabb = aabb ;
	m_boundingCircle = Circle2f( aabb ) ;
	invalidateOBB() ;
}

void Node::onEnter()
{
	for( NodesList::iterator it = m_children.begin(); it != m_children.end(); it++ ) {
		(*it)->onEnter() ;
	}

	GetActionMgr().resumeTarget(this);

	m_running = true;
}

void Node::onExit()
{
	GetActionMgr().pauseTarget(this);

	m_running = false;

	for( NodesList::iterator it = m_children.begin(); it != m_children.end(); it++ ) {
		(*it)->onExit() ;
	}
}

uint64_t Node::getLifeTime() const
{
	return GetAppMgr().getTotalElapsedMs() - m_lifeTime;
}

void Node::insertChild( Node* child, int z )
{
	int index=0;
	bool added = false;

	if(m_children.size() > 0) {
		Node* pNode;
		NodesList::iterator it;
		for( it = m_children.begin(); it != m_children.end(); it++)
		{
			pNode = (*it);

			if ( pNode && pNode->m_ZOrder > z )
			{
				added = true;
				m_children.insert(it,child);
				break;
			}
		}
	}

	if( !added )
	{
		m_children.push_back(child);
	}

	child->setZOrder(z);
}

void Node::setZOrder( int z )
{
	m_ZOrder = z;
}

void Node::detachChild( Node *child, bool doCleanup )
{
//		JAM_ASSERT_MSG( VisitInProgress==false, ("Removing node while updating is not recommended") ) ;

	// IMPORTANT:
	//  -1st do onExit
	//  -2nd cleanup
	if (m_running)
	{
		child->onExit();
	}

	// If you don't do cleanup, the child's actions will not get removed and the
	// its scheduledSelectors_ dict will not get released!
	if (doCleanup)
	{
		child->stopAllActions();
	}

	m_children.remove(child);

	// set parent nil at the end
	child->setParent(nullptr);
}

void Node::setTouchable( bool val/*=true*/ )
{
	m_touchable = val;
}

void Node::startDrag(int32_t touchId)
{
	m_isDragging = true ;
	m_draggingTouchId = touchId ;
}

void Node::endDrag()
{
	m_isDragging = false ;
}

bool Node::isInView() 
{
	m_isInViewCalculated=true;
	m_is_in_view = getTransformedBoundingCircle().intersects( GetAppMgr().getScene()->getBoundingCircle() ) ;
#ifdef JAM_TRACE_ACTIVE_NODES
	if(m_is_in_view) m_totInView++ ;
#endif		
	return m_is_in_view ;
}


bool Node::isInViewActive(bool forceVerify) 
{
	if (!m_isInViewCalculated) 
	{
		m_isInViewCalculated=true;
		if (forceVerify || !isVisible() || !isEnabled()) m_is_in_view=false;
		else return isInView() ;
	}
	return m_is_in_view;
}

Vector2 Node::TransformPoint( const Vector2& v, Node *src, Node *dest/*=0 */ )
{
	Vector2 tformed = v;
	if( src ) tformed = jam::transform( src->getWorldTform(), tformed );
	if( dest ) tformed = jam::transform( glm::inverse(dest->getWorldTform()), tformed );
	return tformed ;
}

Vector2 Node::TransformVector( const Vector2& v, Node *src, Node *dest/*=0 */ )
{
	Vector2 tformed = v;

	Matrix3 m(1.0f) ;
	if( src ) {
		m = src->getWorldTform() ;
		setTranslate(m,Vector2(0,0)) ;
		tformed = transform( m, tformed ) ;
	}

	if( dest ) {
		m = glm::inverse( dest->getWorldTform() ) ;
		setTranslate(m,Vector2(0,0)) ;
		tformed= transform( m, tformed ) ;
	}
	return tformed ;
}

Vector2 Node::TransformNormal( const Vector2& v, Node *src, Node *dest/*=0*/ )
{
	Vector2 tformed = v;
	if( src ) tformed = transformNormal( src->getWorldTform(), tformed );
	if( dest ) tformed = transformNormal( glm::inverse(dest->getWorldTform()), tformed );
	tformed = glm::normalize(tformed);
	return tformed ;
}

float Node::getDistance( const jam::Node* other ) const
{
	return glm::distance( getWorldPos(), other->getWorldPos() );
}

float Node::getDistanceSquared( const jam::Node* other ) const
{
	return glm::distance2( getWorldPos(), other->getWorldPos() );
}

void Node::faceTo( const Node& n, bool global /*= true*/ )
{
	if (global) {
		Vector2 delta = n.getWorldPos() - getWorldPos();
		setWorldRotationAngle(atan2f( delta.x, delta.y));
	}
	else {
		Vector2 delta = n.getPos() - getPos();
		setRotationAngle(atan2f( delta.x, delta.y));
	}
}

void Node::setGrid( GridBase* val )
{
	m_pGrid = val;
}

#ifdef JAM_DEBUG

String Node::getDebugInfo(bool typeInfo/*=true*/)
{
	stringstream oss ;
	oss << getDebugInfo(typeInfo) ;
	oss << " wpos.x=" << getWorldPos().x ;
	oss << " wpos.y=" << getWorldPos().y ;
	oss << " wangle=" << getWorldRotationAngle() ;
	oss << " wscale.x=" << getWorldScale().x ;
	oss << " wscale.y=" << getWorldScale().y ;
	return oss.str();
}

#ifdef JAM_TRACE_ACTIVE_NODES
void Node::addDebugNodeInfo(Node* n)
{
	m_debugNodeInfoTypes[typeid(*n).name()].count++ ;
}

void Node::dumpDebugNodeInfo()
{
	JAM_TRACE(( "Active nodes in scene -----------------------------")) ;
	JAM_TRACE(("Total nodes in scene: %d", m_totCount)) ;
	JAM_TRACE(("Number of nodes in view: %d", m_totInView)) ;
	for( map<String,DebugNodeInfo>::iterator it=m_debugNodeInfoTypes.begin(); it != m_debugNodeInfoTypes.end(); it++ ) {
		JAM_TRACE( ( "Number of nodes of type '%s': %d", it->first.c_str(), it->second.count) );
	}
}

void Node::clearDebugNodeInfo()
{
	m_debugNodeInfoTypes.clear() ;
	m_totInView = 0 ;
}
#endif

#endif

void Node::pauseCollisions()
{
	if (!m_justCollisionPaused)
	{
		m_savedCollType = m_collType;
		m_collType = 0;
		m_justCollisionPaused=true;
	}
}

void Node::resumeCollisions()
{
	if (m_justCollisionPaused)
	{
		m_collType = m_savedCollType;
		m_savedCollType=0;
		m_justCollisionPaused=false;
	}
}

// Note: paused nodes are not considered in collisions detection, see enumEnabled
void Node::pause()
{
	onExit() ;
	onPause() ;
}

void Node::resume()
{
	onEnter() ;
	onResume() ;
}

void Node::togglePause()
{
	if( m_running ) {
		pause() ;
	}
	else {
		resume() ;
	}
}

void Node::onPause()
{
}

void Node::onResume()
{
}

const Polygon2f& Node::getCollisionOBB() const
{
	if( m_obbDirty ) {
		const_cast<Node*>(this)->updateOBB() ;
	}

	if( m_collisionScaleFactor == COLLISION_MANAGER_DEFAULT_SCALE_FACTOR ) {
		return m_obb ;
	}
	else {
		return m_collisionObb ;
	}
}

const Circle2f& Node::getCollisionBoundingCircle() const
{
	if( m_obbDirty ) {
		const_cast<Node*>(this)->updateOBB() ;
	}

	if( m_collisionScaleFactor == COLLISION_MANAGER_DEFAULT_SCALE_FACTOR ) {
		return m_transformedBoundingCircle ;
	}
	else {
		return m_collisionBoundingCircle ;
	}
}

void Node::setCamera( Camera* pCamera )
{
	m_pCamera = pCamera ;
}
	
Camera* Node::getCamera()
{
	return m_pCamera ;
}
	
Camera* Node::getCurrentCamera()
{
	return m_pCurrentCamera ;
}
	
void Node::setCurrentCamera( Camera* pCamera )
{
	m_pCurrentCamera = pCamera ;
}
	
Camera* Node::getAncestorCamera()
{
	if( !m_pCamera ) {
		// safety check!
		if( m_parent == 0 ) return 0 ;
		return m_parent->getAncestorCamera() ;
	}
	
	return m_pCamera ;
}
	
CollisionEventArgs* CollisionEventArgs::create( Node* src, Node* dst )
{
	CollisionEventArgs* collEvtArgs = new (GC) CollisionEventArgs(src,dst) ;
	return collEvtArgs ;
}

void Node::setActionSpeed( float val/*=1.0f*/ )
{
	m_actionSpeed = val;
}

#ifdef JAM_DEBUG
void Node::dumpNodeHierarchy( Node* pNode, int level /*= 0*/ )
{
	Node* pChild;
	NodesList::const_iterator it;

	char buffer[256] = {0};

	if( !pNode ) {
		return ;
	}

	for( int i=0; i<level; i++ ) {
		buffer[i] = '\t' ;
	}
	if( typeid(Scene) == typeid(*pNode) ) {
		sprintf( &buffer[level], "Scene root node (children: %u)", pNode->getChildren().size() ) ;
	}
	else if( typeid(Layer) == typeid(*pNode) ) {
		sprintf( &buffer[level], "Layer (name: \"%s\", z-order: %d, children: %u)",
			pNode->getName().c_str(),
			pNode->getZOrder(),
			pNode->getChildren().size() ) ;
	}
	else if( typeid(ColorLayer) == typeid(*pNode) ) {
		sprintf( &buffer[level], "Color layer (name: \"%s\", z-order: %d, children: %u)",
			pNode->getName().c_str(),
			pNode->getZOrder(),
			pNode->getChildren().size() ) ;
	}
	else if( typeid(Pivot2D) == typeid(*pNode) ) {
		sprintf( &buffer[level], "Pivot2D (name: \"%s\", children: %u)",
			pNode->getName().c_str(),
			pNode->getChildren().size() ) ;
	}
	//else if( dynamic_cast<Sprite*>(pNode) != 0 ) {
	//	sprintf( &buffer[level], "%s (name: \"%s\", frame: \"%s\", sheet: \"%s\", children: %d)", 
	//		typeid(*pNode).name(),
	//		pNode->getName().c_str(), 
	//		dynamic_cast<Sprite*>(pNode)->getFrame()->getName().c_str(),
	//		dynamic_cast<Sprite*>(pNode)->getFrame()->getTexture()->DebugGetName(),
	//		pNode->getChildren().size() ) ;
	//}
	else if( dynamic_cast<TextNode*>(pNode) != 0 ) {
		sprintf( &buffer[level], "TextNode (name: \"%s\", text: \"%s\", sheet: \"%s\", children: %u)", 
			pNode->getName().c_str(), 
			dynamic_cast<TextNode*>(pNode)->getText().c_str(), 
			dynamic_cast<TextNode*>(pNode)->getDrawItemName().c_str(),
			pNode->getChildren().size() ) ;
	}
	else {
		sprintf( &buffer[level], "%s (name: \"%s\", children: %u)",
			typeid(*pNode).name(),
			pNode->getName().c_str(),
			pNode->getChildren().size() ) ;
	}

	if( !pNode->m_enabled ) {
		strcat( &buffer[level], ", NOT-ENABLED" ) ;
	}

	if( !pNode->m_visible ) {
		strcat( &buffer[level], ", NOT-VISIBLE" ) ;
	}

	JAM_TRACE( (buffer) ) ;

	if( pNode->getChildren().size() > 0) {
		memset(buffer,0,sizeof(buffer)) ;
		for( int i=0; i<level; i++ ) {
			buffer[i] = '\t' ;
		}
		strcat( &buffer[level], "{" ) ;
		JAM_TRACE( (buffer) ) ;

		// draw children zOrder < 0 (the ones behind the current node)
		for( it = pNode->getChildren().begin(); it != pNode->getChildren().end(); it++) {
			pChild = (*it);

			if ( pChild && pChild->m_ZOrder < 0 ) {
				dumpNodeHierarchy(pChild, level+1);
			}
			else {
				break;
			}
		}

		// draw children zOrder >= 0 (the ones in front of the current node)
		for ( ; it!=pNode->getChildren().end(); it++ ) {
			pChild = (*it);
			if (pChild) {
				dumpNodeHierarchy(pChild, level+1);
			}
		}

		memset(buffer,0,sizeof(buffer)) ;
		for( int i=0; i<level; i++ ) {
			buffer[i] = '\t' ;
		}
		strcat( &buffer[level], "}" ) ;		
		JAM_TRACE( (buffer) ) ;
	}
}
#endif

}

