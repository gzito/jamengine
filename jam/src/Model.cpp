/**********************************************************************************
* 
* Model.cpp
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

#include <jam/Model.h>
#include <jam/Gfx.h>
#include <jam/Application.h>
#include <jam/Scene.h>
#include <jam/Camera.h>
#include <jam/core/filesystem.h>

namespace jam
{

	//*******************
	//
	// Class Model
	//
	//*******************

	Model::Model() : 
		GameObject(),
		m_meshes(),
		m_folder()
	{
	}

	Model::~Model()
	{
	}

	void Model::draw()
	{
		Camera* pCam = GetAppMgr().getScene()->getCamera() ;

		for( auto pMesh : m_meshes ) {
			Ref<Shader> pShader = pMesh->getMaterial()->getShader() ;
			pShader->use();

			pShader->setModelMatrix(getTransform().getWorldTformMatrix()) ;

			// the 3 following commands are needed if camera is changed since last frame
			// OR gpu program / uniforms are uninitialized
			pShader->setViewMatrix(pCam->getViewMatrix()) ;
			pShader->setProjectionMatrix(pCam->getProjectionMatrix()) ;
			pShader->setViewPosition( pCam->getPosition() ) ;
	
			pMesh->draw();
		}
	}

	void Model::load(const String& modelPath)
	{
		Assimp::Importer import;
		const aiScene *scene = import.ReadFile(modelPath.c_str(), 
											   aiProcess_Triangulate |
											   aiProcess_FlipWindingOrder |
											   aiProcess_GenSmoothNormals |
											   aiProcess_CalcTangentSpace );	
	
		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			JAM_ERROR( "assimp error: %s", import.GetErrorString() ) ;
		}

		m_folder = jam::getDirname( modelPath );

		processNode(scene->mRootNode, scene);
	}
	
	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		// process all the node's meshes (if any)
		for(unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
			m_meshes.push_back(processMesh(mesh, scene));			
		}
		// then do the same for each of its children
		for(unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}	
	}
	
	Mesh* Model::processMesh(aiMesh* pAiMesh, const aiScene* scene)
	{
		unsigned int numOfVertices = pAiMesh->mNumVertices ;

		unsigned int numOfIndices = 0 ;
		for( unsigned int i=0; i<pAiMesh->mNumFaces; i++ ) {
			// check that faces are triangles
			JAM_ASSERT( (pAiMesh->mFaces[i].mNumIndices == 3) ) ;
			numOfIndices += pAiMesh->mFaces[i].mNumIndices ;
		}

		Mesh* pMesh = new Mesh() ;

		pMesh->create(numOfVertices, numOfIndices) ;

		HeapArray<Vector3>& verticesArray = pMesh->getVerticesArray() ;
		HeapArray<Vector3>& normalsArray = pMesh->getNormalsArray() ;
		HeapArray<Vector2>& texCoordsArray = pMesh->getTexCoordsArray() ;
		HeapArray<uint16_t>& elementsArray = pMesh->getElementsArray() ;

		aiVector3D* aiVertices = pAiMesh->mVertices ;
		aiVector3D* aiNormals = pAiMesh->mNormals ;
		size_t bytesize = numOfVertices * sizeof(Vector3) ;

		verticesArray.copyFrom( (Vector3*)aiVertices ) ;
		normalsArray.copyFrom( (Vector3*)aiNormals ) ;

		// get only first set of texture coords (there are a total of 8 sets)
		aiVector3D* aiTextureCoords = pAiMesh->mTextureCoords[0] ;

		for( unsigned int i = 0; i <numOfVertices; i++ )
		{
			texCoordsArray[i].x = aiTextureCoords[i].x ;
			texCoordsArray[i].y = aiTextureCoords[i].y ;
		}

		// process tangent and bitangent values
		pMesh->getTangentsArray().copyFrom( (Vector3*)pAiMesh->mTangents ) ;
		pMesh->getBitangentsArray().copyFrom( (Vector3*)pAiMesh->mBitangents ) ;
		pMesh->doNotCalculateTangents() ;
		pMesh->getMaterial()->setShader( ShaderManager::getSingleton().getNormalMapping() ) ;

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
			aiMaterial *pAiMaterial = scene->mMaterials[pAiMesh->mMaterialIndex];

			Ref<Material> pMaterial = pMesh->getMaterial() ;

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

			// The wavefront object format (.obj) exports normal maps slightly different 
			// as Assimp's aiTextureType_NORMAL doesn't load its normal maps
			// while aiTextureType_HEIGHT does so I often load them.
			// Of course this is different for each type of loaded model and file format.
			std::vector<Ref<Texture2D>> normalMaps ;
			loadMaterialTextures(pAiMaterial, aiTextureType_HEIGHT, normalMaps);
			if( normalMaps.size() > 0 ) {
				pMaterial->setNormalTexture( normalMaps[0] ) ;
			}
			else {
				pMaterial->setNormalTexture( Ref<Texture2D>(0) ) ;
			}
		}

		return pMesh ;
	}
	
	void Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::vector<Ref<Texture2D>>& out )
	{
		unsigned int textureCountForType = mat->GetTextureCount(type) ;
		if( textureCountForType > 0 ) {
			for(unsigned int i = 0; i < textureCountForType; i++)
			{
				Ref<Texture2D> texture( new Texture2D() ) ;

				aiString str;
				mat->GetTexture(type, i, &str);
				texture->load( jam::appendPath(m_folder, str.C_Str()) ) ;
				out.push_back(texture);

				// we wont to load more that 1 texture
				break;
			}
		}
/*
		else {
 			Texture2D* texture = new Texture2D() ;
 			texture->createDefaultEmpty() ;
 			out.push_back(texture);
 		}
*/
	}

}
