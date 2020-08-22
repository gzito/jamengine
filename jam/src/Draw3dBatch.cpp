/**********************************************************************************
* 
* Draw3dBatch.cpp
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

#include "jam/Draw3dBatch.h"
#include "jam/Draw3dManager.h"
#include "jam/Gfx.h"

using namespace std ;

namespace jam
{

Draw3DBatch::Draw3DBatch( uint16_t maxVertex /*= Draw3DStream::DefaultMaxVertexCount*/, uint16_t maxIndex /*= 0*/ ) :
	m_isBatchingInProgress(false),
	m_pCurrentMaterial(0),
	m_currentPrimType(-1),
	m_pVertexBuffer(0),
	m_currentSlotID(INT32_MAX)
{
	m_pVertexBuffer = new StridedVertexBuffer(maxVertex,maxIndex) ;
}


Draw3DBatch::~Draw3DBatch()
{
	JAM_DELETE(m_pVertexBuffer) ;
}


void Draw3DBatch::reset()
{
	m_pVertexBuffer->reset() ;
}

void Draw3DBatch::begin()
{
#ifdef JAM_TRACE_BATCH
	JAM_TRACE( ("Draw3DBatch::begin") ) ;
#endif
	JAM_ASSERT_MSG(m_isBatchingInProgress==false, ("Batch already in use"));
	m_isBatchingInProgress = true ;
	resetState() ;
}


StridedVertexBuffer* Draw3DBatch::check( jam::DrawItem* handle, uint16_t numOfVertex, uint16_t numOfIndex, int32_t slotID, GLenum primType /*= GL_TRIANGLES */ )
{
	StridedVertexBuffer* pVBuff = 0 ;
	if( m_isBatchingInProgress ) {
		Ref<Material> mat = GetGfx().getMaterial(handle) ;

		if( !m_pVertexBuffer->isSpaceAvailable(numOfVertex,numOfIndex) ) {
			JAM_ERROR( "Batch too small. Increase size" ) ;
		}
			
		if( isStateChanged(primType,mat,slotID) )
		{
			flush() ;
			resetState() ;
		}

		if( m_currentPrimType == -1 ) {
			m_currentPrimType = primType ;
		}

		if( !m_pCurrentMaterial ) {
			m_pCurrentMaterial = mat ;
		}

		if( m_currentSlotID == INT32_MAX ) {
			m_currentSlotID = slotID ;
		}

		pVBuff = m_pVertexBuffer ;
	}

	return pVBuff ;
}


void Draw3DBatch::end()
{
#ifdef JAM_TRACE_BATCH
	JAM_TRACE( ("Draw3DBatch::end") ) ;
#endif
	JAM_ASSERT_MSG(m_isBatchingInProgress==true, ("Batch already ended"));
	flush() ;
	m_isBatchingInProgress = false ;
}


void Draw3DBatch::flush()
{
	if( m_isBatchingInProgress && m_pVertexBuffer->getNumOfVertices() > 0 ) {
#ifdef JAM_TRACE_BATCH
		JAM_TRACE( ("Flushing current batch: processing %d vertices", m_pVertexBuffer->getNumOfVertices()) ) ;
#endif
		if( m_pVertexBuffer->isUploaded() ) { 
			m_pVertexBuffer->update() ;
		}
		GetGfx().drawIndexedPrimitive( m_pVertexBuffer, m_pCurrentMaterial.get(), m_currentSlotID, m_pVertexBuffer->getStartIndexCount()*sizeof(U16), (GLenum)m_currentPrimType ) ;
	}
}


void Draw3DBatch::resetState()
{
#ifdef JAM_TRACE_BATCH
	JAM_TRACE( ("Resetting batch...") ) ;
#endif
	m_pCurrentMaterial.reset() ;
	m_currentSlotID = INT32_MAX ;
	m_currentPrimType = -1 ;
	m_pVertexBuffer->setNewBlock() ;
}


bool Draw3DBatch::isStateChanged( GLenum primType, const Ref<Material>& pMateral, int32_t slotID ) const
{
// 	return (m_currentPrimType != -1 && primType != (IwGxPrimType)m_currentPrimType) ||
// 		(m_pCurrentMaterial && pMateral != m_pCurrentMaterial) || 
// 		(m_currentSlotID != INT32_MAX && m_currentSlotID != slotID ) ;
	if (m_currentPrimType != -1 && primType != (GLenum)m_currentPrimType)
	{
#ifdef JAM_TRACE_BATCH
		JAM_TRACE( ("Batching primitive type changed") ) ;
#endif
		return true ;
	}

	if (m_pCurrentMaterial && (pMateral != m_pCurrentMaterial) )
	{
#ifdef JAM_TRACE_BATCH
		JAM_TRACE( ("Batching material changed") ) ;
#endif
		return true ;
	}
	
	if (m_currentSlotID != INT32_MAX && m_currentSlotID != slotID )
	{
#ifdef JAM_TRACE_BATCH
		JAM_TRACE( ("Batch slot changed") ) ;
#endif
		return true ;
	}

	return false ;
}

}
