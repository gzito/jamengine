#ifndef __TestSpritesZOrder_H__
#define __TestSpritesZOrder_H__

// define USE_SPRITE_BATCH to draw using spritebatch; comment it to use sprite as nodes
#define USE_SPRITE_BATCH

#include <jam/Application.h>
#include <jam/SpriteBatch.h>
#include <jam/Pivot2d.h>
#ifndef USE_SPRITE_BATCH
#include <jam/Draw3dBatch.h>
#else
#include <jam/SpriteBatch.h>
#include <jam/Ref.hpp>
#endif



class TestSpritesZOrder : public jam::Application
{
public:
	TestSpritesZOrder() ;

protected:
	virtual bool init() ;
	virtual void destroy() ;
	virtual void render() ;
	virtual void handleInput();

private:
#ifdef USE_SPRITE_BATCH
	jam::Ref<jam::SpriteBatch>		m_pSpriteBatch;
#else
	jam::Draw3DBatch*			m_pBatch ;
#endif
};


#endif // __TestSpritesZOrder_H__

