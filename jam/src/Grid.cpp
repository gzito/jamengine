/**********************************************************************************
* 
* Grid.cpp
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

#include "jam/Grid.h"
#include "jam/Grabber.h"
#include "jam/StridedVertexBuffer.h"
#include "jam/DrawItem.h"
#include "jam/DeviceManager.h"
#include "jam/Draw3dManager.h"
#include "jam/Gfx.h"

namespace jam
{
// implementation of GridBase

GridBase::GridBase() :
	m_bActive(false),
	m_nReuseGrid(false),
	m_sGridSize(),
	m_pDrawItem(0),
	m_obStep(),
	m_pGrabber(0),
	m_bIsTextureFlipped(false),
	m_pVertexBuffer(0),
	m_pOriginalVertexBuffer(0)
{
}

GridBase::~GridBase()
{
	setActive(false);
	JAM_DELETE(m_pGrabber);
	JAM_RELEASE(m_pDrawItem) ;
	JAM_DELETE(m_pVertexBuffer);
	JAM_DELETE(m_pOriginalVertexBuffer);
}

//GridBase* GridBase::gridWithSize(const GridSize& gridSize)
//{
//    GridBase *pGridBase = new GridBase();
//
//	if (pGridBase)
//	{
//        if (pGridBase->initWithSize(gridSize))
//		{
//			pGridBase->autorelease();
//		}
//		else
//		{
//			JAM_RELEASE_NULL(pGridBase);
//		}
//	}
//
//	return pGridBase;
//}
//
//GridBase* GridBase::gridWithSize(const GridSize& gridSize, DrawItem *pDrawItem, bool flipped)
//{
//	GridBase *pGridBase = new GridBase();
//
//	if (pGridBase)
//	{
//		if (pGridBase->initWithSize(gridSize, pDrawItem, flipped))
//		{
//			pGridBase->autorelease();
//		}
//		else
//		{
//			JAM_RELEASE_NULL(pGridBase);
//		}
//	}
//
//	return pGridBase;
//}

bool GridBase::initWithSize(const GridSize& gridSize, DrawItem *pDrawItem, bool bFlipped)
{
	bool bRet = true;

	m_bActive = false;
	m_nReuseGrid = 0;
	m_sGridSize = gridSize;

	m_pDrawItem = pDrawItem;
	JAM_ADDREF(pDrawItem);
	m_bIsTextureFlipped = bFlipped;

	m_obStep.x = m_pDrawItem->getWidth() / m_sGridSize.x;
	m_obStep.y = m_pDrawItem->getHeight() / m_sGridSize.y;

	m_pGrabber = new Grabber();
	if (m_pGrabber) {
		m_pGrabber->grab(m_pDrawItem);
	}
	else {
        bRet = false;
	}

	calculateVertexPoints();

	return bRet;
}

bool GridBase::initWithSize(const GridSize& gridSize)
{
	int32_t textureSize = 8;
	int32_t w = GetDeviceMgr().getNativeDisplayWidth() ;
	int32_t h = GetDeviceMgr().getNativeDisplayHeight() ;
	while (textureSize < w || textureSize < h) {
		textureSize *= 2;
	}

	m_pDrawItem = DrawItem::create(Ref<Texture2D>(0),Rect(0,0,textureSize,textureSize));
	m_pDrawItem->autorelease() ;

	if (initWithSize(gridSize, m_pDrawItem, m_bIsTextureFlipped)) {
		// do something
	}

	return true;
}

// properties
void GridBase::setActive(bool bActive)
{
	m_bActive = bActive;
}

void GridBase::setIsTextureFlipped(bool bFlipped)
{
	if (m_bIsTextureFlipped != bFlipped) {
		m_bIsTextureFlipped = bFlipped;
		calculateVertexPoints();
	}
}

void GridBase::set2DProjection()
{
/*
 	float winSizeWidth = (float)GetDeviceMgr().getNativeDisplayWidth() ;
 	float winSizeHeight = (float)GetDeviceMgr().getNativeDisplayHeight() ;
 	iwfixed right = IW_FIXED(winSizeWidth);
 	iwfixed bottom = IW_FIXED(winSizeWidth);
 	
	// TODO - fixed -> float

// 	iwfixed nearz = 0x8;
//  iwfixed farz = 0xffff;

	float nearz = 0.1f ;
	float farz = -1.0f ;

 	IwGxSetOrtho(0, right, bottom, 0, nearz, farz);
*/
}


void GridBase::set3DProjection()
{
/*
	// The frustrum is generally initialised at the start of the application.
	// The near and far plane clipping plane distances should be set as conservatively as possible;
	// that is, choose the largest near plane value which does not result in polygons disappearing close to the camera,
	// and choose the smallest far plane value which still gives an acceptable draw-distance.
	IwGxSetFarZNearZ(0x4000, 0x20); 
	// Sets the distance from the camera (the "eye point") to the viewing plane.
	IwGxSetPerspMul( IwGxGetScreenWidth() / 2.0f );

	// Horizontal field-of-view can be converted to perspective multiple using the formula: width / (2*atan(fov/2))
	//int32_t fov = IW_FIXED_DIV(IwGxGetScreenWidth()/2, IwGeomTan(60/2)) ;
	//IwGxSetPerspMul( fov ) ;

	// Every frame, before rendering the scene, the user should set the view transform (camera position and orientation): 
	CIwFMat view = CIwFMat::g_Identity;
	// coordinate system is right handed, i.e. positive Z is out of the screen
	CIwFVec3 from = CIwFVec3(0,0,160);
	view.SetTrans(from) ;
	CIwFVec3 to = CIwFVec3::g_Zero ;
	CIwFVec3 up = -CIwFVec3::g_AxisY ;
	view.LookAt(view.GetTrans(),to,up) ;
	IwGxSetViewMatrix(&view);

	// This function call will also use the current view matrix to calculate a combined "model-view" matrix, stored internally on the IwGxState object.
	// For this reason, you should always set the view matrix before setting the model matrix;
	CIwFMat trans = CIwFMat::g_Identity;
	IwGxSetModelMatrix(&trans) ;
*/
}

void GridBase::beforeDraw()
{
//	set2DProjection();
	m_pGrabber->beforeRender();
}

void GridBase::afterDraw(Node *pTarget)
{
	Draw3DBatch* pBatch = GetGfx().getBatch() ;
	if( pBatch && pBatch->isBatchingInProgress() ) {
		pBatch->end() ;
		pBatch->begin();
	}
	m_pGrabber->afterRender();
	set3DProjection();
	blit();
}

void GridBase::blit()
{
/*
	IwGxSetVertStreamModelSpace( m_pVertexBuffer->getVertexArray(), m_pVertexBuffer->getNumOfVertices() );
	IwGxSetColStream( m_pVertexBuffer->getColourArray(), m_pVertexBuffer->getNumOfVertices() );
	IwGxSetUVStream( m_pVertexBuffer->getUVArray() );
	IwGxSetNormStream(0);

	DrawItem* drawItem = DrawItem::create(m_pGrabber->getTexture()) ;
	CIwMaterial* pMaterial = Draw3DManager::getMaterial(drawItem) ;
	IwGxSetMaterial(pMaterial);
	IwGxDrawPrims( IW_GX_QUAD_LIST, m_pVertexBuffer->getIndexArray(), m_pVertexBuffer->getNumOfIndices() );
	JAM_RELEASE(drawItem) ;
*/
}

void GridBase::reuse()
{
	if (m_nReuseGrid > 0)
	{
		JAM_DELETE(m_pOriginalVertexBuffer) ;
		m_pOriginalVertexBuffer = new StridedVertexBuffer(*m_pVertexBuffer) ;
		--m_nReuseGrid;
	}
}


// implementation of Grid3D

Grid3D* Grid3D::gridWithSize(const GridSize& gridSize, DrawItem *pDrawItem, bool bFlipped)
{
	Grid3D *pRet= JAM_ALLOC(Grid3D);

	if (pRet) {
		if (pRet->initWithSize(gridSize, pDrawItem, bFlipped)) {
			pRet->autorelease();
		}
		else {
			JAM_FREE(pRet);
			pRet = NULL;
		}
	}

	return pRet;
}

Grid3D* Grid3D::gridWithSize(const GridSize& gridSize)
{
	Grid3D *pRet= JAM_ALLOC(Grid3D);

	if (pRet) {
		if (pRet->initWithSize(gridSize)) {
			pRet->autorelease();
		}
		else {
			JAM_FREE(pRet);
			pRet = NULL;
		}
	}

	return pRet;
}

void Grid3D::calculateVertexPoints()
{
	m_pVertexBuffer = new StridedVertexBuffer( 4*(m_sGridSize.x+1)*(m_sGridSize.y+1), 4*m_sGridSize.x*m_sGridSize.y ) ;

	float tuStep = (m_pDrawItem->getU2()-m_pDrawItem->getU1()) / m_sGridSize.x ;
	float tvStep = (m_pDrawItem->getV2()-m_pDrawItem->getV1()) / m_sGridSize.y ;


	int16_t v0, v1, v2, v3 ;
	float ii, jj ;
		
	float tv1 = m_pDrawItem->getV1() ;
	for( uint16_t j=0; j < m_sGridSize.y+1; j++ ) {
		jj = j * m_obStep.y ;
		float tu1 = m_pDrawItem->getU1() ;
		for( uint16_t i=0; i < m_sGridSize.x+1; i++ ) {
			ii = i * m_obStep.x ;
			m_pVertexBuffer->addVertex(-m_pDrawItem->getHalfWidth()+ii,m_pDrawItem->getHalfHeight()-jj, 0xffffffff, tu1,tv1) ;
			tu1 += tuStep ;
		}

		tv1 += tvStep ;
	}
		
	for( uint16_t j=0; j<m_sGridSize.y; j++ ) {
		for( uint16_t i=0; i<m_sGridSize.x; i++ ) {
			v0 = (j*(m_sGridSize.x+1))+i ;
			v1 = v0+1 ;
			v2 = ((j+1)*(m_sGridSize.x+1))+i+1;
			v3 = v2-1;
			m_pVertexBuffer->addQuadIndices( v0,v1,v2,v3 ) ;
		}
	}

	m_pOriginalVertexBuffer = new StridedVertexBuffer(*m_pVertexBuffer) ;
}

Vector3 Grid3D::vertex(const GridSize& pos)
{
	int index = (pos.y * (m_sGridSize.x+1) + pos.x) ;
	V3F_C4B_T2F *vertArray = m_pVertexBuffer->getVertexArray();
	return Vector3( vertArray[index].vertex.x, vertArray[index].vertex.y, vertArray[index].vertex.z );
}

Vector3 Grid3D::originalVertex(const GridSize& pos)
{
	int index = (pos.y * (m_sGridSize.x+1) + pos.x);
	V3F_C4B_T2F *vertArray = m_pOriginalVertexBuffer->getVertexArray();
	return Vector3( vertArray[index].vertex.x, vertArray[index].vertex.y, vertArray[index].vertex.z );
}

void Grid3D::setVertex(const GridSize& pos, const Vector3& vertex)
{
	int index = (pos.y * (m_sGridSize.x + 1) + pos.x) ;
	V3F_C4B_T2F *vertArray = m_pVertexBuffer->getVertexArray();
	vertArray[index].vertex.x = vertex.x ;
	vertArray[index].vertex.y = vertex.y ;
	vertArray[index].vertex.z = vertex.z ;
}


// implementation of TiledGrid3D

TiledGrid3D* TiledGrid3D::gridWithSize(const GridSize& gridSize, DrawItem *pDrawItem, bool bFlipped)
{
	TiledGrid3D *pRet= JAM_ALLOC(TiledGrid3D);

	if (pRet) {
		if (pRet->initWithSize(gridSize, pDrawItem, bFlipped)) {
			pRet->autorelease();
		}
		else {
			JAM_FREE(pRet);
			pRet = NULL;
		}
	}

	return pRet;
}

TiledGrid3D* TiledGrid3D::gridWithSize(const GridSize& gridSize)
{
	TiledGrid3D *pRet= JAM_ALLOC(TiledGrid3D);

	if (pRet) {
		if (pRet->initWithSize(gridSize)) {
			pRet->autorelease();
		}
		else {
			JAM_FREE(pRet);
			pRet = NULL;
		}
	}

	return pRet;
}

void TiledGrid3D::calculateVertexPoints()
{
	m_pVertexBuffer = new StridedVertexBuffer(4*m_sGridSize.x*m_sGridSize.y, 4*m_sGridSize.x*m_sGridSize.y ) ;

	float tuStep = (m_pDrawItem->getU2()-m_pDrawItem->getU1()) / m_sGridSize.x ;
	float tvStep = (m_pDrawItem->getV2()-m_pDrawItem->getV1()) / m_sGridSize.x ;

	float tv1 = m_pDrawItem->getV1() ;
	float tv2 = tv1 + tvStep ;

	float ii, jj ;

	for( int32_t j=0; j< m_sGridSize.y; j++ ) {

		jj = j * m_obStep.y ;
		float tu1 = m_pDrawItem->getU1() ;
		float tu2 = tu1 + tuStep ;

		for( int32_t i=0; i<m_sGridSize.x; i++ ) {
			ii = i * m_obStep.x ;
			int16_t v0 = m_pVertexBuffer->addVertex(-m_pDrawItem->getHalfWidth()+ii,			m_pDrawItem->getHalfHeight()-jj,			0xffffffff, tu1,tv1) ;
			int16_t v1 = m_pVertexBuffer->addVertex(-m_pDrawItem->getHalfWidth()+ii+m_obStep.x,	m_pDrawItem->getHalfHeight()-jj,			0xffffffff, tu2,tv1) ;
			int16_t v2 = m_pVertexBuffer->addVertex(-m_pDrawItem->getHalfWidth()+ii+m_obStep.x,	m_pDrawItem->getHalfHeight()-jj-m_obStep.y,	0xffffffff, tu2,tv2) ;
			int16_t v3 = m_pVertexBuffer->addVertex(-m_pDrawItem->getHalfWidth()+ii,			m_pDrawItem->getHalfHeight()-jj-m_obStep.y,	0xffffffff, tu1,tv2) ;
			m_pVertexBuffer->addQuadIndices(v0,v1,v2,v3) ;

			tu1 += tuStep ;
			tu2 = tu1 + tuStep ;
		}

		tv1 += tvStep ;
		tv2 = tv1 + tvStep ;
	}
	
	m_pOriginalVertexBuffer = new StridedVertexBuffer(*m_pVertexBuffer) ;
}

void TiledGrid3D::setTile(const GridSize& pos, const Quad3f& coords)
{
	int idx = ((m_sGridSize.x * pos.y) + pos.x) * 4 ;
	V3F_C4B_T2F *vertArray = m_pVertexBuffer->getVertexArray();
	vertArray[idx].vertex   = coords.tl ;
	vertArray[idx+1].vertex = coords.tr ;
	vertArray[idx+2].vertex = coords.br ;
	vertArray[idx+3].vertex = coords.bl ;
}

Quad3f TiledGrid3D::originalTile(const GridSize& pos)
{
	int idx = ((m_sGridSize.x * pos.y) + pos.x) * 4;
	V3F_C4B_T2F *vertArray = m_pOriginalVertexBuffer->getVertexArray();

	Quad3f ret;
	ret.tl = vertArray[idx].vertex ;
	ret.tr = vertArray[idx+1].vertex;
	ret.br = vertArray[idx+2].vertex ;
	ret.bl = vertArray[idx+3].vertex ;

	return ret;
}

Quad3f TiledGrid3D::tile(const GridSize& pos)
{
	int idx = ((m_sGridSize.x * pos.y) + pos.x) * 4;
	V3F_C4B_T2F *vertArray = m_pVertexBuffer->getVertexArray();

	Quad3f ret;
	ret.tl = vertArray[idx].vertex ;
	ret.tr = vertArray[idx+1].vertex ;
	ret.br = vertArray[idx+2].vertex ;
	ret.bl = vertArray[idx+3].vertex ;

	return ret;
}

}
