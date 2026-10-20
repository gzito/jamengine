
#ifndef __TestSpriteFont_H__
#define __TestSpriteFont_H__



#include <jam/Application.h>

#include <jam/SpriteBatch.h>

#include <jam/Pivot2d.h>

#include <jam/SpriteBatch.h>
#include <jam/SpriteFont.h>
#include <jam/Ref.hpp>

#include <map>

class TestSpriteFont : public jam::Application

{

public:
							TestSpriteFont() ;

protected:

	virtual bool			init() ;

	virtual void			destroy() ;

	virtual void			render() ;
	virtual void			handleInput();



private:
	jam::Ref<jam::SpriteBatch>		m_pSpriteBatch;
	jam::Ref<jam::SpriteFont>		m_pSpriteFont ;
};



#endif // __TestSpriteFont_H__

