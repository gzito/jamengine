/**********************************************************************************
* 
* SkinnedMesh.h
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
 
#ifndef __JAM_SKINNEDMESH_H__
#define __JAM_SKINNEDMESH_H__

#include <GL/glew.h>

#include <jam/jam.h>
#include <jam/Object.h>
#include <jam/BaseManager.hpp>
#include <jam/Singleton.h>
#include <jam/VertexBufferObject.h>
#include <jam/VertexArrayObject.h>
#include <jam/Material.h>
#include <jam/core/array.hpp>

#include <assimp/matrix4x4.h>

namespace jam
{

/*!
	\class SkinnedMesh
*/
class JAM_API SkinnedMesh : public NamedTaggedObject
{
public:
							SkinnedMesh() ;
	virtual					~SkinnedMesh() ;

	Material*				getMaterial() ;

	void					create(size_t numOfVertices, size_t numOfElements) ;
	void					destroy() ;
	void					upload() ;
	void					draw() ;

	bool					isUploaded() const ;

	HeapArray<Vector3>&		getVerticesArray() ;
	HeapArray<Vector3>&		getNormalsArray() ;
	HeapArray<Vector2>&		getTexCoordsArray() ;
	HeapArray<glm::ivec4>&	getBonesIdArray() ;
	HeapArray<glm::vec4>&	getWeightsArray() ;
	HeapArray<uint16_t>&	getElementsArray() ;

private:
	Material*				m_pMaterial ;

	HeapArray<Vector3>		m_vertices ;
	HeapArray<Vector3>		m_normals ;
	HeapArray<Vector2>		m_texCoords ;
	HeapArray<glm::ivec4>	m_bonesId ;
	HeapArray<glm::vec4>	m_weights ;
	HeapArray<uint16_t>		m_elements ;

	Array<VertexBufferObject,6>	m_vbos ;
	VertexArrayObject		m_vao ;

	bool					m_uploaded ;
};			

JAM_INLINE Material*			SkinnedMesh::getMaterial() { return m_pMaterial; }
JAM_INLINE HeapArray<Vector3>&	SkinnedMesh::getVerticesArray() { return m_vertices; }
JAM_INLINE HeapArray<Vector3>&	SkinnedMesh::getNormalsArray() { return m_normals; }
JAM_INLINE HeapArray<Vector2>&	SkinnedMesh::getTexCoordsArray() { return m_texCoords; }
JAM_INLINE HeapArray<glm::ivec4>&	SkinnedMesh::getBonesIdArray() { return m_bonesId; }
JAM_INLINE HeapArray<glm::vec4>&	SkinnedMesh::getWeightsArray() { return m_weights; }
JAM_INLINE HeapArray<uint16_t>&		SkinnedMesh::getElementsArray() { return m_elements; }
JAM_INLINE bool						SkinnedMesh::isUploaded() const { return m_uploaded; }

class JAM_API SkinnedMeshManager : public NamedTaggedObjectManager<SkinnedMesh>, public jam::Singleton<SkinnedMeshManager>
{
	friend class jam::Singleton<SkinnedMeshManager> ;

public:

protected:
							SkinnedMeshManager() = default ;
private:
};

JAM_INLINE  SkinnedMeshManager&		GetSkinnedMeshMgr() { return (SkinnedMeshManager&) SkinnedMeshManager::getSingleton(); }

}

#endif
