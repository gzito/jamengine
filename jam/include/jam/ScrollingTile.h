/**********************************************************************************
* 
* ScollingTile.h
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

#ifndef __JAM_SCROLLINGTILE_H__
#define __JAM_SCROLLINGTILE_H__

#include <jam/jam.h>
#include <jam/TMXLoader.h>
#include <jam/Draw3dManager.h>

#include <string>


namespace jam
{
class ScrollingMap 
{
public :
							ScrollingMap();
							~ScrollingMap();

	virtual void			init();

	virtual void			load(const String& filename, const String& group, int idPosition) ;
	void					renderLayer(int layer, int x, int y);
	void					renderLayer2(int layer, int x, int y);
	void					renderLayerFast(int layer, int x, int y);
//	void					renderLayerUltraFast( int layer, int x_offset, int y_offset );
	void					renderLayerWrap( int layer, int x_offset, int y_offset );

	void					CalculateMargin();

	tiledmap::MapItem&		getMapRef() {return m_tmxLoader.getMap();}
	tiledmap::MapItem*		getMapPtr() {return &m_tmxLoader.getMap();}

	String					getLayerName(int layer) {return m_tmxLoader.getLayerName(layer);}
	size_t					getLayersNum() {return m_tmxLoader.getLayersNum();}
	size_t					getTilesNum() {return m_tmxLoader.getTilesNum();}
	size_t					getGroupsNum() {return m_tmxLoader.getGroupsNum();}
	size_t					getObjectsNum(int level) {return m_tmxLoader.getObjectsNum(level);}
	String					getGroupName(int layer) {return m_tmxLoader.getGroupName(layer);}
	int						getMapColumns() {return m_tmxLoader.getMap().width;}
	int						getMapRows() { return m_tmxLoader.getMap().height; }
	int						getMapHeight() const { return m_mapheight; }
	int						getMapWidth() const { return m_mapwidth; }
	int						getTileWidth() { return m_tmxLoader.getMap().tileWidth; }
	int						getTileHeight() { return m_tmxLoader.getMap().tileHeight; }
//	int						getTileSpacing() { return m_tmxLoader.getTileSpacing(); }
//	int						getTilesetMargin() { return m_tmxLoader.getTilesetMargin(); }

	float					getMapColX(int column) {return (m_tmxLoader.getTileWidth() * jam::Draw3DManager::RatioX)*column;}
	float					getMapRowY(int row) {return m_tmxLoader.getTileHeight()*row * jam::Draw3DManager::RatioY;}
	void					DrawTile( int layer, int sx, int sy, int tile, float scale = 1.0f, bool flipX=false, bool flipY=false);
		
	int						getAt(int c, int r, int layer) {return  m_tmxLoader.getLayerDataAt(c,r,layer);	}
	void					setAt(int c, int r, int v, int layer) {m_tmxLoader.setLayerDataAt(c,r,v,layer);	}
	
	tiledmap::object&		getObjAt(int id, int layer) {return  m_tmxLoader.getGroupDataAt(id,layer);	}
	String					getGroupProperty(const String& name, int layer) {return  m_tmxLoader.getGroupProperty(name,layer);	}
//	void					setAt(int c, int r, int v, int layer) {m_tmxLoader.setLayerDataAt(c,r,v,layer);	}


	int						getX(int layer=0) const { return m_x[layer]; }
//	void					setX(int val) { m_x = val; }
	int						getY(int layer=0) const { return m_y[layer]; }
	void					setPos( int x_offset, int y_offset, int layer );
//	void					getY(int val) { m_y = val; }

protected:
	TMXLoader				m_tmxLoader;
	int						m_drawTileMgrSlot ;

	float					m_hotspotX;
	float					m_hotspotY;

	int						m_mapwidth;
	int						m_mapheight;
	std::vector<int>		m_x; 
	std::vector<int>		m_y;

	int						m_maxsizeX;
	int						m_maxsizeY;

	int						m_minsizeX;
	int						m_minsizeY;
};

}

#endif // __JAM_SCROLLINGTILE_H__
