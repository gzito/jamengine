/**********************************************************************************
* 
* TextNode.cpp
* 
* This file is part of Jam
* 
* Copyright (c) 2014-2019 Giovanni Zito, Gianluca Sclano.
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

#include "jam/Node.h"
#include "jam/Draw3dManager.h"
#include "jam/TextNode.h"

namespace jam
{
	TextNode::TextNode() : m_color(Color::WHITE), m_time(0), m_align(Draw3DManager::DrawAlign::DRAW_ALIGN_CENTER)
	{
	}

	TextNode::~TextNode()
	{
	}

	// GZ: if you declare this textnode as touchable before calling setText,
	//     and then you call seText with FDrawButton param setted to 0,
	//     the text will be set as setTouchable(false)
	//     For this reason it is better to call setText before setTouchable
	int TextNode::setText( const String& drawItemName, const String& FDrastring, Draw3DManager::DrawAlign FDrawAlign /*= Draw3DManager::DRAW_ALIGN_CENTER*/, int FDrawButton/*=0*/, float FKerningWidth/*=0.0f*/ , bool is_complex/*=false*/)
	{
		//
		// GZ : TODO, FDrawButton is no more used to perform mouse-over, param to be changed as bool
		//

		m_drawItemName = drawItemName;
		m_text = FDrastring;
		m_align = FDrawAlign;
		setTouchable(FDrawButton!=0);
		m_kerning=FKerningWidth;
		m_fastParse=!is_complex;
		if( !m_text.empty() )
		{
			float LDrawXSize = (float)getWidth();
			float LDrawHalfYSize = 16 ;	// i.e. 32 / 2

			if( FDrawAlign == Draw3DManager::DrawAlign::DRAW_ALIGN_LEFT) {
				m_hspot.x = 0 ;
			}
			else if ( FDrawAlign == Draw3DManager::DrawAlign::DRAW_ALIGN_CENTER) {
				m_hspot.x = -LDrawXSize / 2 ;
			}
			else {	// Draw3DManager::DRAW_ALIGN_RIGHT
				m_hspot.x = -LDrawXSize ;
			}

			m_hspot.y = LDrawHalfYSize ;

			AABB aabb ;
			aabb.setBounds(m_hspot.x,LDrawHalfYSize, m_hspot.x+LDrawXSize,-LDrawHalfYSize) ;
			setAABB( aabb ) ;
		}
		else {
			setAABB( AABB() ) ;
		}

		return 0;
	}

	int TextNode::getWidth() const
	{
		return GetDraw3DMgr().StringWidth3D(m_drawItemName,m_text) ;
	}

	void TextNode::update()
	{
		JAM_PROFILE("TextNode.upd") ;
		if (m_time>0)
		{
			m_time -= (int64_t)jam::GetAppMgr().getElapsedMs();
			if (m_time<=0)
			{
				destroy() ;	// TODO: da rivedere perchè crasha qui
				return ;
			}
		}
		Node::update() ;
	}

	void TextNode::render()
	{
		JAM_PROFILE("TextNode.render") ;
#ifdef JAM_CHECK_SINGLE_UPDATE_CALL
		JAM_ASSERT_MSG( m_lastRender != GetAppMgr().getTotalElapsedMs(), ("Render called twice") ) ;
		m_lastRender = GetAppMgr().getTotalElapsedMs() ;
#endif

		if( isInViewActive()	&& !m_text.empty() )
		{
			//GetDraw3DMgr().DrawQuad3D(m_obb) ;
			Color tmp = Draw3DManager::ColorT3D ;
			Draw3DManager::ColorT3D = m_color ;
			GetDraw3DMgr().Text3D(m_drawItemName,getWorldPos().x,getWorldPos().y,m_text, (float)m_align, 0, getWorldRotationAngle(),m_kerning,getWorldScale().x, getWorldScale().y,m_fastParse);
			Draw3DManager::ColorT3D = tmp ;
		}
	}

	TextNode* TextNode::create()
	{
		return new TextNode ;
	}

	//
	// TextNodeManager
	//
	TextNode* TextNodeManager::addText( const String& text,float _x, float _y, Node* parent, const String& fontName,
		uint64_t _times/*=1000*/, float size /*=0.5f*/, Draw3DManager::DrawAlign align /* = Draw3DManager::DRAW_ALIGN_CENTER*/,  float kerning/*=0.0f*/ , bool is_complex/*=false*/)
	{
		static char name[50] = {0};
		sprintf(name,"textAt%d,%d",(int)_x,(int)_y) ;
		return addText(text,_x,_y, name, parent,fontName, _times,size,align,kerning,is_complex);
	}
	//*** NB: Se viene chiamata 2 volte con un name simile usa sempre lo stesso oggetto
	//*** Non funzionava se cambiava il parent
	TextNode* TextNodeManager::addText(	const String& text,	float _x, float _y,	const String& name, 	Node* parent, const String& fontName, uint64_t _times/*=1000*/, float size /*=0.5f*/, Draw3DManager::DrawAlign align /*= Draw3DManager::DRAW_ALIGN_CENTER*/, float kerning/*=0.0f*/ , bool is_complex/*=false*/)
	{
		TextNode* t = 0 ;
		if (!name.empty())	t=jam::GetTextNodeMgr().getObject(name);

		if( !t )
		{
			t = TextNode::create() ;
			t->setName(name) ;
			parent->addChild(t) ;
			if (!name.empty())	addObject( t ) ;
		}
		else
		{
			if (t->getParent() != parent)	{	t->newParent(parent);}
		}

		if (!text.empty())
		{
			t->setText(fontName,text,align,0,kerning) ;
			t->setScale(size) ;
			t->setPos(_x,_y) ;
			t->setTime(_times) ;
			t->setEnabled(true);
			t->setFastParse(!is_complex);
			t->setVisible(true);
			t->setEnabled(true);
		}
		else
		{
			t->setVisible(false);
			t->setEnabled(false);
		}

		return t ;
	}

	TextNode* TextNodeManager::text( const String& text, float _x, float _y, Node* parent, const String& fontName,uint64_t _times/*=1000*/, float size /*=0.5f*/, Draw3DManager::DrawAlign align/*=Draw3DManager::DRAW_ALIGN_CENTER*/, float kerning/*=0.0f */, bool is_complex/*=false*/   )
	{
		TextNode* t = TextNode::create() ;
		parent->addChild(t) ;
		if (!text.empty())
		{
			t->setText(fontName,text,align,0,kerning) ;
			t->setScale(size) ;
			t->setPos(_x,_y) ;
			t->setTime(_times) ;
			t->setEnabled(true);
			t->setFastParse(!is_complex);
		}
		else
		{
			t->setVisible(false);
			t->setEnabled(false);
		}

		// GZ autorelease non serve, sta già in TextNode::create()
		// t->autorelease();
		return t ;
	}

	TextNode* TextNodeManager::destroyByName( const String& name )
	{
		TextNode* t = 0 ;
		if (!name.empty()) {
			t = GetTextNodeMgr().getObject(name);
			if( t ) t->destroy();
		}

		return 0;
	}

	TextNode* TextNodeManager::getNode( const String& name )
	{
		TextNode* t = 0 ;
		if (!name.empty()) {
			t = GetTextNodeMgr().getObject(name);
		}
		return t;
	}
	
	void TextNodeManager::destroyAllText()
	{
		clearAll() ;
	}

	TextNode* TextNodeManager::setText( const String& text, const String& name )
	{
		TextNode* t = 0 ;
		if (!name.empty())	t=getObject(name);

		if( !t )
		{
			return 0;
		}
	

		if (!text.empty())
		{
			t->setText(text) ;			
		}
		else
		{
			t->setVisible(false);
			t->setEnabled(false);
		}
		return t ;
	}
}