/**********************************************************************************
* 
* SpriteFont.h
* 
* This file is part of Jam
* 
* Copyright (c) 2014-2020 Giovanni Zito.
* Copyright (c) 2014-2020 Jam contributors (cf. AUTHORS.md)
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

#ifndef __JAM_SPRITEFONT_H__
#define __JAM_SPRITEFONT_H__

#include <jam/jam.h>
#include <jam/Texture2D.h>
#include <jam/Ref.hpp>
#include <jam/core/geom.h>

#include <map>
#include <vector>

namespace jam
{

/**
	
*/
class JAM_API SpriteFont : public RefCountedObject
{
public:
	struct Glyph
	{
		int					id ;
		int					x ;
		int					y ;
		int					width ;
		int					height ;
		int					xoffset ;
		int					yoffset ;
		int					xadvance ;
		int					page ;
	};

	using					GlyphsMap = std::map<int,Glyph> ;
	using					TextureList = std::vector<Ref<Texture2D>> ;

public:
							SpriteFont() = default ;
	void					load( const String& filename ) ;

	Texture2D*				getTexture(int idx) { return m_pTexture[idx].get(); } ;
	const Texture2D*		getTexture(int idx) const { return m_pTexture[idx].get(); } ;

	const GlyphsMap&		getGlyphs() const { return m_glyphs; } ;
	int						getLineHeight() const { return m_lineHeight; }
	int						getBase() const { return m_base; }
	int						getSpacing() const { return m_spacing; }

	/// <summary>
	/// Returns the size of a string when rendered in this font.
	/// </summary>
	/// <param name="text">The text to measure.</param>
	/// <returns>The size, in pixels, of 'text' when rendered in
	/// this font.</returns>
	Vector2					MeasureString(const String& text) ;
	void					MeasureString(const String& text, Vector2& size) ;

protected:
	virtual					~SpriteFont() = default ;

private:
							SpriteFont( const SpriteFont& ) = delete ;
	SpriteFont&				operator=( SpriteFont& ) = delete ;

	int						m_lineHeight ;
	int						m_base ;
	int						m_spacing ;
	int						m_pages ;
	GlyphsMap				m_glyphs ;
	TextureList				m_pTexture ;
};
	
}


#endif // __JAM_SPRITEFONT_H__
