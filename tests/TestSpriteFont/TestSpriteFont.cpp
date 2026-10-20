#include <jam/Application.h>
#include <jam/Draw3dManager.h>
#include <jam/InputManager.h>
#include <jam/DrawItemManager.h>
#include <jam/Animation2dManager.h>
#include <jam/ActionInterval.h>
#include <jam/ActionInstant.h>
#include <jam/ActionEase.h>
#include <jam/CollisionManager.h>
#include <jam/Sprite.h>
#include <jam/Scene.h>
#include <jam/Layer.h>
#include <jam/DeviceManager.h>
#include <jam/Gfx.h>
#include <jam/Camera.h>
#include <jam/core/filesystem.h>

#include "TestSpriteFont.h"

using namespace jam;



TestSpriteFont::TestSpriteFont() :
	m_pSpriteBatch(nullptr)
{
}

bool TestSpriteFont::init()
{
	Ref<Sprite> sprite ;
	Ref<Animation2D> pAnim ;

	Draw3DManager::Origin3D(JAM_WINDOWS_WIDTH,JAM_WINDOWS_HEIGHT) ;
	GetGfx().setClearColor( Color(64,64,64,255) ) ;

	m_pSpriteBatch = new SpriteBatch(10) ;

	Camera* pCamera = new Camera() ;
	pCamera->setOrthographicProjection(	-Draw3DManager::VPWidth/2, Draw3DManager::VPWidth/2,
										-Draw3DManager::VPHeight/2, Draw3DManager::VPHeight/2,
										1.f, 50.f ) ;
	pCamera->lookAt( Vector3(0,0,30), Vector3(0,0,0), Vector3(0,1,0) ) ;
	getScene()->setCamera(pCamera) ;

	m_pSpriteFont = new SpriteFont() ;
	m_pSpriteFont->load(appendPath(JAM_TEST_RESOURCE_DIR,"media/fonts/kf-xml.fnt")) ;

	return true ;
}

void TestSpriteFont::handleInput()
{
}

void TestSpriteFont::render()
{
	m_pSpriteBatch->Begin() ;

	m_pSpriteBatch->DrawString( m_pSpriteFont, "say hello to your game jam", Vector2(0,0), Color::WHITE,
							   0, Vector2(0,0), 1.0f, SpriteEffects::None, 0.0f ) ;

	//m_pSpriteBatch->DrawString( m_pSpriteFont, "your game jam", Vector2(0,0), Color::WHITE,
	//						   JAM_PI_4, Vector2(0,0), 1.0f, SpriteEffects::None, 0.0f ) ;

	m_pSpriteBatch->DrawString( m_pSpriteFont, "say hello to your game jam", Vector2(0,-50), Color::WHITE,
							   0, Vector2(0,0), 1.0f, SpriteEffects::FlipHorizontally, 0.0f ) ;

	m_pSpriteBatch->End() ;
}

void TestSpriteFont::destroy()
{

}



int main( int argc, char** argv ) {
	{
}	
// Create application object
	jam::runEngine<TestSpriteFont>() ;

	return 0 ;
}
