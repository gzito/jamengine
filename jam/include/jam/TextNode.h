/**********************************************************************************
* 
* TextNode.h
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

#ifndef  JAM_TextNode_h
#define  JAM_TextNode_h

#include <jam/jam.h>
#include <jam/BaseManager.hpp>
#include <jam/Sprite.h>
#include <jam/Node.h>
#include <jam/Draw3dManager.h>

namespace jam
{
/**
A node representing bitmapped texts.
*/
class JAM_API TextNode : public Node
{
public:
	virtual					~TextNode();

	int						setText( const String& drawItemName, const String& FDrastring, Draw3DManager::DrawAlign FDrawAlign = Draw3DManager::DrawAlign::DRAW_ALIGN_CENTER, int FDrawButton=0, float FKerningWidth=0.0f, bool is_complex=false );
	void					setText(String val) { m_text = val; }
	String					getText() const { return m_text; }
	int						getWidth() const ;

	void					setTime( int64_t t ) { m_time = t; }

	virtual void			update() override;
	virtual void			render() override;

	virtual const Color&	getColor() const override { return m_color; }
	virtual void			setColor( const Color& c ) override { m_color = c; }

	/** Creates a new TextNode and calls autorelease() on it */
	static	TextNode*		create() ;
	bool					getFastParse() const { return m_fastParse; }
	void					setFastParse(bool val) { m_fastParse = val; }

	String					getDrawItemName() const { return m_drawItemName; }

protected:
							TextNode();

	Color					m_color ;
	String					m_drawItemName ;		// name of drawItem font
	String					m_text;
		
	Draw3DManager::DrawAlign	m_align;
	float					m_kerning;
	int64_t					m_time ;
	bool					m_fastParse;
};

class JAM_API TextNodeManager : public NamedTaggedObjectManager<TextNode>, public jam::Singleton<TextNodeManager>
{
	friend class jam::Singleton<TextNodeManager> ;

public:
	TextNode*				setText( const String& text, const String& name) ;
	TextNode*				addText( const String& text, float _x, float _y, 	Node* parent, const String& fontName,uint64_t _times=1000, float size =0.5f, Draw3DManager::DrawAlign align=Draw3DManager::DrawAlign::DRAW_ALIGN_CENTER, float kerning=0.0f,bool is_complex=false  ) ;
	TextNode*				addText( const String& text, float _x, float _y, const String& name, Node* parent, const String& fontName, uint64_t _times=1000, float size =0.5f, Draw3DManager::DrawAlign align=Draw3DManager::DrawAlign::DRAW_ALIGN_CENTER, float kerning=0.0f ,bool is_complex=false  ) ;
	TextNode*				text( const String& text, float _x, float _y,	Node* parent, const String& fontName,uint64_t _times=1000, float size =0.5f, Draw3DManager::DrawAlign align=Draw3DManager::DrawAlign::DRAW_ALIGN_CENTER, float kerning=0.0f ,bool is_complex=false  ) ;
	TextNode*				destroyByName(const String& name);
	TextNode*				getNode(const String& name);
	void					destroyAllText();
private:
							TextNodeManager() = default ;
};

JAM_INLINE TextNodeManager&	GetTextNodeMgr() { return (TextNodeManager&) TextNodeManager::getSingleton(); }
// ************************************************************************
}

#endif // JAM_TextNode_h
