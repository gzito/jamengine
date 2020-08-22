/**********************************************************************************
* 
* ScrollingTile.cpp
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

#include "jam/ScrollingTile.h"
#include "jam/DrawItem.h"
#include "jam/DrawItemManager.h"
#include "jam/Gfx.h"
#include "jam/core/bmkextras.hpp"

#include <list>


using namespace std ;

#define JAM_ST_SCREEN_HEIGHT	Draw3DManager::Original3DHeight
#define JAM_ST_SCREEN_WIDTH	Draw3DManager::Original3DWidth

namespace jam
{

ScrollingMap::ScrollingMap()
{
	init();
}

ScrollingMap::~ScrollingMap()
{
}


void ScrollingMap::load( const String& filename, const String& group, int idPosition )
{
	m_tmxLoader.loadDocument(filename, group, idPosition);
	CalculateMargin();
}


void ScrollingMap::renderLayer( int layer, int x_offset, int y_offset)
{
	float fScale = 0.5f ; 

	if( layer > m_tmxLoader.getLayersNum() || layer < 0 ) return;
	x_offset=Limit(x_offset,0, m_maxsizeX);	
	y_offset=Limit(y_offset,0, m_maxsizeY);	

	int ty = y_offset / m_tmxLoader.getTileHeight();//(layer);
	int sy = -(y_offset % m_tmxLoader.getTileHeight());

	int counter=0;
	int showed=0;
	setPos(x_offset, y_offset, layer);
	int origtx = x_offset / m_tmxLoader.getTileWidth();
	int origsx = -(x_offset % m_tmxLoader.getTileWidth());

	Draw3DBatch* pBatch = GetGfx().getBatch() ;
	if( pBatch ) pBatch->begin() ;

	while ((sy / fScale) < m_minsizeY)
	{
		ty = ty % m_tmxLoader.getRows();
		int tx = origtx;
		int sx = origsx;
		while ((sx / fScale) < m_minsizeX)
		{
			counter++;
			int tile = m_tmxLoader.getLayerDataAt ( tx , ty , layer);

			bool flipX = (tile & 0x80000000) != 0 ;
			bool flipY = (tile & 0x40000000) != 0 ;
			bool flipD = (tile & 0x20000000) != 0 ;

			tile &= 0x1fffffff ;

			if (tile != 0)
			{
				DrawTile (layer, (int)(sx+m_hotspotX), (int)(sy+m_hotspotY), tile, fScale, flipX, flipY);
				showed++;
			}

			tx++;
			sx += (int)(m_tmxLoader.getTileWidth() * fScale);
		}
		ty++;
		sy += (int)(m_tmxLoader.getTileHeight() * fScale);
	}

	if( pBatch ) pBatch->end() ;

	int remain=counter-showed;
}


void ScrollingMap::renderLayer2( int layer, int x_offset, int y_offset)
{
	float fScale = 0.28f ; 

	if( layer > m_tmxLoader.getLayersNum() || layer < 0 ) return;
	x_offset=Limit(x_offset,0, m_maxsizeX);	
	y_offset=Limit(y_offset,0, m_maxsizeY);	

	int ty = y_offset / m_tmxLoader.getTileHeight();
	int sy = -(y_offset % m_tmxLoader.getTileHeight());

	int counter=0;
	int showed=0;
	setPos(x_offset, y_offset, layer);
	int origtx = x_offset / m_tmxLoader.getTileWidth();
	int origsx = -(x_offset % m_tmxLoader.getTileWidth());

	Draw3DBatch* pBatch = GetGfx().getBatch() ;
	if( pBatch ) pBatch->begin() ;

	while ( ty < m_tmxLoader.getLayerHeight(layer) )
	{
		ty = ty % m_tmxLoader.getRows();
		int tx = origtx;
		int sx = origsx;
		while ( tx < m_tmxLoader.getLayerWidth(layer) )
		{
			counter++;
			//int tile = m_tmxLoader.getLayerDataAt ( tx % m_tmxLoader.getColumns(), ty % m_tmxLoader.getRows(), layer);

// 			string rowStr = m_tmxLoader.dump(layer,0) ;
// 			IwDebugTraceLinePrintf ("%s\n",rowStr.c_str());

			int tile = m_tmxLoader.getLayerDataAt ( tx , ty , layer);
			
			bool flipX = (tile & 0x80000000) != 0 ;
			bool flipY = (tile & 0x40000000) != 0 ;
			bool flipD = (tile & 0x20000000) != 0 ;

			tile &= 0x1fffffff ;

			if (tile != 0)
			{
				DrawTile (layer, (int)(sx+m_hotspotX), (int)(sy+m_hotspotY), tile, fScale, flipX, flipY);
				showed++;
			}

			tx++;
			sx += (int)(m_tmxLoader.getTileWidth() * fScale);
		}
		ty++;
		sy += (int)(m_tmxLoader.getTileHeight() * fScale);
	}

	if( pBatch ) pBatch->end() ;

	int remain=counter-showed;
}



void ScrollingMap::renderLayerFast( int layer, int x_offset, int y_offset )
{
	//	if (layer>m_tmxLoader.getLayersNum() || layer<0) return;
	x_offset=Limit(x_offset,0, m_maxsizeX);
	y_offset=Limit(y_offset,0, m_maxsizeY);

	int ty = y_offset / m_tmxLoader.getTileHeight();//(layer);
	int sy = -(y_offset % m_tmxLoader.getTileHeight());
	int counter=0;
	int showed=0;
	DrawItem** items = new DrawItem*[m_tmxLoader.getColumns()] ;
	setPos(x_offset, y_offset, layer);
	int origtx = x_offset / m_tmxLoader.getTileWidth();
	int origsx = -(x_offset % m_tmxLoader.getTileWidth());
	int sxOrig = 0 ;

	while (sy < m_minsizeY)
	{
		ty = ty % m_tmxLoader.getRows();
		int tx = origtx;
		int sx = origsx;
		sxOrig = sx ;
		int index = 0 ;
		while (sx < m_minsizeX)
		{
			counter++;
			int tile = m_tmxLoader.getLayerDataAt ( tx , ty , layer);
			if (tile > 0)
			{
				jam::DrawItem* handleImg = jam::GetDrawItemMgr().getPtrById(tile);
				items[index++] = handleImg ;
				showed++;
			}
			else {
				items[index++] = nullptr ;
			}

			tx++;
			sx += m_tmxLoader.getTileWidth();
		}
		
		GetGfx().setRenderLevel(m_drawTileMgrSlot) ;
		GetDraw3DMgr().DrawTiledMapRow(items,index, CX3D(sxOrig+m_hotspotX),CY3D(sy+m_hotspotY),m_tmxLoader.getTileWidth());
		index = 0 ;
		ty++;
		sy += m_tmxLoader.getTileHeight();
	}
	JAM_DELETE_ARRAY(items) ;

	int remain=counter-showed;
}



void ScrollingMap::renderLayerWrap( int layer, int x_offset, int y_offset )
{
	if( layer > m_tmxLoader.getLayersNum() || layer < 0 ) return;

	float fScale = 0.25f ;

	x_offset=max(x_offset,0);	
	y_offset=max(y_offset,0);	

	int ty = y_offset / m_tmxLoader.getTileHeight();//(layer);
	int sy = -(y_offset %m_tmxLoader.getTileHeight());
	int counter=0;
	int showed=0;
	setPos(x_offset, y_offset, layer);

	Draw3DBatch* pBatch = GetGfx().getBatch() ;
	if( pBatch ) pBatch->begin() ;

	while (sy < JAM_ST_SCREEN_HEIGHT)
	{
		ty = ty % m_tmxLoader.getRows();
		int tx = x_offset / m_tmxLoader.getTileWidth();
		int sx = -(x_offset % m_tmxLoader.getTileWidth());
		while (sx < JAM_ST_SCREEN_WIDTH)
		{
			counter++;
			//int tile = m_tmxLoader.getLayerDataAt ( tx % m_tmxLoader.getMap().width, ty % m_tmxLoader.getMap().height, layer);	// se metto questo va lentissimo
			int tile = m_tmxLoader.getLayerDataAt ( tx % m_tmxLoader.getColumns(), ty % m_tmxLoader.getRows(), layer);

			bool flipX = (tile & 0x80000000) != 0 ;
			bool flipY = (tile & 0x40000000) != 0 ;
			bool flipD = (tile & 0x20000000) != 0 ;

			tile &= 0x1fffffff ;

			if (tile > 0)
			{
				DrawTile (layer, (int)(sx+m_hotspotX), (int)(sy+m_hotspotY), tile, fScale, flipX, flipY);
				showed++;
			}

			tx++;
			sx += (int)(m_tmxLoader.getTileWidth() * fScale) ;
		}
		ty++;
		sy += (int)(m_tmxLoader.getTileHeight() * fScale) ;
	}

	if( pBatch ) pBatch->end() ;

	int res=counter;
	int remain=counter-showed;
}


/*
void ScrollingMap::renderLayerUltraFast( int layer, int x_offset, int y_offset )
{
//	if (layer>m_tmxLoader.getLayersNum() || layer<0) return;
	x_offset=Limit(x_offset,0, m_maxsizeX);
	y_offset=Limit(y_offset,0, m_maxsizeY);
	
	int ty = y_offset / m_tmxLoader.getTileHeight();//(layer);
	int sy = -(y_offset % m_tmxLoader.getTileHeight());
	int counter=0;
	int showed=0;
	DrawItem** items = new DrawItem*[m_tmxLoader.getRows()*m_tmxLoader.getColumns()] ;
	setPos(x_offset, y_offset, layer);
	int origtx = x_offset / m_tmxLoader.getTileWidth();
	int origsx = -(x_offset % m_tmxLoader.getTileWidth());
	int sxOrig = 0 ;
	while (sy < m_minsizeY)
	{
		ty = ty % m_tmxLoader.getRows();
		int tx = origtx;
		int sx = origsx;
		sxOrig = sx ;
		int index = 0 ;
		while (sx < m_minsizeX)
		{
			counter++;
			int tile = m_tmxLoader.getLayerDataAt ( tx , ty , layer);
			if (tile > 0)
			{
				jam::DrawItem* handleImg = jam::GetDrawItemMgr().getPtrById(tile);
				items[index++] = handleImg ;
				showed++;
			}
			else {
				items[index++] = 0 ;
			}

			tx++;
			sx += m_tmxLoader.getTileWidth();
		}
		ty++;
		sy += m_tmxLoader.getTileHeight();
	}
	m_drawTileMgr->DrawTiledMap( items, m_tmxLoader.getRows(), m_tmxLoader.getColumns(), CX3D(sxOrig+m_hotspotX),CY3D(sy+m_hotspotY),m_tmxLoader.getTileWidth(), m_tmxLoader.getTileHeight() );
	JAM_DELETEARRAY(items) ;

	int remain=counter-showed;
}
*/


void ScrollingMap::DrawTile( int layer, int sx, int sy, int tile, float scale /*= 1.0f*/, bool flipX/*=false*/, bool flipY/*=false */)
{
	jam::DrawItem* handleImg = jam::GetDrawItemMgr().getPtrById(tile);
	GetGfx().setRenderLevel(m_drawTileMgrSlot) ;
	GetDraw3DMgr().DrawImage3D(
		handleImg,
		sx-jam::Draw3DManager::ScaledHalfVPWidth,
		-sy+jam::Draw3DManager::ScaledHalfVPHeight,
		0,				// button
		0.0f,			// angle
		scale,			// scale
		0.0f, 0.0f,		// hotspot
		0,				// color
		flipX,
		flipY );

//	m_drawTileMgr->DrawImage3D(*handleImg,CX3D(sx),CY3D(sy),0);
}

void ScrollingMap::init()
{
	m_drawTileMgrSlot = 10 ;
	m_tmxLoader.cleanup();
}

void ScrollingMap::CalculateMargin()
{
	m_x.clear();m_y.clear();
	m_hotspotX=m_tmxLoader.getTileWidth()/2.0f;
	m_hotspotY=m_tmxLoader.getTileHeight()/2.0f;

	m_mapwidth=m_tmxLoader.getColumns()* m_tmxLoader.getTileWidth();
	m_mapheight=m_tmxLoader.getRows()* m_tmxLoader.getTileHeight();
	m_maxsizeX=max(JAM_ST_SCREEN_WIDTH - m_mapwidth, m_mapwidth-JAM_ST_SCREEN_WIDTH);
	m_maxsizeY=max(JAM_ST_SCREEN_HEIGHT - m_mapheight, m_mapheight-JAM_ST_SCREEN_HEIGHT);

	m_minsizeX=min(JAM_ST_SCREEN_WIDTH , m_mapwidth);
	m_minsizeY=min(JAM_ST_SCREEN_HEIGHT, m_mapheight);

	for (size_t i=0; i<m_tmxLoader.getLayersNum(); i++)
	{
		m_x.push_back(0); m_y.push_back(0);
	}
}

void ScrollingMap::setPos( int x_offset, int y_offset, int layer )
{
	if (m_x.size()<=(size_t)layer) return;
	m_x[layer]=x_offset; m_y[layer]=y_offset;
}

}

