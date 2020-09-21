/**********************************************************************************
* 
* Scene.cpp
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
#include "jam/Scene.h"
#include "jam/Draw3dManager.h"
#include "jam/Draw3dBatch.h"
#include "jam/StateMachine.h"
#include "jam/Gfx.h"
#include "jam/core/geom.h"
#include "jam/Camera.h"

namespace jam
{

	Scene::Scene() :
		m_fogOfViewRing(),
		m_fogOfViewInnerRadius(0.0f),
		m_fogOfViewOuterRadius(0.0f),
		m_fogOfViewEnabled(false)
	{
	}

	void Scene::init()
	{
		float LDrawHalfXSize = (float)Draw3DManager::HalfOriginal3DWidth;
		float LDrawHalfYSize = (float)Draw3DManager::HalfOriginal3DHeight;

		AABB aabb( 0,0, LDrawHalfXSize, LDrawHalfYSize ) ;
		setAABB( aabb ) ;
		invalidateOBB() ;

		resetFogOfView();

		if( !m_pCamera ) {
			Camera* pCamera = new Camera() ;
			setCamera( pCamera ) ;
		}
	}


	void Scene::visitGraph( Draw3DBatch* pBatch )
	{
		Matrix4 globalScaleMat = jam::createScaleMatrix3D( Vector3(Draw3DManager::RatioX, Draw3DManager::RatioY, 1.0f) ) ;
		GetShaderMgr().getCurrent()->setModelMatrix(globalScaleMat) ;

#ifdef JAM_TRACE_BATCH
		JAM_TRACE( ("Draw3DBatch::begin visitGraph") ) ;
#endif

		Node::VisitInProgress = true ;
		if( pBatch ) pBatch->begin() ;
		visit() ;
		game::State* gState = game::GetStateMachine().isStarted() ? game::GetStateMachine().getCurrentState() : 0 ;
		if( gState ) {
			gState->extraRender() ;
		}
		
		if( pBatch ) pBatch->end() ;
		Node::VisitInProgress = false ;

#ifdef JAM_TRACE_BATCH
		JAM_TRACE( ("Draw3DBatch::end visitGraph") ) ;
#endif
	}

	void Scene::destroy()
	{
		while( !m_children.empty() ) {
			Node* n = m_children.front() ;
			n->destroy() ;	// destroy also remove from m_children
		}
	}

	void Scene::resetFogOfView()
	{
		float LDrawHalfXSize = (float)Draw3DManager::HalfOriginal3DWidth;
		float LDrawHalfYSize = (float)Draw3DManager::HalfOriginal3DHeight;
		if( m_fogOfViewEnabled ) {

			// apply default ring size
			if( m_fogOfViewRing.isZero() ) {
				Ring2f fogOfView( Circle2f(Vector2(0,0),LDrawHalfXSize), m_boundingCircle ) ;
				setFogOfViewRing( fogOfView ) ;
			}

			// apply user defined ring size
			if( m_fogOfViewInnerRadius != 0.0f || m_fogOfViewOuterRadius != 0.0f ) {
				if( m_fogOfViewInnerRadius != 0) {
					m_fogOfViewRing.setInnerRing(Circle2f(Vector2(0,0),m_fogOfViewInnerRadius)) ;
				}
				if( m_fogOfViewOuterRadius != 0) {
					m_fogOfViewRing.setOuterRing(Circle2f(Vector2(0,0),m_fogOfViewOuterRadius)) ;
				}
			}
		}
	}


	void Scene::addTouchableNode( Node* pNode )
	{
		Ref<Node> rNode( pNode, true ) ;
		m_touchableNodes.push_front(rNode) ;
	}


	void Scene::clearTouchableNodes()
	{
		m_touchableNodes.clear() ;
		for( size_t i=0; i<JAM_MAX_TOUCHES; i++ ) {
			m_touches[i] = false ;
			m_touchedNodes[i] = 0 ;
		}
	}


	void Scene::updateTouchableNodes()
	{
		size_t n = m_touchableNodes.size() ;
		for( NodesList::iterator it = m_touchableNodes.begin(); it != m_touchableNodes.end(); it++) {
			(*it)->updateTouches() ;
		}
	}

}
