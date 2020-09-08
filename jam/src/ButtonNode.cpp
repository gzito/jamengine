/**********************************************************************************
* 
* ButtonNode.cpp
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
#include "jam/ButtonNode.h"
#include "jam/TextNode.h"

namespace jam
{

ButtonNode::ButtonNode() : 
	m_idleDrawItem(0),m_downDrawItem(0),m_hilitedDrawItem(0),m_disabledDrawItem(0),
	m_disabledState(false), m_stillDown(0), m_stillUp(0), m_gScale(1.0f), m_label(0),
	m_firstDraw(10)
{
	setTouchable(true) ;
	setVisible(true);
	setForceDraw(true) ;
}

ButtonNode::ButtonNode( DrawItem* idle, DrawItem* down /*= 0*/, DrawItem* hilited /*= 0*/, DrawItem* disabled /*= 0 */ ) : 
m_idleDrawItem(idle),	m_downDrawItem(down),	m_hilitedDrawItem(hilited),	m_disabledDrawItem(disabled),
	m_disabledState(false), m_stillDown(0),	m_stillUp(0), m_gScale(1.0f), m_label(0),
	m_firstDraw(10)
{
	setTouchable(true) ;
	setVisible(true);
	setFrame(m_idleDrawItem) ;
	setDisabledState(false);
	setForceDraw(true) ;
}

ButtonNode::ButtonNode( const String& idleName ) : 
m_idleDrawItem(0),	m_downDrawItem(0),	m_hilitedDrawItem(0),	m_disabledDrawItem(0),
	m_disabledState(false), m_stillDown(0),	m_stillUp(0), m_gScale(1.0f), m_label(0),
	m_firstDraw(10)
{
	m_idleDrawItem = GetDrawItemMgr().getObject(idleName);
	setTouchable(true) ;
	setVisible(true);
	setFrame(m_idleDrawItem) ;
	setDisabledState(false);
	setForceDraw(true) ;
}


void ButtonNode::update()
{
	Sprite::update() ;
	if (m_firstDraw)
	{
		m_firstDraw--;
		if (m_firstDraw<0) m_firstDraw=0;
		onRedrawFirstTime();		
	}
	else	if( !m_disabledState ) 
	{
		for(int i=0; i<JAM_MAX_TOUCHES; i++) {
			if( isReleased(i) ) {
				onRedrawReleased();
				break ;
			}
			else if( isPressed(i)) 
			{
				onRedrawPressed();
				break ;
			}
			else if( isDown(i) ) {
				onRedrawPressed() ;
				break ;
			}
		}
	}
}


void ButtonNode::setDisabledState( bool bDisabled /*= true */ )
{
	m_disabledState = bDisabled ;
	if( bDisabled ) {
		if( m_disabledDrawItem ) {
			setFrame(m_disabledDrawItem) ;
		}
		setTouchable(false) ;
		// 		setVisible(false);
	}
	else {
		setFrame(m_idleDrawItem) ;
		setTouchable(true) ;
		//		setVisible(true);
	}
}

void ButtonNode::onRedrawReleased()
{
	//JAM_TRACE((JAM_FUNCTION_NAME)) ;
	setFrame(m_idleDrawItem) ;
	if( !m_downDrawItem ) {
		setColor(Color::WHITE) ;
		setScale(1.0f*m_gScale);
	}
	m_stillDown=0;
	m_stillUp++;

}

void ButtonNode::onRedrawPressed()
{
	//JAM_TRACE((JAM_FUNCTION_NAME)) ;
	if( m_downDrawItem )
	{
		setFrame(m_downDrawItem) ;
	}
	else
	{
		setFrame(m_idleDrawItem) ;
		setColor(Color::YELLOW) ;
		setScale(0.95f*m_gScale);
	}
	m_stillDown++;
	m_stillUp=0;
}

void ButtonNode::onRedrawFirstTime()
{
	setFrame(m_idleDrawItem) ;
	setColor(Color::WHITE) ;
	setScale(m_gScale);
}

void ButtonNode::setLabel( const String& label, const String& fontName )
{
	if( !m_label ) {
		m_label = TextNode::create() ;
		m_label->setText(fontName,label) ;
		addChild(m_label,false) ;
		m_label->setScale(0.65f) ;
	}
}

}
