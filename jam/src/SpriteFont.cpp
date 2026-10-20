/**********************************************************************************
* 
* SpriteFont.cpp
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

#include <jam/SpriteFont.h>
#include <jam/DrawItemManager.h>
#include <jam/core/filesystem.h>
#include <jam/StringTokenizer.h>

#include <tinyxml2.h>

namespace jam
{

void SpriteFont::load(const jam::String& filename)
{
	tinyxml2::XMLDocument xmlDoc ;
	tinyxml2::XMLError eResult = xmlDoc.LoadFile( filename.c_str() ) ;
	if( eResult != tinyxml2::XML_SUCCESS ) {
		JAM_ERROR( "Cannot open xml file" ) ;
	}

	tinyxml2::XMLNode* pRoot = xmlDoc.FirstChild() ;
	tinyxml2::XMLElement* fontElem = pRoot->NextSiblingElement("font");
	tinyxml2::XMLElement* infoElem = fontElem->FirstChildElement("info") ;
	const char* spacing = infoElem->Attribute("spacing") ;
	StringTokenizer stok(spacing,false,",") ;
	m_spacing = std::stoi(stok.nextToken()) ;
	tinyxml2::XMLElement* commonElem = fontElem->FirstChildElement("common") ;
	m_lineHeight = commonElem->IntAttribute("lineHeight") ;
	m_base = commonElem->IntAttribute("base") ;
	m_pages = commonElem->IntAttribute("pages") ;
	m_pTexture.resize(m_pages) ;
	tinyxml2::XMLElement* pagesElem = fontElem->FirstChildElement("pages") ;
	tinyxml2::XMLElement* pageElem = pagesElem->FirstChildElement("page") ;
	while( pageElem != nullptr ) {
		int pageId = pageElem->IntAttribute("id") ;
		const char* pageFile = pageElem->Attribute("file") ;
		m_pTexture[pageId].assign( GetDrawItemMgr().loadTexture( jam::appendPath( jam::getDirname(filename), pageFile ), "fonts" )->getTexture(), true ) ;
		
		pageElem = pageElem->NextSiblingElement("page") ;
	}

	tinyxml2::XMLElement* charsElem = fontElem->FirstChildElement("chars") ;

	tinyxml2::XMLElement* charElem = charsElem->FirstChildElement("char") ;
	while( charElem != nullptr ) {
		Glyph g ;
		g.id		= charElem->IntAttribute("id") ;	
		g.x			= charElem->IntAttribute("x") ;	
		g.y			= charElem->IntAttribute("y") ;	
		g.width		= charElem->IntAttribute("width") ;	
		g.height	= charElem->IntAttribute("height") ;	
		g.xoffset	= charElem->IntAttribute("xoffset") ;	
		g.yoffset	= charElem->IntAttribute("yoffset") ;	
		g.xadvance	= charElem->IntAttribute("xadvance") ;
		g.page		= charElem->IntAttribute("page") ;
		m_glyphs.insert( std::pair<int,Glyph>(g.id,g) ) ;

		charElem = charElem->NextSiblingElement("char") ;
	}
}

/// <summary>
/// Returns the size of a string when rendered in this font.
/// </summary>
/// <param name="text">The text to measure.</param>
/// <returns>The size, in pixels, of 'text' when rendered in
/// this font.</returns>

Vector2 SpriteFont::MeasureString(const String& text)
{
	Vector2 size;
	MeasureString(text,size);
	return size;
}

void SpriteFont::MeasureString(const String& text,Vector2& size)
{
	if (text.size() == 0)
    {
		size = Vector2(0,0);
		return;
	}

	float width = 0.0f;
	float finalLineHeight = (float)getLineHeight();
            
	Vector2 offset = Vector2(0,0);
    bool firstGlyphOfLine = true;

    for (auto i = 0; i < text.size(); ++i)
    {
        String::value_type c = text[i];

        if (c == '\r')
            continue;

        if (c == '\n')
        {
            finalLineHeight = getLineHeight();

            offset.x = 0;
            offset.y += getLineHeight();
            firstGlyphOfLine = true;
            continue;
        }

        auto glyphs = getGlyphs() ;
        const auto& pCurrentGlyph = glyphs[c] ;

        // The first character on a line might have a negative left side bearing.
        // In this scenario, SpriteBatch/SpriteFont normally offset the text to the right,
        //  so that text does not hang off the left side of its rectangle.
        if (firstGlyphOfLine) {
            offset.x = 0 ;
            firstGlyphOfLine = false;
        }

//		offset.x += pCurrentGlyph.width ;
		if( i != text.size()-1 ) {
			offset.x += pCurrentGlyph.xadvance ;
		}
		else {
			offset.x += pCurrentGlyph.width ;
		}

        if (pCurrentGlyph.height > finalLineHeight)
            finalLineHeight = pCurrentGlyph.height;
    }

    size.x = offset.x;
    size.y = offset.y + finalLineHeight;
}

}
