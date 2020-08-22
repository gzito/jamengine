/**********************************************************************************
* 
* SkinnedModel.h
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

#ifndef __JAM_MODEL_H__
#define __JAM_MODEL_H__

#include <GL/glew.h>

#include <jam/jam.h>
#include <jam/SkinnedMesh.h>
#include <jam/GameObject.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace jam
{
/*!
	\class SkinnedModel
*/
class JAM_API SkinnedModel : public GameObject
{
public:
							SkinnedModel() ;
	virtual 				~SkinnedModel() ;

	void					draw() ;
	void					load(const String& modelPath) ;

	// it must to be called after load(), otherwise it will return -1
	int						getNumOfAnimations() const ;
		
	void					boneTransform(float TimeInSeconds, size_t animationIdx, std::vector<Matrix4>& Transforms) ;

private:
	using BonesMap = std::map<String,unsigned int> ;
	using AnimationsMap = std::map<String,const aiNodeAnim*> ;

	static Matrix4			assimpToGlmMatrix( const aiMatrix4x4& ) ;
	static Matrix4			assimpToGlmMatrix( const aiMatrix3x3& ) ;

	void					processNode(aiNode* node) ;
	SkinnedMesh*			processMesh(aiMesh* mesh) ;
	void					loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::vector<Ref<Texture2D>>& out ) ;
	void					loadBones( const aiMesh* pMesh, SkinnedMesh* pSkinnedMesh ) ;
//	void					addBoneData(unsigned int vertexIdx, unsigned int boneId, float weight) ;

	void					calcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void					calcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
	void					calcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim);    
	size_t					findScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
	size_t					findRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
	size_t					findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);
	void					readNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4& ParentTransform, size_t animationIdx);
	const aiNodeAnim*		findNodeAnim(size_t animIdx, const String& NodeName);

	// stores animations in maps for faster retrieval 
	void					storeAnim() ;
	void					storeNodeAnim(const aiAnimation* pAnimation, AnimationsMap& animMap) ;

private:
	struct BoneInfo
	{
		Matrix4				BoneOffset;	// brings the vertices from their local space position into their node space
		Matrix4				FinalTransformation;        

		BoneInfo() : BoneOffset(1.0f), FinalTransformation(1.0f) {} ;
	};

	std::vector<SkinnedMesh*>	m_meshes ;
	String					m_folder ;

	// maps a bone name to its index
	unsigned int			m_numBones ;

	Matrix4					m_globalInverseTransform ;

	std::vector<BoneInfo>	m_boneInfo ;
	const aiScene*			m_pScene;
	Assimp::Importer		m_import;

	BonesMap				m_boneMapping ;
	HeapArray<AnimationsMap>	m_animMapping ;
};

}

#endif

