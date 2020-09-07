/**********************************************************************************
* 
* SpriteMesh.h
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

#ifndef __JAM_SPRITEMESH_H__
#define __JAM_SPRITEMESH_H__


#include <jam/jam.h>
#include <jam/VertexBufferObject.h>
#include <jam/VertexArrayObject.h>
#include <jam/Material.h>
#include <jam/core/array.hpp>
#include <jam/Draw2d.h>

namespace jam
{

/**
	Represents a sprite vertex buffer in which each vertex attribute is tight packed in its dedicated array
*/
class JAM_API SpriteMesh : public Collectible
{
public:
							SpriteMesh() ;
	virtual					~SpriteMesh() ;

	Material*				getMaterial() ;
	void					setMaterial( Material* pMaterial ) ;

	void					create(int numOfVertices, int numOfElements) ;
	void					destroy() ;
	void					upload() ;

	bool					isUploaded() const ;

	HeapArray<Vector2>&		getVerticesArray() ;
	HeapArray<Vector2>&		getTexCoordsArray() ;
	HeapArray<uint16_t>&	getElementsArray() ;

	void					set( float hw, float hh ) ;

private:
	Material*				m_pMaterial ;

	HeapArray<Vector2>		m_vertices ;
	HeapArray<Vector2>		m_texCoords ;
	HeapArray<uint16_t>		m_elements ;

	VertexBufferObject		m_verticesVbo ;
	VertexBufferObject		m_texCoordsVbo ;
	VertexBufferObject		m_elementsVbo ;
	VertexArrayObject		m_vao ;

	bool					m_uploaded ;
};			

JAM_INLINE Material*				SpriteMesh::getMaterial() { return m_pMaterial; }
JAM_INLINE HeapArray<Vector2>&		SpriteMesh::getVerticesArray() { return m_vertices; }
JAM_INLINE HeapArray<Vector2>&		SpriteMesh::getTexCoordsArray() { return m_texCoords; }
JAM_INLINE HeapArray<uint16_t>&		SpriteMesh::getElementsArray() { return m_elements; }
JAM_INLINE bool						SpriteMesh::isUploaded() const { return m_uploaded; }

}

#endif // __JAM_SPRITEMESH_H__ 
