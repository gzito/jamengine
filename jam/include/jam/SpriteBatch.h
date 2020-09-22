/**********************************************************************************
* 
* SpriteBatch.h
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

#ifndef __JAM_SPRITEBATCH_H__
#define __JAM_SPRITEBATCH_H__


#include <jam/jam.h>
#include <jam/VertexBufferObject.h>
#include <jam/VertexArrayObject.h>
#include <jam/Material.h>
#include <jam/core/array.hpp>
#include <jam/Draw2d.h>
#include <jam/Ref.hpp>

#include <climits>

namespace jam
{

enum SpriteEffects
{
    /// <summary>
    /// No options specified.
    /// </summary>
	None = 0,
    /// <summary>
    /// Render the sprite reversed along the X axis.
    /// </summary>
    FlipHorizontally = 1,
    /// <summary>
    /// Render the sprite reversed along the Y axis.
    /// </summary>
    FlipVertically = 2
};

enum class SpriteSortMode
{
    /// <summary>
    /// All sprites are drawing when <see cref="SpriteBatch.End"/> invokes, in order of draw call sequence. Depth is ignored.
    /// </summary>
    Deferred,
    /// <summary>
    /// Each sprite is drawing at individual draw call, instead of <see cref="SpriteBatch.End"/>. Depth is ignored.
    /// </summary>
    Immediate,
    /// <summary>
    /// Same as <see cref="SpriteSortMode.Deferred"/>, except sprites are sorted by texture prior to drawing. Depth is ignored.
    /// </summary>
    Texture,
    /// <summary>
    /// Same as <see cref="SpriteSortMode.Deferred"/>, except sprites are sorted by depth in back-to-front order prior to drawing.
    /// An unstable sort is used, which means sprites with equal depth may not have their order preserved.
    /// </summary>
    BackToFront,
    /// <summary>
    /// Same as <see cref="SpriteSortMode.Deferred"/>, except sprites are sorted by depth in front-to-back order prior to drawing.
    /// An unstable sort is used, which means sprites with equal depth may not have their order preserved.
    /// </summary>
    FrontToBack
};

/**
	Represents a sprite vertex buffer in which each vertex attribute is tight packed in its dedicated array
*/
class JAM_API SpriteBatch : public RefCountedObject
{
private:

    // internal class SpriteBatchItem
    class SpriteBatchItem : public RefCountedObject
    {
    public:
	    Ref<Texture2D>			Texture ;
	    float					SortKey ;

	    V3F_C4B_T2F				vertexTL ;
	    V3F_C4B_T2F				vertexTR ;
	    V3F_C4B_T2F				vertexBL ;
	    V3F_C4B_T2F				vertexBR ;

    public:
							    SpriteBatchItem() = default ;
	    void					set( float x, float y, float dx, float dy, float w, float h, float sin, float cos, Color color, Vector2 texCoordTL, Vector2 texCoordBR, float depth ) ;
	    void					set( float x, float y, float w, float h, Color color, Vector2 texCoordTL, Vector2 texCoordBR, float depth ) ;
	    static bool 			comparator( SpriteBatchItem* i, SpriteBatchItem* j ) ;

    protected:
        virtual                 ~SpriteBatchItem() = default ;

    private:
                                SpriteBatchItem( const SpriteBatchItem& ) = delete ;
        SpriteBatchItem&        operator=( const SpriteBatchItem& ) = delete ;
    };

    //     // internal class SpriteBatcher
    class SpriteBatcher : public RefCountedObject
    {
    public:
                                SpriteBatcher(Material* pMaterial, int capacity = 0 ) ;
        SpriteBatchItem*        CreateBatchItem() ;
        void                    uploadVertexBuffer() ;
        void                    updateVertexBuffer() ;
        void                    DrawBatch(SpriteSortMode sortMode) ;
    
    protected:
                                ~SpriteBatcher() = default ;
    private:
        void                    EnsureArrayCapacity(int numBatchItems) ;
	    void                    FlushVertexArray(int start, int end, Texture2D* texture) ;

                                SpriteBatcher( const SpriteBatcher& ) = delete ;
        SpriteBatcher&          operator=( const SpriteBatcher& ) = delete ;

    private:
	    const int               InitialBatchSize = 256;
	    const int               MaxBatchSize = SHRT_MAX / 6;	// 6 = 4 vertices unique and 2 shared, per quad
	    const int               InitialVertexArraySize = 256;

        using SpriteBatchItemArray = std::vector<Ref<SpriteBatchItem>> ;

	    int                     _batchItemCount = 0 ;
        SpriteBatchItemArray    _batchItemList ;
	    HeapArray<U16>          _index ;
	    HeapArray<V3F_C4B_T2F>  _vertexArray ;
	    VertexBufferObject      m_vbo ;
	    VertexBufferObject      m_ebo ;
        VertexArrayObject   	m_vao ;
        Ref<Material>           m_pMaterial ;
	    bool                    m_uploaded ;
    };


public:
                            SpriteBatch(int capacity) ;
    void                    Begin( SpriteSortMode sortMode = SpriteSortMode::Deferred, BlendMode blendMode = BlendMode::AlphaBlend ) ;

    /// <summary>
    /// Submit a sprite for drawing in the current batch.
    /// </summary>
    /// <param name="texture">A texture.</param>
    /// <param name="position">The drawing location on screen.</param>
    /// <param name="sourceRectangle">An optional region on the texture which will be rendered. If null - draws full texture.</param>
    /// <param name="color">A color mask.</param>
    /// <param name="rotation">A rotation of this sprite.</param>
    /// <param name="origin">Center of the rotation. 0,0 by default.</param>
    /// <param name="scale">A scaling of this sprite.</param>
    /// <param name="effects">Modificators for drawing. Can be combined.</param>
    /// <param name="layerDepth">A depth of the layer of this sprite.</param>
    void                    Draw(   Texture2D* texture,
				                    Vector2 position,
				                    Rect* sourceRectangle,
				                    Color color,
				                    float rotation,
				                    Vector2 origin,
				                    Vector2 scale,
				                    SpriteEffects effects,
                                    float layerDepth) ;

    void                    End() ;

protected:
    virtual                 ~SpriteBatch() = default ;

private:
                            SpriteBatch( const SpriteBatch& ) = delete ;
    SpriteBatch&            operator=( const SpriteBatch& ) = delete ;

    void                    Setup() ;
    void                    CheckValid( Texture2D* texture ) ;
    void                    FlushIfNeeded() ;

private:
    Ref<SpriteBatcher>      _batcher;
    bool                    _beginCalled;
    Ref<Material>           _pMaterial ;
	SpriteSortMode          _sortMode;
	Rect                    _tempRect ;
	Vector2                 _texCoordTL ;
	Vector2                 _texCoordBR ;
};			


}

#endif // __JAM_SPRITEBATCH_H__ 
