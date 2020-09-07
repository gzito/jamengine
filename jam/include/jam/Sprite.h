/**********************************************************************************
* 
* Sprite.h
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

#ifndef __JAM_SPRITE_H__
#define __JAM_SPRITE_H__

#include <jam/jam.h>
#include <jam/Anim2d.h>
#include <jam/Timer.h>
#include <jam/ExtAnimator.h>
#include <jam/Node.h>
#include <jam/DrawItemManager.h>

namespace jam
{
/**
A node representing animated 2D image.
*/
class JAM_API Sprite : public Node, public IDrawable2D
{
public:
	/** Default constructor */
							Sprite() ;

	/** Constructs a sprite with the given frame */
							Sprite( DrawItem* pFrame ) ;

	/** Destructor */
	virtual					~Sprite();

	/**
	Sets sprite's frame
	\remark Passing 0 as argument sets a sprite with no frame (i.e. the sprite will not be rendered)
	*/
	virtual void			setFrame(DrawItem* pNewFrame) override ;

	/** Sets sprite's frame by name */
	virtual void			setFrameName(const String& name) { setFrame(GetDrawItemMgr().getObject(name));}

	/** Returns sprite's frame */
	virtual DrawItem*		getFrame() const override { return m_pFrame; }

	/** Sets sprite's color */
	virtual void			setColor( const Color& c ) override { m_color = c ; }

	/** Returns sprite's color */
	virtual const Color&	getColor() const override { return m_color; }

	/** Sets whether or not to flip the sprite frame around x-axis */
	void					setFlipX( bool flipX ) { m_flipX = flipX; }

	/** Returns whether or not the sprite frame is flipped around x-axis */
	bool					getFlipX() const { return m_flipX; }

	/** Sets whether or not to flip the sprite frame around y-axis */
	void					setFlipY( bool flipY ) { m_flipY = flipY; }

	/** Returns whether or not the sprite frame is flipped around y-axis */
	bool					getFlipY() const { return m_flipY; }

	/**
	Returns the animator instance used to animate the sprite, creating it as needed
	\remark This method creates the animator if it doesn't exist
	*/
	ExtAnimator&			getAnimator() ;

	/** Returns whether or not the animator instance is null */
	bool					isAnimatorNull() const { return m_pAnimator==0; }

	/** Destroy the animator instance */
	void					destroyAnimator();

	/**
	Updates the sprite
	\remark Overrides the implementation of class Node
	*/
	virtual void			update() ;

	/**
	Renders the sprite
	\remark Overrides the implementation of class Node
	*/
	virtual void			render() ;

	/**
	Returns whether or not this sprite is colliding with another Node instance
	\remark Overrides the implementation of class Node
	*/
	virtual bool			collide(Node* other, CollisionManager::Method m) const ;

	virtual void			scaleFix(float width, float height);

#ifdef JAM_DEBUG
	static void				debugDrawBoundingBox(bool enable = true) ;
	static void				debugDrawBoundingCircle(bool enable = true) ;

	virtual String			getDebugInfo(bool typeInfo/*=true*/);
#endif

#ifdef JAM_TRACE_INVIEW_SPRITES
	static int32			getTotalSprites() { return m_totalSprites; }

	static int32			getTotalInView() { return m_totalInView; }
	static void				setTotalInView(int32 val) { m_totalInView = val; }
#endif

	bool					isEnableFog() const { return m_enableFogOnMe; }
	void					setEnableFog(bool val) { m_enableFogOnMe = val; }

	bool					getForceDraw() const { return m_forceDraw; }
	void					setForceDraw(bool val) { m_forceDraw = val; }

protected:
	virtual ExtAnimator*	createAnimator();

	bool					intersectPixels( Sprite* other ) const ;
	bool					intersectPixelsSlow( Sprite* other ) const ;

protected:
	Color					m_color;
	bool					m_flipX;
	bool					m_flipY;

	ExtAnimator*			m_pAnimator ;

private:
	DrawItem*				m_pFrame;

#ifdef JAM_DEBUG
	static	bool			m_debugDrawBoundingBox ;
	static	bool			m_debugDrawBoundingCircle ;
#endif

#ifdef JAM_TRACE_INVIEW_SPRITES
	static int32			m_totalSprites ;
	static int32			m_totalInView ;
#endif
	bool					m_enableFogOnMe;
	bool					m_forceDraw ;		// force draw even it isn't in view
	
private:
							Sprite( const Sprite& ) = delete ;
	Sprite&					operator=( const Sprite& ) = delete ;
};



class NullSprite : public Sprite
{
public:
							NullSprite() ;

	virtual void			render() override ;

	/// set width and height
	void					setWidthAndHeight(float w, float h);
};

}


#endif // __JAM_SPRITE_H__