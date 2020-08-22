/**********************************************************************************
* 
* AliveSprite.cpp
* 
* This file is part of Jam
* 
* Copyright (c) 2014-2019 Giovanni Zito, Gianluca Sclano
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

#include "jam/AliveSprite.h"

#include <jam/Application.h>
#include <jam/Draw3dManager.h>
#include <jam/SpriteManager.h>
#include <jam/DrawItemManager.h>
#include <jam/Draw3dManager.h>
#include <jam/core/math.h>
#include <jam/Animation2dManager.h>
#include <jam/Scene.h>

#include <math.h>

using namespace std ;

namespace jam
{

AliveSprite::AliveSprite() :
	Sprite(), m_direction(1),m_autodestroy(false)
{
	init() ;
}

AliveSprite::AliveSprite( const String& frameName ) :
	Sprite(), m_direction(1)
{
	show(getX(), getY(), frameName);
}

AliveSprite::AliveSprite( DrawItem* item ) :
	Sprite(item), m_direction(1)
{

}


AliveSprite::~AliveSprite()
{
}

// *************************************************************************************

void AliveSprite::setLifeCycleStatus(LifeCycleStatus val) 
{
	setOldStatus(getLifeCycleStatus());
#ifdef CHECKLIFE
	if (!GetNodeStateMgr().check(to_string((LifeCycleStatus)getStatus()),to_string(val))) {
		JAM_TRACE( "Cannot change status from [%d] to [%d]",getStatus(),val );
	}
#endif		
	setStatus((int)val); 
}

void AliveSprite::init()
{	
	setPos( Vector2(0) ) ;
	setScale(1.0f);
	setLifeCycleStatus(INITIALIZED);
}

void AliveSprite::update()
{
	Sprite::update();
	switch (getStatus())
	{
	case INITIALIZED:
		setVisible(false);
		return; // *** No Rendering
		break;
	case DESTROYED:
		// *** Marcami da distruggere!
		if(m_autodestroy) 
			destroy();
		return; // *** No Rendering
		break;
	case WAITING:
		setVisible(false);
		// *** Can be created
		break;

	case READY:	
		setVisible(true);
		// ** Positioned by emitter
		break;
	}

}


void AliveSprite::hitted()
{
	setLifeCycleStatus(HITTED);	
	stopAllActions();
}
void AliveSprite::killed()
{
	setLifeCycleStatus(DESTROYED);	
	stopAllActions();
}

void AliveSprite::created()
{
	setLifeCycleStatus(CREATED);	
}

void AliveSprite::show()
{
	setLifeCycleStatus(READY);	
}

void AliveSprite::show( float x, float y, const String& frameName /* = "" */ )
{
	setPos( Vector2(x,y) );
	setLifeCycleStatus(READY);	
	if ( !frameName.empty() )	{
		getAnimator().setAnimation( jam::GetAnim2DMgr().getPtrByName(frameName), true, 0 ) ;
		getAnimator().setSpeed(1);
	}
}

jam::Animation2D* AliveSprite::animate( const String& animName, int startframe/*=0*/, bool followdirection/*=true*/, bool looping/*=true*/ )
{
	jam::Animation2D* currAnim=jam::GetAnim2DMgr().getPtrByName((animName));
	currAnim->setLoop(looping); 
	if (followdirection) currAnim->setFlipAllX(m_direction<0);
	getAnimator().setAnimation( currAnim, true,(startframe) ) ;
	return currAnim;
}


float AliveSprite::facingToWorldPos( const Vector2& vdest )
{
	Vector2 delta = vdest - getWorldPos();
	float angle = ToDegree(atan2f(delta.x,delta.y)) ;
	setWorldRotationAngle(angle);
	return angle;
}

float AliveSprite::facingToPos( const Vector2& vdest )
{
	Vector2 delta = vdest - getPos();
	float angle = ToDegree(atan2f(delta.x,delta.y)) ;
	setRotationAngle(angle);
	return angle;
}


void AliveSprite::render()
{
	m_oldPos = getPos();
	Sprite::render() ;
}


String AliveSprite::retrieveName( LifeCycleStatus status )
{
	//	INITIALIZED=-10, DISABLED=-8, CREATED=-9, IDLE=-1, WAITING=0, STARTING=1, READY=2, WORKING=10,COLLIDED=18, HITTED=20, ARRIVED=30, THROWING=31,DESTROYED=99
	String statusName = "---";	
	if (status == INITIALIZED) statusName = "INITIALIZED";
	else if (status == DISABLED) statusName = "DISABLED";
	else if (status == CREATED) statusName = "CREATED";
	else if (status == IDLE) statusName = "IDLE";
	else if (status == WAITING) statusName = "WAITING";
	else if (status == APPEARED) statusName = "APPEARED";
	else if (status == READY) statusName = "READY";
	else if (status == WORKING) statusName = "WORKING";
	else if (status == COLLIDED) statusName = "COLLIDED";
	else if (status == HITTED) statusName = "HITTED";
	else if (status == ARRIVED) statusName = "ARRIVED";
	else if (status == THROWING) statusName = "THROWING";
	else if (status == DESTROYED) statusName = "DESTROYED";
	return statusName;
}


IAliveSpriteFactory::~IAliveSpriteFactory()
{

}

// ************************************************************************
// *** AliveSprite Factory
// ************************************************************************

AliveSprite* AliveSpriteFactory::create()
{
	return JAM_ALLOC(AliveSprite);
}

// ************************************************************************
// *** AliveSprite Factory Manager
// ************************************************************************

std::vector<AliveSprite*> AliveSpriteFactoryManager::createSome( const TagType& tag, int howmany/*=1*/, Node* parent/*=SCENE_PARENT*/ )
{
	std::vector<AliveSprite*> resv;
	resv.reserve(howmany);
	resv.clear();

	AliveSprite* pCreated=0;
	IAliveSpriteFactory* factory=0;
	for( int i = 0; i < howmany; i++) {
		factory = m_FactoryMap[tag];
		pCreated = factory->create();
		pCreated->autorelease() ;
		pCreated->setTag(tag);
		add(pCreated) ;

		if (parent == SCENE_PARENT)
			parent=jam::GetAppMgr().getScene();

		parent->addChild(pCreated);

		resv.push_back(pCreated);
	}
	return resv;	
}


AliveSprite* AliveSpriteFactoryManager::create( const TagType& tag, int zOrder/*=0*/, Node* parent/*=SCENE_PARENT*/ )
{
	AliveSprite* pCreated=0;
	IAliveSpriteFactory* factory=0;
	size_t i = m_FactoryMap.size();
	
	factory = m_FactoryMap[tag];
	JAM_ASSERT_MSG(factory!=0,  "AliveSpriteFactoryManager::Create failed on [%s]", tag.c_str() );
	pCreated = factory->create();
	pCreated->autorelease() ;
	pCreated->setTag(tag);
	add(pCreated) ;

	if (parent==SCENE_PARENT)
		parent=jam::GetAppMgr().getScene();

	if (parent)
		parent->addChild(pCreated, zOrder);

	return pCreated;
}


void AliveSpriteFactoryManager::update()
{
	// ** Qui dovrebbe verificare quelli da distruggere
}

AliveSpriteFactoryManager::AliveSpriteFactoryManager() : SpriteManager()
{
	m_FactoryMap.clear();
}

AliveSpriteFactoryManager::~AliveSpriteFactoryManager()
{
	destroyAll();	
}

void AliveSpriteFactoryManager::subscribe( const TagType& tag, IAliveSpriteFactory* factory )
{
	m_FactoryMap[tag] = factory; // *** gestire errore se già esiste
}

void AliveSpriteFactoryManager::destroyAll()
{
	std::map<TagType, IAliveSpriteFactory*>::iterator iter ;
	for(iter = m_FactoryMap.begin(); iter != m_FactoryMap.end(); iter++) {
		IAliveSpriteFactory* cf = (IAliveSpriteFactory*) iter->second ;
		JAM_DELETE(cf) ;
		JAM_TRACE( "Delete AliveSpriteFactory[%s]", (iter->first).c_str() ) ;
	}

	m_FactoryMap.clear() ;
}

int AliveSpriteFactoryManager::garbageDestroyed( const TagType& tag )
{
	std::vector<jam::Sprite*> allTargets ;
	size_t n = queryByTag(tag,allTargets);
	if (n==0) return 0;
	AliveSprite* tg=0;
	int totalDestroyed=0;
	for (size_t i=0; i<n; i++) { 
		tg=(AliveSprite*)allTargets[i];
		if (tg && tg->getStatus()==AliveSprite::DESTROYED) {	tg->destroy(); totalDestroyed++; 	}	
	}
	return totalDestroyed;
}

// ***************************************************************************************


void NodeStateManager::attach( const String& nodeFrom, const String& nodeTo )
{
	NodeMap nmap=m_nodeLinks[(NodeState&)nodeFrom];
	nmap[(NodeState&)nodeTo]=true;
	m_nodeLinks[(NodeState&)nodeFrom] = nmap;
}

bool NodeStateManager::check( const String& nodeFrom, const String& nodeTo )
{
	if (m_nodeLinks.size()==0) return true;
	NodeMap nmap=m_nodeLinks[(NodeState&)nodeFrom];
	return nmap[(NodeState&)nodeTo];	
}

void NodeStateManager::clearAll()
{
	m_nodeLinks.clear();
}

void NodeStateManager::detach( const String& nodeFrom, const String& nodeTo )
{
	NodeMap nmap=m_nodeLinks[(NodeState&)nodeFrom];
	nmap[(NodeState&)nodeTo]=false;
	m_nodeLinks[(NodeState&)nodeFrom] = nmap;
}

}
