/**********************************************************************************
* 
* TestMesh.cpp
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
#include <jam/Shader.h>
#include <jam/Gfx.h>
#include <jam/Primitives.h>
#include <jam/Scene.h>
#include <jam/Camera.h>
#include <jam/Light.h>
#include <jam/Model.h>
#include <jam/core/bmkextras.hpp>
#include <jam/core/filesystem.h>


using namespace jam;


#define MEDIA_PATH				"../../"

class TestMeshApp : public jam::Application
{
public:
						TestMeshApp() ;
	virtual				~TestMeshApp() ;

protected:
	void				createLights() ;

	virtual bool		init() ;
	virtual void		handleInput() ;
	virtual void		render() ;
	virtual void		destroy() ;

private:
	//
	Model*				m_pModel ;

	// light
	Light*				m_pLight[2] ;
};

TestMeshApp::TestMeshApp()
	: m_pLight(), m_pModel(0)
{
}

TestMeshApp::~TestMeshApp()
{
}

void TestMeshApp::createLights()
{
	m_pLight[0] = new Light() ;
	m_pLight[0]->setType( Light::Type::DIRECTIONAL ) ;
	m_pLight[0]->setAmbientColor( Color(128,128,128) ) ;
	m_pLight[0]->setDiffuseColor( Color::GRAY ) ;
	m_pLight[0]->setSpecularColor( Color::WHITE ) ;
	m_pLight[0]->setPosition( Vector3( -10.0f, -5.0f, 10.0f ) ) ;
	m_pLight[0]->setDirection( Vector3( 0.0f, -1.0f, 0.0f ) ) ;

	m_pLight[1] = new Light() ;
	m_pLight[1]->setType( Light::Type::DIRECTIONAL ) ;
	m_pLight[1]->setAmbientColor( Color(128,128,128) ) ;
	m_pLight[1]->setDiffuseColor( Color::GRAY ) ;
	m_pLight[1]->setSpecularColor( Color::WHITE ) ;
	m_pLight[1]->setPosition( Vector3( 10.0f, -5.0f, 10.0f ) ) ;
	m_pLight[1]->setDirection( Vector3( 0.0f, 0.0f, -1.0f ) ) ;
}

//*************************************************************************
bool TestMeshApp::init()
{
	Draw3DManager::Origin3D(1920,1080);

	// this is the default
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL) ;
	
	// Wireframe
//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//	front face is clockwise
 	glFrontFace(GL_CW) ;

 	glCullFace(GL_BACK) ;
 	glEnable(GL_CULL_FACE) ;
//	glDisable(GL_CULL_FACE) ;

	createLights() ;

	m_pModel = new Model() ;
	m_pModel->load( appendPath(MEDIA_PATH,"models/nanosuit/nanosuit.obj") ) ;
//	m_pModel->load( appendPath(MEDIA_PATH,"models/Cyborg/Cyborg.obj") ) ;

	m_pModel->getTransform().setWorldPosition( Vector3(0,-5,0) ) ;
	m_pModel->getTransform().setWorldScale( Vector3(0.5f,0.5f,0.5f) ) ;

	return true ;
}

void TestMeshApp::handleInput()
{
	float speed = 7.5f ;

	Camera* pCam = getScene()->getCamera() ;

	if( GetInputMgr().keyDown(GLFW_KEY_W) ) 
	{
		Vector3 forward = pCam->getForward() ;
		pCam->translate( -forward * speed * GetAppMgr().getElapsed() ) ;
	}

	if( GetInputMgr().keyDown(GLFW_KEY_S) ) 
	{
		Vector3 forward = pCam->getForward() ;
		pCam->translate( forward * speed * GetAppMgr().getElapsed() ) ;
	}

	if( GetInputMgr().keyDown(GLFW_KEY_A) ) 
	{
		Vector3 right = pCam->getRight() ;
		pCam->translate( -right * speed * GetAppMgr().getElapsed() ) ;
	}

	if( GetInputMgr().keyDown(GLFW_KEY_D) ) 
	{
		Vector3 right = pCam->getRight() ;
		pCam->translate( right * speed * GetAppMgr().getElapsed() ) ;
	}

	if( GetInputMgr().getPointerState(GLFW_MOUSE_BUTTON_LEFT) == InputManager::DOWN ) 
	{
		float touchSpeedX = GetInputMgr().getTouchSpeedX(0) ;
		if( fabs(touchSpeedX) > FLT_EPSILON ) {
			Vector3 eulerAngles = pCam->getEulerAngles() ;
			float a = eulerAngles.y + (touchSpeedX * 0.25f) ;
			eulerAngles.y = WrapAngle(a) ;
			pCam->yawPitchRoll(eulerAngles) ;
		}

		float touchSpeedY = GetInputMgr().getTouchSpeedY(0) ;
		if( fabs(touchSpeedY) > FLT_EPSILON ) {
			Vector3 eulerAngles = pCam->getEulerAngles() ;
			float a = eulerAngles.x + (touchSpeedY * 0.25f) ;
			eulerAngles.x = WrapAngle(a) ;
			pCam->yawPitchRoll(eulerAngles) ;
		}
	}
}

void TestMeshApp::render()
{
	ShaderManager::getSingleton().getNormalMapping()->use() ;

	m_pLight[0]->update(0) ;
	m_pLight[1]->update(1) ;

	m_pModel->draw() ;
}


void TestMeshApp::destroy()
{
}


//*************************************************************************

int main() 
{
	jam::runEngine<TestMeshApp>() ;
	return 0 ;
}
