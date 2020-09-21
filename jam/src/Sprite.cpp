/**********************************************************************************
* 
* Sprite.cpp
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

#include "stdafx.h"

#include "jam/Sprite.h"
#include "jam/core/bmkextras.hpp"
#include "jam/InputManager.h"
#include "jam/Application.h"
#include "jam/Scene.h"
#include "jam/Draw3dManager.h"

#include <sstream>
#include <math.h>

namespace jam
{
	Sprite::Sprite() :
Node(),
	m_color(Color::WHITE),
	m_flipX(false),
	m_flipY(false),
	m_pAnimator(0),
	m_pFrame(0),
	m_enableFogOnMe(true),
	m_forceDraw(false)
{
#ifdef JAM_TRACE_INVIEW_SPRITES
	m_totalSprites++ ;
#endif
}

Sprite::Sprite( DrawItem* pFrame ) :
Node(),
	m_color(Color::WHITE),
	m_flipX(false),
	m_flipY(false),
	m_pAnimator(0),
	m_pFrame(0),
	m_enableFogOnMe(true)
{
	setFrame(pFrame) ;
#ifdef JAM_TRACE_INVIEW_SPRITES
	m_totalSprites++ ;
#endif
}

Sprite::~Sprite()
{
	destroyAnimator() ;
#ifdef JAM_TRACE_INVIEW_SPRITES
	m_totalSprites-- ;
#endif
}

void Sprite::setFrame( DrawItem* pNewFrame )
{
	if( m_pFrame == pNewFrame ) {  // the new frame is the current frame?
		return ;
	}

	if( pNewFrame ) {
		if( m_pFrame == 0 || (m_pFrame !=0 && (m_pFrame->getWidth() != pNewFrame->getWidth() || m_pFrame->getHeight() != pNewFrame->getHeight())) ) {
			float LDrawHalfXSize = pNewFrame->getHalfWidth() ;
			float LDrawHalfYSize = pNewFrame->getHalfHeight() ;
			AABB aabb( m_hspot.x, m_hspot.y, LDrawHalfXSize, LDrawHalfYSize) ;
			setAABB( aabb ) ;
		}
	}
	else {
		setAABB( AABB() ) ;
	}

	m_pFrame = pNewFrame;
}

ExtAnimator* Sprite::createAnimator()
{
	m_pAnimator = new ExtAnimator(this);
	return m_pAnimator;
}

void Sprite::update()
{
	JAM_PROFILE("Sprite.upd") ;

	if( m_pAnimator!=0 )	m_pAnimator->update();

	Node::update();
}

void Sprite::render()
{
	JAM_PROFILE("Sprite.render") ;

#ifdef JAM_CHECK_SINGLE_UPDATE_CALL
	JAM_ASSERT_MSG( m_lastRender != GetAppMgr().getTotalElapsedMs(), ("Render called twice") ) ;
	m_lastRender = GetAppMgr().getTotalElapsedMs() ;
#endif

#ifndef JAM_SPRITES_DISABLED
	// commented because this check is done in Node::visitForRender()
	// if( isEnabled() && isVisible() )
	if( m_pFrame && (getForceDraw() ? true : isInViewActive(false)) ) {
#ifdef JAM_TRACE_INVIEW_SPRITES
		m_totalInView++ ;
#endif
		Color c = m_color ;

		if( GetAppMgr().getScene()->isFogOfViewEnabled() ) {
			if (m_enableFogOnMe)
			{
				uint8_t acol = (uint8_t) ( GetAppMgr().getScene()->getFogOfViewRing().isIn(getWorldPos()) * c.a ) ;
				c.a = (acol>255) ? 255 : acol ;
			}
		}

		bool flipX = m_flipX ;
		bool flipY = m_flipY ;

		if( !isAnimatorNull() ) {
			const Animation2D* anim = m_pAnimator->getAnimation() ;
			if( anim ) {
				int li = m_pAnimator->getLastFrameIndex() ;
				AnimFrame2D* frame = anim->getFrame(li) ;
				if( frame ) {
					flipX = frame->getFlipX();
					flipY = frame->getFlipY();
				}
			}
		}

		// TODO IMPORTANT! test with non-zero hot-spot
		GetDraw3DMgr().DrawTransformedQuad3D( m_pFrame, getTransformedAABB(), (m_touchable ? -1 : 0), &c, flipX,flipY ) ;

#ifdef JAM_DEBUG
		// DEBUG DRAW

		// Bounding box
		if( Sprite::m_debugDrawBoundingBox ) {
			Draw3DManager::ColorN3D=Color::YELLOW ;
			GetDraw3DMgr().DrawPoly( getTransformedAABB() );
			if( canCollide() && getCollisionScaleFactor() != COLLISION_MANAGER_DEFAULT_SCALE_FACTOR ) {
				Draw3DManager::ColorN3D=Color::RED ;
				GetDraw3DMgr().DrawPoly( getCollisionOBB() );
			}
		}

		// Bounding circle
		if( Sprite::m_debugDrawBoundingCircle ) {
			Draw3DManager::ColorN3D=Color::YELLOW ;
			GetDraw3DMgr().DrawCircle( getTransformedBoundingCircle(), 0, 20 );
			if( canCollide() && getCollisionScaleFactor() != COLLISION_MANAGER_DEFAULT_SCALE_FACTOR ) {
				Draw3DManager::ColorN3D=Color::RED ;
				GetDraw3DMgr().DrawCircle( getCollisionBoundingCircle(), 0, 20 );
			}
		}
#endif
	}
#endif
}

bool Sprite::collide( Node* other, CollisionManager::Method m ) const
{
	bool isInCollision = false ;

	const Circle2f& c1 = getCollisionBoundingCircle();
	const Circle2f& c2 = other->getCollisionBoundingCircle();
	isInCollision = c1.intersects(c2) ;

	if( isInCollision && m == CollisionManager::Method::BoundingBox ) {
		const Polygon2f& p1 = getCollisionOBB();
		const Polygon2f& p2 = other->getCollisionOBB();
		isInCollision = p1.intersects(p2) ;
		if( isInCollision && m == CollisionManager::Method::PerPixel ) {
			Sprite* otherSprite = dynamic_cast<Sprite*>(other) ;
			if( otherSprite ) {
				isInCollision = intersectPixels(otherSprite) ;
			}
		}
	}

	return isInCollision ;
}

ExtAnimator& Sprite::getAnimator()
{
	if( m_pAnimator==0 ) {
		createAnimator() ;
	}
	return *m_pAnimator;
}

void Sprite::destroyAnimator()
{
	m_pAnimator = nullptr ;
}

bool Sprite::intersectPixelsSlow( Sprite* other ) const
{
/*
	Matrix2f transformA = this->getWorldTform() ;
	DrawItem* pDrawItemA = this->getFrame() ;
	Matrix2f transformB = other->getWorldTform() ;
	DrawItem* pDrawItemB = other->getFrame() ;

	// Calculate a matrix which transforms from A's local space into
	// world space and then into B's local space
	Matrix2f transformAToB = transformA * transformB.getInverse() ;

	uint32* dataA = (uint32*)pDrawItemA->getTexture()->GetTexels() ;
	uint32* dataB = (uint32*)pDrawItemB->getTexture()->GetTexels() ;
	uint32 m_widthA = pDrawItemA->getTexture()->GetWidth() ;
	uint32 m_widthB = pDrawItemB->getTexture()->GetWidth() ;

	uint32 colorA_alpha ;
	uint32 colorB_alpha ;

	// For each row of pixels in A
	for (int yA = 0; yA < pDrawItemA->getRect().getHeight(); yA++) {
		// For each pixel in this row
		for (int xA = 0; xA < pDrawItemA->getRect().getWidth(); xA++) {
			// Calculate this pixel's location in B
			Vector2f positionInB = transformAToB.transformVec( Vector2f((float)xA,(float)-yA) ) ;

			// Round to the nearest pixel
			int xB = (int)roundf(positionInB.x);
			int yB = (int)roundf(-positionInB.y);

			// If the pixel lies within the bounds of B
			if ((xB >= 0 && xB <= pDrawItemB->getRect().getWidth()) &&
				(yB >= 0 && yB <= pDrawItemB->getRect().getHeight()) )
			{
				// Get the colors of the overlapping pixels
				colorA_alpha = dataA[(pDrawItemA->getRect().left+xA) + (pDrawItemA->getRect().top+yA) * m_widthA] >> 24;
				colorB_alpha = dataB[(pDrawItemB->getRect().left+xB) + (pDrawItemB->getRect().top+yB) * m_widthB] >> 24;

				// If both pixels are not completely transparent,
				if (colorA_alpha != 0 && colorB_alpha != 0) {
					// then an intersection has been found
					return true;
				}
			}
		}
	}
*/
	// No intersection found
	return false;
}

/*
 \brief Determines if there is overlap of the non-transparent pixels between two sprites.
 \return True if non-transparent pixels overlap; false otherwise
*/
bool Sprite::intersectPixels( Sprite* other ) const
{
/*
	// transformA World transform of the first sprite
	// widthA Width of the first sprite's texture
	// heightA Height of the first sprite's texture
	// dataA Pixel color data of the first sprite
	// transformB World transform of the second sprite
	// widthB Width of the second sprite's texture
	// heightB Height of the second sprite's texture
	// dataB Pixel color data of the second sprite

	Matrix2f transformA = this->getWorldTform() ;
	DrawItem* pDrawItemA = this->getFrame() ;
	Matrix2f transformB = other->getWorldTform() ;
	DrawItem* pDrawItemB = other->getFrame() ;

	// Calculate a matrix which transforms from A's local space into
	// world space and then into B's local space
	Matrix2f transformAToB = transformA * transformB.getInverse() ;

	// When a point moves in A's local space, it moves in B's local space with a
	// fixed direction and distance proportional to the movement in A.
	// This algorithm steps through A one pixel at a time along A's X and Y axes
	// Calculate the analogous steps in B:
	Vector2f stepX = transformAToB.transformNormal(Vector2f::UnitX);
	Vector2f stepY = transformAToB.transformNormal(-Vector2f::UnitY);

	// Calculate the top left corner of A in B's local space
	// This variable will be reused to keep track of the start of each row
	Vector2f yPosInB = transformAToB.transformVec(Vector2f::Zero);

	uint32* dataA = (uint32*)pDrawItemA->getTexture()->GetTexels() ;
	uint32* dataB = (uint32*)pDrawItemB->getTexture()->GetTexels() ;
	uint32 m_widthA = pDrawItemA->getTexture()->GetWidth() ;
	uint32 m_widthB = pDrawItemB->getTexture()->GetWidth() ;

	uint32 colorA_alpha ;
	uint32 colorB_alpha ;

	// For each row of pixels in A
	for (int yA = 0; yA < pDrawItemA->getRect().getHeight(); yA++)
	{
		// Start at the beginning of the row
		Vector2f posInB = yPosInB;
		// For each pixel in this row
		for (int xA = 0; xA < pDrawItemA->getRect().getWidth(); xA++)
		{
			// Round to the nearest pixel
			int xB = (int)roundf(posInB.x);
			int yB = (int)roundf(-posInB.y);
			// If the pixel lies within the bounds of B
			if ( (xB>=0 && xB <= pDrawItemB->getRect().getWidth()) &&
				(yB >= 0 && yB <= pDrawItemB->getRect().getHeight()) )
			{
				// Get the colors of the overlapping pixels
				colorA_alpha = dataA[(pDrawItemA->getRect().left+xA) + (pDrawItemA->getRect().top+yA) * m_widthA] >> 24;
				colorB_alpha = dataB[(pDrawItemB->getRect().left+xB) + (pDrawItemB->getRect().top+yB) * m_widthB] >> 24;
				// If both pixels are not completely transparent,
				if (colorA_alpha != 0 && colorB_alpha != 0)
				{
					// then an intersection has been found
					return true;
				}
			}
			// Move to the next pixel in the row
			posInB += stepX;
		}

		// Move to the next row
		yPosInB += stepY;
	}
*/
	// No intersection found
	return false;
}


void Sprite::scaleFix( float width, float height )
{
	if (!m_pFrame) return;
	float wframe= m_pFrame->getWidth();
	float hframe= m_pFrame->getHeight();
	float mW=width/wframe;
	float mH=height/hframe;
	float ratio= Max(mW, mH);
	setScale(ratio);
}
#ifdef JAM_DEBUG
void Sprite::debugDrawBoundingBox( bool enable /*= true*/)
{
	m_debugDrawBoundingBox = enable ;
}

void Sprite::debugDrawBoundingCircle( bool enable /*= true */)
{
	m_debugDrawBoundingCircle = enable ;
}

String Sprite::getDebugInfo(bool typeInfo=true)
{
	std::stringstream oss ;
	oss << Node::getDebugInfo(typeInfo) ;
	oss << " frameName=" << (m_pFrame ? m_pFrame->getName() : "") ;
	return oss.str();
}



bool Sprite::m_debugDrawBoundingCircle = false;
bool Sprite::m_debugDrawBoundingBox = false;
#endif

#ifdef JAM_TRACE_INVIEW_SPRITES
int32 Sprite::m_totalSprites = 0 ;
int32 Sprite::m_totalInView = 0 ;
#endif

NullSprite::NullSprite()
{
}

void NullSprite::render()
{
#ifdef JAM_CHECK_SINGLE_UPDATE_CALL
	JAM_ASSERT_MSG( m_lastRender != GetAppMgr().getTotalElapsedMs(), ("Render called twice") ) ;
	m_lastRender = GetAppMgr().getTotalElapsedMs() ;
#endif
}

void NullSprite::setWidthAndHeight( float w, float h )
{
	AABB aabb( m_hspot.x, m_hspot.y, w/2, h/2) ;
	setAABB( aabb ) ;
}

}
