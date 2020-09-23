/**********************************************************************************
* 
* TestNormalMapping.cpp
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
#include <jam/TightVertexBuffer.h>
#include <jam/core/bmkextras.hpp>
#include <jam/core/filesystem.h>
#include <jam/core/geom.h>

using namespace jam;


#define MEDIA_PATH				"./"

class TestNormalMappingApp : public jam::Application
{
public:
						TestNormalMappingApp() ;
						virtual ~TestNormalMappingApp() {} ;

protected:
	void				createMaterials() ;
	void				createLights() ;

	virtual bool		init() ;
	virtual void		handleInput() ;
	virtual void		render() ;
	virtual void		destroy() ;

private:
	// quad
	TightVertexBuffer*	m_pQuadVBuf ;
	Mesh*				m_pMesh ;
	Ref<Material>		m_pQuadMaterial ;
	Ref<Material>		m_pQuadMaterialWithNormalMap ;
	Ref<Texture2D>		m_pQuadDiffuseTexture ;
	Ref<Texture2D>		m_pQuadNormalTexture ;

	// light
	Light*				m_pLight[2] ;
};

TestNormalMappingApp::TestNormalMappingApp() :
	m_pQuadVBuf(0), m_pMesh(0), m_pQuadMaterial(0), m_pQuadMaterialWithNormalMap(0),
	m_pQuadDiffuseTexture(), m_pQuadNormalTexture(), m_pLight()
{
}

void TestNormalMappingApp::createMaterials()
{
	m_pQuadDiffuseTexture = Ref<Texture2D>( new Texture2D() ) ;
	m_pQuadDiffuseTexture->load( appendPath(MEDIA_PATH,"images/brickwall.jpg") ) ;
//	GetTextureMgr().addByName( m_pQuadDiffuseTexture.get(), "brickwall" ) ;

	m_pQuadNormalTexture = Ref<Texture2D>( new Texture2D() ) ;
	m_pQuadNormalTexture->load( appendPath(MEDIA_PATH,"images/brickwall_normal.jpg") ) ;
//	GetTextureMgr().addByName( m_pQuadNormalTexture.get(), "brickwall_normal" ) ;

	m_pQuadMaterial = Ref<Material>( new Material() ) ;
	m_pQuadMaterial->setDiffuseTexture(m_pQuadDiffuseTexture) ;
	m_pQuadMaterial->setShader( ShaderManager::getSingleton().getDefaultLit() ) ;

	m_pQuadMaterialWithNormalMap = Ref<Material>( new Material() ) ;
	m_pQuadMaterialWithNormalMap->setDiffuseTexture(m_pQuadDiffuseTexture) ;
	m_pQuadMaterialWithNormalMap->setNormalTexture(m_pQuadNormalTexture) ;
//	m_pQuadMaterialWithNormalMap->setShininess(5.0f) ;
	m_pQuadMaterialWithNormalMap->setShader( ShaderManager::getSingleton().getNormalMapping() ) ;
}

void TestNormalMappingApp::createLights()
{
	m_pLight[0] = new Light() ;
	m_pLight[0]->setType( Light::Type::DIRECTIONAL ) ;
	m_pLight[0]->setAmbientColor( Color(255,255,255) ) ;
	m_pLight[0]->setDiffuseColor( Color::GRAY ) ;
	m_pLight[0]->setSpecularColor( Color::GRAY ) ;
	m_pLight[0]->setDirection( Vector3( 1.0f, -1.0f, -1.0f ) ) ;

	m_pLight[1] = new Light() ;
	m_pLight[1]->setType( Light::Type::DIRECTIONAL ) ;
	m_pLight[1]->setAmbientColor( Color(255,255,255) ) ;
	m_pLight[1]->setDiffuseColor( Color::GRAY ) ;
	m_pLight[1]->setSpecularColor( Color::GRAY ) ;
	m_pLight[1]->setDirection( Vector3( -1.0f, -1.0f, -1.0f ) ) ;
}

//*************************************************************************
bool TestNormalMappingApp::init()
{
	Draw3DManager::Origin3D(1920,1080);

	// this is the default
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL) ;

	//	front face is clockwise
 	glFrontFace(GL_CW) ;

	glCullFace(GL_BACK) ;
 	glEnable(GL_CULL_FACE) ;
//	glDisable(GL_CULL_FACE) ;

	createMaterials() ;
	createLights() ;

	// create quad
	Primitives::createQuad( m_pQuadVBuf ) ;

	// create mesh
	m_pMesh = Primitives::createQuadMesh() ; 
	m_pMesh->setMaterial( m_pQuadMaterialWithNormalMap ) ; 

	return true ;
}

void TestNormalMappingApp::render()
{
	Camera* pCam = getScene()->getCamera() ;

	Matrix4 modelMat(1.0f) ;
	Matrix4 scaleMat(1.0f) ;
	Matrix4 translateMat(1.0f) ;
	Matrix4 rotateMat(1.0f) ;

	// draw quad without normal map
	Shader* pShader = m_pQuadMaterial->getShader() ;
	pShader->use() ;
	m_pLight[0]->update(0) ;
	m_pLight[1]->update(1) ;
	pShader->setViewMatrix( pCam->getViewMatrix() ) ;
	pShader->setProjectionMatrix( pCam->getProjectionMatrix() ) ;
	pShader->setViewPosition( pCam->getPosition() ) ;

	translateMat = createTranslationMatrix3D( Vector3(-3,0,0) ) ;
	scaleMat = createScaleMatrix3D( Vector3(5,5,1) ) ;
	rotateMat = createRotationMatrix3D( 45.0f, 0.0f, 0.0f ) ;
	pShader->setModelMatrix(translateMat * rotateMat * scaleMat) ;
	GetGfx().draw( m_pQuadVBuf, m_pQuadMaterial.get() ) ;


	// draw quad with normal map
	pShader = m_pQuadMaterialWithNormalMap->getShader() ;
	pShader->use() ;
	m_pLight[0]->update(0) ;
	m_pLight[1]->update(1) ;
	pShader->setViewMatrix( pCam->getViewMatrix() ) ;
	pShader->setProjectionMatrix( pCam->getProjectionMatrix() ) ;
	pShader->setViewPosition( pCam->getPosition() ) ;

	translateMat = createTranslationMatrix3D( Vector3(3,0,0) ) ;
	scaleMat = createScaleMatrix3D( Vector3(5,5,1) ) ;
	rotateMat = createRotationMatrix3D( 45.0f, 0.0f, 0.0f ) ;
	pShader->setModelMatrix(translateMat * rotateMat * scaleMat) ;
	m_pMesh->draw() ;

}


void TestNormalMappingApp::handleInput()
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


void TestNormalMappingApp::destroy()
{
	JAM_DELETE(m_pQuadVBuf) ;
	JAM_DELETE(m_pMesh) ;
}


//*************************************************************************

int main(int argc, char** argv) 
{
	jam::runEngine<TestNormalMappingApp>() ;
	return 0 ;
}
