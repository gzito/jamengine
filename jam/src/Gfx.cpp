/**********************************************************************************
* 
* Gfx.cpp
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

#include "jam/Gfx.h"
#include "jam/IVertexBuffer.hpp"
#include "jam/StridedVertexBuffer.h"
#include "jam/VertexArrayObject.h"
#include "jam/Draw3DBatch.h"

#include <glm/gtc/type_ptr.hpp>

using namespace std ;

namespace jam
{

Gfx::Gfx() : m_batch(0), m_renderLevel(0), m_pVBuff(0), m_handle(0), m_pType(GL_TRIANGLES)
{
}

Gfx::~Gfx()
{
}

void Gfx::setViewport(int x, int y,int width,int height)
{
	glViewport(x,y,width,height) ;
}

/** 
	When depth testing is enabled OpenGL tests the depth value of a fragment against
	the content of the depth buffer. OpenGL performs a depth test and if this test passes,
	the depth buffer is updated with the new depth value.
	If the depth test fails, the fragment is discarded.

	\param fEnable
		true - enable z-buffer
		false - disable z-buffer
*/
void Gfx::setDepthTest(bool fEnable)
{
	if( fEnable ) {
		glEnable(GL_DEPTH_TEST) ;
	}
	else {
		glDisable(GL_DEPTH_TEST) ;
	}
}

/**
	Enable or disable writing to the depth buffer

	\param fEnable
		true - normal z-buffer
		false - disable writing to the depth buffer (read-only depth buffer)
*/
void Gfx::setDepthMask(bool fEnable)
{
	glDepthMask( fEnable ? GL_TRUE : GL_FALSE ) ;
}


/**
	Sets the function to use for depth testing 

	funct-enum	Description
	GL_ALWAYS	The depth test always passes.
	GL_NEVER	The depth test never passes.
	GL_LESS		Passes if the fragment's depth value is less than the stored depth value.
	GL_EQUAL	Passes if the fragment's depth value is equal to the stored depth value.
	GL_LEQUAL	Passes if the fragment's depth value is less than or equal to the stored depth value.
	GL_GREATER	Passes if the fragment's depth value is greater than the stored depth value.
	GL_NOTEQUAL	Passes if the fragment's depth value is not equal to the stored depth value.
	GL_GEQUAL	Passes if the fragment's depth value is greater than or equal to the stored depth value.

	By default the depth function GL_LESS is used that discards all the fragments that have a depth value
	higher than or equal to the current depth buffer's value.
*/
void Gfx::setDepthFunc(GLenum funct)
{
	glDepthFunc(funct) ;
}

void Gfx::setFrontFace(GLenum mode)
{
	// GL_CW and GL_CCW are accepted. The initial value is GL_CCW
	glFrontFace(mode) ;
}

void Gfx::setCullFace(GLenum mode)
{
	// mode can be set to GL_FRONT, GL_BACK, or GL_FRONT_AND_BACK
	glCullFace(mode) ;
}

void Gfx::setFaceCulling(bool value)
{
	if( value == true ) {
		glEnable(GL_CULL_FACE) ;
	}
	else {
		glDisable(GL_CULL_FACE) ;
	}
}

void Gfx::setClearColor(const Color & clearColor)
{
	glm::vec4 c  = clearColor.getFloatingComponents() ;
	glClearColor( c.r, c.g, c.b, c.a ) ;
}

// Bitwise OR of masks that indicate the buffers to be cleared.
// The four masks are GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT, GL_ACCUM_BUFFER_BIT, and GL_STENCIL_BUFFER_BIT.
void Gfx::clear( GLbitfield mask )
{
	glClear( mask ) ;
}

void Gfx::drawPrimitive( VertexArrayObject * pVao, size_t numOfVertices, Material * pMaterial, GLenum pType)
{
	pVao->bind() ;

	pMaterial->bind() ;
	glDrawArrays( pType, 0, (GLsizei)numOfVertices );
	pMaterial->unbind() ;

	pVao->unbind() ;
}

void Gfx::drawIndexedPrimitive( VertexArrayObject* pVao, size_t numOfElements, Material* pMaterial, U16 offset /*= 0*/, GLenum pType /*= GL_TRIANGLES */ )
{
	pVao->bind() ;

	pMaterial->bind() ;
	glDrawElements( pType, (GLsizei)numOfElements, GL_UNSIGNED_SHORT, (const void*)offset );
	pMaterial->unbind() ;

	pVao->unbind() ;
}

void Gfx::drawIndexedPrimitive( IVertexBuffer* pVBuff, Material* pMaterial, int32_t slotID, U16 offset /*= 0*/, GLenum pType /*= GL_TRIANGLES */ )
{
	pVBuff->bindVao() ;
	pMaterial->bind() ;
	glDrawElements( pType, pVBuff->getNumOfIndices(), GL_UNSIGNED_SHORT, (const void*)offset );
	pMaterial->unbind() ;
	pVBuff->unbindVao() ;
}

void Gfx::setRenderLevel(int level)
{
	// TODO
}

void Gfx::draw( IVertexBuffer* pVBuff, DrawItem* item, GLenum pType /*= GL_TRIANGLES*/ )
{
	Ref<Material> pMat = getMaterial( item ) ;
	draw(pVBuff, pMat.get(), pType) ;
}

void Gfx::draw( IVertexBuffer* pVBuff, Material* pMaterial, GLenum pType /*= GL_TRIANGLES*/ )
{
	if( !isBatchingInProgress() && pVBuff->getNumOfVertices() > 0 ) {
		drawIndexedPrimitive(pVBuff, pMaterial, 0, 0U, pType) ;
	}
}

void Gfx::setBatch( Draw3DBatch* batch )
{
	m_batch = batch ;
}

bool Gfx::isBatchingInProgress() const
{
	return m_batch && m_batch->isBatchingInProgress() ;
}


StridedVertexBuffer& Gfx::getVertexBuffer( DrawItem* handle, uint16_t vCount, uint16_t iCount, int32_t slotID /*= 0*/ , GLenum pType /*= GL_TRIANGLES */ )
{
	m_handle = handle ;
	m_pType = pType ;
	m_lastSlotID = slotID ;

	if( isBatchingInProgress() ) {
		m_pVBuff = getBatch()->check(handle,vCount,iCount,slotID,pType);
	}
	else {
		m_pVBuff = new StridedVertexBuffer( vCount, iCount );
	}

	return *m_pVBuff ;
}

void Gfx::appendOrDraw()
{
	if( !isBatchingInProgress() ) {
		draw( m_pVBuff, m_handle, m_pType ) ;
		JAM_DELETE(m_pVBuff) ;
	}
}

Ref<Material> Gfx::getMaterial( DrawItem* item )
{
	Ref<Material> pMat = item->getMaterial() ;
	JAM_ASSERT_MSG( pMat, "Null material in DrawItem" ) ;
	return pMat ;
}

}
