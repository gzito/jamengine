/**********************************************************************************
* 
* DrawItemManager.cpp
* 
* This file is part of Jam
* 
* Copyright (c) 2014-2019 Giovanni Zito & Gianluca Sclano.
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

#include "jam/DrawItemManager.h"
#include "jam/Anim2d.h"
#include "jam/Animation2dManager.h"
#include "jam/core/filesystem.h"

#include <tinyxml.h>
#include <assert.h>
#include <stdexcept>

using namespace std ;

namespace jam
{

/*
	Load texture from file and upload it to the gpu
*/
DrawItem* DrawItemManager::loadTexture( const String& filename, const String& groupName, const String& name/*=""*/,	float gfxScale /*= 1.0f*/ )
{
	assert(filename.size()) ;
	assert(groupName.size()) ;

	String aName = name ;

	if( aName.empty() ) {
		aName = jam::getFileNameWithoutExtension( jam::getBasename(filename) );
	}

	Texture2D* pTexture = new Texture2D() ;
	pTexture->load(filename) ;
	pTexture->setName(aName) ;

	// create a single drawitem
	return grab(pTexture, 0,0,pTexture->getWidth(),pTexture->getHeight(), 0,0, aName, gfxScale);
}

/*
	Load texture from file and DOESN'T upload it to the gpe.
	Use loadTexture instead if you want upload it
*/
DrawItem* DrawItemManager::loadTextureFromFileSystem( const String& filename,
	int id /*= -1*/,
	const String& name/*=""*/,
	float gfxScale /*= 1.0f*/ )
{
	assert(filename.size()) ;

	Texture2D* pTexture = new Texture2D() ;
	pTexture->load(filename,true,false) ;			// load and do not upload it!

	String aName = name ;

	if( aName.empty() ) {
		aName = jam::getFileNameWithoutExtension( jam::getBasename(filename) );
	}

	pTexture->setName(aName) ;

	// create a single drawitem
	return grab(pTexture, 0,0,pTexture->getWidth(),pTexture->getHeight(), 0,0, aName, gfxScale);
}

// ************************************************************************
// *** Load Texture and create regular Sheet
// ************************************************************************
int DrawItemManager::loadSheet( const String& filename, const String& groupName,
							   int xsize,int ysize,
							   int cols,int rows,
							   int offsetX/*=0*/,int offsetY/*=0*/,
							   const String& name/*="" */,
							   int spacingx/*=0*/,int spacingy/*=0*/,
							   float gfxScale /*= 1.0f*/,
							   bool zeroBasedCuts /* = true */ )
{
	if( m_drawItemsPerSheet[filename].size() ) {
		m_drawItemsPerSheet[filename].clear() ;
	}

	Texture2D* pTexture = new Texture2D() ;
	pTexture->load(filename);
	
	if( cols < 0 ) {
		cols=int((pTexture->getWidth()-offsetX)/(xsize+spacingx));
	}

	if( rows < 0 ) {
		rows=int((pTexture->getHeight()-offsetY)/(ysize+spacingy));
	}

	std::list<String> listOfIds ;
	String aName = name ;

	if (aName.empty()) {
		aName = jam::getFileNameWithoutExtension( jam::getBasename(filename) ); ;
	}

	int x0, y0;
	String cutName ;
	int id_position = zeroBasedCuts ? 0 : 1 ;

	for (int r=0; r<rows; r++) {
		for (int c=0; c<cols; c++) {
			x0=offsetX + c*(xsize+spacingx);	
			y0=offsetY + r*(ysize+spacingy);	
			cutName = aName + "_" + to_string(id_position) ;
			id_position ++ ;
			DrawItem* cutted = DrawItem::create(pTexture, x0,y0, x0+xsize, y0+ysize, gfxScale);
			cutted->setName(cutName) ;
			addObject(cutted);
			listOfIds.push_back(aName) ;
			JAM_TRACE( "Create cut %s", cutName.c_str() );
		}
	}

	m_drawItemsPerSheet[filename] = listOfIds ;
	return zeroBasedCuts ? id_position : id_position-1 ;
}


// ************************************************************************
// *** Load regular sheet and create a unique big animation 
// ************************************************************************
int DrawItemManager::loadAnimationSheet( const String& sheetFileName, const String& sheetName, const String& grpName, float timing, 
										int xsize,int ysize,
										int cols,int rows, 
										int offsetX/*=0*/, int offsetY/*=0*/,
										const String& name/*="" */,
										int spacingx/*=0*/, int spacingy/*=0*/,
										float gfxScale /*= 1.0f*/,
										bool zeroBasedCuts /* = true */ )
{
	int framesCount = loadSheet(sheetFileName, grpName, xsize,ysize, cols,rows, offsetX,offsetY, name, spacingx,spacingy, gfxScale, zeroBasedCuts);

	// if sheetName.empty() works as loadSheet()
	if (sheetName.empty()) return framesCount;

	Animation2D* pAnim = Animation2D::create();
	pAnim->setLoop(true);
	pAnim->setName(sheetName);
	GetAnim2DMgr().addObject(pAnim);

	for( auto it=m_drawItemsPerSheet[sheetFileName].begin(); it != m_drawItemsPerSheet[sheetFileName].end(); ++it) 
	{
		pAnim->addFrame( GetDrawItemMgr().getObject(*it), timing ) ;
	}
	return framesCount;
}

// ************************************************************************
// *** Load XML from Zwoptex file format (no animations) and create frames / drawtems
// ************************************************************************
int DrawItemManager::loadXmlSheet( const String& xmlFilePath, const String& sheetFilename, const String& grpName )
{
	Texture2D* pTexture = new Texture2D() ;
	pTexture->load(sheetFilename);
	
	TiXmlDocument xmlDoc;
	if( !xmlDoc.LoadFile(xmlFilePath.c_str()) ) {
		JAM_ERROR( "Error loading file: %s", xmlFilePath.c_str() ) ;
	}
	TiXmlElement* xmlRoot = xmlDoc.RootElement() ;
	TiXmlElement* topDictElem = xmlRoot->FirstChildElement("dict") ;
	TiXmlElement* topFramesDict = topDictElem->FirstChildElement("dict")->NextSiblingElement("dict") ;

	TiXmlElement* nel = topFramesDict->FirstChildElement("key") ;	
	TiXmlElement* node = topFramesDict->FirstChildElement("dict") ;
	String prefix="";
	String number="";

	std::list<String> listOfIds ;

	int id_position = 0 ;
	for( ;; ) {
		if( node==0 ) break ;

		String name = nel->GetText() ;
		// skip key, handle dict
		TiXmlElement* el = node->FirstChildElement("integer");
		String x = el->GetText() ;
		el = el->NextSiblingElement("integer") ;
		String y = el->GetText() ;
		el = el->NextSiblingElement("integer") ;
		String w = el->GetText() ;
		el = el->NextSiblingElement("integer") ;
		String h = el->GetText() ;
		el = node->FirstChildElement("real");
		String offsetX = el->GetText() ;
		el = el->NextSiblingElement("real") ;
		String offsetY = el->GetText() ;

		JAM_TRACE( ("Grabbing [%s]: [%s]",sheetFilename.c_str(),name.c_str()) );
		DrawItem* item = grab(pTexture,stoi(x),stoi(y),stoi(w),stoi(h),0,0,name) ;
		item->setOffsetX(stof(offsetX)) ;
		item->setOffsetY(stof(offsetY)) ;
		listOfIds.push_back(item->getName()) ;
		id_position++ ;

		nel = nel->NextSiblingElement("key") ;
		node = node->NextSiblingElement("dict") ;
	}

	m_drawItemsPerSheet[sheetFilename] = listOfIds ;
	return id_position ;
}

// ************************************************************************
// *** Load Zwoptex XML and create Animation
// *** You must load frames in advance using loadXmlSheet()
// ************************************************************************
int DrawItemManager::loadXmlAnimationSheet( const String& xmlFilePath, const String& sheetFilename, const String& grpName, float timing )
{

	TiXmlDocument xmlDoc;
	if( !xmlDoc.LoadFile(xmlFilePath.c_str()) ) {
		JAM_ERROR( "Error loading file: %s", xmlFilePath.c_str() ) ;
	}
	TiXmlElement* xmlRoot = xmlDoc.RootElement() ;
	TiXmlElement* topDictElem = xmlRoot->FirstChildElement("dict") ;
	TiXmlElement* topFramesDict = topDictElem->FirstChildElement("dict")->NextSiblingElement("dict") ;

	TiXmlElement* nel = topFramesDict->FirstChildElement("key") ;	
	TiXmlElement* node = topFramesDict->FirstChildElement("dict") ;

	std::map<String,int> animMap;
	std::list<String> listOfIds ;
	jam::Animation2D* pAnim = 0 ;			
	String lastPrefix="";
	String prefix="";
	int id_position=0;
	int number=0;		
	int total=0;
	int idanim=0;
		
	for( ;; ) {
		if( node==0 ) break ;
		String name = nel->GetText() ;		// frame name
		prefix=findPrefix(prefix,name,number);
		
		if( prefix != lastPrefix ) { // new animation found in plist xml file
			pAnim =	Animation2D::create();
			JAM_TRACE( "Create Animation [%s][anim:%s]",sheetFilename.c_str(),prefix.c_str() );
			pAnim->setLoop(true);
			pAnim->setName(prefix) ;
			GetAnim2DMgr().addObject(pAnim);
			lastPrefix=prefix;					
			idanim=0;
			animMap[prefix]=0;
			listOfIds.push_back( pAnim->getName() ) ;
		}				
		total++;
		JAM_TRACE( "+--->Add [anim:%s] = [%s] from pos[:%d / %d]",prefix.c_str(),  name.c_str(),id_position, total );
		pAnim->addFrame( jam::GetDrawItemMgr().getObject(name), timing ) ;
		animMap[prefix]=++idanim;	// count the number of frames for animation "prefix"
		id_position++;
		nel = nel->NextSiblingElement("key") ;
		node = node->NextSiblingElement("dict") ;
	}

	JAM_TRACE( "Imported in [%s] with [%zu] Animations.",sheetFilename.c_str(),animMap.size() );	
	std::map<String, int>::iterator iter ;
	for(iter = animMap.begin(); iter != animMap.end(); iter++) {
		JAM_TRACE( "Animation [%s] of [%d] frames", (iter->first).c_str(),(iter->second) );	
	}

	m_animationsPerSheet[sheetFilename] = listOfIds ;

	return id_position;
}

// ************************************************************************
// *** LOAD XML from TexturePacker file format (no animations)
// ************************************************************************
int DrawItemManager::loadXmlPackSheet( const String& xmlFilePath, const String& sheetName, const String& grpName, float gfxScale /*= 1.0f*/ )
{
	Texture2D* pTexture = new Texture2D() ;
	pTexture->load(sheetName);
	pTexture->setName(sheetName.c_str()) ;
	printf( "> Loading file: %s\n", xmlFilePath.c_str() ) ;
	TiXmlDocument xmlDoc;
	if( !xmlDoc.LoadFile(xmlFilePath.c_str()) ) {
		JAM_ERROR( "Error loading file: %s", xmlFilePath.c_str() ) ;
	}
	TiXmlElement* xmlRoot = xmlDoc.RootElement() ;
	TiXmlElement* topDictElem = xmlRoot->FirstChildElement("dict") ;
		
	TiXmlElement* node = topDictElem->FirstChildElement("dict")->FirstChildElement("dict") ;
	TiXmlElement* nel = topDictElem->FirstChildElement("dict")->FirstChildElement("key") ;	

	std::list<String> listOfIds ;
	int x=0,y=0,w=0,h=0;
	float offsetX=0,offsetY=0;

	int id_position = 0 ;

	for( ;; ) {
		if( node==0 ) break ;

		String name = nel->GetText() ;
		// skip key, handle dict
		TiXmlElement* nextkey=node->FirstChildElement("key");	
		if( strcmp(node->FirstChildElement("key")->GetText(),"frame")==0 ) 
		{
			std::string rect = node->FirstChildElement("string")->GetText() ;
			while (stringReplace(rect,"{",""));
			while (stringReplace(rect,"}",""));

			std::vector<std::string> rectInfo;
			splitString(rect, ",",rectInfo, false,false,true) ;
			x=atoi(rectInfo[0].c_str());  y=atoi(rectInfo[1].c_str()); 
			w=atoi(rectInfo[2].c_str()); h=atoi(rectInfo[3].c_str()); 
		}
		nextkey= nextkey->NextSiblingElement("key");						
		if (strcmp(nextkey->GetText(),"offset")==0) 
		{
			std::string rect = nextkey->NextSiblingElement("string")->GetText() ;
			while (stringReplace(rect,"{",""));
			while (stringReplace(rect,"}",""));

			std::vector<std::string> rectInfo;
			splitString(rect, ",",rectInfo, false,false,true) ;
			offsetX=(float)atof(rectInfo[0].c_str());  offsetY=(float)atof(rectInfo[1].c_str()); 
				
		}
		JAM_TRACE( "> import : [%s] (from %s)\n", name.c_str(), sheetName.c_str() ) ;	
		JAM_TRACE( "Grabbing [%s]: [%d][%s]",sheetName.c_str(),id_position, name.c_str() );
		DrawItem* item = grab(pTexture,x,y,w,h,0,0,name,gfxScale) ;
		item->setOffsetX(offsetX) ;
		item->setOffsetY(offsetY) ;

		listOfIds.push_back(item->getName()) ;
			
		nel = nel->NextSiblingElement("key") ;
		node = node->NextSiblingElement("dict") ;
	}

	m_drawItemsPerSheet[sheetName] = listOfIds ;
	return id_position ;
}

// ************************************************************************
// *** Load TexturePacker XML and create Animation
// ************************************************************************
int DrawItemManager::importXmlPackAnimationSheet( const String& xmlFilePath, const String& sheetFilename, const String& grpName, float timing )
{
	TiXmlDocument xmlDoc;
	if( !xmlDoc.LoadFile(xmlFilePath.c_str()) ) {
		JAM_ERROR( "Error loading file: %s", xmlFilePath.c_str() ) ;
	}
	TiXmlElement* xmlRoot = xmlDoc.RootElement() ;
	TiXmlElement* topDictElem = xmlRoot->FirstChildElement("dict") ;
	TiXmlElement* topFramesDict = topDictElem->FirstChildElement("dict")->NextSiblingElement("dict") ;

	TiXmlElement* node = topDictElem->FirstChildElement("dict")->FirstChildElement("dict") ;
	TiXmlElement* nel = topDictElem->FirstChildElement("dict")->FirstChildElement("key") ;	

	std::map<String,int> animMap;	animMap.clear();
	std::list<String> listOfIds ;
	jam::Animation2D* pAnim = 0 ;			
	String lastPrefix="";
	String prefix="";
	int id_position=0;
	int number=0;		
	int total=0;
	int idanim=0;
		
	for( ;; ) {
		if( node==0 ) break ;
			
		String name = nel->GetText() ;
		prefix=findPrefix(prefix,name,number);
		if (prefix != lastPrefix)	// new animation found
		{
			pAnim =	Animation2D::create() ;
			JAM_TRACE( "Create Animation [%s][anim:%s]",sheetFilename.c_str(),prefix.c_str() );
			pAnim->setLoop(true);
			pAnim->setName(prefix);

			jam::GetAnim2DMgr().addObject(pAnim);
			lastPrefix=prefix;					
			idanim=0;
			animMap[prefix]=0;
			listOfIds.push_back( pAnim->getName() ) ;
		}				
		total++;
		JAM_TRACE( "+--->Add [anim:%s] = [%s] from pos[:%d / %d]",prefix.c_str(),  name.c_str(),id_position, total );

		pAnim->addFrame( jam::GetDrawItemMgr().getObject(name), timing ) ;
		animMap[prefix]=++idanim;
		id_position++;
		nel = nel->NextSiblingElement("key") ;
		node = node->NextSiblingElement("dict") ;
	}

	JAM_TRACE( "Imported in [%s] with [%zu] Animations.",sheetFilename.c_str(),animMap.size() );	
	std::map<String, int>::iterator iter ;
	for(iter = animMap.begin(); iter != animMap.end(); iter++)
	{
		JAM_TRACE( "Animation [%s] of [%d] frames", (iter->first).c_str(), (iter->second) );	
	}

	m_animationsPerSheet[sheetFilename] = listOfIds ;

	return  id_position;
}

DrawItem* DrawItemManager::grab( Texture2D* pTexture, int _x0, int _y0, int xsize, int ysize, int offsetX/*=0*/,int offsetY/*=0*/,
								const String& name/*=""*/, float gfxScale /*= 1.0f*/ )
{
	DrawItem* cutted = DrawItem::create(pTexture,
		_x0+offsetX,
		_y0+offsetY,
		_x0+xsize+offsetX,
		_y0+ysize+offsetY, gfxScale);

	if( !name.empty() ) {
		cutted->setName(name) ;
	}

	addObject(cutted) ;

	return cutted;
}

//
// This method destroys all draw items associated with the given sheet (by sheet name)
// Also it destroys all the animations defined in the given sheet
//
void DrawItemManager::destroySheet( const String& name)
{
	// destroy DrawItems
	auto it = m_drawItemsPerSheet.find(name);
	if( it != m_drawItemsPerSheet.end() ) {
		list<String>& listOfIds = it->second ; 
		for( auto idsIt = listOfIds.begin(); idsIt!=listOfIds.end(); idsIt++ ) {
			eraseObject(*idsIt) ;
		}
		m_drawItemsPerSheet.erase(it) ;
	}

	// Destroy Animations
	it = m_animationsPerSheet.find(name);
	if( it != m_animationsPerSheet.end() ) {
		list<String>& listOfIds = it->second ; 
		for( auto idsIt = listOfIds.begin(); idsIt!=listOfIds.end(); idsIt++ ) {
			GetAnim2DMgr().eraseObject(*idsIt) ;
		}
		m_animationsPerSheet.erase(it) ;
	}
}

int DrawItemManager::deleteSheetCuts( int cols, int rows, const String& aName )
{
	int nextId=0;
	for (int r=0; r<rows; r++)
	{
		for (int c=0; c<cols; c++)
		{
			String  cutName = aName + "_" + to_string(++nextId) ;
			eraseObject(cutName);
			JAM_TRACE( "Remove cut %s", cutName.c_str() );
		}
	}
	return nextId;
}

// ************************************************************************
// *** Find a prefix from string
// ************************************************************************
String DrawItemManager::findPrefix( const String& prefix, const String& str, int& number )
{
	String retVal = prefix ;
	size_t pos = str.find(prefix);    // position of "prefix" in str
	if (pos==String::npos || prefix.empty() || pos!=0) {
		pos = str.rfind('_');    // position of "prefix" in str
		if (pos==String::npos) pos = str.rfind('-');    // position of "prefix" in str
		if (pos==String::npos) return String();  /// *** ERROR?
		retVal=str.substr(0,pos);   // get from "prefix" to the end
	}
	else {
		pos+=prefix.length();
	}

	number = stoi(str.substr(pos+1));   // get from "prefix" to the end		
	return retVal;
}

// ************************************************************************
int DrawItemManager::splitString( const std::string& input, const std::string& delimiter, std::vector<std::string>& results, 
	bool includeEmpties, bool includeLast, bool trimResults )
{
	int iPos = 0;
	int newPos = -1;
	int sizeS2 = (int)delimiter.size();
	int isize = (int)input.size();

	if( ( isize == 0 )||	( sizeS2 == 0 ))	{ return 0; }
	if (input.empty()) 	{ return 0; }
	std::vector<int> positions;

	newPos = (int)input.find (delimiter, 0);
	int numFound = 0;

	if( newPos < 0 )	// *** Only 1 element
	{ 
		numFound++;	positions.push_back((int)input.size()-1); 
		goto SPLIT_PHASE2;
	}

	while( newPos >= iPos )
	{
		numFound++;
		positions.push_back(newPos);
		iPos = newPos;
		newPos = (int)input.find (delimiter, iPos+sizeS2);
	}
	if (includeLast)	{ positions.push_back((int)input.length()); numFound++;	}
	if( numFound == 0 )	{ return 0;}

SPLIT_PHASE2:
	for( size_t i=0; i <= positions.size(); ++i )
	{
		std::string s("");
		if( i == 0 ) { s = input.substr( i, positions[i] ); }
		int offset=0;
		if (i>0)	{ offset	= positions[i-1] + sizeS2; }
		if( offset < isize )
		{
			if( i == positions.size() ){ s = input.substr(offset);}
			else if( i > 0 ) { s = input.substr( positions[i-1] + sizeS2, positions[i] - positions[i-1] - sizeS2 );	}
		}

		std::string res= trimResults ? jam::trim(s) : s;
		if( includeEmpties || ( s.size() > 0 ) ) { results.push_back(res);}
	}
	return numFound;
}


//Replaces all the instances of search with replace in string; returns the number of substitutions done
unsigned int DrawItemManager::stringReplace(std::string& str,const std::string& search,const std::string& replace)
{
	unsigned int ret=0;
	for(std::string::size_type pos=str.find(search);pos!=str.npos;ret++,pos=str.find(search,++pos))
	{
		if(search.length()>replace.length())
			str.erase(pos,search.length()-replace.length());
		if(search.length()<replace.length())
			str.insert(pos,replace.length()-search.length(),' ');
		str.replace(pos,replace.length(),replace);
	}
	return ret;
}

}

