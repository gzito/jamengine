/**********************************************************************************
* 
* SpriteBatch.cpp
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

#include "jam/SpriteBatch.h"
#include "jam/Gfx.h"

#include <algorithm>

using namespace std ;


namespace jam
{

bool SpriteBatchItem::comparator( SpriteBatchItem* i, SpriteBatchItem* j ) {
    return i->SortKey < j->SortKey ;
}

void SpriteBatchItem::set(float x,float y,float dx,float dy,float w,float h,float sin,float cos,Color color,Vector2 texCoordTL,Vector2 texCoordBR,float depth)
{
	vertexTL.vertex.x = x+dx*cos-dy*sin;
    vertexTL.vertex.y = y+dx*sin+dy*cos;
    vertexTL.vertex.z = depth;
    vertexTL.color = color;
    vertexTL.texCoords.x = texCoordTL.x;
    vertexTL.texCoords.y = texCoordTL.y;

	vertexTR.vertex.x = x+(dx+w)*cos-dy*sin;
    vertexTR.vertex.y = y+(dx+w)*sin+dy*cos;
    vertexTR.vertex.z = depth;
    vertexTR.color = color;
    vertexTR.texCoords.x  = texCoordBR.x;
    vertexTR.texCoords.y = texCoordTL.y;

	vertexBL.vertex.x = x+dx*cos-(dy+h)*sin;
    vertexBL.vertex.y = y+dx*sin+(dy+h)*cos;
    vertexBL.vertex.z = depth;
    vertexBL.color = color;
    vertexBL.texCoords.x  = texCoordTL.x;
    vertexBL.texCoords.y = texCoordBR.y;

	vertexBR.vertex.x = x+(dx+w)*cos-(dy+h)*sin;
    vertexBR.vertex.y = y+(dx+w)*sin+(dy+h)*cos;
    vertexBR.vertex.z = depth;
    vertexBR.color = color;
    vertexBR.texCoords.x  = texCoordBR.x;
    vertexBR.texCoords.y = texCoordBR.y;
}

void SpriteBatchItem::set(float x,float y,float w,float h,Color color,Vector2 texCoordTL,Vector2 texCoordBR,float depth)
{
	vertexTL.vertex.x = x ;
    vertexTL.vertex.y = y;
    vertexTL.vertex.z = depth;
    vertexTL.color = color;
    vertexTL.texCoords.x = texCoordTL.x;
    vertexTL.texCoords.y = texCoordTL.y;

	vertexTR.vertex.x = x + w;
    vertexTR.vertex.y = y;
    vertexTR.vertex.z = depth;
    vertexTR.color = color;
    vertexTR.texCoords.x  = texCoordBR.x;
    vertexTR.texCoords.y = texCoordTL.y;

	vertexBL.vertex.x = x;
    vertexBL.vertex.y = y + h;
    vertexBL.vertex.z = depth;
    vertexBL.color = color;
    vertexBL.texCoords.x  = texCoordTL.x;
    vertexBL.texCoords.y = texCoordBR.y;

	vertexBR.vertex.x = x + w;
    vertexBR.vertex.y = y + h;
    vertexBR.vertex.z = depth;
    vertexBR.color = color;
    vertexBR.texCoords.x  = texCoordBR.x;
    vertexBR.texCoords.y = texCoordBR.y;
}

SpriteBatcher::SpriteBatcher(Material* pMaterial, int capacity /* = 0 */) :
    m_vbo(GL_ARRAY_BUFFER), m_ebo(GL_ELEMENT_ARRAY_BUFFER), m_vao(), m_pMaterial(pMaterial)
{
    if (capacity <= 0)
        capacity = InitialBatchSize;
    else
        capacity = (capacity + 63) & (~63); // ensure chunks of 64.

	_batchItemList.resize(capacity) ;
    _batchItemCount = 0;

    for (int i = 0; i < capacity; i++)
        _batchItemList[i] = new SpriteBatchItem();

    EnsureArrayCapacity(capacity);
}

SpriteBatchItem* SpriteBatcher::CreateBatchItem()
{
    if (_batchItemCount >= _batchItemList.size() )
    {
        size_t oldSize = _batchItemList.size();
        size_t newSize = oldSize + oldSize/2; // grow by x1.5
        newSize = (newSize + 63) & (~63); // grow in chunks of 64.
        _batchItemList.resize( newSize );
        for(int i=oldSize; i<newSize; i++)
            _batchItemList[i]=new SpriteBatchItem();

        EnsureArrayCapacity(Min(newSize, MaxBatchSize));
    }
    auto item = _batchItemList[_batchItemCount++];
    return item;
}

void SpriteBatcher::uploadVertexBuffer()
{
    if( m_uploaded ) {
        return ;
    }

	Shader* p = GetShaderMgr().getCurrent() ;
    m_vao.create() ;
    m_vbo.create() ;
    m_ebo.create() ;

    m_vao.bind() ;

    m_vbo.bind() ;
    m_vbo.bufferData( _vertexArray.byteSize(), _vertexArray.data(), GL_DYNAMIC_DRAW ) ;

	glVertexAttribPointer( p->attrib(JAM_PROGRAM_ATTRIB_POSITION), 3, GL_FLOAT,	GL_FALSE, sizeof(V3F_C4B_T2F), (void*)0 ) ;
	glEnableVertexAttribArray( p->attrib(JAM_PROGRAM_ATTRIB_POSITION) ) ;

	glVertexAttribPointer( p->attrib(JAM_PROGRAM_ATTRIB_COLOR), 4, GL_UNSIGNED_BYTE, GL_TRUE,  sizeof(V3F_C4B_T2F), (void*)sizeof(Vertex3f) ) ;
	glEnableVertexAttribArray( p->attrib(JAM_PROGRAM_ATTRIB_COLOR) ) ;

	glVertexAttribPointer( p->attrib(JAM_PROGRAM_ATTRIB_TEXCOORDS), 2, GL_FLOAT,	GL_FALSE, sizeof(V3F_C4B_T2F), (void*)(sizeof(Vertex3f) + sizeof(Color)) ) ;
	glEnableVertexAttribArray( p->attrib(JAM_PROGRAM_ATTRIB_TEXCOORDS) ) ;

    m_ebo.bind() ;
    m_ebo.bufferData( _index.byteSize(), _index.data(), GL_STATIC_DRAW ) ;

    m_vao.unbind() ;

    m_vbo.unbind() ;
    m_ebo.unbind() ;

    m_uploaded = true ;
}

void SpriteBatcher::updateVertexBuffer()
{
    JAM_ASSERT(m_uploaded) ;

    m_vbo.bind() ;
	glBufferSubData( GL_ARRAY_BUFFER, (GLintptr)(0), _vertexArray.byteSize(), _vertexArray.data() ) ;
    m_vbo.unbind() ;
}

void SpriteBatcher::DrawBatch(SpriteSortMode sortMode)
{
    // nothing to do
    if (_batchItemCount == 0)
	    return;
			
    // sort the batch items
    switch ( sortMode )
    {
    case SpriteSortMode::Texture :                
    case SpriteSortMode::FrontToBack :
    case SpriteSortMode::BackToFront :
        std::sort( _batchItemList.begin(), _batchItemList.end(), SpriteBatchItem::comparator ) ;
	    break;
    }

    // Determine how many iterations through the drawing code we need to make
    int batchIndex = 0;
    int batchCount = _batchItemCount;
            
    // Iterate through the batches, doing short.MaxValue sets of vertices only.
    while(batchCount > 0)
    {
        // setup the vertexArray array
        int startIndex = 0;
        int index = 0;
        Texture2D* tex = nullptr;

        int numBatchesToProcess = batchCount;
        if (numBatchesToProcess > MaxBatchSize)
        {
            numBatchesToProcess = MaxBatchSize;
        }
        // Avoid the array checking overhead by using pointer indexing!
        V3F_C4B_T2F* vertexArrayFixedPtr = _vertexArray.data() ;
        auto vertexArrayPtr = vertexArrayFixedPtr;

        // Draw the batches
        for (int i = 0; i < numBatchesToProcess; i++, batchIndex++, index += 4, vertexArrayPtr += 4)
        {
            SpriteBatchItem* item = _batchItemList[batchIndex];
            // if the texture changed, we need to flush and bind the new texture
            bool shouldFlush = item->Texture != tex;
            if (shouldFlush)
            {
                FlushVertexArray(startIndex, index, tex);

                tex = item->Texture;
                startIndex = index = 0;
                vertexArrayPtr = vertexArrayFixedPtr;
                m_pMaterial->setDiffuseTexture(tex);
            }

            // store the SpriteBatchItem data in our vertexArray
            *(vertexArrayPtr+0) = item->vertexTL;
            *(vertexArrayPtr+1) = item->vertexTR;
            *(vertexArrayPtr+2) = item->vertexBL;
            *(vertexArrayPtr+3) = item->vertexBR;

            // Release the texture.
            item->Texture = nullptr;
        }
        // flush the remaining vertexArray data
        FlushVertexArray(startIndex, index, tex);
        // Update our batch count to continue the process of culling down
        // large batches
        batchCount -= numBatchesToProcess;
    }
    // return items to the pool.  
    _batchItemCount = 0;
}

void SpriteBatcher::EnsureArrayCapacity(int numBatchItems)
{
    int neededCapacity = 6 * numBatchItems;
    if (_index.length()>0 && neededCapacity <= _index.length())
    {
        // Short circuit out of here because we have enough capacity.
        return;
    }
    HeapArray<U16> newIndex(6 * numBatchItems) ;
    int start = 0;
    if (_index.length() > 0)
    {
        _index.copyTo(newIndex, 0);
        start = _index.length() / 6;
    }

    U16* indexPtr = newIndex.data() + (start * 6);
    for (auto i = start; i < numBatchItems; i++, indexPtr += 6)
    {
        /*
            *  TL    TR
            *   0----1 0,1,2,3 = index offsets for vertex indices
            *   |   /| TL,TR,BL,BR are vertex references in SpriteBatchItem.
            *   |  / |
            *   | /  |
            *   |/   |
            *   2----3
            *  BL    BR
            */
        // Triangle 1
        *(indexPtr + 0) = (short)(i * 4);
        *(indexPtr + 1) = (short)(i * 4 + 1);
        *(indexPtr + 2) = (short)(i * 4 + 2);
        // Triangle 2
        *(indexPtr + 3) = (short)(i * 4 + 1);
        *(indexPtr + 4) = (short)(i * 4 + 3);
        *(indexPtr + 5) = (short)(i * 4 + 2);
    }
    _index = newIndex;

    _vertexArray.create(4 * numBatchItems);
}

void SpriteBatcher::FlushVertexArray(int start,int end,Texture2D* texture)
{
    if (start == end)
        return;

    if( !m_uploaded ) {
        uploadVertexBuffer() ;
    }
    else {
        updateVertexBuffer() ;
    }

    int indexCount = (end - start) * 1.5f ;
    GetGfx().drawIndexedPrimitive( &m_vao, indexCount, m_pMaterial, start*sizeof(U16) ) ;
}

SpriteBatch::SpriteBatch(int capacity) : _sortMode(SpriteSortMode::Deferred)
{
    _pMaterial = new (GC) Material() ;
    _batcher = new (GC) SpriteBatcher(_pMaterial,capacity) ;
    _beginCalled = false ;
}

void SpriteBatch::Begin(SpriteSortMode sortMode,BlendMode blendMode)
{
    _sortMode = sortMode;
    _pMaterial->setBlendEnabled(true) ;
    _pMaterial->setBlendMode(blendMode) ;

    // Setup things now so a user can change them.
    if (sortMode == SpriteSortMode::Immediate)
    {
        Setup();
    }

    _beginCalled = true;

}

void SpriteBatch::Draw(Texture2D* texture,Vector2 position,Rect* sourceRectangle,Color color,float rotation,Vector2 origin,Vector2 scale,SpriteEffects effects,float layerDepth)
{
    auto item = _batcher->CreateBatchItem();
    item->Texture = texture;

    // set SortKey based on SpriteSortMode.
    switch ( _sortMode )
    {
    // Comparison of Texture objects.
    case SpriteSortMode::Texture:
        item->SortKey = texture->getSortingKey();
        break;
    // Comparison of Depth
    case SpriteSortMode::FrontToBack:
        item->SortKey = layerDepth;
        break;
    // Comparison of Depth in reverse
    case SpriteSortMode::BackToFront:
        item->SortKey = -layerDepth;
        break;
    }
                        
    origin = origin * scale;
            
    float w, h;
    if (sourceRectangle!=nullptr)
    {
        Rect srcRect = *sourceRectangle ;
        w = srcRect.getWidth() * scale.x;
        h = srcRect.getHeight() * scale.y;
        float texelWidth = 1.0f / texture->getWidth() ;
        float texelHeight = 1.0f / texture->getHeight() ;
        _texCoordTL.x = srcRect.left * texelWidth;
        _texCoordTL.y = srcRect.top * texelHeight ;
        _texCoordBR.x = (srcRect.left + srcRect.getWidth()) * texelWidth;
        _texCoordBR.y = (srcRect.top + srcRect.getHeight()) * texelHeight;
    }
    else
    {
        w = texture->getWidth() * scale.x;
        h = texture->getHeight() * scale.y;
        _texCoordTL = Vector2(0,0);
        _texCoordBR = Vector2(1,1);
    }
            
    if ((effects & SpriteEffects::FlipVertically) != 0)
    {
        auto temp = _texCoordBR.y;
	    _texCoordBR.y = _texCoordTL.y;
	    _texCoordTL.y = temp;
    }
    if ((effects & SpriteEffects::FlipHorizontally) != 0)
    {
        auto temp = _texCoordBR.x;
	    _texCoordBR.x = _texCoordTL.x;
	    _texCoordTL.x = temp;
    }
            
    if (rotation == 0.0f)
    {
        item->set(position.x - origin.x,
                position.y - origin.y,
                w,
                h,
                color,
                _texCoordTL,
                _texCoordBR,
                layerDepth);
    }
    else
    {
        item->set(position.x,
                position.y,
                -origin.x,
                -origin.y,
                w,
                h,
                (float)sinf(rotation),
                (float)cosf(rotation),
                color,
                _texCoordTL,
                _texCoordBR,
                layerDepth);
    }
            
    FlushIfNeeded();
}



void SpriteBatch::End()
{
    if (!_beginCalled)
        JAM_ERROR("Begin must be called before calling End.");

	_beginCalled = false;

	if (_sortMode != SpriteSortMode::Immediate)
		Setup();
            
    _batcher->DrawBatch(_sortMode);
}

void SpriteBatch::Setup()
{
}

void SpriteBatch::CheckValid(Texture2D* texture)
{
    if (texture == nullptr)
        JAM_ERROR("texture");
    if (!_beginCalled)
        JAM_ERROR("Draw was called, but Begin has not yet been called. Begin must be called successfully before you can call Draw.");
}

void SpriteBatch::FlushIfNeeded()
{
    if (_sortMode == SpriteSortMode::Immediate)
    {
        _batcher->DrawBatch( _sortMode );
    }
}

}
