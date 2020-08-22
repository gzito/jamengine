/**********************************************************************************
* 
* SkinnedModel.cpp
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

#include <jam/SkinnedModel.h>
#include <jam/Gfx.h>
#include <jam/Application.h>
#include <jam/Scene.h>
#include <jam/Camera.h>
#include <jam/Transform.h>
#include <jam/core/filesystem.h>
#include <jam/core/geom.h>

namespace jam
{

	//*******************
	//
	// Class SkinnedModel
	//
	//*******************

	SkinnedModel::SkinnedModel() :
		GameObject(),
		m_meshes(),
		m_folder(),
		m_numBones(0),
		m_globalInverseTransform(),
		m_boneInfo(),
		m_pScene(0),
		m_import(),
		m_boneMapping(),
		m_animMapping()
	{
	}

	SkinnedModel::~SkinnedModel()
	{
	}

	void SkinnedModel::draw()
	{
		static char buff[32] = {0} ;

		Camera* pCam = GetAppMgr().getScene()->getCamera() ;

		SkinnedMesh* pMesh = 0 ;
		Material* pMaterial = 0 ;

		std::vector<Matrix4> transforms;
               
		float runningTime = GetAppMgr().getTotalElapsed() ;

		boneTransform(runningTime, 0, transforms);
        
		bool boneTransformSet = false ;

		for( size_t i = 0; i < m_meshes.size(); i++ ) {
			Ref<Shader> pProg(0) ;
			pMesh = m_meshes[i] ;
			pMaterial = pMesh->getMaterial() ;
			pProg = pMaterial->getShader() ;
			pProg->use();

			if( !boneTransformSet ) {
				JAM_ASSERT(transforms.size() < 100);
				for (unsigned int i = 0 ; i < transforms.size() ; i++) {				
					sprintf(buff,JAM_PROGRAM_UNIFORM_BONES,i) ;
					pProg->setUniform( buff, transforms[i] ) ;
				}
				boneTransformSet = true ;
			}

			pProg->setModelMatrix(getTransform().getWorldTformMatrix()) ;

			// the 3 following commands are needed if camera is changed since last frame
			// OR gpu program / uniforms are uninitialized
			pProg->setViewMatrix(pCam->getViewMatrix()) ;
			pProg->setProjectionMatrix(pCam->getProjectionMatrix()) ;
			pProg->setViewPosition( pCam->getPosition() ) ;

			m_meshes[i]->draw();
		}
	}

	void SkinnedModel::load(const String& modelPath)
	{
		m_pScene = m_import.ReadFile(modelPath.c_str(), aiProcess_LimitBoneWeights | aiProcess_GenNormals | aiProcess_Triangulate | aiProcess_FlipWindingOrder );	
	
		if( !m_pScene || m_pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_pScene->mRootNode ) 
		{
			JAM_ERROR( "assimp error: %s", m_import.GetErrorString() ) ;
			return;
		}
		m_folder = jam::getDirname( modelPath );

		// we take the model (root) matrix and invert it
		m_globalInverseTransform = glm::inverse( assimpToGlmMatrix(m_pScene->mRootNode->mTransformation) ) ;

		if( getNumOfAnimations() == 0 ) {
			JAM_ERROR( "No animations found" ) ;
		}

		storeAnim() ;
		processNode(m_pScene->mRootNode);
	}

	int SkinnedModel::getNumOfAnimations() const
	{
		return ((m_pScene != 0) ? m_pScene->mNumAnimations : -1) ;
	}
	
	void SkinnedModel::boneTransform(float TimeInSeconds, size_t animationIdx, std::vector<Matrix4>& Transforms)
	{
		Matrix4 Identity(1.0f);
    
		float TicksPerSecond = (float)(m_pScene->mAnimations[animationIdx]->mTicksPerSecond != 0 ? m_pScene->mAnimations[animationIdx]->mTicksPerSecond : 25.0f);
		float TimeInTicks = TimeInSeconds * TicksPerSecond;
		float AnimationTime = fmod(TimeInTicks, (float)m_pScene->mAnimations[animationIdx]->mDuration);

		readNodeHeirarchy(AnimationTime, m_pScene->mRootNode, Identity, animationIdx);

		Transforms.resize(m_numBones);
		for( size_t i = 0 ; i < m_numBones ; i++ ) {
			Transforms[i] = m_boneInfo[i].FinalTransformation;
		}
	}

	void SkinnedModel::processNode(aiNode* node)
	{
		// process all the node's meshes (if any)
		for( size_t i = 0; i < node->mNumMeshes; i++ )
		{
			aiMesh *mesh = m_pScene->mMeshes[node->mMeshes[i]]; 
			m_meshes.push_back(processMesh(mesh));			
		}
		// then do the same for each of its children
		for( size_t i = 0; i < node->mNumChildren; i++ )
		{
			processNode(node->mChildren[i]);
		}	
	}
	
	SkinnedMesh* SkinnedModel::processMesh(aiMesh* pAiMesh)
	{
		size_t numOfVertices = pAiMesh->mNumVertices ;

		size_t numOfIndices = 0 ;
		for( size_t i=0; i<pAiMesh->mNumFaces; i++ ) {
			// check that faces are triangles
			JAM_ASSERT(pAiMesh->mFaces[i].mNumIndices == 3) ;
			numOfIndices += pAiMesh->mFaces[i].mNumIndices ;
		}

		SkinnedMesh* pMesh = new SkinnedMesh() ;
		pMesh->getMaterial()->setShader( ShaderManager::getSingleton().getSkinningLit() ) ;

		pMesh->create(numOfVertices, numOfIndices) ;

		HeapArray<Vector3>& verticesArray = pMesh->getVerticesArray() ;
		HeapArray<Vector3>& normalsArray = pMesh->getNormalsArray() ;
		HeapArray<Vector2>& texCoordsArray = pMesh->getTexCoordsArray() ;
		HeapArray<uint16_t>& elementsArray = pMesh->getElementsArray() ;

		aiVector3D* aiVertices = pAiMesh->mVertices ;
		aiVector3D* aiNormals = pAiMesh->mNormals ;
		size_t bytesize = numOfVertices * sizeof(Vector3) ;

		memcpy( verticesArray.data(), aiVertices, bytesize ) ;
		memcpy( normalsArray.data(), aiNormals, bytesize ) ;

		// get only first set of texture coords (there are a total of 8 sets)
		aiVector3D* aiTextureCoords = pAiMesh->mTextureCoords[0] ;

		for( size_t i = 0; i <numOfVertices; i++ )
		{
			texCoordsArray[i].x = aiTextureCoords[i].x ;
			texCoordsArray[i].y = aiTextureCoords[i].y ;
		}

		if( pAiMesh->HasBones() ) {
			loadBones( pAiMesh, pMesh ) ;
		}

		// process indices
		unsigned int idx = 0 ;
		unsigned int* aiIndices = 0 ;
		for(unsigned int i = 0; i < pAiMesh->mNumFaces; i++)
		{
			aiIndices = pAiMesh->mFaces[i].mIndices;
			idx = i*3 ;
			elementsArray[idx]   = aiIndices[0] ;
			elementsArray[idx+1] = aiIndices[1] ;
			elementsArray[idx+2] = aiIndices[2] ;
		}  

		// process material
		if(pAiMesh->mMaterialIndex >= 0)
		{
			aiMaterial *pAiMaterial = m_pScene->mMaterials[pAiMesh->mMaterialIndex];

			Material* pMaterial = pMesh->getMaterial() ;

			float matShininess = 0.0f ;
			if( AI_SUCCESS == (pAiMaterial->Get( AI_MATKEY_SHININESS, matShininess )) ) {
				pMaterial->setShininess( matShininess ) ;
			}

			std::vector<Ref<Texture2D>> diffuseMaps ;
			loadMaterialTextures(pAiMaterial, aiTextureType_DIFFUSE, diffuseMaps);
			if( diffuseMaps.size() > 0 ) {
				pMaterial->setDiffuseTexture( diffuseMaps[0] ) ;
			}
			else {
				pMaterial->setDiffuseTexture( Ref<Texture2D>(0) ) ;
			}

			std::vector<Ref<Texture2D>> specularMaps ;
			loadMaterialTextures(pAiMaterial, aiTextureType_SPECULAR, specularMaps);
			if( specularMaps.size() > 0 ) {
				pMaterial->setSpecularTexture( specularMaps[0] ) ;
			}
			else {
				pMaterial->setSpecularTexture( Ref<Texture2D>(0) ) ;
			}
		}

		return pMesh ;
	}
	
	void SkinnedModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::vector<Ref<Texture2D>>& out )
	{
		unsigned int textureCountForType = mat->GetTextureCount(type) ;
		if( textureCountForType > 0 ) {
			for( size_t i = 0; i < textureCountForType; i++ )
			{
				Ref<Texture2D> texture( new Texture2D() ) ;

				aiString str;
				mat->GetTexture(type, (unsigned int)i, &str);
				texture->load( appendPath(m_folder, str.C_Str()) ) ;
				out.push_back(texture);

				// we wont to load more that 1 texture
				break;
			}
		}
// 		else {
// 			Texture2D* texture = new Texture2D() ;
// 			texture->createDefaultEmpty() ;
// 			out.push_back(texture);
// 		}
	}

	void SkinnedModel::loadBones( const aiMesh* pMesh, SkinnedMesh* pSkinnedMesh )
	{
		BonesMap::iterator it ;
		unsigned int boneId = 0;        
		size_t vertexIdx = 0 ;
		float weight = 0.0f ;
		unsigned int i, k ;
		size_t j ;

		glm::ivec4* pBonesId = pSkinnedMesh->getBonesIdArray().data() ;
		glm::vec4*  pWeights = pSkinnedMesh->getWeightsArray().data() ;

		for ( i = 0 ; i < pMesh->mNumBones ; i++) {                
			String boneName = pMesh->mBones[i]->mName.data ;
        
			it = m_boneMapping.find(boneName) ;
			if( it == m_boneMapping.end() ) {
				// Allocate an index for a new bone
				boneId = m_numBones;
				m_numBones++;            
				BoneInfo bi;			
				bi.BoneOffset = assimpToGlmMatrix( pMesh->mBones[i]->mOffsetMatrix );            
				m_boneInfo.push_back(bi);
				m_boneMapping[boneName] = boneId;
			}
			else {
				boneId = it->second ;
			}                      
        
			aiVertexWeight* pAiWeights = pMesh->mBones[i]->mWeights ;
			for( j = 0 ; j < pMesh->mBones[i]->mNumWeights ; j++ ) {
				vertexIdx = pAiWeights[j].mVertexId;
				weight    = pAiWeights[j].mWeight; 

				for( k = 0 ; k < 4 ; k++ ) {
					if (pWeights[vertexIdx][k] == 0.0f) {
						pBonesId[vertexIdx][k] = boneId ;
						pWeights[vertexIdx][k] = weight ;
						break ;
					}

					// should never get here - more bones per vertex than we have space for
					if( k == 3 ) {
						JAM_ASSERT(0);
					} 
				}
			}
		}
	}

	Matrix4 SkinnedModel::assimpToGlmMatrix( const aiMatrix4x4& m )
	{
		Matrix4 out (
			m.a1, m.b1, m.c1, m.d1,		// <- first column
			m.a2, m.b2, m.c2, m.d2,
			m.a3, m.b3, m.c3, m.d3,
			m.a4, m.b4, m.c4, m.d4 );

		return out ;
	}

	Matrix4 SkinnedModel::assimpToGlmMatrix( const aiMatrix3x3& m )
	{
		Matrix4 out (
			m.a1, m.b1, m.c1, 0.0f,
			m.a2, m.b2, m.c2, 0.0f,
			m.a3, m.b3, m.c3, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		return out ;
	}

	size_t SkinnedModel::findPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{    
		for( size_t i = 0 ; i < pNodeAnim->mNumPositionKeys - 1 ; i++ ) {
			if( AnimationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime ) {
				return i;
			}
		}
    
		JAM_ASSERT(0);

		return 0;
	}


	size_t SkinnedModel::findRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		JAM_ASSERT(pNodeAnim->mNumRotationKeys > 0);

		for( size_t i = 0 ; i < pNodeAnim->mNumRotationKeys - 1 ; i++ ) {
			if( AnimationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime ) {
				return i;
			}
		}
    
		JAM_ASSERT(0);

		return 0;
	}


	size_t SkinnedModel::findScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		JAM_ASSERT(pNodeAnim->mNumScalingKeys > 0);
    
		for( size_t i = 0 ; i < pNodeAnim->mNumScalingKeys - 1 ; i++ ) {
			if( AnimationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime ) {
				return i;
			}
		}
    
		JAM_ASSERT(0);

		return 0;
	}


	void SkinnedModel::calcInterpolatedPosition(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if( pNodeAnim->mNumPositionKeys == 1 ) {
			Out = pNodeAnim->mPositionKeys[0].mValue;
			return;
		}
            
		size_t PositionIndex = findPosition(AnimationTime, pNodeAnim);
		size_t NextPositionIndex = (PositionIndex + 1);
		JAM_ASSERT(NextPositionIndex < pNodeAnim->mNumPositionKeys);
		float DeltaTime = (float)(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - pNodeAnim->mPositionKeys[PositionIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mPositionKeys[PositionIndex].mTime) / DeltaTime;
		JAM_ASSERT(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = pNodeAnim->mPositionKeys[PositionIndex].mValue;
		const aiVector3D& End = pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}


	void SkinnedModel::calcInterpolatedRotation(aiQuaternion& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		// we need at least two values to interpolate...
		if( pNodeAnim->mNumRotationKeys == 1 ) {
			Out = pNodeAnim->mRotationKeys[0].mValue;
			return;
		}
    
		size_t RotationIndex = findRotation(AnimationTime, pNodeAnim);
		size_t NextRotationIndex = (RotationIndex + 1);
		JAM_ASSERT(NextRotationIndex < pNodeAnim->mNumRotationKeys);
		float DeltaTime = (float)(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - pNodeAnim->mRotationKeys[RotationIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mRotationKeys[RotationIndex].mTime) / DeltaTime;
		JAM_ASSERT(Factor >= 0.0f && Factor <= 1.0f);
		const aiQuaternion& StartRotationQ = pNodeAnim->mRotationKeys[RotationIndex].mValue;
		const aiQuaternion& EndRotationQ   = pNodeAnim->mRotationKeys[NextRotationIndex].mValue;    
		aiQuaternion::Interpolate(Out, StartRotationQ, EndRotationQ, Factor);
		Out = Out.Normalize();
	}


	void SkinnedModel::calcInterpolatedScaling(aiVector3D& Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
	{
		if( pNodeAnim->mNumScalingKeys == 1 ) {
			Out = pNodeAnim->mScalingKeys[0].mValue;
			return;
		}

		size_t ScalingIndex = findScaling(AnimationTime, pNodeAnim);
		size_t NextScalingIndex = (ScalingIndex + 1);
		JAM_ASSERT(NextScalingIndex < pNodeAnim->mNumScalingKeys);
		float DeltaTime = (float)(pNodeAnim->mScalingKeys[NextScalingIndex].mTime - pNodeAnim->mScalingKeys[ScalingIndex].mTime);
		float Factor = (AnimationTime - (float)pNodeAnim->mScalingKeys[ScalingIndex].mTime) / DeltaTime;
		JAM_ASSERT(Factor >= 0.0f && Factor <= 1.0f);
		const aiVector3D& Start = pNodeAnim->mScalingKeys[ScalingIndex].mValue;
		const aiVector3D& End   = pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
		aiVector3D Delta = End - Start;
		Out = Start + Factor * Delta;
	}


	void SkinnedModel::readNodeHeirarchy(float AnimationTime, const aiNode* pNode, const Matrix4& ParentTransform, size_t animationIdx)
	{    
		Matrix4 NodeTransformation = assimpToGlmMatrix( pNode->mTransformation );
     
		const aiAnimation* pAnimation = m_pScene->mAnimations[animationIdx];
		const char* NodeName = pNode->mName.data ;
		const aiNodeAnim* pNodeAnim = findNodeAnim(animationIdx, NodeName);
    
		if( pNodeAnim ) {
			// Interpolate scaling and generate scaling transformation matrix
			aiVector3D Scaling;
			calcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
			Matrix4 ScalingM(1.0f);
			ScalingM = glm::scale(ScalingM,Vector3(Scaling.x, Scaling.y, Scaling.z)) ;
        
			// Interpolate rotation and generate rotation transformation matrix
			aiQuaternion RotationQ;
			calcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);       
			Matrix4 RotationM(1.0f);
			RotationM = assimpToGlmMatrix( RotationQ.GetMatrix() );

			// Interpolate translation and generate translation transformation matrix
			aiVector3D Translation;
			calcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
			Matrix4 TranslationM(1.0f);
			TranslationM = glm::translate(TranslationM,Vector3(Translation.x, Translation.y, Translation.z)) ;
        
			// Combine the above transformations
			NodeTransformation = TranslationM * RotationM * ScalingM ;
		}

		Matrix4  GlobalTransformation = ParentTransform * NodeTransformation;
    
		if( m_boneMapping.find(NodeName) != m_boneMapping.end() ) {
			size_t BoneIndex = m_boneMapping[NodeName];
			m_boneInfo[BoneIndex].FinalTransformation = m_globalInverseTransform * GlobalTransformation * m_boneInfo[BoneIndex].BoneOffset;
		}
   
		for( size_t i = 0 ; i < pNode->mNumChildren ; i++ ) {
			readNodeHeirarchy(AnimationTime, pNode->mChildren[i], GlobalTransformation, animationIdx);
		}
	}


	const aiNodeAnim* SkinnedModel::findNodeAnim(size_t animIdx, const String& NodeName)
	{
		const aiNodeAnim* out = 0 ;

		auto it = m_animMapping[animIdx].find(NodeName) ;
		if( it != m_animMapping[animIdx].end() ) {
			out = it->second ;
		}

		return out;
	}

	void SkinnedModel::storeAnim()
	{
		size_t numOfAnimations = m_pScene->mNumAnimations ;
		m_animMapping.create(numOfAnimations) ;
		for( size_t i=0; i<numOfAnimations; i++ ) {
			storeNodeAnim( m_pScene->mAnimations[i], m_animMapping[i] ) ;
		}
	}

	void SkinnedModel::storeNodeAnim( const aiAnimation* pAnimation, AnimationsMap& animMap )
	{
		for( size_t i = 0 ; i < pAnimation->mNumChannels ; i++ ) {
			const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];
			if( pNodeAnim->mNodeName.data ) {
				animMap[String(pNodeAnim->mNodeName.data)] = pNodeAnim ;
			}
		}
	}
}
