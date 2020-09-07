/**********************************************************************************
* 
* DrawItem.h
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

#ifndef __JAM_DRAWITEM_H__
#define __JAM_DRAWITEM_H__


#include <jam/jam.h>
#include <jam/Draw2d.h>
#include <jam/Object.h>
#include <jam/Texture2D.h> 
#include <jam/Material.h> 

namespace jam
{

/**
* Represents a portion of a texture to be drawn and its drawing attributes, such as blending mode
*/
class JAM_API DrawItem : public NamedObject
{
public:
	static const BlendMode DEFAULT_BLEND_MODE ;

	Material*				getMaterial() const ;

	/** Returns the DrawItem's texture */
	Texture2D*				getTexture() const ;

	/** Sets the DrawItem's texture */
	void					setTexture( Texture2D* val ) ;

	/** Returns the DrawItem's shader, 0 if there isn't a shader */
	Shader*					getShader() const ;

	/** Sets the DrawItem's shader (default is no shader set) */
	void					setShader( Shader* pShader ) ;

	/**
		Return the DrawItem's rectangle. 
		This is the actual size, it doesn't include gfxScale param
	*/
	const jam::Rect&		getRect() const { return m_rect;}

	float					getOffsetX() const { return m_offsetX; }
	void					setOffsetX(float val) { m_offsetX = val; }

	float					getOffsetY() const { return m_offsetY; }
	void					setOffsetY(float val) { m_offsetY = val; }

	/** 
	 Returns the blending mode.
	 \remark The default blending mode is alpha blend
	*/
	BlendMode				getBlendingMode() const { return m_pMaterial->getBlendMode(); }

	/** Sets the blending mode. */
	void					setBlendingMode(BlendMode val);

	/** Returns DraItem's texture coordinate u1 component. */
	float					getU1() const { return m_u1; }

	/** Returns DraItem's texture coordinate v1 component. */
	float					getV1() const { return m_v1; }

	/** Returns DraItem's texture coordinate u2 component. */
	float					getU2() const { return m_u2; }

	/** Returns DraItem's texture coordinate v2 component. */
	float					getV2() const { return m_v2; }

	/** Returns DraItem's width. */
	float					getWidth() const { return (float)m_rect.getWidth(); }

	/** Returns DraItem's height. */
	float					getHeight() const { return (float)m_rect.getHeight(); }

	/** Returns DraItem's half width. */
	float					getHalfWidth() const { return m_halfWidth; }

	/** Returns DraItem's half height. */
	float					getHalfHeight() const { return m_halfHeight; }

	float					getGfxScale() const { return m_gfxScale; }

	/**
	 Constructs an empty DrawItem with no texture 
	 \remark Useful to draw with solid color. Rect is set to empty and not used
	*/
	static DrawItem*		create() ;

	/**
	 Constructs a DrawItem with the given texture portion identified by the supplied rectangle.
	 \remark If rect is not supplied or 0 is given, then the whole texture is used
	*/
	static DrawItem*		create(Texture2D* pTxtr, jam::Rect* pCut = 0, float gfxScale = 1.0f);
	
	/**
	 Constructs a DrawItem with the given texture portion identified by the supplied rectangle.
	*/
	static DrawItem*		create(Texture2D* pTxtr, const jam::Rect& cut, float gfxScale = 1.0f);

	/**
	 Constructs a DrawItem with the given texture portion identified by the supplied rectangle coordinates.
	*/
	static DrawItem*		create(Texture2D* pTxtr, int32_t x0, int32_t y0, int32_t x1, int32_t y1, float gfxScale = 1.0f);

protected:
	/**
	 Default constructor, constructs an empty DrawItem with no texture 
	 \remark Useful to draw with solid color. Rect is set to empty and not used
	*/
							DrawItem();

	/**
	 Constructs a DrawItem with the given texture portion identified by the supplied rectangle.
	 \remark If rect is not supplied or 0 is given, then the whole texture is used
	*/
							DrawItem(Texture2D* pTxtr, jam::Rect* pCut = 0, float gfxScale = 1.0f );
	
	/**
	 Constructs a DrawItem with the given texture portion identified by the supplied rectangle.
	*/
							DrawItem(Texture2D* pTxtr, const jam::Rect& cut, float gfxScale = 1.0f );

	Material*				m_pMaterial ;
	jam::Rect				m_rect;

	// used by Zwoptex
	float					m_offsetX ;
	float					m_offsetY ;
	
	// texture coordinates caching
	float					m_u1, m_v1, m_u2, m_v2 ;

	// precomputed half size for drawing quad
	float					m_halfWidth, m_halfHeight ;

	// to scale graphics
	float					m_gfxScale ;

private:
	void					init();
};

/** IDrawable2D interface */
class IDrawable2D
{
public:
	virtual DrawItem*		getFrame() const = 0 ;
	virtual void			setFrame( DrawItem* ) = 0 ;
};

}

#endif // __JAM_DRAWITEM_H__
