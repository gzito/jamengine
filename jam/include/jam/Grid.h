/**********************************************************************************
* 
* Grid.h
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

#ifndef __JAM_GRID_H__
#define __JAM_GRID_H__

#include <jam/RefCountedObject.h>
#include <jam/Node.h>
#include <jam/Texture2D.h>

namespace jam
{
class Grabber;
class DrawItem ;
class StridedVertexBuffer ;

/**
	Base class for the other Grid based classes
*/
class GridBase : public jam::RefCountedObject
{
public:
	virtual ~GridBase();

	/** wheter or not the grid is active */
	JAM_INLINE bool isActive() { return m_bActive; }
	void setActive(bool bActive);

	/** number of times that the grid will be reused */
	JAM_INLINE int getReuseGrid() { return m_nReuseGrid; }
	JAM_INLINE void setReuseGrid(int nReuseGrid) { m_nReuseGrid = nReuseGrid; }

	/** size of the grid */
	JAM_INLINE GridSize getGridSize() { return m_sGridSize; }
	JAM_INLINE void setGridSize(GridSize gridSize) { m_sGridSize = gridSize; }

	/** pixels between the grids */
	JAM_INLINE Vertex2f getStep() { return m_obStep; }
	JAM_INLINE void setStep(Vertex2f step) { m_obStep = step; }

	/** is texture flipped */
	JAM_INLINE bool isTextureFlipped() { return m_bIsTextureFlipped; }
	void setIsTextureFlipped(bool bFlipped);

	bool initWithSize(const GridSize& gridSize, DrawItem *pDrawItem, bool bFlipped);
	bool initWithSize(const GridSize& gridSize);

	void beforeDraw();
	void afterDraw(Node *pTarget);
	virtual void blit();
	virtual void reuse();
	virtual void calculateVertexPoints() = 0;

public:
	//static GridBase* gridWithSize(const GridSize& gridSize, DrawItem *pDrawItem, bool flipped);
	//static GridBase* gridWithSize(const GridSize& gridSize);
	void set2DProjection();
	void set3DProjection();

protected:
	GridBase() ;
		
protected:
	bool m_bActive;
	int  m_nReuseGrid;
	GridSize m_sGridSize;
	DrawItem* m_pDrawItem;
	Vertex2f m_obStep;
	Grabber *m_pGrabber;
	bool m_bIsTextureFlipped;

	StridedVertexBuffer*	m_pVertexBuffer ;
	StridedVertexBuffer*	m_pOriginalVertexBuffer;
};

/**
	Grid3D is a 3D grid implementation. Each vertex has 3 dimensions: x,y,z
	*/
class Grid3D : public GridBase
{
public:
	/** returns the vertex at a given position */
	Vector3 vertex(const GridSize& pos);
	/** returns the original (non-transformed) vertex at a given position */
	Vector3 originalVertex(const GridSize& pos);
	/** sets a new vertex at a given position */
	void setVertex(const GridSize& pos, const Vector3& vertex);

	virtual void calculateVertexPoints();

public:
	static Grid3D* gridWithSize(const GridSize& gridSize, DrawItem* pDrawItem, bool bFlipped);
	static Grid3D* gridWithSize(const GridSize& gridSize);
};


/**
	TiledGrid3D is a 3D grid implementation. It differs from Grid3D in that
	the tiles can be separated from the grid.
*/
class TiledGrid3D : public GridBase
{
public:
	/** returns the tile at the given position */
	Quad3f tile(const GridSize& pos);
	/** returns the original tile (untransformed) at the given position */
	Quad3f originalTile(const GridSize& pos);
	/** sets a new tile */
	void setTile(const GridSize& pos, const Quad3f& coords);

	virtual void calculateVertexPoints();

public:
	static TiledGrid3D* gridWithSize(const GridSize& gridSize, DrawItem* pDrawItem, bool bFlipped);
	static TiledGrid3D* gridWithSize(const GridSize& gridSize);
};

}

#endif
