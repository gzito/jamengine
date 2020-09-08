/**********************************************************************************
* 
* ButtonNode.h
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

#ifndef __JAM_BUTTONNODE_H__
#define __JAM_BUTTONNODE_H__

#include <jam/jam.h>
#include <jam/Sprite.h>

namespace jam
{

class TextNode ;

/**
A node representing a clickable button
*/
class JAM_API ButtonNode : public Sprite
{
public:
							ButtonNode( DrawItem* idle, DrawItem* down = 0, DrawItem* hilited = 0, DrawItem* disabled = 0 ) ;
							ButtonNode( const String& idleName) ;

	void					setIdleDrawItem(DrawItem* val) { m_idleDrawItem = val; }
	void					setDownDrawItem(DrawItem* val) { m_downDrawItem = val; }
	void					setHilitedDrawItem(DrawItem* val) { m_hilitedDrawItem = val; }
	void					setDisabledDrawItem(DrawItem* val) { m_disabledDrawItem = val; }

	void					setLabel( const String& label, const String& fontName ) ;
	TextNode*				getLabelNode() { return m_label; } 

	/**
	Updates the button
	\remark Overrides the implementation of class Sprite
	*/
	virtual void			update();
		
	virtual void			onRedrawPressed() ;
	virtual void			onRedrawReleased() ;
	virtual void			onRedrawFirstTime();
		
	// to be implemented: onEnter / onLeave

	void					setDisabledState( bool bDisabled = true ) ;
	bool					getDisabledState() const { return m_disabledState; }

	DrawItem*				getHilitedDrawItem() const { return m_hilitedDrawItem; }
	DrawItem*				getDisabledDrawItem() const { return m_disabledDrawItem; }
	DrawItem*				getDownDrawItem() const { return m_downDrawItem; }
	DrawItem*				getIdleDrawItem() const { return m_idleDrawItem; }


	float					getButtonScale() const { return m_gScale; }
	void					setButtonScale(float val) { m_gScale = val; }

	bool					isStillDown() const { return (m_stillDown>1); }
	bool					isStillUp() const { return (m_stillUp>1); }


protected:
							ButtonNode() ;
	U32						m_stillDown;
	U32						m_stillUp;

private:
	DrawItem*				m_idleDrawItem ;
	DrawItem*				m_downDrawItem ;
	DrawItem*				m_hilitedDrawItem ;
	DrawItem*				m_disabledDrawItem ;

	TextNode*				m_label ;
	bool					m_disabledState ;
	float					m_gScale;
	int						m_firstDraw;
};

}

#endif // __JAM_BUTTONNODE_H__
