/**********************************************************************************
* 
* DrawItemManager.h
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

#ifndef __JAM_DRAWITEMMANAGER_H__
#define __JAM_DRAWITEMMANAGER_H__

#include <jam/jam.h>
#include <jam/DrawItem.h>
#include <jam/Bank.h>
#include <jam/Singleton.h>
#include <jam/String.h>

#include <list>
#include <map>


namespace jam
{

class JAM_API DrawItemManager : public Singleton<DrawItemManager>, public Bank<DrawItem>
{
	friend class Singleton<DrawItemManager> ;

public:

	/**
	 Create a DrawItem from a texture filename.
	 \param filename The name of texture to be loaded, as reported in group file, with no pathname nor extension
	 \param groupName The name of the group the texture belongs, as reported in group file
	 \param id The id to assign to the DrawItem. If id is -1, then it is automatically generated
	 \param name The name to assign to draw item. If name is an empty string, then it is taken from filename
	 \param gfxScale Scaling factor for the texture, default to 1
	*/
	DrawItem*				loadTexture( const String& filename, const String& groupName, int id=-1, const String& name="", float gfxScale = 1.0f );

	DrawItem*				loadTextureFromFileSystem( const String& filename, int id=-1, const String& name="", float gfxScale = 1.0f );

	/**
	 Load a "regular" sheet file (based on the specified grid size)
	 Create cuts from a texture name at starting the given id_position.
	 
	 \remark The id_position argument specifies the bank id the cuts will be created at. 
	  When a DrawItem is created from a sheet a default name is created 
	  with texture name and number (es: textname_1, textname_10)
	*/
	int						loadSheet(const String& filename, const String& groupName, int xsize,int ysize, int cols,int rows, int id_position, int offsetX=0,int offsetY=0, const String& name="" , int  spacingx=0, int spacingy=0, float gfxScale = 1.0f);

	/**
	 Load regular sheet and create a unique big animation.
	*/
	int						loadAnimationSheet(const String& xmlFilePath, const String& sheetFilename,const String& grpName, float timing , int xsize,int ysize, int cols,int rows, int id_position, int offsetX=0,int offsetY=0, const String& name="" , int  spacingx=0, int spacingy=0);

	/**
	 Load XML from Zwoptex file format (no animations)
	*/
	int						loadXmlSheet(const String& xmlFilePath, const String& sheetFilename, const String& grpName, int id_position);

	/**
	 Load XML from Zwoptex file and create Animation
	*/
	int						loadXmlAnimationSheet( const String& xmlFilePath, const String& sheetFilename, const String& grpName, int position, float timing );

	/**
	 Load XML from TexturePacker file format (no animations)
	*/
	int						loadXmlPackSheet( const String& xmlFilePath, const String& sheetName, const String& grpName, int id_position, float gfxScale = 1.0f );

	/**
	 Load XML from TexturePacker file and create Animation
	*/
	int						importXmlPackAnimationSheet( const String& xmlFilePath, const String& sheetFilename, const String& grpName, int position, float timing );

	DrawItem*				grab( const Ref<Texture2D>& pTexture, int _x0, int _y0, int xsize, int ysize, int position=-1, int offsetX=0, int offsetY=0, const String& name="", float gfxScale = 1.0f );

	/**
	 This method destroys all draw items associated with the given sheet (by sheet name)
	 Also it destroys all the animations defined in the given sheet
	*/
	void					destroySheet( const String& name );

	/**
    This method destroys cuts made from a sheet (doesn't remove the sheet itself)
	*/
	int						deleteSheetCuts( int cols,int rows,int id_position, const String& aName ) ;

private:
	DrawItemManager();
	virtual ~DrawItemManager();

	static String			findPrefix(const String& prefix, const String& str, int& number);	
	static int				splitString( const std::string& input, const std::string& delimiter, std::vector<std::string>& results, bool includeEmpties, bool includeLast, bool trimResults );
	static unsigned int		stringReplace(std::string& str,const std::string& search,const std::string& replace);

	
	std::map<String,std::list<int> >		m_drawItemsPerSheet ;
	std::map<String,std::list<int> >		m_animationsPerSheet ;
};

/** Returns the singleton instance */
JAM_INLINE DrawItemManager& GetDrawItemMgr() { return DrawItemManager::getSingleton(); }

}

#endif // __JAM_DRAWITEMMANAGER_H__
