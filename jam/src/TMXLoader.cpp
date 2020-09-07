/**********************************************************************************
* 
* TMXLoader.cpp
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

#include "stdafx.h"

#include "jam/TMXLoader.h"
#include "jam/DrawItemManager.h"
#include "jam/core/filesystem.h"
#include "jam/core/bmkextras.hpp"

#include <sstream>

namespace jam
{

TMXLoader::TMXLoader(): m_isLoaded(0)
{
	//mapWidth = mapHeight = 0;
	//    img_tileset = NULL;
	// Debug crap
	//ofstream myfile;
	m_Group="";
	m_IdPosition=0;
	m_nextIdPosition=0;
	m_filename="";
	m_completeName="";
	m_lastParent="map";
	//m_file.open ("debug.txt");
}

TMXLoader::~TMXLoader()
{
	//dtor
	for (size_t i=0; i<m_map.tileSetList.size(); i++)
	{
		jam::GetDrawItemMgr().destroySheet(m_map.tileSetList[i].imageSheet.name);
	}
}

void TMXLoader::cleanup()
{
	//Free the surface
	//SDL_FreeSurface( img_tileset );
	//SDL_FreeSurface( img_map );
	m_isLoaded=false;
	//m_file.close();
}

bool TMXLoader::VisitEnter  (const TiXmlDocument  &doc)
{
	return true; //TODO: for performance, we may not want to return true for each of these callbacks for the visitor pattern.
}

bool TMXLoader::VisitExit (const TiXmlDocument &doc)
{
	return true;
}

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

/* esempio multitile
<tileset firstgid="1" name="enemy_sheet" tilewidth="64" tileheight="64" spacing="2" margin="2">
<image source="enemy_sheet.png"/>
</tileset>
<tileset firstgid="10" name="blocks2" tilewidth="32" tileheight="32" spacing="2" margin="2">
<image source="blocks2.png"/>
</tileset>
*/

bool TMXLoader::VisitEnter (const TiXmlElement &elem, const TiXmlAttribute *attrib)
{
	tiledmap::TileLayer lev;
	tiledmap::TileSet tset;
	tiledmap::ObjectGroup objGroup;
	tiledmap::object	obj;
	if ( strcmp("map", elem.Value()) == 0 ) {
		elem.Attribute("width", &m_map.width);
		elem.Attribute("height", &m_map.height);
		elem.Attribute("tilewidth", &m_map.tileWidth);
		elem.Attribute("tileheight", &m_map.tileHeight);
		const char* attrib = elem.Attribute("orientation");
		m_map.orientation=attrib;
		m_lastParent="map";
	}
	else if (strcmp("property", elem.Value()) == 0)
	{
		String nonno= (elem.Parent()->Parent()->Value());
		//const char* attribN = nonno.Attribute("name");

		String key = String(elem.Attribute("name"));
		const char* attrib =elem.Attribute("value");
		String value = String(attrib);
		if (m_lastParent=="map")
		{
			m_map.properties[key]=value;
		}
		else if (m_lastParent=="object")
		{
			if (nonno=="objectgroup")
			{
				objGroup=m_map.objlayers[ m_map.objlayers.size()-1]; // ** get the last one
				objGroup.properties[key]=value;
				m_map.objlayers[ m_map.objlayers.size()-1]=objGroup;
				//			obj=objGroup.objectItems[objGroup.objectItems.size()-1];
			}
			else
			{
				objGroup=m_map.objlayers[m_map.objlayers.size()-1]; // ** get the last one
				obj=objGroup.objectItems[objGroup.objectItems.size()-1];
				obj.properties[key]=value;
				objGroup.objectItems[objGroup.objectItems.size()-1]=obj;
				m_map.objlayers[m_map.objlayers.size()-1]=objGroup;
			}
		}
	}
	else if (strcmp("tileset",elem.Value())==0) {
		tset=tiledmap::TileSet();
		const char* attrib = elem.Attribute("name");
		tset.name=attrib;

		elem.Attribute("tilewidth", &tset.tilewidth);
		elem.Attribute("tileheight", &tset.tileheight);
		elem.Attribute("spacing", &tset.spacing);
		elem.Attribute("margin", &tset.margin);
		elem.Attribute("firstgid", &tset.firstgid);

		// Need a new class called Tileset
		/*elem.Attribute("tilewidth", &m_map.tileSet.tilewidth);
		elem.Attribute("tileheight", &m_map.tileSet.tileheight);
		elem.Attribute("spacing", &m_map.tileSet.spacing);
		elem.Attribute("margin", &m_map.tileSet.margin);
		elem.Attribute("firstgid", &m_map.tileSet.firstgid);
		*/
		m_map.tileSetList.push_back(tset);
	}
	else if (strcmp("image",elem.Value())==0 && m_map.properties["nosheet"].empty()) {
		tset=m_map.tileSetList[m_map.tileSetList.size()-1]; // ** get the last one
		//m_map.tileSetList.push_back(tset);
		const char* attrib = elem.Attribute("source");
		tset.imageSheet.filename=attrib;
		//m_map.imageSource.filename=attrib;
		String str = jam::getBasename(tset.imageSheet.filename) ;
//			IwPathGetFilename(tset.imageSheet.filename.c_str(),str,0); // use file extension
		tset.imageSheet.name = str ;
		// *** Here load imageSheet
		m_nextIdPosition = jam::GetDrawItemMgr().loadSheet(tset.imageSheet.name ,m_Group, tset.tilewidth, tset.tilewidth, 
														   -1,-1, tset.margin,tset.margin,
														   jam::getFileNameWithoutExtension(str),
														   tset.spacing, tset.spacing );
		//m_map.tileSetList.push_back(tset);
		m_map.tileSetList[m_map.tileSetList.size()-1]=tset;
	}
	else if (strcmp("layer",elem.Value())==0)
	{
		lev=tiledmap::TileLayer();
		const char* attrib = elem.Attribute("name");
		lev.name=attrib;
		elem.Attribute("width", &lev.width);
		elem.Attribute("height", &lev.height);
		m_map.layers.push_back(lev);
	}
	else if (strcmp("data",elem.Value())==0)
	{
		lev=m_map.layers[m_map.layers.size()-1]; // ** get the last one
		const char* text = elem.GetText();
		lev.data=decode_and_store_map_data( text );
		m_map.layers[m_map.layers.size()-1]=lev;
	}
	/*
	<objectgroup name="Object Layer 1" width="16" height="16">
	<object gid="36" x="0" y="384"/>
	<object gid="36" x="960" y="832"/>
	<object gid="36" x="128" y="1024"/>
	<object gid="41" x="448" y="64"/>
	<object name="Player one" type="Player" gid="1" x="512" y="576">
	<properties>
	<property name="start" value="0"/>
	</properties>
	</object>
	<object gid="2" x="448" y="640"/>
	</objectgroup>
	*/
	else if (strcmp("objectgroup",elem.Value())==0) {
		objGroup=tiledmap::ObjectGroup();
		const char* attrib = elem.Attribute("name");
		objGroup.name=attrib;
// 			elem.Attribute("width", &objGroup.width);
// 			elem.Attribute("height", &objGroup.height);
		m_map.objlayers.push_back(objGroup);
	}
	else if (strcmp("object",elem.Value())==0)
	{
		objGroup=m_map.objlayers[m_map.objlayers.size()-1]; // ** get the last one
		obj=tiledmap::object();
		const char* attrib = elem.Attribute("name");
		if (attrib) obj.name=attrib;
		const char* attrib2 = elem.Attribute("type");
		if (attrib2) obj.type=attrib2;

		elem.Attribute("gid", &obj.gid);
		elem.Attribute("x", &obj.x);
		elem.Attribute("y", &obj.y);
// 			elem.Attribute("width", &obj.width);
// 			elem.Attribute("height", &obj.height);

		obj.row = Limit((obj.y/m_map.tileHeight)-1,0,m_map.tileHeight-1);
		obj.column = Limit((obj.x/m_map.tileWidth),0,m_map.tileWidth-1);

		objGroup.objectItems.push_back(obj);
		m_map.objlayers[m_map.objlayers.size()-1]=objGroup;
		m_lastParent="object";
		//m_map.objlayers.push_back(objGroup);
	}
	// 	else if (string("property") == elem.Value())
	// 	{
	// 		String key = String(elem.Attribute("name"));
	// 		const char* attrib =elem.Attribute("value");
	// 		String value = String(attrib);
	// 		m_map.properties[key]=value;
	// 	}
	return true;
}

bool TMXLoader::VisitExit (const TiXmlElement &elem)
{
	return true;
}

bool TMXLoader::Visit (const TiXmlDeclaration &dec)
{
	return true;
}

bool TMXLoader::Visit (const TiXmlText &text)
{
	return true;
}

bool TMXLoader::Visit (const TiXmlComment &comment)
{
	return true;
}

bool TMXLoader::Visit (const TiXmlUnknown &unknown)
{
	return true;
}

// **************************************************************************************
// *** LoadDocument
// **************************************************************************************
bool TMXLoader::loadDocument( const String& filename, const String& group, int idPosition )
{
	//	String m_path = "./media/data/";
	m_Group=group;
	m_IdPosition=idPosition;
	m_filename=jam::getFileNameWithoutExtension( jam::getBasename(filename) );
	m_completeName=filename;
	TiXmlDocument doc((char*)m_completeName.c_str());
	if ( ! doc.LoadFile() ) {
		return false;
	}

	//TiXmlElement* elem = doc.RootElement();

	JAM_TRACE( "Loading map data [%s]",filename.c_str() );
	doc.Accept(this);
	JAM_TRACE( "...Done, map loaded." );
	JAM_TRACE( "Columns: %d - Rows: %d" , m_map.width,m_map.height );
	JAM_TRACE( "Layers: %zu", m_map.layers.size() );
	for( size_t i= 0; i<  m_map.layers.size(); i++ )
		JAM_TRACE( "   TileLayer %zu [%s]",i, m_map.layers[i].name.c_str() );

	JAM_TRACE ("Tilesets: %zu", m_map.tileSetList.size() );
	for (size_t i= 0; i<  m_map.tileSetList.size(); i++)
		JAM_TRACE ("   Tiles %zu [%s]",i, m_map.tileSetList[i].name.c_str() );

	m_isLoaded=true;

	// 	for (int row = 0; row < m_map.height; row++)
	// 	{
	// 		IwDebugTraceLinePrintf ("Loading map data [%s]",dump(0,row).c_str());
	// 	}
	return true;
}

// ************************************************************************
std::vector<std::vector<int> > TMXLoader::decode_and_store_map_data( String encoded_data )
{
	//const int NUM_LAYER_COLS = 3;
	// const int NUM_LAYER_ROWS = 3;
	// const int NUM_TILES = NUM_LAYER_ROWS * NUM_LAYER_COLS;
	//int m_LayerData[NUM_LAYER_ROWS][NUM_LAYER_COLS];
	//string encoded_data = elem.GetText();
	//string unencoded_data = base64_decode(encoded_data);
	//const char* unencoded_c_str = unencoded_data.c_str();
	//const char* unencoded_c_str = unencoded_data.c_str();
	//m_LayerData.push_back( vector<> );

	std::vector<std::vector<int> > layerDataRes;

	vector< int > layerDataRow(m_map.width );
	int m_LayerRow = 0;
	int m_LayerCol = 0;

	vector<int> unencoded_data = base64_decode(encoded_data);

	for (int i = 0; i < m_map.height; i++)
	{
		layerDataRes.push_back( layerDataRow );
	}

	for (size_t i = 0; i < unencoded_data.size(); i += 4)
	{
		// Get the grid ID

		int a = unencoded_data[i];
		int b = unencoded_data[i + 1];
		int c = unencoded_data[i + 2];
		int d = unencoded_data[i + 3];

		int gid = a | b << 8 | c << 16 | d << 24;
		if (gid>0)	{
			gid += (m_IdPosition-1);
		}
		else if (gid < 0) {
			uint32_t flagMask = gid & 0xe0000000 ;	// save flip flags
			gid &= 0x1fffffff ;						// remove flip flags
			gid += (m_IdPosition-1);				// add id position
			gid |= flagMask ;						// restore flip flags
		}

		layerDataRes[m_LayerRow][m_LayerCol] = gid ;

		if ((i + 4) % ( m_map.width * 4) == 0) {
			m_LayerRow++;
			m_LayerCol = 0;
			//myfile << endl;
		}
		else {
			m_LayerCol++;
		}
	}
	return layerDataRes;
}

String TMXLoader::dump(int layer, int row)
{
	std::stringstream oss;
	for (int col = 0; col < m_map.width; col++)
	{
		if (m_map.layers[layer].data[row][col]>m_nextIdPosition)
			oss << "***" ;
		oss << m_map.layers[layer].data[row][col] ;
		oss << "," ;
	}
	return oss.str();
}

/*****************************************************/

// jam::Texture2D* TMXLoader::load_Image( String filename )
// {
//     //The image that's loaded
//      jam::Texture2D* loadedImage = NULL;
//
//     //The optimized surface that will be used
//      jam::Texture2D* optimizedImage = NULL;
//
//     //Load the image
//     loadedImage = jam::GetDrawItemMgr().LoadTexture( filename.c_str() ,m_Group);
//
//     //If the image loaded
//     if( loadedImage != NULL )
//     {
// /*        //Create an optimized surface
//         optimizedImage = SDL_DisplayFormat( loadedImage );
//
//         //Free the old surface
//         SDL_FreeSurface( loadedImage );
//
//         //If the surface was optimized
//         if( optimizedImage != NULL )
//         {
//             //Color key surface
//             SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
//         }
// 		  */
//     }
//
//     //Return the optimized surface
//     return optimizedImage;
// }

void TMXLoader::buildMapImage()
{
	/*
	// We must find a way to get these values from the layer at runtime!
	const int NUM_TILESET_COLS = 18;
	const int NUM_TILESET_ROWS = 11;
	int x,y,w,h;
	int x1,y1,w1,h1;
	//const int MARGIN = 2;
	//const int NUM_TILES = NUM_LAYER_ROWS * NUM_LAYER_COLS;

	//     img_map = SDL_CreateRGBSurface( SDL_SWSURFACE, getNumMapColumns() * 32, getNumMapRows() * 32, 32, 0, 0, 0, 0 );
	//
	//     //SDL_Surface *src;
	//     //SDL_Surface *dst;
	//     SDL_Rect srcrect;
	//     SDL_Rect dstrect;
	//
	for (int row = 0; row < m_map.height; row++)
	{
	for (int col = 0; col < m_map.width; col++)
	{
	//  int gid = m_LayerData[row][col];
	int gid=0;

	if (gid == 0)
	continue;

	myfile << "\nGID is " << gid;

	int tileset_col;
	int tileset_row;

	gid--; // convert to something we can use with the 0 based system.

	tileset_col = (gid % NUM_TILESET_COLS);
	tileset_row = gid / NUM_TILESET_COLS;

	//
	// 			x = m_map.tileSet.margin + ( m_map.tileSet.spacing + 32) * tileset_col;
	// 			y = m_map.tileSet.margin + (m_map.tileSet.spacing + 32) * tileset_row;
	// 			w = m_map.tileSet.tilewidth;
	// 			h = m_map.tileSet.tileheight;
	//
	// 			x1 = col * m_map.tileSet.tilewidth;
	// 			y1 = row * m_map.tileSet.tileheight;
	// 			w1 = m_map.tileSet.tilewidth;
	// 			h1 = m_map.tileSet.tileheight;

	//SDL_BlitSurface(img_tileset, &srcrect, img_map, &dstrect);
	}
	}
	*/
}

}
