/**********************************************************************************
* 
* Draw3dBatch.h
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

#ifndef __JAM_DRAW3DBATCH_H__
#define __JAM_DRAW3DBATCH_H__


#include <jam/jam.h>

#include <jam/StridedVertexBuffer.h>
#include <jam/DrawItem.h>

namespace jam
{

class Draw3DManager ;

class Draw3DBatch
{
public:
							Draw3DBatch( uint16_t maxVertex = StridedVertexBuffer::DefaultMaxVertexCount, uint16_t maxIndex = 0 ) ;
							~Draw3DBatch() ;

	void					reset() ;
	void					begin() ;
	StridedVertexBuffer*	check( jam::DrawItem* handle, uint16_t numOfVertex, uint16_t numOfIndex, int32_t slotID, GLenum primType = GL_TRIANGLES ) ;
	void					flush() ;
	void					end() ;

	bool					isBatchingInProgress() const { return m_isBatchingInProgress; }
	StridedVertexBuffer*	getVertexBuffer() { return m_pVertexBuffer ; }

private:
	void					resetState() ;
	bool					isStateChanged(GLenum primType, Material* pMateral, int32_t slotID) const ;

private:
	bool					m_isBatchingInProgress ;
	Material*				m_pCurrentMaterial ;
	int32_t					m_currentPrimType ;
	int32_t					m_currentSlotID ;
	StridedVertexBuffer*	m_pVertexBuffer ;

	// to prevent the use
							Draw3DBatch( const Draw3DBatch& ) = delete ;
	Draw3DBatch&			operator=(const Draw3DBatch&) = delete ;
};

}

#endif // __JAM_DRAW3DBATCH_H__
