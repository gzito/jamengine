/**********************************************************************************
* 
* SpritesDemo.cpp
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

#include <jam/Application.h>
#include <jam/Draw3dManager.h>
#include <jam/InputManager.h>
#include <jam/DrawItemManager.h>
#include <jam/Animation2dManager.h>
#include <jam/ActionInterval.h>
#include <jam/ActionInstant.h>
#include <jam/ActionEase.h>
#include <jam/CollisionManager.h>
#include <jam/Scene.h>
#include <jam/Sprite.h>
#include <jam/Camera.h>
#include <jam/Gfx.h>
#include <jam/Shader.h>
#include <jam/core/geom.h>
#include <jam/core/bmkextras.hpp>
#include <jam/core/filesystem.h>

#include "SpritesDemo.h"


using namespace jam;


SpritesDemo::SpritesDemo() :
	m_pSprMgr(nullptr),
	m_angle(0),
	m_initialSpeed(1000),
	m_sonicName( "sonwalk2_" ),
	m_gemSheetName( "gemsheet_" )
{
}

bool SpritesDemo::init()
{
	Ref<Sprite> aSprite ;
	Animation2D* pAnim = 0 ;

	Draw3DManager::Origin3D(1920,1080) ;
	setClearColor( Color::MIDNIGHTBLUE ) ;

	//	front face is clockwise
 	glFrontFace(GL_CW) ;
 	glCullFace(GL_BACK) ;
 	glEnable(GL_CULL_FACE) ;

	GetDrawItemMgr().loadSheet("./images/sonwalk2.png","spritesheets",64,64,8,1);
	GetDrawItemMgr().loadSheet("./images/gemsheet.png","spritesheets",64,64,15,14);

	m_pSprMgr = new SpriteManager() ;

	GetCollMgr().setCollisions(2,2,CollisionManager::Method::BoundingBox,0) ;

	// create sonic animations
	// sonwalk_right

	pAnim = Animation2D::create();
	pAnim->setName("sonwalk_right") ;
	GetAnim2DMgr().addObject(pAnim);
	pAnim->addFrame( GetDrawItemMgr().getObject(m_sonicName+"0"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_sonicName+"1"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_sonicName+"2"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_sonicName+"3"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_sonicName+"4"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_sonicName+"5"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_sonicName+"6"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_sonicName+"7"), 0.1f ) ;
	pAnim->setLoop(true);

	// sonwalk_left
	pAnim = Animation2D::create();
	pAnim->setName("sonwalk_left") ;
	GetAnim2DMgr().addObject(pAnim);
	pAnim->addFrame( GetDrawItemMgr().getObject(m_sonicName+"0"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_sonicName+"1"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_sonicName+"2"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_sonicName+"3"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_sonicName+"4"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_sonicName+"5"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_sonicName+"6"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_sonicName+"7"), 0.1f ) ;
	pAnim->setFlipAllX(true);	// use setFlipAllX to flip animation around X
	pAnim->setLoop(true);

	// define gem animation : yellow_gem_anim
	pAnim = Animation2D::create();
	pAnim->setName("yellow_gem_anim") ;
	GetAnim2DMgr().addObject(pAnim);
	pAnim->addFrame( GetDrawItemMgr().getObject(m_gemSheetName+"0"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_gemSheetName+"1"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_gemSheetName+"2"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_gemSheetName+"3"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_gemSheetName+"4"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_gemSheetName+"5"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_gemSheetName+"6"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_gemSheetName+"7"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_gemSheetName+"8"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_gemSheetName+"9"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_gemSheetName+"10"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_gemSheetName+"11"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_gemSheetName+"12"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_gemSheetName+"13"), 0.1f ) ;
	pAnim->addFrame( GetDrawItemMgr().getObject(m_gemSheetName+"14"), 0.1f ) ;
	pAnim->setLoop(true);

	// create the sonic sprite
	aSprite = new Sprite() ;
	aSprite->setTag("player") ;
	aSprite->setScale(2) ;
	aSprite->setX(-120);
	aSprite->getAnimator().setAnimation( GetAnim2DMgr().getObject("sonwalk_right"), false, 4 ) ;
	aSprite->getAnimator().setSpeed(1);
	aSprite->setFrame( GetDrawItemMgr().getObject(m_sonicName+"4") ) ;
	aSprite->setName("sonic") ;
	m_pSprMgr->addObject(aSprite) ;
	this->getScene()->addChild(aSprite);

	// create the gem sprite (yellow)
	aSprite = new Sprite() ;
	aSprite->setCollisionType(2) ;
	aSprite->setTag("gems") ;
	aSprite->setPos(Vector2(0,60));
	aSprite->getAnimator().setAnimation( GetAnim2DMgr().getObject("yellow_gem_anim"), true ) ;
	aSprite->getAnimator().setSpeed(4) ;
	aSprite->setName("yellow_gem") ;
	m_pSprMgr->addObject(aSprite) ;
	this->getScene()->addChild(aSprite);

	// create second gem (gray)
	aSprite = new Sprite() ;
	aSprite->setCollisionType(2) ;
	aSprite->setTag("gems") ;
	aSprite->setFrame( GetDrawItemMgr().getObject(m_gemSheetName+"30") ) ;
	aSprite->setPos( Vector2(0,-60) );
	RotateBy* actRotateBy = RotateBy::actionWithDuration(2,360);
	EaseInOut* actEase = EaseInOut::actionWithAction(actRotateBy,2);
	EaseInOut* actEaseReverted = dynamic_cast<EaseInOut*>(actEase->reverse());
	Sequence* actSequence = Sequence::actionOneTwo(actEase,actEaseReverted) ;
	RepeatForever* actRepeatForever = RepeatForever::actionWithAction(actSequence) ;
	aSprite->runAction(actRepeatForever);
	aSprite->setName("gray_gem") ;
	m_pSprMgr->addObject(aSprite) ;
	this->getScene()->addChild(aSprite);

	// create third gem (orange)
	aSprite = new Sprite() ;
	aSprite->setCollisionType(2) ;
	aSprite->setTag("gems") ;
	aSprite->setFrame( GetDrawItemMgr().getObject(m_gemSheetName+"150") ) ;
	aSprite->setPos( Vector2(0,-180) );
	actRotateBy = RotateBy::actionWithDuration(1,360);
	Repeat* actRepeat = Repeat::actionWithAction(actRotateBy,20) ;
	EaseIn* actEaseIn = EaseIn::actionWithAction(actRepeat,2);

	actRotateBy = RotateBy::actionWithDuration(1,-360);
	actRepeat = Repeat::actionWithAction(actRotateBy,20) ;
	EaseIn* actEaseIn2 = EaseIn::actionWithAction(actRepeat,2);

	actSequence = Sequence::actionOneTwo(actEaseIn,actEaseIn2) ;
	actRepeatForever = RepeatForever::actionWithAction(actSequence) ;
	aSprite->runAction(actRepeatForever);
	aSprite->setName("orange_gem") ;
	m_pSprMgr->addObject(aSprite) ;
	this->getScene()->addChild(aSprite);

	// create forth gem (green)
	aSprite = new Sprite() ;
	aSprite->setCollisionType(2) ;
	aSprite->setTag("gems") ;
	aSprite->setFrame( GetDrawItemMgr().getObject(m_gemSheetName+"180") ) ;
	aSprite->setPos( Vector2(100,60) );
	aSprite->setName("green_gem") ;
	m_pSprMgr->addObject(aSprite) ;
	actRotateBy = RotateBy::actionWithDuration(2,360);
	actRepeat = Repeat::actionWithAction(actRotateBy,5) ;
	DelayTime* delayTime = DelayTime::actionWithDuration(3) ;	
	actSequence = Sequence::actionOneTwo(actRepeat,delayTime);
	actRepeatForever = RepeatForever::actionWithAction(actSequence);
	aSprite->runAction(actRepeatForever);
	this->getScene()->addChild(aSprite);

	// create the fifth gem (purple)
	aSprite = new Sprite() ;
	aSprite->setTag("gems") ;
	aSprite->setFrame( GetDrawItemMgr().getObject(m_gemSheetName+"120") ) ;
	aSprite->setName("purple_gem") ;
	m_pSprMgr->addObject(aSprite) ;
	aSprite->setScale(0.5);
	m_pSprMgr->getObject("sonic")->addChild(aSprite) ;
	aSprite->setPos( Vector2(0,60) );				// position relative to the parent

	aSprite = new Sprite() ;
	aSprite->setCollisionType(2) ;
	aSprite->setTag("gems") ;
	aSprite->setFrame( GetDrawItemMgr().getObject(m_gemSheetName+"90") ) ;
	aSprite->setName("red_gem") ;
	m_pSprMgr->addObject(aSprite) ;
	m_pSprMgr->getObject("purple_gem")->addChild(aSprite) ;
	aSprite->setScale(0.5);
	aSprite->setPos( Vector2(0,60) );				// position relative to the parent

	// create the sixth gem (gray)
	aSprite = new Sprite() ;
	aSprite->setTag("gems") ;
	aSprite->setFrame( GetDrawItemMgr().getObject(m_gemSheetName+"30") ) ;
	aSprite->setPos( Vector2(-100,120) );
	aSprite->setName("gray_gem2") ;
	m_pSprMgr->addObject(aSprite) ;
	JumpTo* actJump = JumpTo::actionWithDuration(2,Vector2(100,-120),80,1) ;
	EaseIn* actJumpEaseIn = EaseIn::actionWithAction(actJump,1.5f) ;
	aSprite->setColor(Color::RED) ;
	aSprite->runAction(actJumpEaseIn);
	this->getScene()->addChild(aSprite);

	// create the sixth gem (gray)
	aSprite = new Sprite() ;
	aSprite->setTag("gems") ;
	aSprite->setFrame( GetDrawItemMgr().getObject(m_gemSheetName+"30") ) ;
	aSprite->setPos( Vector2(-100,120) );
	aSprite->setName("gray_gem3") ;
	m_pSprMgr->addObject(aSprite) ;
	actJump = JumpTo::actionWithDuration(2,Vector2(100,-120),80,1) ;
	DestroyTarget* actDestroy = DestroyTarget::action(aSprite) ;
	actSequence = Sequence::actionOneTwo(actJump,actDestroy) ;
	aSprite->runAction(actSequence);
	this->getScene()->addChild(aSprite);

	// set camera
	Camera* pCamera = new Camera() ;
	pCamera->setOrthographicProjection(	-Draw3DManager::VPWidth/2, Draw3DManager::VPWidth/2,
										-Draw3DManager::VPHeight/2, Draw3DManager::VPHeight/2,
										1.f, 50.f ) ;
	pCamera->lookAt( Vector3(0,0,30), Vector3(0,0,0), Vector3(0,1,0) ) ;
	getScene()->setCamera(pCamera) ;

	GetShaderMgr().getDefaultUnlit()->use() ;

	return true ;
}


void SpritesDemo::handleInput()
{
	Sprite* s = m_pSprMgr->getObject("sonic") ;

 	if( GetInputMgr().getPointerState(SDL_BUTTON_LEFT) == InputManager::DOWN ) {
 		if( s->getX() > Draw3DManager::MouseX3D ) {
 			s->getAnimator().setAnimation( GetAnim2DMgr().getObject("sonwalk_left"), true ) ;
		}
 		else {
 			s->getAnimator().setAnimation( GetAnim2DMgr().getObject("sonwalk_right"), true ) ;
 		}
 
		s->stopAllActions() ;
		MoveTo* moveToAction = MoveTo::actionWithDuration( 2.5f, Vector2(Draw3DManager::MouseX3D,Draw3DManager::MouseY3D) ) ;
		s->runAction( moveToAction ) ;
	}

	if( GetInputMgr().keyPressed(SDL_SCANCODE_SPACE) ) {
		Sprite* s = m_pSprMgr->getObject("orange_gem") ;
		if( s->isRunning() ) {
			m_pSprMgr->getObject("orange_gem")->pause() ;
		}
		else {
			m_pSprMgr->getObject("orange_gem")->resume() ;
		}
	}
}


void SpritesDemo::beforeSceneUpdate()
{
	m_angle = 0 ;
	//m_angle = Wrap(m_angle+1,0,359) ;
	m_angle += 2 ;

	//Draw3DManager::Origin3D(Draw3DManager::Original3DWidth,Draw3DManager::Original3DHeight) ;
	//Draw3DManager::Clear3D();

	Sprite* sonic = m_pSprMgr->getObject("sonic") ;

	if( !sonic->isMoving() && sonic->getAnimator().isPlaying() ) {
		sonic->getAnimator().completeAnimation(0);
		sonic->setFrame( GetDrawItemMgr().getObject(m_sonicName+"4") ) ;
	}

	Sprite* gem = m_pSprMgr->getObject("purple_gem") ;
	Ref<Pivot2D> m_piv( new Pivot2D() );
	sonic->addChild(m_piv,false) ;
	m_piv->addChild(gem) ;
 
 	m_piv->setWorldRotationAngle(m_angle) ;
 
	sonic->addChild(gem);

	sonic->removeChild(m_piv,true) ;

	gem = m_pSprMgr->getObject("red_gem") ;
	int ncoll = gem->countCollisions() ;
	if( ncoll ) {
		Node* p = gem->collided(2) ;
		ncoll = p->countCollisions() ;
		p->setEnabled(false) ;
	}

}


void SpritesDemo::destroy() 
{
	JAM_DELETE( m_pSprMgr ) ;
 	GetDrawItemMgr().deleteSheetCuts(8,1,"sonwalk2");
 	GetDrawItemMgr().deleteSheetCuts(15,14,"gemsheet");
}


int main(int argc, char *args[]) 
{
	jam::runEngine<SpritesDemo>() ;
	return 0 ;
}
