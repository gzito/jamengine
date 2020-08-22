/**********************************************************************************
* 
* FallingBoxes.cpp
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
#include <jam/DrawItemManager.h>
#include <jam/InputManager.h>
#include <jam/Scene.h>
#include <jam/Camera.h>
#include <jam/core/bmkextras.hpp>
#include <jam/core/geom.h>
#include <jam/core/filesystem.h>

#include "FallingBoxes.h"


using namespace jam;

#define MEDIA_PATH				"./"
#define PTM_RATIO				32.0f

FallingBoxes::FallingBoxes() :
	m_pWorld(0),
	m_pTimer(0),
	m_gameStarted(false),
	m_pFontTex(0)
{
}


bool FallingBoxes::init()
{
	Draw3DManager::Origin3D(960,540) ;
	setClearColor( Color::MIDNIGHTBLUE ) ;

	//	front face is clockwise
 	glFrontFace(GL_CW) ;
 	glCullFace(GL_BACK) ;
 	glEnable(GL_CULL_FACE) ;
	
	DrawItemManager::getSingleton().loadTexture(
		appendPath(MEDIA_PATH,"images/Box.png"), "sprites", -1, "Box"
	) ;

	m_pFontTex = GetDraw3DMgr().LoadFont3D( appendPath(MEDIA_PATH,"images/Babylon1.png"),0,0 ) ;

	b2Vec2 screenBounds[] = { 
		b2Vec2(-Draw3DManager::HalfOriginal3DWidth/PTM_RATIO, Draw3DManager::HalfOriginal3DHeight/PTM_RATIO),
		b2Vec2( Draw3DManager::HalfOriginal3DWidth/PTM_RATIO, Draw3DManager::HalfOriginal3DHeight/PTM_RATIO),
		b2Vec2( Draw3DManager::HalfOriginal3DWidth/PTM_RATIO,-Draw3DManager::HalfOriginal3DHeight/PTM_RATIO),
		b2Vec2(-Draw3DManager::HalfOriginal3DWidth/PTM_RATIO,-Draw3DManager::HalfOriginal3DHeight/PTM_RATIO)
	} ;

	// we set the gravity to zero, because in our breakout game there will not be any gravity
	b2Vec2 gravity(0.0f, -9.8f);
	bool doSleep = true ;
	m_pWorld = new b2World(gravity) ;
	m_pWorld->SetAllowSleeping(doSleep) ;

	// Create edges around the entire screen

	// Create a body using a bodydef
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0,0);
	b2Body* pGroundBody = m_pWorld->CreateBody(&groundBodyDef);

	b2EdgeShape groundBox;
	b2FixtureDef groundBoxDef;
	groundBoxDef.shape = &groundBox;

	// bottom edge
	groundBox.Set(screenBounds[2], screenBounds[3] );
	pGroundBody->CreateFixture(&groundBoxDef);
	
	// Create box and add it to the layer
	Sprite* box = new Sprite( GetDrawItemMgr().getPtrByName("Box") );
	box->autorelease() ;
	box->setPos( Vector2(0, 240) );
	box->setColor( Color::YELLOW);
	getScene()->addChild(box);

	// Create box body
	b2BodyDef boxBodyDef;
	boxBodyDef.type = b2_dynamicBody;
	boxBodyDef.position.Set(0, 240/PTM_RATIO);
	boxBodyDef.userData = box;
	b2Body* m_pBoxBody = m_pWorld->CreateBody(&boxBodyDef);

	// Create box shape
	b2PolygonShape boxShape;
	boxShape.SetAsBox( GetDrawItemMgr().getByName("Box").getHalfWidth()/PTM_RATIO, 
		GetDrawItemMgr().getByName("Box").getHalfHeight()/PTM_RATIO );

	// Create shape definition and add to body
	b2FixtureDef boxShapeDef;
	boxShapeDef.shape = &boxShape;
	boxShapeDef.density = 0.0f;
	boxShapeDef.friction = 0.3f;
	b2Fixture* boxFixture = m_pBoxBody->CreateFixture(&boxShapeDef); 

	m_pTimer = Timer::create() ;
	m_pTimer->setSweep(1000) ;
	m_pTimer->setRepeatSweep(-1) ;
	m_pTimer->start();

	// set camera
	Camera* pCamera = new Camera() ;
	pCamera->autorelease() ;
	pCamera->setOrthographicProjection(	-Draw3DManager::VPWidth/2, Draw3DManager::VPWidth/2,
										-Draw3DManager::VPHeight/2, Draw3DManager::VPHeight/2,
										1.f, 50.f ) ;
	pCamera->lookAt( Vector3(0,0,30), Vector3(0,0,0), Vector3(0,1,0) ) ;
	getScene()->setCamera(pCamera) ;

	return true ;
}

void FallingBoxes::handleInput()
{
	if( !m_gameStarted && GetInputMgr().keyPressed(SDL_SCANCODE_SPACE) ) {
		m_gameStarted = true ;
	}
}


void FallingBoxes::beforeSceneUpdate()
{
	if( !m_gameStarted )
		return ;

	m_pTimer->update() ;
	if( m_pTimer->isSweep() ) {
		// Create box and add it to the layer
		Sprite* box = new Sprite( GetDrawItemMgr().getPtrByName("Box") );
		box->autorelease() ;
		float x = (float)RANGERAND(-160,160) ;
		box->setPos( Vector2(x, 240) );
		box->setColor( Color(RND(256), RND(256), RND(256), 255) ) ;

		float a = RND(2) == 0 ? 0.0f : 90.0f ;
		box->setRotationAngle(-a) ;
		getScene()->addChild(box);

		// Create box body
		b2BodyDef boxBodyDef;
		boxBodyDef.type = b2_dynamicBody;
		boxBodyDef.position.Set(x/PTM_RATIO,240/PTM_RATIO);
		boxBodyDef.angle = ToRadian(a) ;
		boxBodyDef.userData = box;
		b2Body* m_pBoxBody = m_pWorld->CreateBody(&boxBodyDef);

		// Create box shape
		b2PolygonShape boxShape;
		boxShape.SetAsBox( GetDrawItemMgr().getByName("Box").getHalfWidth()/PTM_RATIO, 
			GetDrawItemMgr().getByName("Box").getHalfHeight()/PTM_RATIO );

		// Create shape definition and add to body
		b2FixtureDef boxShapeDef;
		boxShapeDef.shape = &boxShape;
		boxShapeDef.density = 1.0f;
		boxShapeDef.friction = 0.3f;
		b2Fixture* boxFixture = m_pBoxBody->CreateFixture(&boxShapeDef); 
	}

	m_pWorld->Step(getElapsed(), 10, 10);

	b2Body *b = m_pWorld->GetBodyList();
	while( b ) {    
		if (b->GetUserData() != 0) {
			Sprite* sprite = (Sprite *)b->GetUserData(); 

			sprite->setPos(Vector2(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
			sprite->setRotationAngle( -ToDegree(b->GetAngle()) );
		}

		b=b->GetNext() ;
	}

}

void FallingBoxes::render()
{
	static char fps_str[32] = {0};

	Matrix4 modelMat(1.0f) ;
	Ref<Shader> pShader = ShaderManager::getSingleton().getDefaultUnlit() ;
	pShader->use() ;
	pShader->setModelMatrix(modelMat) ;
	pShader->setViewMatrix( getScene()->getCamera()->getViewMatrix() ) ;
	pShader->setProjectionMatrix( getScene()->getCamera()->getProjectionMatrix() ) ;
	Draw3DManager::ColorT3D = jam::Color::CORAL ;
	size_t n = GetAppMgr().getScene()->getChildren().size() ;
	GetDraw3DMgr().Text3D( m_pFontTex, -470, 250, std::to_string(n), Draw3DManager::DrawAlign::DRAW_ALIGN_LEFT, 0, 0.0f, 0.0f, 1.0f ) ;

	sprintf(fps_str,"%.2f",GetAppMgr().getFpsAvg()) ;
	GetDraw3DMgr().Text3D( m_pFontTex, 470, 250, fps_str, Draw3DManager::DrawAlign::DRAW_ALIGN_RIGHT, 0, 0.0f, 0.0f, 1.0f ) ;
}


void FallingBoxes::afterSceneUpdate()
{
	if( !m_gameStarted )
		return ;

	b2Body *b = m_pWorld->GetBodyList();
	while( b ) {    
		if (b->GetUserData() != 0) {
			Sprite* sprite = (Sprite *)b->GetUserData(); 

			if( !sprite->getTransformedAABB().intersects( getScene()->getTransformedAABB() ) ) {
				b2Body* killed = b ;
				b = b->GetNext() ;
				m_pWorld->DestroyBody(killed) ;
				sprite->destroy();
			}
			else {
				b = b->GetNext() ;
			}
		}
		else {
			b=b->GetNext() ;
		}
	}
}


void FallingBoxes::destroy() 
{
	JAM_RELEASE(m_pTimer) ;
	JAM_DELETE(m_pWorld) ;
}


// SDL expects main() to be defined with canonical arguments argc & args
int main(int argc, char *args[]) 
{
	jam::runEngine<FallingBoxes>();
	return 0;
}
