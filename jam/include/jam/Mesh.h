/**********************************************************************************
* 
* Mesh.h
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
 
#ifndef __JAM_MESH_H__
#define __JAM_MESH_H__

#include <GL/glew.h>

#include <jam/jam.h>
#include <jam/Singleton.h>
#include <jam/VertexBufferObject.h>
#include <jam/VertexArrayObject.h>
#include <jam/Material.h>
#include <jam/Ref.hpp>
#include <jam/core/array.hpp>
#include <jam/core/geom.h>

namespace jam
{

/*!
	\class Mesh
*/
class JAM_API Mesh : public RefCountedObject
{
public:
							Mesh() ;
	virtual					~Mesh() ;

	Material*				getMaterial() ;
	void					setMaterial( Material* pMaterial ) ;

	void					create(int numOfVertices, int numOfElements) ;
	void					destroy() ;
	void					upload() ;
	void					draw() ;

	bool					isUploaded() const ;

	void					disableTangents( bool value = true ) ;

	void					doNotCalculateTangents() ;

	HeapArray<Vector3>&		getVerticesArray() ;
	HeapArray<Vector3>&		getNormalsArray() ;
	HeapArray<Vector2>&		getTexCoordsArray() ;
	HeapArray<Vector3>&		getTangentsArray() ;
	HeapArray<Vector3>&		getBitangentsArray() ;
	HeapArray<uint16_t>&	getElementsArray() ;

private:
	void					calculateTangents() ;

	Ref<Material>			m_pMaterial ;

	HeapArray<Vector3>		m_vertices ;
	HeapArray<Vector3>		m_normals ;
	HeapArray<Vector2>		m_texCoords ;
	HeapArray<Vector3>		m_tangents ;
	HeapArray<Vector3>		m_bitangents ;
	HeapArray<uint16_t>		m_elements ;

	Array<VertexBufferObject,4>	m_vbos ;
	VertexBufferObject		m_elementsVbo ;
	VertexArrayObject		m_vao ;

	bool					m_uploaded ;
	bool					m_tangentsDisabled ;
	bool					m_needsCalculateTangents ;
};			

JAM_INLINE Material*				Mesh::getMaterial() { return m_pMaterial; }
JAM_INLINE HeapArray<Vector3>&		Mesh::getVerticesArray() { return m_vertices; }
JAM_INLINE HeapArray<Vector3>&		Mesh::getNormalsArray() { return m_normals; }
JAM_INLINE HeapArray<Vector2>&		Mesh::getTexCoordsArray() { return m_texCoords; }
JAM_INLINE HeapArray<Vector3>&		Mesh::getTangentsArray() { return m_tangents; }
JAM_INLINE HeapArray<Vector3>&		Mesh::getBitangentsArray() { return m_bitangents; }
JAM_INLINE HeapArray<uint16_t>&		Mesh::getElementsArray() { return m_elements; }
JAM_INLINE bool						Mesh::isUploaded() const { return m_uploaded; }

}

#endif

