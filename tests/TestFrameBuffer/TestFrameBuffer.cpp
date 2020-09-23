/**********************************************************************************
* 
* TestFrameBuffer.cpp
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
#include <jam/FrameBufferObject.h>
#include <jam/RenderBufferObject.h>
#include <jam/DeviceManager.h>
#include <jam/core/bmkextras.hpp>
#include <jam/core/filesystem.h>
#include <jam/core/geom.h>

#include "Quad2D.h"

using namespace jam;


#define MEDIA_PATH				"./"


class TestFrameBufferApp : public jam::Application
{
public:
							TestFrameBufferApp() ;
							virtual ~TestFrameBufferApp() {} ;

protected:
	void					loadTextures() ;
	void					createLights() ;
	void					createCameras() ;

	virtual bool			init() override ;
	virtual void			handleInput() override ;
	virtual void			render() override ;
	virtual void			destroy() override ;

private:
	// cube
	Mesh*					m_pCubeMesh ;
	Ref<Material>			m_pCubeMaterial ;
	Ref<Texture2D>			m_pCubeTextureDiffuse ;
	Ref<Texture2D>			m_pCubeTextureSpecular ;
	FrameBufferObject*		m_pFBO ;
	Ref<Texture2D>			m_pFBOTex ;
	RenderBufferObject*		m_pRBO ;
	Quad2D*					m_pQuad2d ;

	Vector3				m_cubeAngles ;

	// light
	Light*					m_pLight[2] ;

	// ortho camera
	Camera*					m_pPerspCamera ;
	Camera*					m_pOrthoCamera ;
};

TestFrameBufferApp::TestFrameBufferApp() :
	m_pCubeMesh(nullptr),
	m_pCubeMaterial(nullptr),
	m_pCubeTextureDiffuse(nullptr),
	m_pCubeTextureSpecular(nullptr),
	m_pFBO(nullptr),
	m_pFBOTex(nullptr),
	m_pRBO(nullptr),
	m_pQuad2d(nullptr),
	m_cubeAngles(0.0f),
	m_pLight(),
	m_pPerspCamera(nullptr),
	m_pOrthoCamera(nullptr)
{
}

void TestFrameBufferApp::loadTextures()
{
	m_pCubeTextureDiffuse = Ref<Texture2D>( new Texture2D() ) ;
	m_pCubeTextureDiffuse->load( appendPath(MEDIA_PATH,"images/container2.png") ) ;
	//GetTextureMgr().addByName( m_pCubeTextureDiffuse.get(), "container2" ) ;

	m_pCubeTextureSpecular = Ref<Texture2D>( new Texture2D() ) ;
	m_pCubeTextureSpecular->load( appendPath(MEDIA_PATH,"images/container2_specular.png") ) ;
	//GetTextureMgr().addByName( m_pCubeTextureSpecular.get(), "container2_specular" ) ;
}

void TestFrameBufferApp::createLights()
{
	m_pLight[0] = new Light() ;
	m_pLight[0]->setType( Light::Type::DIRECTIONAL ) ;
	m_pLight[0]->setAmbientColor( Color(32,32,32) ) ;
	m_pLight[0]->setDiffuseColor( Color::GRAY ) ;
	m_pLight[0]->setSpecularColor( Color::WHITE ) ;
	m_pLight[0]->setPosition( Vector3( -10.0f, -5.0f, 10.0f ) ) ;
	m_pLight[0]->setDirection( Vector3( 0.0f, -1.0f, 0.0f ) ) ;

	m_pLight[1] = new Light() ;
	m_pLight[1]->setType( Light::Type::DIRECTIONAL ) ;
	m_pLight[1]->setAmbientColor( Color(32,32,32) ) ;
	m_pLight[1]->setDiffuseColor( Color::GRAY ) ;
	m_pLight[1]->setSpecularColor( Color::WHITE ) ;
	m_pLight[1]->setPosition( Vector3( 10.0f, -5.0f, 10.0f ) ) ;
	m_pLight[1]->setDirection( Vector3( 0.0f, 0.0f, -1.0f ) ) ;
}

void TestFrameBufferApp::createCameras()
{
	// create perspective camera
	m_pPerspCamera = new Camera() ;

	m_pPerspCamera->setPerspectiveProjection( 60.0f, 
											 GetDeviceMgr().getNativeDisplayWidth() / (float)GetDeviceMgr().getNativeDisplayHeight(),
											 0.1f,
											 1000.f) ;
	m_pPerspCamera->lookAt( Vector3(0,0,100), Vector3(0,0,0), Vector3(0,1,0) ) ;

	// create ortho camera
	m_pOrthoCamera = new Camera() ;
	m_pOrthoCamera->setOrthographicProjection(	-Draw3DManager::VPWidth/2, Draw3DManager::VPWidth/2,
										-Draw3DManager::VPHeight/2, Draw3DManager::VPHeight/2,
										0.1f, 1000.f ) ;
	m_pOrthoCamera->lookAt( Vector3(0,0,100), Vector3(0,0,0), Vector3(0,1,0) ) ;
}

//*************************************************************************
bool TestFrameBufferApp::init()
{
	Draw3DManager::Origin3D(1920,1080);

	// this is the default
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL) ;

	//	front face is clockwise
 	glFrontFace(GL_CW) ;

 	glCullFace(GL_BACK) ;
 	glEnable(GL_CULL_FACE) ;
//	glDisable(GL_CULL_FACE) ;

	loadTextures() ;
	createLights() ;

	createCameras() ;

	// create frame buffer
	m_pFBO = new FrameBufferObject() ;
	m_pFBO->create() ;
	m_pFBO->bind() ;
	m_pFBOTex = Ref<Texture2D>( new Texture2D() ) ;
	m_pFBOTex->create( GetDeviceMgr().getNativeDisplayWidth(), GetDeviceMgr().getNativeDisplayHeight(), GL_RGB) ;

	m_pRBO = new RenderBufferObject() ;
	m_pRBO->create() ;
	m_pRBO->bind() ;
	m_pRBO->setStorage( GL_DEPTH24_STENCIL8, GetDeviceMgr().getNativeDisplayWidth(), GetDeviceMgr().getNativeDisplayHeight() ) ;
	m_pRBO->unbind() ;

	m_pFBO->attachTexture2D( m_pFBOTex, GL_COLOR_ATTACHMENT0, 0 ) ;
	m_pFBO->attachRenderBuffer( m_pRBO, GL_DEPTH_STENCIL_ATTACHMENT ) ;
	JAM_ASSERT( m_pFBO->isGood() ) ;
	m_pFBO->unbind() ;

	m_pQuad2d = new Quad2D() ;
	m_pQuad2d->setTexture( m_pFBOTex ) ;

	m_pCubeMesh = Primitives::createCubeMesh() ;
	m_pCubeMaterial = Ref<Material>( new Material() ) ;
	m_pCubeMaterial->setShader( ShaderManager::getSingleton().getDefaultLit() ) ;
	m_pCubeMaterial->setDiffuseTexture(m_pCubeTextureDiffuse) ;
	m_pCubeMaterial->setSpecularTexture(m_pCubeTextureSpecular) ;
	m_pCubeMaterial->setShininess(10.5f) ;
	m_pCubeMesh->setMaterial(m_pCubeMaterial) ;

	return true ;
}

void TestFrameBufferApp::handleInput()
{
	float speed = 7.5f ;

	Camera* pCam = getScene()->getCamera() ;

	if( GetInputMgr().keyDown(SDL_SCANCODE_W) ) 
	{
		Vector3 forward = pCam->getForward() ;
		pCam->translate( -forward * speed * GetAppMgr().getElapsed() ) ;
	}

	if( GetInputMgr().keyDown(SDL_SCANCODE_S) ) 
	{
		Vector3 forward = pCam->getForward() ;
		pCam->translate( forward * speed * GetAppMgr().getElapsed() ) ;
	}

	if( GetInputMgr().keyDown(SDL_SCANCODE_A) ) 
	{
		Vector3 right = pCam->getRight() ;
		pCam->translate( -right * speed * GetAppMgr().getElapsed() ) ;
	}

	if( GetInputMgr().keyDown(SDL_SCANCODE_D) ) 
	{
		Vector3 right = pCam->getRight() ;
		pCam->translate( right * speed * GetAppMgr().getElapsed() ) ;
	}

	if( GetInputMgr().getPointerState(SDL_BUTTON_LEFT) == InputManager::DOWN ) 
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

void TestFrameBufferApp::render()
{
	Matrix4 modelMat(1.0f) ;
	Matrix4 scaleMat(1.0f) ;
	Matrix4 translateMat(1.0f) ;
	Matrix4 rotateMat(1.0f) ;

	// BEGIN DRAW ON TO FRAMEBUFFER
	m_pFBO->bind() ; 
	GetGfx().setClearColor( Color::GREEN ) ;
	GetGfx().clear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;

	Shader* pShader = m_pCubeMesh->getMaterial()->getShader() ;
	pShader->use() ;

	m_pLight[0]->update(0) ;
	m_pLight[1]->update(1) ;
	 
	// draw cube
	translateMat = createTranslationMatrix3D( Vector3(0,0,0) ) ;
	scaleMat = createScaleMatrix3D( Vector3(10,10,10) ) ;
	rotateMat = createRotationMatrix3D( m_cubeAngles.y, m_cubeAngles.x, m_cubeAngles.z ) ;

	m_pPerspCamera->setActive() ;

	// when using a shader program for the first time, it's necessary to set ViewMatrix & ProjectionMatrix uniforms
	// if changing camera transform or settings between frames, it's also necessary to update them
	pShader->setViewMatrix( m_pPerspCamera->getViewMatrix() ) ;
	pShader->setProjectionMatrix( m_pPerspCamera->getProjectionMatrix() ) ;
	pShader->setViewPosition( m_pPerspCamera->getPosition() ) ;
	pShader->setModelMatrix(translateMat * scaleMat * rotateMat) ;

	m_cubeAngles.y = WrapAngle((m_cubeAngles.y + 1.0f)) ;
	m_cubeAngles.x = WrapAngle((m_cubeAngles.x + 0.5f)) ;
	m_cubeAngles.z = WrapAngle((m_cubeAngles.z + 0.25f)) ;
	GetGfx().setDepthTest(true) ;
	m_pCubeMesh->draw() ;

	m_pFBO->unbind() ; 

	// END DRAW ON TO FRAMEBUFFER

	GetGfx().setClearColor( Color::YELLOW ) ;
	GetGfx().clear( GL_COLOR_BUFFER_BIT ) ;
	m_pOrthoCamera->setActive() ;
	m_pQuad2d->m_pProg->use() ;
//	GetGfx().setProjectionMatrix( m_pOrthoCamera->getProjectionMatrix() ) ;
//	GetGfx().setModelMatrix( Matrix4(1.0f) ) ;
	m_pQuad2d->draw() ;
}


void TestFrameBufferApp::destroy()
{
	m_pOrthoCamera->release() ;
	m_pPerspCamera->release() ;
}


//*************************************************************************

int main(int argc, char** argv) 
{
	jam::runEngine<TestFrameBufferApp>() ;
	return 0 ;
}

