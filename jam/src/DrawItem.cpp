/**********************************************************************************
* 
* DrawItem.cpp
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

#include "StdAfx.h"
#include "jam/DrawItem.h"


namespace jam
{

const BlendMode DrawItem::DEFAULT_BLEND_MODE = BlendMode::Normal ;

Material* DrawItem::getMaterial() const
{
	return const_cast<Material*>(m_pMaterial.get()) ;
}


Texture2D* DrawItem::getTexture() const
{ 
	Material* pMat = const_cast<Material*>(m_pMaterial.get()) ;
	return pMat->getDiffuseTexture();
}

/** Sets the DrawItem's texture */
void DrawItem::setTexture( Texture2D* val)
{
	m_pMaterial->setDiffuseTexture( val ) ;
}

/** Returns the DrawItem's shader, 0 if there isn't a shader */
Shader* DrawItem::getShader() const
{
	return m_pMaterial->getShader();
}

/** Sets the DrawItem's shader (default is no shader set) */
void DrawItem::setShader( Shader* pShader )
{
	m_pMaterial->setShader( pShader );
}

DrawItem::DrawItem() :
	m_pMaterial(0), m_rect(), m_offsetX(0.0f), m_offsetY(0.0f),
	m_u1(0.0f), m_v1(0.0f), m_u2(1.0f), m_v2(1.0f),
	m_halfWidth(0), m_halfHeight(0), m_gfxScale(1.0f)
{
	m_pMaterial = new Material() ;
	Texture2D* tex = new Texture2D() ;
	tex->createDefaultEmpty() ;
	m_pMaterial->setDiffuseTexture( tex ) ;
	m_pMaterial->setBlendEnabled(true) ;
}

DrawItem::DrawItem( Texture2D* pTxtr, jam::Rect* pCut /*= 0*/, float gfxScale /*= 1.0f*/ ) :
	m_pMaterial(0), m_rect(), m_offsetX(0.0f), m_offsetY(0.0f),
	m_u1(0.0f), m_v1(0.0f), m_u2(1.0f), m_v2(1.0f), 
	m_halfWidth(0), m_halfHeight(0), m_gfxScale(gfxScale)
{
	m_pMaterial = new Material() ;
	m_pMaterial->setDiffuseTexture( pTxtr ) ;
	m_pMaterial->setBlendEnabled(true) ;

	if( pCut != 0 ) {
		m_rect = *pCut ;
	}
	else if( pTxtr ) {
		m_rect = Rect( 0,0, pTxtr->getWidth(), pTxtr->getHeight() ) ;
	}
	else {
		// not texture and no rect
	}

	init() ;
}

DrawItem::DrawItem( Texture2D* pTxtr, const jam::Rect& cut, float gfxScale /*= 1.0f*/ ) :
	m_pMaterial(0), m_rect(cut), m_offsetX(0.0f), m_offsetY(0.0f),
	m_u1(0.0f), m_v1(0.0f), m_u2(1.0f), m_v2(1.0f),
	m_halfWidth(0), m_halfHeight(0), m_gfxScale(gfxScale)
{
	m_pMaterial = new Material() ;
	m_pMaterial->setDiffuseTexture( pTxtr ) ;
	m_pMaterial->setBlendEnabled(true) ;

	init() ;
}

void DrawItem::init()
{
	Texture2D* pTex = getTexture() ;
	if( pTex ) {
		m_u1 = m_rect.left / (float)pTex->getWidth() ;
		m_v1 = 1.0f - m_rect.top / (float)pTex->getHeight() ;
		m_u2 = m_rect.right / (float)pTex->getWidth() ;
		m_v2 = 1.0f - m_rect.bottom / (float)pTex->getHeight() ;
	}

	m_rect.scale(m_gfxScale) ;
	m_halfWidth = m_rect.getWidth() / 2.0f ;
	m_halfHeight = m_rect.getHeight() / 2.0f ;
}

DrawItem* DrawItem::create()
{
	return new DrawItem();
}

DrawItem* DrawItem::create( Texture2D* pTxtr, jam::Rect* pCut /*= 0*/, float gfxScale /*= 1.0f*/ )
{
	return new DrawItem(pTxtr,pCut,gfxScale) ;
}

DrawItem* DrawItem::create( Texture2D* pTxtr, int32_t x0, int32_t y0, int32_t x1, int32_t y1, float gfxScale /*= 1.0f*/ )
{
	jam::Rect c(x0,y0,x1,y1);
	return new DrawItem(pTxtr,c,gfxScale);
}

DrawItem* DrawItem::create( Texture2D* pTxtr, const jam::Rect& cut, float gfxScale /*= 1.0f*/ )
{
	return new DrawItem(pTxtr,cut,gfxScale);
}

void DrawItem::setBlendingMode( BlendMode val )
{
	m_pMaterial->setBlendMode( val );
}

}
