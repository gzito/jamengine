/**********************************************************************************
* 
* TMXLoader.h
* 
* This file is part of Jam
* 
* Copyright (c) 2014-2019 Giovanni Zito, Gianluca Sclano.
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

#ifndef __JAM_TMXLOADER_H__
#define __JAM_TMXLOADER_H__

//
// TMX File format
// https://github.com/bjorn/tiled/wiki/TMX-Map-Format
//

#include <jam/jam.h>
#include <jam/Base64.h>
#include <jam/Texture2D.h>

#include <tinyxml.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string>
#include <map>


using namespace std;

/*
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE map SYSTEM "http://mapeditor.org/dtd/1.0/map.dtd">
	<map version="1.0" orientation="orthogonal" width="50" height="50" tilewidth="24" tileheight="24">
	<tileset firstgid="1" name="sewer_tileset" tilewidth="24" tileheight="24">
		<image source="sewer_tileset.png" trans="#ff00ff"/>
	</tileset>
	<layer name="Bottom" width="50" height="50">
		<data encoding="base64" compression="gzip">
			H4sIAAAAAAAAA+3X2QrCMBAF0OKbSwWrCC4vdV/+w///JKeQgWFIatJ0SfE+HJDGonfaTJI8y7IlORkLkotrZ3I0SjUux2zfCSX/h/7tX/T9/5jjQl7kalSfb4nk2JAJ2Y78eUzJjMwjcnAtQnHt2sixIgVZR+TgWtjca8a4dvy+viNyaE1ycC2kh+WaZqvdUDmeppYxQp5DV313KG3n2JE9OYw8R2m5rw+638WuHzw/mrzjMWS/81k/ZM6U5ofsdz7rh8yZ0vxw9VtX361yfkzOlOZHSC/Xa4NtDgw1PwAgDvdSre/eGot7aV1PHgPbWTW1/a5vDn1WTW1f4ptDn1Vd+5KUyf1IkdXvS1LmOqti7wEAAAAAAAAAAF37AlFWzCEQJwAA
		</data>
	</layer>
	<layer name="Top" width="50" height="50" opacity="0.49">
		<data encoding="base64" compression="gzip">
			H4sIAAAAAAAAA+3WOwqAMBBAQQu1Uq9i4a8QO+9/JrcwF5BADMzAI92y6bZp/mUvvUAm/vEvtf5jjpa3NdrKrvPZEZ3R/b5Xprlt1EV9pnmlDNEYTaUXAQCgCukOTtV6D6c7OOUeBgAAAAAghwe55M5JECcAAA==
		</data>
	</layer>
</map>

*/

namespace jam
{

namespace tiledmap
{

struct ImageSource
{
	String					name;
	String					trans;
	String					filename;
};


struct TileSet
{
	int						firstgid;
	String					name;
	int						tilewidth;
	int						tileheight;
	int						spacing;
	int						margin;
	ImageSource				imageSheet;
};


struct TileLayer
{
	String					name;
	int						width;
	int						height;
	std::vector< std::vector< int > >	data;
};

struct object
{
	String					name;
	String					type;
	int						gid;
	int						x; 
	int						y;
	int						row;
	int						column;
	int						width;
	int						height;
	std::map<String, String>	properties;
};

struct ObjectGroup
{
	String					name;
//	int						width;
//	int						height;
	std::vector<object>		objectItems;
	std::map<String, String>	properties;
};

struct MapItem
{		
	int						width;
	int						height;
	int						tileWidth;
	int						tileHeight;
	String					orientation;
	std::vector<TileLayer>	layers;
	std::vector<ObjectGroup>	objlayers;	
// 	TileSet					tileSet;
// 	ImageSource				imageSource;
	std::vector<TileSet>	tileSetList;
	std::map<String, String>	properties;
};

	
};

/**
	This class allows to load a tiled map.

	\sa http://www.mapeditor.org/
*/
class TMXLoader : public TiXmlVisitor
{
public:
							TMXLoader();
	virtual					~TMXLoader();
	void					cleanup();

	bool					loadDocument(const String& filename, const String& group, int idPosition);

	jam::Texture2D*			getTilesetImage() { return m_pImgTileset; }
	jam::Texture2D*			getMapImage() {return m_pImgMap;}

	tiledmap::MapItem&		getMap()  { return m_map; }
	tiledmap::MapItem*		getMapPtr()  { return &m_map; }
	 
	int						getTileWidth(){return m_map.tileWidth;}
	int						getTileHeight(){return m_map.tileHeight;}
	size_t					getTilesNum(){return m_map.tileSetList.size();}

	int						getColumns(){return m_map.width;}
	int						getRows(){return m_map.height;}
		
	int						getLayerDataAt(int col, int row, int layer=0) const { return m_map.layers[layer].data[row][col]; }
	void					setLayerDataAt(int col, int row, int v, int layer=0) {  m_map.layers[layer].data[row][col]=v; }
	size_t					getLayersNum(){return m_map.layers.size();}
	String					getLayerName(int layer){return m_map.layers[layer].name;}
	int						getLayerWidth(int layer) { return m_map.layers[layer].width; }
	int						getLayerHeight(int layer) { return m_map.layers[layer].height; }

	size_t					getObjectsNum(int level){return m_map.objlayers[level].objectItems.size();}
	tiledmap::object&		getGroupDataAt(int id, int layer=0)  { return m_map.objlayers[layer].objectItems[id]; }
	void					setGroupDataAt(int id, tiledmap::object&  v, int layer=0) {  m_map.objlayers[layer].objectItems[id]=v; }
	size_t					getGroupsNum(){return m_map.objlayers.size();}
	String					getGroupName(int layer){return m_map.objlayers[layer].name;}
	String					getGroupProperty(const String& name, int layer){return m_map.objlayers[layer].properties[name];}
		
	String					dump(int layer, int row);
	
	int						getMapColumns() {return m_map.width;}
	int						getMapRows() { return m_map.height; }
	int						getMapHeight() const { return m_map.height* m_map.tileHeight; }
	int						getMapWidth() const { return m_map.width* m_map.tileWidth;}
	int						getAt(int c, int r, int layer) {return  getLayerDataAt(c,r,layer);	}
	void					setAt(int c, int r, int v, int layer) {setLayerDataAt(c,r,v,layer);	}

	int						getObjAt(int id, int layer) {return  getGroupDataAt(id,layer).gid;	}
	void					seObjtAt(int id, int v, int layer) { m_map.objlayers[layer].objectItems[id].gid=v;	}

	String					getFilename() const { return m_filename; } ;


protected:
	virtual bool 			VisitEnter  (const TiXmlDocument  &);
	virtual bool 			VisitExit (const TiXmlDocument &);
	virtual bool 			VisitEnter (const TiXmlElement &, const TiXmlAttribute *);
	virtual bool 			VisitExit (const TiXmlElement &);
	virtual bool 			Visit (const TiXmlDeclaration &);
	virtual bool 			Visit (const TiXmlText &);
	virtual bool 			Visit (const TiXmlComment &);
	virtual bool 			Visit (const TiXmlUnknown &);

private:
	tiledmap::MapItem		m_map;
	
// 	int						m_TileWidth;
// 	int						m_TileHeight;
// 	int						m_TileSpacing;
// 	int						m_TilesetMargin;
// 	int						m_NumMapColumns;
// 	int						m_NumMapRows;

	jam::Texture2D*			m_pImgTileset ;
	jam::Texture2D*			m_pImgMap;

//	ofstream				m_file;

//	vector<vector<int>>		m_LayerData;

	std::vector<std::vector<int> >	decode_and_store_map_data( String encoded_data );
	//jam::Texture2D* load_Image( String filename );

	String					m_Group;
	int						m_IdPosition;
	bool					m_isLoaded;
	
	String					m_filename;
	String					m_completeName;
	int						m_nextIdPosition;
	String					m_lastParent;
	void					buildMapImage();
//	TiXmlDocument			*doc;

};

}

#endif // __JAM_TMXLOADER_H__
