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

#include "TestSpritesZOrder.h"


using namespace jam;

TestSpritesZOrder::TestSpritesZOrder() :
#ifdef USE_SPRITE_BATCH
	m_pSpriteBatch(nullptr)
#else
	m_pBatch(nullptr)
#endif
{
}


bool TestSpritesZOrder::init()
{
	Ref<Sprite> sprite ;
	Ref<Animation2D> pAnim ;

	Draw3DManager::Origin3D(JAM_WINDOWS_WIDTH,JAM_WINDOWS_HEIGHT) ;
	GetGfx().setClearColor( Color(64,64,64,255) ) ;

	GetDrawItemMgr().loadSheet("./media/gemsheet.png","spritesheets",64,64,15,14);

#ifndef USE_SPRITE_BATCH

	Ref<Layer> bl = Ref<Layer>(new Layer()) ;

	// create yellow gem
	sprite = new Sprite() ;
	sprite->setFrame( GetDrawItemMgr().getObject("gemsheet_0") ) ;
	sprite->setPos(Vector2(-60,0));
	sprite->setName("yellow_gem");
	bl->addChild(sprite);

	// create gray gem
	sprite = new Sprite() ;
	sprite->setFrame( GetDrawItemMgr().getObject("gemsheet_30") ) ;
	sprite->setPos( Vector2(-30,0) );
	sprite->setName("gray_gem");
	bl->addChild(sprite);

	// create red gem
	sprite = new Sprite() ;
	sprite->setFrame( GetDrawItemMgr().getObject("gemsheet_90") ) ;
	sprite->setPos( Vector2(0,0) );
	sprite->setName("red_gem") ;
	bl->addChild(sprite);

	Ref<Layer> l = Ref<Layer>(new Layer()) ;
	
	// create purple gem
	sprite = new Sprite() ;
	sprite->setFrame( GetDrawItemMgr().getObject("gemsheet_120") ) ;
	sprite->setPos( Vector2(-60,-32) );
	sprite->setName("purple_gem") ;
	l->addChild(sprite);

	// create orange gem
	sprite = new Sprite() ;
	sprite->setFrame( GetDrawItemMgr().getObject("gemsheet_150") ) ;
	sprite->setPos( Vector2(-30,-32) );
	sprite->setName("orange_gem") ;
	l->addChild(sprite,-1);				// The z-order of the node relative to it's "brothers", i.e. children of the same parent.
										// Negative values will be drawn first, positive values after.

	// create green gem
	sprite = new Sprite() ;
	sprite->setFrame( GetDrawItemMgr().getObject("gemsheet_180") ) ;
	sprite->setPos( Vector2(0,-32) );
	sprite->setName("green_gem") ;
	l->addChild(sprite,-2);

	this->getScene()->addChild(bl,-2) ;
	this->getScene()->addChild(l,2) ;

#else

	m_pSpriteBatch = new SpriteBatch(10) ;

#endif

//	GetAppMgr().setAnimationInterval(1/60.0f) ;
	Camera* pCamera = new Camera() ;
	pCamera->setOrthographicProjection(	-Draw3DManager::VPWidth/2, Draw3DManager::VPWidth/2,
										-Draw3DManager::VPHeight/2, Draw3DManager::VPHeight/2,
										1.f, 50.f ) ;
	pCamera->lookAt( Vector3(0,0,30), Vector3(0,0,0), Vector3(0,1,0) ) ;
	getScene()->setCamera(pCamera) ;

	return true ;
}


void TestSpritesZOrder::render()
{
#ifdef USE_SPRITE_BATCH
	m_pSpriteBatch->Begin( SpriteSortMode::BackToFront) ;

	DrawItem* dItem = nullptr ;
	Texture2D* tex = nullptr ;

	// create yellow gem
	dItem = GetDrawItemMgr().getObject("gemsheet_0") ;
	tex = dItem->getTexture() ;
	m_pSpriteBatch->Draw( tex, Vector2(-60,0), const_cast<Rect*>(&dItem->getRect()), Color::WHITE, 0.0f, 
						 Vector2(dItem->getHalfWidth(), dItem->getHalfHeight()),
						 Vector2(1,1), SpriteEffects::None, 0.2f ) ;

	// create gray gem
	dItem = GetDrawItemMgr().getObject("gemsheet_30") ;
	tex = dItem->getTexture() ;
	m_pSpriteBatch->Draw( tex, Vector2(-30,0), const_cast<Rect*>(&dItem->getRect()), Color::WHITE, 0.0f, 
						 Vector2(dItem->getHalfWidth(), dItem->getHalfHeight()),
						 Vector2(1,1), SpriteEffects::None, 0.2f ) ;

	// create red gem
	dItem = GetDrawItemMgr().getObject("gemsheet_90") ;
	tex = dItem->getTexture() ;
	m_pSpriteBatch->Draw( tex, Vector2(0,0), const_cast<Rect*>(&dItem->getRect()), Color::WHITE, 0.0f, 
						 Vector2(dItem->getHalfWidth(), dItem->getHalfHeight()),
						 Vector2(1,1), SpriteEffects::None, 0.2f ) ;

	// create purple gem
	dItem = GetDrawItemMgr().getObject("gemsheet_120") ;
	tex = dItem->getTexture() ;
	m_pSpriteBatch->Draw( tex, Vector2(-60,-32), const_cast<Rect*>(&dItem->getRect()), Color::WHITE, 0.0f, 
						 Vector2(dItem->getHalfWidth(), dItem->getHalfHeight()),
						 Vector2(1,1), SpriteEffects::None, 0.0f ) ;

	// create orange gem
	dItem = GetDrawItemMgr().getObject("gemsheet_150") ;
	tex = dItem->getTexture() ;
	m_pSpriteBatch->Draw( tex, Vector2(-30,-32), const_cast<Rect*>(&dItem->getRect()), Color::WHITE, 0.0f, 
						 Vector2(dItem->getHalfWidth(), dItem->getHalfHeight()),
						 Vector2(1,1), SpriteEffects::None, 0.1f ) ;

	// create green gem
	dItem = GetDrawItemMgr().getObject("gemsheet_180") ;
	tex = dItem->getTexture() ;
	m_pSpriteBatch->Draw( tex, Vector2(0,-32), const_cast<Rect*>(&dItem->getRect()), Color::WHITE, 0.0f, 
						 Vector2(dItem->getHalfWidth(), dItem->getHalfHeight()),
						 Vector2(1,1), SpriteEffects::None, 0.15f ) ;

	m_pSpriteBatch->End() ;
#endif
}


void TestSpritesZOrder::destroy() 
{
}

void TestSpritesZOrder::handleInput()
{
#ifndef USE_SPRITE_BATCH
	if( GetInputMgr().keyPressed(SDL_SCANCODE_SPACE) ) {
		if( m_pBatch == 0 ) { m_pBatch = GetGfx().getBatch(); }
		Draw3DBatch* pBatch = GetGfx().getBatch() ;
		if( pBatch != 0 ) {
			GetGfx().setBatch(0) ;
		}
		else {
			GetGfx().setBatch(m_pBatch) ;
		}
	}
#endif
}

int main( int argc, char** argv ) 
{
	// Create application object
	jam::runEngine<TestSpritesZOrder>() ;

	return 0 ;
}
