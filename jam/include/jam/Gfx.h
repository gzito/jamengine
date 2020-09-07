/**********************************************************************************
* 
* Gfx.h
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

#ifndef __JAM_GFX_H__
#define __JAM_GFX_H__


#include <jam/jam.h>
#include <jam/Singleton.h>
#include <jam/Texture2D.h>

#include <map>


namespace jam
{
class IVertexBuffer ;
class StridedVertexBuffer ;
class Draw3DBatch ;
class DrawItem ;
class Material ;
class VertexArrayObject ;

class JAM_API Gfx : public Singleton<Gfx>
{
private:
	friend class Singleton<Gfx>;

public:
	void					setViewport( int x, int y, int width, int height) ;

	void					setDepthTest( bool fEnable ) ;
	void					setDepthMask( bool fEnable ) ;
	void					setDepthFunc( GLenum funct ) ;

	void					setFrontFace( GLenum mode ) ;
	void					setCullFace( GLenum mode ) ;
	void					setFaceCulling( bool value ) ;

	void					setClearColor( const Color& clearColor ) ;
	void					clear( GLbitfield mask ) ;

	void					draw( IVertexBuffer* pVertexBuffer, DrawItem* item, GLenum pType = GL_TRIANGLES ) ;
	void					draw( IVertexBuffer* pVBuff, Material* pMaterial, GLenum pType = GL_TRIANGLES ) ;

	void					setBatch( Draw3DBatch* batch ) ;
	Draw3DBatch*			getBatch() ;
	bool					isBatchingInProgress() const ;

	StridedVertexBuffer&	getVertexBuffer( DrawItem* handle, uint16_t vCount, uint16_t iCount, int32_t slotID = 0, GLenum pType = GL_TRIANGLES ) ;
	void					appendOrDraw();
	void					drawPrimitive( VertexArrayObject* pVao, size_t numOfVertices, Material* pMaterial, GLenum pType = GL_TRIANGLES ) ;
	void					drawIndexedPrimitive( VertexArrayObject* pVao, size_t numOfElements, Material* pMaterial, U16 offset = 0 , GLenum pType = GL_TRIANGLES ) ;
	void					drawIndexedPrimitive( IVertexBuffer* pVBuff, Material* pMaterial, int32_t slotID, U16 offset = 0 , GLenum pType = GL_TRIANGLES ) ;
	void					setRenderLevel( int level ) ;

	Material*				getMaterial( DrawItem* item ) ;

private:
							Gfx() ;
	virtual					~Gfx() ;

	Draw3DBatch*			m_batch ;

	int						m_renderLevel ;

	// batching stuff
	StridedVertexBuffer*	m_pVBuff;
	DrawItem*				m_handle ;
	GLenum					m_pType ;
	int32_t					m_lastSlotID ;
};

JAM_INLINE Gfx& GetGfx() { return Gfx::getSingleton(); }

JAM_INLINE Draw3DBatch* Gfx::getBatch() { return m_batch ; }
}

#endif // __JAM_GFX_H__
