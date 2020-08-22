/**********************************************************************************
* 
* SkyBox.h
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
 
#ifndef __JAM_SKYBOX_H__
#define __JAM_SKYBOX_H__

#include <GL/glew.h>

#include <jam/jam.h>
#include <jam/Component.h>
#include <jam/VertexBufferObject.h>
#include <jam/VertexArrayObject.h>
#include <jam/TextureCubemap.h>
#include <jam/core/array.hpp>
#include <jam/core/geom.h>

namespace jam
{

/*!
	\class Mesh
*/
class JAM_API SkyBox : public Component
{
public:
							SkyBox( GameObject* gameObject ) ;
	virtual					~SkyBox() ;

	TextureCubemap*			getTexture3D() ;
	void					setTexture3D( TextureCubemap* pMaterial ) ;

	void					create() ;
	void					destroy() ;
	void					upload() ;
	void					draw() ;

	bool					isUploaded() const ;

	HeapArray<Vector3>&		getVerticesArray() ;
//	HeapArray<uint16_t>&	getElementsArray() ;

private:
	TextureCubemap*			m_pTexture3D ;

	HeapArray<Vector3>		m_vertices ;
//	HeapArray<uint16_t>		m_elements ;

	VertexBufferObject		m_verticesVbo ;
//	VertexBufferObject		m_elementsVbo ;
	VertexArrayObject		m_vao ;

	bool					m_uploaded ;
};			

JAM_INLINE TextureCubemap* SkyBox::getTexture3D() { return m_pTexture3D; }
JAM_INLINE HeapArray<Vector3>& SkyBox::getVerticesArray() { return m_vertices; }
//	JAM_INLINE HeapArray<uint16_t>& SkyBox::getElementsArray() { return m_elements; }
JAM_INLINE bool SkyBox::isUploaded() const { return m_uploaded; }

}

#endif

