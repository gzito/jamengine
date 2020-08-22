/**********************************************************************************
* 
* Quad2D.h
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

#ifndef __QUAD2D_H__
#define __QUAD2D_H__

#include <jam/jam.h>
#include <jam/Texture2D.h>
#include <jam/Shader.h>
#include <jam/Color.h>
#include <jam/VertexBufferObject.h>
#include <jam/VertexArrayObject.h>
#include <jam/core/array.hpp>
#include <jam/core/geom.h>


struct Quad2D
{
											Quad2D() ;
											~Quad2D() ;

	void									upload();
	void									draw() ;
	void									setTexture( const jam::Ref<jam::Texture2D>& pTex ) { m_pTexture = pTex ; }

	jam::HeapArray<jam::Vector2>			m_vertices ;
	jam::HeapArray<jam::Vector2>			m_texCoords ;
	jam::HeapArray<jam::Color>				m_colors ;
	jam::HeapArray<jam::U16>				m_elements ;

	jam::Array<jam::VertexBufferObject,3>	m_vbos ;
	jam::VertexBufferObject					m_vboElements ;
	jam::VertexArrayObject					m_vao ;
	jam::Ref<jam::Shader>					m_pProg ;
	jam::Ref<jam::Texture2D>				m_pTexture ;

	bool									m_uploaded ;
};

#endif	// __QUAD2D_H__
