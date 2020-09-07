/**********************************************************************************
* 
* SpriteRenderer.h
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

#ifndef __JAM_SPRITERENDERER_H__
#define __JAM_SPRITERENDERER_H__

#include <jam/jam.h>
#include <jam/Sprite.h>

namespace jam
{

class JAM_API SpriteRenderer
{
public:
							SpriteRenderer() ;

	/** Whether or not to flip the sprite frame around x-axis */
	bool					getFlipX() const ;
	void					setFlipX( bool flipX ) ;

	/** Whether or not to flip the sprite frame around y-axis */
	bool					getFlipY() const ;
	void					setFlipY( bool flipY ) ;

	/** The sprite to render */
	Sprite*					getSprite() const ;
	void					setSprite( Sprite* sprite ) ;

	/** Rendering color for the sprite graphics */
	Color					getColor() const ;
	void					setColor( const Color& color ) ;

private:
	Color					m_color ;
	bool					m_flipX ;
	bool					m_flipY ;
	Sprite*					m_sprite ;
};

JAM_INLINE void SpriteRenderer::setFlipX(bool flipX) {
	m_flipX = flipX ;
}

JAM_INLINE bool SpriteRenderer::getFlipX() const {
	return m_flipX;
}

JAM_INLINE void SpriteRenderer::setFlipY(bool flipY) {
	m_flipY = flipY ;
}

JAM_INLINE bool SpriteRenderer::getFlipY() const {
	return m_flipY;
}

JAM_INLINE Sprite* SpriteRenderer::getSprite() const {
	return m_sprite;
}

JAM_INLINE void SpriteRenderer::setSprite( Sprite* sprite) {
	m_sprite = sprite ;
}

JAM_INLINE Color SpriteRenderer::getColor() const {
	return m_color ;
}

JAM_INLINE void SpriteRenderer::setColor(const Color& color) {
	m_color = color ;
}

}


#endif // __JAM_SPRITERENDERER_H__
