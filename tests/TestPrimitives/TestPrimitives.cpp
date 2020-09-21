/**********************************************************************************
* 
* TestPrimitives.cpp
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
#include <jam/core/filesystem.h>
#include <jam/core/bmkextras.hpp>
#include <jam/core/geom.h>
#include <jam/DrawItemManager.h>
#include <jam/Anim2d.h>
#include <jam/Animation2dManager.h>
#include <jam/Sprite.h>
#include <jam/ActionInterval.h>


using namespace jam;


#define MEDIA_PATH				"./"


class TestPrimitivesApp : public jam::Application
{
public:
							TestPrimitivesApp() ;
							virtual ~TestPrimitivesApp() {} ;

protected:
	void					loadTextures() ;
	void					createLights() ;

	virtual bool			init() ;
	virtual void			handleInput() ;
	virtual void			beforeSceneUpdate() override ;
	virtual void			render() ;
	virtual void			destroy() ;

private:
	Shader*					pUnlitProgram ;
	Shader*					pLitProgram ;
	Shader*					pNormalMapProgram ;
	
	// quad
	Mesh*					pQuadMesh ;
	Texture2D*				pQuadTexture ;

	// cube
	Mesh*					pCubeMesh ;
	Texture2D*				pCubeTextureDiffuse ;
	Texture2D*				pCubeTextureSpecular ;

	Vector3					cubeAngles ;
	float					quadYRotation ;

	// text
	Texture2D*				pFontTex ;

	// light
	Light*					pLight[2] ;

	Sprite*					marioSprite ;
};

TestPrimitivesApp::TestPrimitivesApp() :
	pUnlitProgram(nullptr), pLitProgram(nullptr), pNormalMapProgram(nullptr),
	pQuadMesh(nullptr), pQuadTexture(nullptr), 
	pCubeMesh(nullptr), pCubeTextureDiffuse(nullptr), pCubeTextureSpecular(nullptr), cubeAngles(0.0f),
	quadYRotation(0.0f), pFontTex(nullptr), 
	pLight()
{
}

void TestPrimitivesApp::loadTextures()
{
	pQuadTexture = new Texture2D() ;
	pQuadTexture->load( appendPath(MEDIA_PATH,"images/nvidia.jpg") ) ;
	pQuadTexture->setName("nvidia") ;

	pCubeTextureDiffuse = new Texture2D() ;
	pCubeTextureDiffuse->load( appendPath(MEDIA_PATH,"images/container2.png") ) ;
	pCubeTextureDiffuse->setName("container2") ;

	pCubeTextureSpecular = new Texture2D() ;
	pCubeTextureSpecular->load( appendPath(MEDIA_PATH,"images/container2_specular.png") ) ;
	pCubeTextureSpecular->setName("container2_specular") ;
//	GetTextureMgr().addObject( pCubeTextureSpecular ) ;

	pFontTex = GetDraw3DMgr().LoadFont3D( appendPath(MEDIA_PATH,"images/Babylon1.png"),0,0 ) ;
}

void TestPrimitivesApp::createLights()
{
	pLight[0] = new Light() ;
	pLight[0]->setType( Light::Type::DIRECTIONAL ) ;
	pLight[0]->setAmbientColor( Color(128,128,128) ) ;
	pLight[0]->setDiffuseColor( Color::GRAY ) ;
	pLight[0]->setSpecularColor( Color::WHITE ) ;
	pLight[0]->setPosition( Vector3( -10.0f, -5.0f, 10.0f ) ) ;
	pLight[0]->setDirection( Vector3( 0.0f, -1.0f, 0.0f ) ) ;

	pLight[1] = new Light() ;
	pLight[1]->setType( Light::Type::DIRECTIONAL ) ;
	pLight[1]->setAmbientColor( Color(128,128,128) ) ;
	pLight[1]->setDiffuseColor( Color::GRAY ) ;
	pLight[1]->setSpecularColor( Color::WHITE ) ;
	pLight[1]->setPosition( Vector3( 10.0f, -5.0f, 10.0f ) ) ;
	pLight[1]->setDirection( Vector3( 0.0f, 0.0f, -1.0f ) ) ;
}

//*************************************************************************
bool TestPrimitivesApp::init()
{
	Draw3DManager::Origin3D(960,540);

	// this is the default
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL) ;

	//	front face is clockwise
 	glFrontFace(GL_CW) ;

 	glCullFace(GL_BACK) ;
 	glEnable(GL_CULL_FACE) ;
//	glDisable(GL_CULL_FACE) ;

	setClearColor( Color::MIDNIGHTBLUE ) ;

	pUnlitProgram = GetShaderMgr().getDefaultUnlit() ;
	pLitProgram = GetShaderMgr().getDefaultLit() ;
	pNormalMapProgram = GetShaderMgr().getNormalMapping() ;


	GetDrawItemMgr().loadSheet("images/TerDjjW.png","spritesheets",60,95,7,1,0,0,"mariowalk");
	Animation2D* pAnim = Animation2D::create();
	pAnim->setName("mariowalk");
	GetAnim2DMgr().addObject(pAnim);
	for( int i = 0; i < 7; i++ ) {
		pAnim->addFrame( GetDrawItemMgr().getObject(String("mariowalk_")+std::to_string(i)), 0.1f ) ;
	}
	pAnim->setLoop(true);

	marioSprite = new Sprite() ;
	marioSprite->setTag("player") ;
	marioSprite->setScale(0.025f) ;
	marioSprite->setPos(-12.5f,-4.5f);
	marioSprite->getAnimator().setAnimation( GetAnim2DMgr().getObject("mariowalk"), true, 0 ) ;
	marioSprite->getAnimator().setSpeed(1);
	marioSprite->setFrame( GetDrawItemMgr().getObject("mariowalk_0") ) ;
	this->getScene()->addChild(marioSprite);
	
	loadTextures() ;
	createLights() ;

	pQuadMesh = Primitives::createQuadMesh() ;
	Material* quadMat = new Material() ;
	pQuadMesh->disableTangents() ;
	quadMat->setShader(pLitProgram) ;
	quadMat->setDiffuseTexture(pQuadTexture) ;
	pQuadMesh->setMaterial(quadMat) ;

	pCubeMesh = Primitives::createCubeMesh() ;
	pCubeMesh->disableTangents() ;
	Material* cubeMat = new Material() ;
	cubeMat->setShader(pLitProgram) ;
	cubeMat->setDiffuseTexture(pCubeTextureDiffuse) ;
	cubeMat->setSpecularTexture(pCubeTextureSpecular) ;
	cubeMat->setShininess(10.5f) ;
	pCubeMesh->setMaterial(cubeMat) ;

	return true ;
}

void TestPrimitivesApp::handleInput()
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

void TestPrimitivesApp::beforeSceneUpdate()
{
	if( !marioSprite->isMoving() ) {
		marioSprite->setPos( -12.5f,-4.5f ) ;
		MoveTo* moveToAction = MoveTo::actionWithDuration( 10.0f, Vector2(12.5f,-4.5f) ) ;
		marioSprite->runAction( moveToAction ) ;
	}
}

void TestPrimitivesApp::render()
{
	Camera* pCam = getScene()->getCamera() ;

	Matrix4 modelMat(1.0f) ;
	Matrix4 scaleMat(1.0f) ;
	Matrix4 translateMat(1.0f) ;
	Matrix4 rotateMat(1.0f) ;

	// draw quad
	Shader* pShader = pQuadMesh->getMaterial()->getShader() ;
	pShader->use() ;
	translateMat = createTranslationMatrix3D( Vector3(7,3,0) ) ;
	rotateMat = createRotationMatrix3D( quadYRotation, 0.0f, 0.f ) ;
	scaleMat = createScaleMatrix3D( Vector3(4,4,1) ) ;
	pLight[0]->update(0) ;
	pLight[1]->update(1) ;

	quadYRotation = WrapAngle((quadYRotation + 1.0f)) ;
	modelMat = translateMat * scaleMat * rotateMat ;
	pShader->setModelMatrix(modelMat) ;
	// when using a shader program for the first time, it's necessary to set ViewMatrix & ProjectionMatrix uniforms
	// if changing camera transform or settings between frames, it's also necessary to update them
	pShader->setViewMatrix(pCam->getViewMatrix()) ;
	pShader->setProjectionMatrix(pCam->getProjectionMatrix()) ;
	pShader->setViewPosition( pCam->getPosition() ) ;

	glDisable(GL_CULL_FACE) ;

	pQuadMesh->draw() ;

	// draw cube
	pShader = pCubeMesh->getMaterial()->getShader() ;
	pShader->use() ;
	pLight[0]->update(0) ;
	pLight[1]->update(1) ;
	translateMat = createTranslationMatrix3D( Vector3(0,0,0) ) ;
	scaleMat = createScaleMatrix3D( Vector3(1.5,1.5,1.5) ) ;
	rotateMat = createRotationMatrix3D( cubeAngles.y, cubeAngles.x, cubeAngles.z ) ;

	modelMat = translateMat * scaleMat * rotateMat ;
	pShader->setModelMatrix( modelMat ) ;
	pShader->setViewMatrix( pCam->getViewMatrix() ) ;
	pShader->setProjectionMatrix( pCam->getProjectionMatrix() ) ;
	pShader->setViewPosition( pCam->getPosition() ) ;

	cubeAngles.y = WrapAngle((cubeAngles.y + 1.0f)) ;
	cubeAngles.x = WrapAngle((cubeAngles.x + 0.5f)) ;
	cubeAngles.z = WrapAngle((cubeAngles.z + 0.25f)) ;
	
	glEnable(GL_CULL_FACE) ;

	pCubeMesh->draw() ;

	// draw text
	modelMat = Matrix4(1.0f) ;
	pShader = pUnlitProgram ;
	pShader->use() ;
	pShader->setModelMatrix(modelMat) ;
	pShader->setViewMatrix( pCam->getViewMatrix() ) ;
	pShader->setProjectionMatrix( pCam->getProjectionMatrix() ) ;
	Draw3DManager::ColorT3D = jam::Color::CORAL ;
	GetDraw3DMgr().Text3D( pFontTex, 0, -3.0f, "HELLO JAM! ;)", Draw3DManager::DrawAlign::DRAW_ALIGN_CENTER, 0, 0.0f, 0.0f, 1/32.0f ) ;
	JAM_ASSERT_MSG( glGetError() == GL_NO_ERROR, "Check for errors" ) ;
}


void TestPrimitivesApp::destroy()
{
	JAM_DELETE(pCubeMesh) ;
	JAM_DELETE(pQuadMesh) ;
}


//*************************************************************************

int main(int argc, char *args[]) 
{
	jam::runEngine<TestPrimitivesApp>() ;
	return 0 ;
}
