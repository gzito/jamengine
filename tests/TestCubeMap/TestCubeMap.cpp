/**********************************************************************************
* 
* TestCubeMap.cpp
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
#include <jam/SkyBox.h>

#include <jam/core/array.hpp>
#include <jam/core/bmkextras.hpp>
#include <jam/core/filesystem.h>

using namespace jam;


#define MEDIA_PATH				"../../images/"

class TestCubeMapApp : public jam::Application
{
public:
						TestCubeMapApp() ;
						virtual ~TestCubeMapApp() {} ;

protected:
	void				loadTextures() ;

	virtual bool		init() ;
	virtual void		handleInput() ;
	virtual void		render() ;
	virtual void		destroy() ;

private:
	TextureCubemap*		m_pQuadTexture ;
	SkyBox*				m_pSkyBox ;
};

TestCubeMapApp::TestCubeMapApp() :
	m_pQuadTexture(nullptr),
	m_pSkyBox(nullptr)
{
}

void TestCubeMapApp::loadTextures()
{
	m_pQuadTexture = new TextureCubemap() ;
	std::vector<String> filenames ;
	filenames.push_back( appendPath(MEDIA_PATH,"arrakisday_rt.tga") ) ; 
	filenames.push_back( appendPath(MEDIA_PATH,"arrakisday_lf.tga") ) ; 
	filenames.push_back( appendPath(MEDIA_PATH,"arrakisday_up.tga") ) ; 
	filenames.push_back( appendPath(MEDIA_PATH,"arrakisday_dn.tga") ) ; 
	filenames.push_back( appendPath(MEDIA_PATH,"arrakisday_ft.tga") ) ; 
	filenames.push_back( appendPath(MEDIA_PATH,"arrakisday_bk.tga") ) ;

	m_pQuadTexture->load( filenames, false ) ;
	GetTextureCubemapMgr().addByName( m_pQuadTexture, "skybox" ) ;
}

//*************************************************************************
bool TestCubeMapApp::init()
{
	Draw3DManager::Origin3D(800,600);

	// this is the default
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL) ;

	//	front face is clockwise
 	glFrontFace(GL_CW) ;

 	glCullFace(GL_BACK) ;
 	glEnable(GL_CULL_FACE) ;
	glDisable(GL_CULL_FACE) ;

	loadTextures() ;

	m_pSkyBox = new SkyBox( getScene()->getCamera() ) ;
	m_pSkyBox->create() ;
	m_pSkyBox->setTexture3D( m_pQuadTexture ) ;

	return true ;
}

void TestCubeMapApp::handleInput()
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

void TestCubeMapApp::render()
{
	Camera* pCam = getScene()->getCamera() ;

	//Matrix4 modelMat(1.0f) ;
	//Matrix4 scaleMat(1.0f) ;
	//Matrix4 translateMat(1.0f) ;
	//Matrix4 rotateMat(1.0f) ;

	// draw quad
	//translateMat = createTranslationMatrix3D( Vector3(3,0,0) ) ;
	//scaleMat = createScaleMatrix3D( Vector3(4,4,1) ) ;
	//GetGfx().setModelMatrix(translateMat * scaleMat) ;

	m_pSkyBox->draw() ;
}


void TestCubeMapApp::destroy()
{
}


//*************************************************************************

int main() 
{
	jam::runEngine<TestCubeMapApp>() ;
	return 0 ;
}
