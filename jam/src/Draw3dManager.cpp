/**********************************************************************************
* 
* Draw3dManager.cpp
* 
* This file is part of Jam
* 
* Copyright (c) 2014-2019 Giovanni Zito, Gianluca Sclano.
* Copyright (c) 2014-2019 Jam contributors (cf. AUTHORS.md)
*
* Based on Draw3D library for Blitz3D written by hectic
* Draw3D online help at: http://www.blitzforum.de/help/?cat=8
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

#include "jam/Draw3dManager.h"
#include "jam/Gfx.h"
#include "jam/core/math.h"
#include "jam/InputManager.h"
#include "jam/Application.h"
#include "jam/DrawItemManager.h"
#include "jam/DeviceManager.h"
#include "jam/Pivot2d.h"
#include "jam/StridedVertexBuffer.h"

#include <cmath>
#include <algorithm>

//Rect-Drawing-Art-Constant (Less Than 0.25)
#define DRAWRECTOFFSET		0.0
#define DRAWFONTUVSTEP		(1.0f/16.0f)
#define DRAW3DTEXT_TOLLERANCE	0

using namespace std ;

namespace jam
{

Draw3DManager::TFontsTilesMap*	Draw3DManager::m_pFontsSizeMap = nullptr ;

int Draw3DManager::MouseDown3D = 0;
int Draw3DManager::MouseOver3D = 0;
int Draw3DManager::MouseHit3D = 0;
int Draw3DManager::MousePit3D = 0;
int Draw3DManager::MouseUp3D = 0;
int Draw3DManager::MouseX3D = 0;
int Draw3DManager::MouseY3D = 0;
int Draw3DManager::MouseSpeedX3D = 0;
int Draw3DManager::MouseSpeedY3D = 0;
int Draw3DManager::LastMouseX3D = 0;
int Draw3DManager::LastMouseY3D = 0;

char Draw3DManager::m_mouseText3D[255] = {0};

int  Draw3DManager::GDrawXSize = 0 ; //X-Scaling factor for MouseX3D
int  Draw3DManager::GDrawYSize = 0 ; //Y-Scaling factor for MouseY3D
int  Draw3DManager::GDrawMouseHit = 0 ; //For Own MouseHit Calculation
char Draw3DManager::m_drawMousePit[255] = { 0 }; //For Own MousePit Calculation

float Draw3DManager::VPWidth = 0;
float Draw3DManager::VPHeight = 0 ;

float Draw3DManager::HalfVPWidth = 0;
float Draw3DManager::HalfVPHeight = 0 ;

float Draw3DManager::RatioX = 1.0f ;
float Draw3DManager::RatioY = 1.0f ;

bool Draw3DManager::UseRatioScale = true ;

int Draw3DManager::Original3DWidth = 0;
int Draw3DManager::Original3DHeight = 0 ;

int Draw3DManager::HalfOriginal3DWidth = 0 ;
int Draw3DManager::HalfOriginal3DHeight = 0 ;

float Draw3DManager::ScaledHalfVPWidth = 0.0f ;
float Draw3DManager::ScaledHalfVPHeight = 0.0f ;

float Draw3DManager::ScaledVPWidth = 0.0f ;
float Draw3DManager::ScaledVPHeight = 0.0f ;

Color Draw3DManager::ColorT3D = Color(255,255,255,255) ;
Color Draw3DManager::ColorN3D = Color(255,255,255,255) ;
Color Draw3DManager::ColorG3D = Color(255,255,255,255) ;

bool Draw3DManager::MouseDown3DEventDetected = false ;
bool Draw3DManager::MouseOver3DEventDetected = false ;

float  Draw3DManager::GDrawSFont = 1.0f;
float  Draw3DManager::GDrawHFont = 1.0f;
float  Draw3DManager::GDrawPFont = 0.0f;
float  Draw3DManager::GDrawIFont = 0.0f;

Draw3DManager::Draw3DManager() :
	m_draw3DTextTollerance(DRAW3DTEXT_TOLLERANCE)
{
	// static instance
	m_pFontsSizeMap = new TFontsTilesMap();
}

Draw3DManager::~Draw3DManager()
{
	JAM_DELETE( m_pFontsSizeMap ) ;
}

void Draw3DManager::CheckQuad3D(float FDrawX1,float FDrawY1,float FDrawX2,float FDrawY2,float FDrawX3,float FDrawY3,float FDrawX4,float FDrawY4,int FDrawButton,const String& FDrawOver)
{
/*
	// Variables precalculation
	int isPressed=MouseDown3D;
	MouseOver3D=0;
	MouseDown3D=0;
	MouseHit3D=0;
	MousePit3D=0;
	MouseUp3D=0;

	// Quad collision calculation
	// Metodo dell'equazione della retta passante per 2 punti
	// See solution 3 at http://paulbourke.net/geometry/insidepoly/
	if ((FDrawY2-FDrawY1)*(MouseX3D-FDrawX2)-(FDrawX2-FDrawX1)*(MouseY3D-FDrawY2)>=0){
		if ((FDrawY3-FDrawY2)*(MouseX3D-FDrawX3)-(FDrawX3-FDrawX2)*(MouseY3D-FDrawY3)>=0){
			if ((FDrawY4-FDrawY3)*(MouseX3D-FDrawX4)-(FDrawX4-FDrawX3)*(MouseY3D-FDrawY4)>=0){
				if ((FDrawY1-FDrawY4)*(MouseX3D-FDrawX1)-(FDrawX1-FDrawX4)*(MouseY3D-FDrawY1)>=0) {
					// Global mouse variable calculation
					s3ePointerState pState = GetInputMgr().getPointerState(S3E_POINTER_BUTTON_SELECT) ;
					MouseDown3D = (pState & S3E_POINTER_STATE_DOWN) || (pState & S3E_POINTER_STATE_PRESSED);
					if( GDrawMouseHit == 0 ) MouseHit3D=MouseDown3D;
					if( GDrawMousePit() != FDrawOver ) MousePit3D=1;
					MouseUp3D = isPressed;
					GDrawMouseHit = MouseDown3D;
					GDrawMousePit( FDrawOver );
					MouseText3D( FDrawOver );
					MouseOver3D = 1;
					MouseOver3DEventDetected = true ;
					if (MouseDown3D)
						MouseDown3DEventDetected |= (MouseDown3D!=0);
				}
			}
		}
	}
*/
}

void Draw3DManager::Clear3D()
{
	if (MouseText3D()=="") GDrawMousePit(""); else MouseText3D("");

	LastMouseX3D=MouseX3D;
	LastMouseY3D=MouseY3D;

	MouseX3D = (int)GetInputMgr().getPointerX() ;
	MouseY3D = (int)GetInputMgr().getPointerY() ;

	MouseSpeedX3D = MouseX3D - LastMouseX3D ;
	MouseSpeedY3D = MouseY3D - LastMouseY3D;

	MouseDown3DEventDetected = false ;
	MouseOver3DEventDetected = false ;
}

void Draw3DManager::Origin3D(int w, int h)
{
	if( (Original3DWidth != w && w!=-1)|| (Original3DHeight != h && h!=-1) )
	{
		Original3DWidth = w;
		Original3DHeight = h;
		HalfOriginal3DWidth = w / 2 ;
		HalfOriginal3DHeight = h / 2 ;

		VPWidth = (float)GetDeviceMgr().getNativeDisplayWidth() ;
		VPHeight = (float)GetDeviceMgr().getNativeDisplayHeight() ;
		HalfVPWidth  = VPWidth / 2.0f ;
		HalfVPHeight = VPHeight / 2.0f ;

		float scale = 0, hPos = 0 , vPos = 0 ;
		GetDeviceMgr().calculateLetterBox(w,h,scale,hPos,vPos) ;
		RatioX = RatioY = scale ;

		// Half screen scaled
		ScaledHalfVPWidth = HalfVPWidth / RatioX;
		ScaledHalfVPHeight = HalfVPHeight / RatioY;

		ScaledVPWidth = ScaledHalfVPWidth*2;
		ScaledVPHeight =ScaledHalfVPHeight*2;
	}
}

int Draw3DManager::getAscAtPos( const String& FDrastring,size_t* IDrawLoop, bool* isComplex )
{
	int pos = (*IDrawLoop);
	*isComplex=false;
	if (FDrastring[pos]==60)
	{
		int endpos = pos+1;
		if ( endpos<(int)FDrastring.size() &&  FDrastring[endpos]==35) //if (FDrastring.substr(*IDrawLoop,1)=="#" )
		{	
			endpos = pos+8;
			if ( endpos<(int)FDrastring.size() && FDrastring[endpos]==62) //if (FDrastring.substr(*IDrawLoop+4,1)==">" ) 	*IDrawLoop+=5;
			{
				(*IDrawLoop)+=9;	*isComplex=true;return ((*IDrawLoop)<(int)FDrastring.size()) ? FDrastring[(*IDrawLoop)-0] : 0 ;
			}
			else
			{
				endpos = pos+10;
				if ( endpos<(int)FDrastring.size() && FDrastring[endpos]==62) //if ( FDrastring.substr(*IDrawLoop+5,1)==">")	*IDrawLoop+=6;
				{
					(*IDrawLoop)+=11;*isComplex=true;return ((*IDrawLoop)<(int)FDrastring.size()) ? FDrastring[(*IDrawLoop)-0] : 0;
				}
			}
		}
		// 			if ( FDrastring[pos+1]==37) //if (FDrastring.substr(*IDrawLoop,1)=="%" )	// *** <%1.0503>
		// 			{
		// 				size_t  found= FDrastring.find(37,pos+2)
		// 				if (found!=string::npos)
		// 				//if (FDrastring[pos+8]==62) //if (FDrastring.substr(*IDrawLoop+4,1)==">" ) 	*IDrawLoop+=5;
		// 				{(*IDrawLoop)+=(found+1);	return ((*IDrawLoop)<(int)FDrastring.size()) ? FDrastring[(*IDrawLoop)-0] : 0 ;}
		// 			}
		else if (FDrastring.substr((pos+1),3)=="/#>")//;<ENDE-TAG>ÜBERSPRINGEN
		{(*IDrawLoop) +=4;*isComplex=true;return ((*IDrawLoop)<(int)FDrastring.size()) ? FDrastring[(*IDrawLoop)-0] : 0;}
		else if (FDrastring.substr(pos+1,2)=="P>")//;<ENDE-TAG>ÜBERSPRINGEN
		{(*IDrawLoop) +=3;*isComplex=true;return ((*IDrawLoop)<(int)FDrastring.size()) ? FDrastring[(*IDrawLoop)-0] : 0;}
	}
	//int i=FDrastring[pos];
	return FDrastring[pos] ;
}


int Draw3DManager::Text3D(
	const String& drawItemName,
	float FDrawX, float FDrawY,
	const String& FDrastring,
	float FDrawAlign,
	int FDrawButton,
	float FDrawAngle,
	float FKerningWidth,
	float FZoom,
	float FZoomY,
	bool fastParse/*=true*/, float FKerningHeight/*=1.0f*/)
{
	if (drawItemName.empty()) return 0;

	DrawItem* handle = GetDrawItemMgr().getObject(drawItemName) ;

	return Text3D(handle,
		FDrawX, FDrawY,
		FDrastring,
		FDrawAlign,
		FDrawButton,
		FDrawAngle,
		FKerningWidth,
		FZoom,
		FZoomY,
		fastParse, FKerningHeight);
}

// *************************************************************************

int Draw3DManager::Text3D(
	Texture2D* pTexture,
	float FDrawX, float FDrawY,
	const String& FDrastring,
	float FDrawAlign,
	int FDrawButton,
	float FDrawAngle,
	float FKerningWidth,
	float FZoom, float fZoomY,
	bool fastParse/*=true*/, float FKerningHeight/*=1.0f*/)
{
	DrawItem* handle = DrawItem::create(pTexture) ;

	// questo è il default, pertanto è inutile specificarlo
	//handle->setBlendingMode(Material::ALPHA_BLEND) ;

	int iRetCode = Text3D(handle,
		FDrawX, FDrawY,
		FDrastring,
		FDrawAlign,
		FDrawButton,
		FDrawAngle,
		FKerningWidth,
		FZoom,
		fZoomY,
		fastParse, FKerningHeight);

	return iRetCode ;
}

//	************************************************************************

int Draw3DManager::Text3D( DrawItem* handle,
	float FDrawX, float FDrawY,
	const String& FDrastring,
	float FDrawAlign/*=0.0f*/,
	int FDrawButton/*=0*/,
	float FDrawAngle/*=0.0f*/,
	float FKerningWidth/*=0.0f*/,
	float FZoom/*=1.0f*/ ,
	float fZoomY/*=0*/,
	bool fastParse/*=true*/, 
	float FKerningHeight/*=1.0f*/)
{
#ifndef JAM_TEXT3D_DISABLED
	if (FDrastring.empty()) return 0;

	StridedVertexBuffer& LDrawFace = GetGfx().getVertexBuffer(handle,FDrastring.size()*4,FDrastring.size()*6) ;
	Texture2D* pTexture = handle->getTexture();
	int LDrawSize = pTexture->getWidth() / 16 ;
	std::vector<std::pair<int,int> >& sizes = (*m_pFontsSizeMap)[pTexture] ;
	if (!fZoomY) fZoomY=FZoom ;

	size_t IDrawLen		= FDrastring.length() ;
	float IDrawXVector	= cosf(ToRadian(-FDrawAngle)) * FZoom ;
	float IDrawYVector	= sinf(ToRadian(-FDrawAngle)) * fZoomY ;
	float IDrawXI		= IDrawXVector * GDrawIFont * GDrawHFont ;
	float IDrawYI		= IDrawYVector * GDrawIFont * GDrawHFont ;
	float fSize1		= IDrawYVector*GDrawHFont*0.5f*LDrawSize ;
	float fSize2		= IDrawXVector*GDrawHFont*0.5f*LDrawSize ;

	float IDrawX1Pos	= FDrawX - fSize1 + IDrawXI ;
	float IDrawY1Pos	= FDrawY + fSize2 + IDrawYI ;
	float IDrawX4Pos	= FDrawX + fSize1 - IDrawXI ;
	float IDrawY4Pos	= FDrawY - fSize2 - IDrawYI ;
	float IDrawX2Pos	= 0 ;
	float IDrawY2Pos	= 0 ;
	float IDrawX3Pos	= 0 ;
	float IDrawY3Pos	= 0 ;
	size_t IDrawLoop;

	float IDrawU1Map	= 0.0f;
	float IDrawV1Map	= 0.0f;
	float IDrawU2Map	= 0.0f;
	float IDrawV2Map	= 0.0f;

	float fStepX		= GDrawPFont*IDrawXVector;
	float fStepY		= GDrawPFont*IDrawYVector;

	uint16_t IDrawV0 = 0;
	uint16_t IDrawV1 = 0;
	uint16_t IDrawV2 = 0;
	uint16_t IDrawV3 = 0;

	//String GGF-alignment
	int IDrawAsc=0;
	std::pair<int,int> p ;
	bool isComplex=false;

	//************************************************************************
	// CHECK ALIGN
	//************************************************************************

	if (FDrawAlign != 0)
	{
		for( IDrawLoop=0; IDrawLoop < IDrawLen; IDrawLoop++ ) {
			isComplex=false;
			IDrawAsc= getAscAtPos(FDrastring,&IDrawLoop,&isComplex);	// Calcola il carattere tenendo conto anche della formattazione
			p = sizes[IDrawAsc] ;
			int LDrawWidth = p.second - p.first + 1 + pTexture->getWidth()/512;
			IDrawX1Pos = IDrawX1Pos - (LDrawWidth*IDrawXVector + fStepX)*FDrawAlign*0.5f;
			IDrawY1Pos = IDrawY1Pos - (LDrawWidth*IDrawYVector + fStepY)*FDrawAlign*0.5f;
			IDrawX4Pos = IDrawX4Pos - (LDrawWidth*IDrawXVector + fStepX)*FDrawAlign*0.5f;
			IDrawY4Pos = IDrawY4Pos - (LDrawWidth*IDrawYVector + fStepY)*FDrawAlign*0.5f;
		}

		if( (int)FDrawAngle == 0 ) {
			IDrawX1Pos = floorf(IDrawX1Pos);
			IDrawY1Pos = floorf(IDrawY1Pos);
			IDrawX4Pos = floorf(IDrawX4Pos);
			IDrawY4Pos = floorf(IDrawY4Pos);
		}
	}
	float IDrawX1Temp = IDrawX1Pos;
	float IDrawY1Temp = IDrawY1Pos;
	float IDrawX4Temp = IDrawX4Pos;
	float IDrawY4Temp = IDrawY4Pos;

	uint32_t diffuse = Draw3DManager::ColorT3D.getRgba();
	uint32_t origDiffuse = diffuse;
	jam::Color  basecolor;
	String mid ;

	// Start edge/save
	float IDrawX1Sav=IDrawX1Pos;
	float IDrawY1Sav=IDrawY1Pos;
	float IDrawX4Sav=IDrawX4Pos;
	float IDrawY4Sav=IDrawY4Pos;

	int IDrawSpace=0;

	//************************************************************************
	// Main Loop
	//*** Inizia il loop di tutti i caratteri
	//************************************************************************
	for( IDrawLoop=1; IDrawLoop<=IDrawLen; IDrawLoop++ )
	{
		IDrawAsc = FDrastring[IDrawLoop-1];
		//************************************************************************
		// CHECK FORMAT
		//************************************************************************
		if (!fastParse)	// se c'è formattazione
		{
			if ((IDrawAsc==60)) //find character '<' then Start Parsing
			{
				if (FDrastring.substr(IDrawLoop,2)=="P>")//;<END-TAG>
				{
					// FOUND return;
					IDrawSpace=0;//Int(Mid(FDrawString,IDrawCount+2,3));
					float dy=-(1.0f+((FKerningHeight)*0.5f*LDrawSize));//(IDrawY3Pos+fStepY*/);

					IDrawX1Pos=IDrawX1Sav+IDrawXVector*IDrawSpace;
					IDrawY1Pos=IDrawY1Sav+IDrawYVector*IDrawSpace+dy;
					IDrawX4Pos=IDrawX4Sav+IDrawXVector*IDrawSpace;
					IDrawY4Pos=IDrawY4Sav+IDrawYVector*IDrawSpace+dy;

					IDrawX1Sav=IDrawX1Pos;
					IDrawY1Sav=IDrawY1Pos;
					IDrawX4Sav=IDrawX4Pos;
					IDrawY4Sav=IDrawY4Pos;

					IDrawLoop+=2;
					continue;
					//IDrawMode=1
				}
				else if ( FDrastring[IDrawLoop]==35) // second character "#" means the color modification
				{
					//*** Color control
					if (FDrastring[IDrawLoop+7]==62) // find last character ">" of rgb hex string  (eg:  <#F0f0ff>)
					{
						Color::hextoRgb(FDrastring.substr(IDrawLoop+1,6),basecolor);
						diffuse = basecolor.getRgba();
						IDrawLoop+=8;
						continue;
					}
					//*** Color control with alpha
					else if (FDrastring[IDrawLoop+9]==62)  // find last character ">" of alfa rgb hex (eg:  <#F0f0ffAA>)
					{
						Color::hextoRgba(FDrastring.substr(IDrawLoop+1,8),basecolor);
						diffuse = basecolor.getRgba();
						IDrawLoop+=10;
						continue;
					}
				}
				//*** End Tag
				else if (FDrastring.substr(IDrawLoop,3)=="/#>")//;<END-TAG> SKIP	if </#>
				{
					IDrawLoop +=3;
					diffuse=	origDiffuse;	// Reset the original color
					continue;
				}
			}
		}
		//************************************************************************

		// Continue without parsing
		p = sizes[IDrawAsc] ;
		int LDrawWidth = p.second - p.first + 1 + pTexture->getWidth()/512;
		float m1=IDrawXVector*LDrawWidth;
		float m2=IDrawYVector*LDrawWidth;
		IDrawX2Pos = IDrawX1Pos + m1;
		IDrawY2Pos = IDrawY1Pos + m2;
		IDrawX3Pos = IDrawX4Pos + m1;
		IDrawY3Pos = IDrawY4Pos + m2;

		IDrawU1Map = p.first/(float)pTexture->getWidth();
		IDrawV1Map = 1.0f - (floorf(IDrawAsc/16.0f)*DRAWFONTUVSTEP);
		IDrawU2Map = p.second/(float)pTexture->getWidth();
		IDrawV2Map = IDrawV1Map-DRAWFONTUVSTEP;

		// add vertices in clockwise order
		IDrawV0 = LDrawFace.addVertex(IDrawX1Pos,IDrawY1Pos, diffuse, IDrawU1Map, IDrawV1Map) ;
		IDrawV1 = LDrawFace.addVertex(IDrawX2Pos,IDrawY2Pos, diffuse, IDrawU2Map, IDrawV1Map) ;
		IDrawV2 = LDrawFace.addVertex(IDrawX3Pos,IDrawY3Pos, diffuse, IDrawU2Map, IDrawV2Map) ;
		IDrawV3 = LDrawFace.addVertex(IDrawX4Pos,IDrawY4Pos, diffuse, IDrawU1Map, IDrawV2Map) ;
		LDrawFace.addQuadIndices(IDrawV0,IDrawV1,IDrawV2,IDrawV3) ;

		IDrawX1Pos=IDrawX2Pos+fStepX;
		IDrawY1Pos=IDrawY2Pos+fStepY;
		IDrawX4Pos=IDrawX3Pos+fStepX;
		IDrawY4Pos=IDrawY3Pos+fStepY;
	}

	GetGfx().appendOrDraw() ;

	if (FDrawButton != 0)
	{
		CheckQuad3D(IDrawX1Temp,IDrawY1Temp,IDrawX2Pos,IDrawY2Pos,IDrawX3Pos,IDrawY3Pos,IDrawX4Temp,IDrawY4Temp,FDrawButton,FDrastring);
	}
#endif
	return 0;
}

//************************************************************************
int Draw3DManager::StringWidth3D(const String& drawItemName,const String& FDrastring)
{
	Texture2D* pTexture = GetDrawItemMgr().getObject(drawItemName)->getTexture() ;
	return StringWidth3D(pTexture,FDrastring) ;
}

int Draw3DManager::StringWidth3D(Texture2D* pTexture ,const String& FDrastring)
{
	std::vector<std::pair<int,int> > sizes = (*m_pFontsSizeMap)[pTexture] ;

	size_t IDrawLen =FDrastring.length();
	float IDrawWidth=0;
	size_t IDrawLoop=0 ;
	int IDrawAsc=0;
	for (IDrawLoop=0;IDrawLoop<IDrawLen;IDrawLoop++){
		//int IDrawAsc=FDrastring[IDrawLoop];
		bool isComplex=false;
		IDrawAsc= getAscAtPos(FDrastring,&(IDrawLoop), &isComplex);
		if (IDrawLoop>=IDrawLen) continue;
		//IwDebugTraceLinePrintf("Parsing %c",IDrawAsc);
		int LDrawWidth = sizes[IDrawAsc].second - sizes[IDrawAsc].first + 1 + pTexture->getWidth()/512;
		IDrawWidth=IDrawWidth+LDrawWidth+Draw3DManager::GDrawPFont;
	}
	return (int)(IDrawWidth * Draw3DManager::GDrawSFont - Draw3DManager::GDrawPFont) ;
}

Texture2D* Draw3DManager::LoadFont3D( const String& FDrawFile, int charstart /*= 0 */, uint32_t tollerance/*=0*/, bool fixed/*=false*/, int width_min/*=0*/ )
{
	int IDrawXLoop =0;
	int IDrawYLoop =0;
	int IDrawValue =0;

	int oldTollerance = getDraw3DTextTollerance();
	setDraw3DTextTollerance(tollerance);
	Texture2D* t = GetDrawItemMgr().loadTextureFromFileSystem(FDrawFile)->getTexture() ;

	int IDrawXMain = t->getWidth();
	int IDrawYMain = t->getHeight();

	int IDrawXSize =IDrawXMain/16;
	int IDrawYSize =IDrawYMain/16;

	int charNum=charstart;
	// *** Si puo' ottimizzare scegliendo solo i font necessari

	std::vector<std::pair<int,int> >	sizes ;
	// se esiste su file lo carico
	sizes.resize(256);

	for (IDrawYLoop=0; IDrawYLoop < 16; IDrawYLoop++)
	{
		for (int IDrawXLoop=0; IDrawXLoop < 16; IDrawXLoop++)
		{
			int l = 0 ;	int r = 0 ;
			// find left & right border
			if (fixed)
			{
				l = IDrawXLoop*(IDrawXMain/16) ;
				r = (IDrawXLoop+1)*(IDrawXMain/16)-1 ; 
				sizes[charNum] = std::pair<int,int>(l,r) ;
			}
			else if( XDrawTT3D(t,IDrawXMain,IDrawYMain,IDrawXLoop,IDrawYLoop, l, r) )
			{
				if ((r-l)<width_min) {
					r = l + width_min;
				}	
				sizes[charNum] = std::pair<int,int>(l,r) ;
			}
			charNum++;
		}

	}

	(*m_pFontsSizeMap)[t] = sizes ;

	t->upload();
	setDraw3DTextTollerance(oldTollerance);
	return t ;
}

bool Draw3DManager::XDrawTT3D(Texture2D* FDrawHandle,int FDrawXT ,int FDrawYT ,int FDrawXP ,int FDrawYP, int& IDrawLSet, int& IDrawRSet )
{
	// FDrawXT, FDrawYT = Width, height della texture (sheet)
	// FDrawXP, FDrawYP = Posizione del tile nella matrice dello sheet
	uint32_t tollerance = getDraw3DTextTollerance();
	// Size in pixel di un tile
	int IDrawXSize =FDrawXT/16;
	int IDrawYSize =FDrawYT/16;

	// coordinate (top/left) del title relative alla texture
	int IDrawXTile =IDrawXSize*FDrawXP;
	int IDrawYTile =IDrawYSize*FDrawYP;

	int IDrawXLoop =0;
	int IDrawYLoop =0;
	uint32_t IDrawValue =0;
	int IDrawXPos =0;

	uint32_t* data = (uint32_t*)FDrawHandle->getData() ;
	uint32_t pitch = FDrawHandle->getWidth()/4;	// Skip between rows of image
	// Diviso 4 perchè la GetPitch è in bytes e sto indirizzando word32

	// Search left border of symbol
	IDrawLSet = 0 ;
	for (IDrawXLoop=0;IDrawXLoop<=IDrawXSize-1;IDrawXLoop++) {
		for (IDrawYLoop=0;IDrawYLoop<=IDrawYSize-1;IDrawYLoop++) {
			if( (data[((IDrawYTile+IDrawYLoop)*(pitch)) + (IDrawXTile+IDrawXLoop)] >> 24) >  tollerance ) IDrawLSet=IDrawXLoop;
		}
		if( IDrawLSet != 0 ) break ;
	}
	// Search right border of symbol
	IDrawRSet = 0 ;
	for (IDrawXLoop=IDrawXSize-1;IDrawXLoop>=0;IDrawXLoop--) {
		for (IDrawYLoop=0;IDrawYLoop<=IDrawYSize-1;IDrawYLoop++) {
			if( (data[((IDrawYTile+IDrawYLoop)*(pitch)) + (IDrawXTile+IDrawXLoop)] >> 24)  >  tollerance ) IDrawRSet=IDrawXLoop+1;
		}
		if( IDrawRSet != 0 ) break ;
	}

	IDrawLSet += IDrawXTile ;
	IDrawRSet += IDrawXTile ;

	int AscPos=1+(FDrawXP)+(FDrawYP*16);
	bool res=true;
	// fix space (32) null size
	if(IDrawLSet==IDrawRSet)
	{
		res=false;
		//if (AscPos==31)
		{		IDrawRSet+=8 ; res=true;}
	}
	return res;
}

void Draw3DManager::DrawImage3D(Texture2D* pTexture,
	jam::Rect* pSrcRect,
	float FDrawX, float FDrawY,
	int FDrawButton /*= 0*/,
	float FDrawAngle /*= 0.0f*/,	// angle is in degrees
	float FDrawScale /*= 1.0f*/,
	float LHotSpotX /*= 0.0f*/,
	float LHotSpotY /*= 0.0f*/,
	Color* color /*= 0*/,
	bool SwapU /*= false*/,
	bool SwapV /*= false*/ )
{
	DrawItem* item = DrawItem::create( pTexture, pSrcRect ) ;
	DrawImage3D( item, FDrawX, FDrawY, FDrawButton, FDrawAngle, FDrawScale, LHotSpotX, LHotSpotY, color, SwapU, SwapV ) ;
	item = nullptr;
}

void Draw3DManager::DrawImage3D(DrawItem* handle,
	float FDrawX, float FDrawY,
	int FDrawButton /*= 0*/,
	float FDrawAngle /*= 0.0f*/,
	float FDrawScale /*= 1.0f*/,
	float LHotSpotX /*= 0.0f*/,
	float LHotSpotY /*= 0.0f*/,
	Color* color /*= 0*/,
	bool SwapU /*= false*/,
	bool SwapV /*= false*/)
{
/*
	float LDrawU1Map = handle->getU1() ;
	float LDrawV1Map = handle->getV1() ;
	float LDrawU2Map = handle->getU2() ;
	float LDrawV2Map = handle->getV2() ;
	float LDrawXSize = handle->getHalfWidth() ;
	float LDrawYSize = handle->getHalfHeight() ;

	if( FDrawScale != 1.0f ) {
		LDrawXSize *= FDrawScale ;
		LDrawYSize *= FDrawScale ;
	}

	if( SwapU ) {
		std::swap<float>(LDrawU1Map,LDrawU2Map) ;
	}

	if( SwapV ) {
		std::swap<float>(LDrawV1Map,LDrawV2Map) ;
	}

	//Hotspot management
	FDrawX += -LHotSpotX + handle->getOffsetX();
	FDrawY += -LHotSpotY + handle->getOffsetY();

	float IDrawAngle = 0.0f;
	float IDrawRadius = 0.0f;
	float IDrawXPos1 = 0.0f;
	float IDrawYPos1 = 0.0f;
	float IDrawXPos2 = 0.0f;
	float IDrawYPos2 = 0.0f;

	if (FDrawAngle != 0.0f)
	{
		float FAngleDeg = ToRadian(FDrawAngle) ;
		IDrawAngle  = atan2f(LDrawYSize,LDrawXSize);
		IDrawRadius = Q_SQRT((LDrawXSize*LDrawXSize)+(LDrawYSize*LDrawYSize));
		IDrawXPos1  = cosf(IDrawAngle+FAngleDeg)*IDrawRadius;
		IDrawYPos1  = sinf(IDrawAngle+FAngleDeg)*IDrawRadius;
		IDrawXPos2  = cosf(IDrawAngle-FAngleDeg)*IDrawRadius;
		IDrawYPos2  = sinf(IDrawAngle-FAngleDeg)*IDrawRadius;
	}
	else
	{
		IDrawXPos1 = LDrawXSize;
		IDrawYPos1 = LDrawYSize;
		IDrawXPos2 = LDrawXSize;
		IDrawYPos2 = LDrawYSize;
	}

	uint32_t diffuse = (!color) ? Draw3DManager::ColorG3D.getRgba() : color->getRgba() ;

	// add vertices in clockwise order
	Draw3DVertexBuffer& surface = getVertexBuffer(handle,4,4,this->getSlotID(),IW_GX_QUAD_LIST) ;

	int16_t v0 = surface.addVertex( FDrawX-IDrawXPos1,FDrawY+IDrawYPos1, diffuse, LDrawU1Map, LDrawV1Map ) ;
	int16_t v1 = surface.addVertex( FDrawX+IDrawXPos2,FDrawY+IDrawYPos2, diffuse, LDrawU2Map, LDrawV1Map ) ;
	int16_t v2 = surface.addVertex( FDrawX+IDrawXPos1,FDrawY-IDrawYPos1, diffuse, LDrawU2Map, LDrawV2Map ) ;
	int16_t v3 = surface.addVertex( FDrawX-IDrawXPos2,FDrawY-IDrawYPos2, diffuse, LDrawU1Map, LDrawV2Map ) ;
	surface.addQuadIndices(v0,v1,v2,v3) ;

	appendOrDraw() ;

	if (FDrawButton != 0) // ** Can be -1 if no glow effect is needed
	{
		CheckQuad3D(FDrawX-IDrawXPos1,FDrawY+IDrawYPos1,
			FDrawX+IDrawXPos2,FDrawY+IDrawYPos2,
			FDrawX+IDrawXPos1,FDrawY-IDrawYPos1,
			FDrawX-IDrawXPos2,FDrawY-IDrawYPos2,
			FDrawButton);
	}
*/
}

// optimized version, specially used for drawing particles - also take in account for non-uniform scaling
void Draw3DManager::DrawImage3D(DrawItem* handle,
	const Vector2& position,
	float FDrawAngle,
	const Vector2& scale,
	const Color& color )
{
/*
	float LDrawU1Map = handle->getU1() ;
	float LDrawV1Map = handle->getV1() ;
	float LDrawU2Map = handle->getU2() ;
	float LDrawV2Map = handle->getV2() ;
	float LDrawXSize = handle->getHalfWidth() ;
	float LDrawYSize = handle->getHalfHeight() ;

	if( scale.x != 1.0f ) {
		LDrawXSize *= scale.x ;
	}

	if( scale.y != 1.0f ) {
		LDrawYSize *= scale.y ;
	}

	//Hotspot management
	float FDrawX = position.x + handle->getOffsetX();
	float FDrawY = position.y + handle->getOffsetY();

	float IDrawAngle = 0.0f;
	float IDrawRadius = 0.0f;
	float IDrawXPos1 = 0.0f;
	float IDrawYPos1 = 0.0f;
	float IDrawXPos2 = 0.0f;
	float IDrawYPos2 = 0.0f;

	if (FDrawAngle != 0.0f)
	{
		float FAngleDeg = ToRadian(FDrawAngle) ;
		IDrawAngle  = atan2f(LDrawYSize,LDrawXSize);
		IDrawRadius = Q_SQRT((LDrawXSize*LDrawXSize)+(LDrawYSize*LDrawYSize));
		IDrawXPos1  = cosf(IDrawAngle+FAngleDeg)*IDrawRadius;
		IDrawYPos1  = sinf(IDrawAngle+FAngleDeg)*IDrawRadius;
		IDrawXPos2  = cosf(IDrawAngle-FAngleDeg)*IDrawRadius;
		IDrawYPos2  = sinf(IDrawAngle-FAngleDeg)*IDrawRadius;
	}
	else
	{
		IDrawXPos1 = LDrawXSize;
		IDrawYPos1 = LDrawYSize;
		IDrawXPos2 = LDrawXSize;
		IDrawYPos2 = LDrawYSize;
	}

	uint32_t diffuse = color.getRgba() ;

	// add vertices in clockwise order
	Draw3DVertexBuffer& surface = getVertexBuffer(handle,4,4,this->getSlotID(),IW_GX_QUAD_LIST) ;

	int16_t v0 = surface.addVertex( FDrawX-IDrawXPos1,FDrawY+IDrawYPos1, diffuse, LDrawU1Map, LDrawV1Map ) ;
	int16_t v1 = surface.addVertex( FDrawX+IDrawXPos2,FDrawY+IDrawYPos2, diffuse, LDrawU2Map, LDrawV1Map ) ;
	int16_t v2 = surface.addVertex( FDrawX+IDrawXPos1,FDrawY-IDrawYPos1, diffuse, LDrawU2Map, LDrawV2Map ) ;
	int16_t v3 = surface.addVertex( FDrawX-IDrawXPos2,FDrawY-IDrawYPos2, diffuse, LDrawU1Map, LDrawV2Map ) ;
	surface.addQuadIndices(v0,v1,v2,v3) ;

	appendOrDraw() ;
*/
}

void Draw3DManager::DrawImageTransform3D(DrawItem* handle,
	const Matrix3& transform /*= Matrix3f::Identity*/,
	int FDrawButton /*= 0*/,
	const Vector2& hotSpot /*= Vector2::Zero */,
	Color* color /*= 0*/,
	bool SwapU /*= false*/,
	bool SwapV /*= false*/)
{
/*
	float LDrawU1Map = handle->getU1() ;
	float LDrawV1Map = handle->getV1() ;
	float LDrawU2Map = handle->getU2() ;
	float LDrawV2Map = handle->getV2() ;
	float LDrawXSize = handle->getHalfWidth() ;
	float LDrawYSize = handle->getHalfHeight() ;

	if( SwapU ) {
		std::swap<float>(LDrawU1Map,LDrawU2Map) ;
	}

	if( SwapV ) {
		std::swap<float>(LDrawV1Map,LDrawV2Map) ;
	}

	// TODO - here I changed hotspot sign.
	float FDrawX = -hotSpot.x + handle->getOffsetX();
	float FDrawY = -hotSpot.y + handle->getOffsetY();

	uint32_t diffuse = (!color) ? Draw3DManager::ColorG3D.getRgba() : color->getRgba() ;

	Vector2 p1(FDrawX-LDrawXSize, FDrawY+LDrawYSize) ;
	Vector2 p2(FDrawX+LDrawXSize, FDrawY+LDrawYSize) ;
	Vector2 p3(FDrawX+LDrawXSize, FDrawY-LDrawYSize) ;
	Vector2 p4(FDrawX-LDrawXSize, FDrawY-LDrawYSize) ;

	if( !transform.isIdentity() ) {
		p1 = transform.transformVec(p1) ;
		p2 = transform.transformVec(p2) ;
		p3 = transform.transformVec(p3) ;
		p4 = transform.transformVec(p4) ;
	}

	// add vertices in clockwise order
	Draw3DVertexBuffer& surface = getVertexBuffer(handle,4,4,this->getSlotID(),IW_GX_QUAD_LIST) ;
	int16_t v0 = surface.addVertex( p1.x,p1.y, diffuse, LDrawU1Map, LDrawV1Map ) ;
	int16_t v1 = surface.addVertex( p2.x,p2.y, diffuse, LDrawU2Map, LDrawV1Map ) ;
	int16_t v2 = surface.addVertex( p3.x,p3.y, diffuse, LDrawU2Map, LDrawV2Map ) ;
	int16_t v3 = surface.addVertex( p4.x,p4.y, diffuse, LDrawU1Map, LDrawV2Map ) ;
	surface.addQuadIndices(v0,v1,v2,v3) ;

	appendOrDraw() ;

	if (FDrawButton != 0) // ** Can be -1 if no glow effect is needed
	{
		CheckQuad3D(p1.x,p1.y,
			p2.x,p2.y,
			p3.x,p3.y,
			p4.x,p4.y,
			FDrawButton);
	}
*/
}

void Draw3DManager::DrawTransformedQuad3D(DrawItem* handle,
	const Polygon2f& poly,
	int FDrawButton /*= 0*/,
	Color* color /*= 0*/,
	bool SwapU /*= false*/,
	bool SwapV /*= false*/)
{
	float LDrawU1Map = handle->getU1() ;
	float LDrawV1Map = handle->getV1() ;
	float LDrawU2Map = handle->getU2() ;
	float LDrawV2Map = handle->getV2() ;

	if( SwapU ) {
		std::swap(LDrawU1Map,LDrawU2Map) ;
	}

	if( SwapV ) {
		std::swap(LDrawV1Map,LDrawV2Map) ;
	}

	uint32_t diffuse = (!color) ? Draw3DManager::ColorG3D.getRgba() : color->getRgba() ;

	StridedVertexBuffer&	pVBuff = GetGfx().getVertexBuffer(handle,6,6,GL_TRIANGLES) ;

	// Note: polygon points are in clockwise order
	pVBuff.addQuad( poly, diffuse, LDrawU1Map, LDrawV1Map, LDrawU2Map, LDrawV2Map ) ;
	GetGfx().appendOrDraw() ;
}

void Draw3DManager::ImageLine3D( DrawItem* handle,float FDrawX1,float FDrawY1,float FDrawX2,float FDrawY2,float FDrawSize,int FDrawMode )
{
/*
#ifndef JAM_IMAGELINE3D_DISABLED
	if (FDrawX1 != FDrawX2 || FDrawY1 != FDrawY2)
	{
		Texture2D* pTexture = handle->getTexture();
		//			jam::Rect srcRect  = handle->getRect();

		// texture or cut X-size
		float LDrawXSize ;
		//if( !srcRect.isEmpty() ) {
		if( handle->getWidth()!=0 || handle->getHeight()!=0 ) {
			//LDrawXSize = (float)srcRect.right - srcRect.left ;
			LDrawXSize = handle->getWidth() ;
		}
		else {
			LDrawXSize = (float)pTexture->getWidth() ;
		}

		Vector2 n = Vector2(FDrawX2-FDrawX1,FDrawY2-FDrawY1) ;
		n.normalize();
		float IDrawXTForm = n.x * FDrawSize;
		float IDrawYTForm = n.y * FDrawSize;

		float LDrawU1Map = handle->getU1() ;
		float LDrawV1Map = handle->getV1() ;
		float LDrawU2Map = handle->getU2() ;
		float LDrawV2Map = handle->getV2() ;

		//Modus-Rollen
		if (FDrawMode==1)//GS:TBD doesn't work!
		{
			float IDrawLineSize=Q_SQRT((FDrawX1-FDrawX2)*(FDrawX1-FDrawX2)+(FDrawY1-FDrawY2)*(FDrawY1-FDrawY2));
			LDrawU2Map=LDrawU1Map+float(IDrawLineSize/float(pTexture->getWidth()))/2.0f;
		}
		else if (FDrawMode==2) 	//Modus-Absolut
		{
			FDrawX2 = FDrawX1 + n.x*LDrawXSize*2;
			FDrawY2 = FDrawY1 + n.y*LDrawXSize*2;
		}

		uint32_t diffuse = Draw3DManager::ColorG3D.getRgba() ;

		Draw3DVertexBuffer&	surface = getVertexBuffer(handle,4,4,this->getSlotID(),IW_GX_QUAD_LIST) ;

		// bottom left
		unsigned short v0 = surface.addVertex(FDrawX1-IDrawYTForm,FDrawY1+IDrawXTForm, diffuse, LDrawU1Map,LDrawV1Map);
		// bottom right
		unsigned short v1 = surface.addVertex(FDrawX2-IDrawYTForm,FDrawY2+IDrawXTForm, diffuse, LDrawU2Map,LDrawV1Map);
		// top right
		unsigned short v2 = surface.addVertex(FDrawX2+IDrawYTForm,FDrawY2-IDrawXTForm, diffuse, LDrawU2Map,LDrawV2Map);
		// top left
		unsigned short v3 = surface.addVertex(FDrawX1+IDrawYTForm,FDrawY1-IDrawXTForm, diffuse, LDrawU1Map,LDrawV2Map);

		surface.addQuadIndices(v0,v1,v2,v3) ;

		appendOrDraw() ;
	}
#endif
*/
}

void Draw3DManager::DrawQuad3D(  const Polygon2f& poly, Color* color /*= 0*/ )
{
/*
	uint32_t diffuse = (!color) ? Draw3DManager::ColorG3D.getRgba() : color->getRgba() ;

	DrawItem* handle = DrawItem::create();
	Draw3DVertexBuffer&	surface = getVertexBuffer(handle,4,4,this->getSlotID(),IW_GX_QUAD_LIST) ;

	// add vertices in clockwise order
	int16_t v0 = surface.addVertex( poly.getVertex(0).x, poly.getVertex(0).y, diffuse ) ;
	int16_t v1 = surface.addVertex( poly.getVertex(1).x, poly.getVertex(1).y, diffuse ) ;
	int16_t v2 = surface.addVertex( poly.getVertex(2).x, poly.getVertex(2).y, diffuse ) ;
	int16_t v3 = surface.addVertex( poly.getVertex(3).x, poly.getVertex(3).y, diffuse ) ;
	surface.addQuadIndices(v0,v1,v2,v3) ;

	appendOrDraw() ;
	JAM_RELEASE_NULL(handle) ;
*/
}

// not batched, use own private vertex buffer
void Draw3DManager::DrawQuad3D( Material* pMat, const jam::Rect& dstRect, Color* color /*= 0*/ )
{
/*
	uint32_t diffuse = (!color) ? Draw3DManager::ColorG3D.getRgba() : color->getRgba() ;

	Draw3DVertexBuffer surface( 4, 4, true );

	float LDrawU1Map = 0.0f ;
	float LDrawV1Map = 0.0f ;
	float LDrawU2Map = 1.0f ;
	float LDrawV2Map = 1.0f ;

	// add vertices in clockwise order
	int16_t v0 = surface.addVertex( (float)dstRect.left,  (float)dstRect.top,    diffuse, LDrawU1Map, LDrawV1Map ) ;
	int16_t v1 = surface.addVertex( (float)dstRect.right, (float)dstRect.top,    diffuse, LDrawU2Map, LDrawV1Map ) ;
	int16_t v2 = surface.addVertex( (float)dstRect.right, (float)dstRect.bottom, diffuse, LDrawU2Map, LDrawV2Map ) ;
	int16_t v3 = surface.addVertex( (float)dstRect.left,	 (float)dstRect.bottom, diffuse, LDrawU1Map, LDrawV2Map ) ;
	surface.addQuadIndices(v0,v1,v2,v3) ;

	draw(&surface,pMat,IW_GX_QUAD_LIST) ;
*/
}

void Draw3DManager::DrawTexture( Texture2D* pTexture, const jam::Rect& dstRect, Color* color /*= 0*/, Shader* shader /*=0*/)
{
/*
	uint32_t diffuse = (!color) ? Draw3DManager::ColorG3D.getRgba() : color->getRgba() ;

	DrawItem* handle = DrawItem::create(pTexture);
	handle->setShader(shader) ;

	Draw3DVertexBuffer&	surface = getVertexBuffer(handle,4,4,this->getSlotID(),IW_GX_QUAD_LIST) ;

	float LDrawU1Map = handle->getU1() ;
	float LDrawV1Map = handle->getV1() ;
	float LDrawU2Map = handle->getU2() ;
	float LDrawV2Map = handle->getV2() ;

	// add vertices in clockwise order
	int16_t v0 = surface.addVertex( (float)dstRect.left,  (float)dstRect.top,    diffuse, LDrawU1Map, LDrawV1Map ) ;
	int16_t v1 = surface.addVertex( (float)dstRect.right, (float)dstRect.top,    diffuse, LDrawU2Map, LDrawV1Map ) ;
	int16_t v2 = surface.addVertex( (float)dstRect.right, (float)dstRect.bottom, diffuse, LDrawU2Map, LDrawV2Map ) ;
	int16_t v3 = surface.addVertex( (float)dstRect.left,	 (float)dstRect.bottom, diffuse, LDrawU1Map, LDrawV2Map ) ;
	surface.addQuadIndices(v0,v1,v2,v3) ;

	appendOrDraw() ;
	JAM_RELEASE_NULL(handle) ;
*/
}

void Draw3DManager::VerifyQuad3D( DrawItem* handle, float FDrawX, float FDrawY, float FDrawAngle /*= 0.0f*/, float FDrawScale /*= 1.0f*/, float LHotSpotX /*= 0.0f*/, float LHotSpotY /*= 0.0f*/ )
{
	float LDrawXSize = handle->getHalfWidth() ;
	float LDrawYSize = handle->getHalfHeight() ;

	if( FDrawScale != 1.0f ) {
		LDrawXSize *= FDrawScale ;
		LDrawYSize *= FDrawScale ;
	}

	//Hotspot management
	FDrawX += -LHotSpotX;
	FDrawY += -LHotSpotY;

	float IDrawAngle = 0.0f;
	float IDrawRadius = 0.0f;
	float IDrawXPos1 = 0.0f;
	float IDrawYPos1 = 0.0f;
	float IDrawXPos2 = 0.0f;
	float IDrawYPos2 = 0.0f;

	if (FDrawAngle != 0.0f)
	{
		float FAngleDeg = ToRadian(FDrawAngle) ;
		IDrawAngle  = atan2f(LDrawYSize,LDrawXSize);
		IDrawRadius = jam::Q_sqrt((LDrawXSize*LDrawXSize)+(LDrawYSize*LDrawYSize));
		IDrawXPos1  = cosf(IDrawAngle+FAngleDeg)*IDrawRadius;
		IDrawYPos1  = sinf(IDrawAngle+FAngleDeg)*IDrawRadius;
		IDrawXPos2  = cosf(IDrawAngle-FAngleDeg)*IDrawRadius;
		IDrawYPos2  = sinf(IDrawAngle-FAngleDeg)*IDrawRadius;
	}
	else
	{
		IDrawXPos1 = LDrawXSize;
		IDrawYPos1 = LDrawYSize;
		IDrawXPos2 = LDrawXSize;
		IDrawYPos2 = LDrawYSize;
	}

	CheckQuad3D(FDrawX-IDrawXPos1,FDrawY+IDrawYPos1,
		FDrawX+IDrawXPos2,FDrawY+IDrawYPos2,
		FDrawX+IDrawXPos1,FDrawY-IDrawYPos1,
		FDrawX-IDrawXPos2,FDrawY-IDrawYPos2);
}

void Draw3DManager::DrawTiledMapRow( DrawItem** items, unsigned int count, float FDrawX, float FDrawY, int tileXSize )
{
/*
	float LDrawU1Map=0;
	float LDrawV1Map=0;
	float LDrawU2Map=0;
	float LDrawV2Map=0;
	float LDrawXSize=0;
	float LDrawYSize=0;

	uint32_t diffuse = Draw3DManager::ColorG3D.getRgba() ;

	DrawItem* dItem = 0 ;

	Draw3DVertexBuffer surface(4*count, 4*count, true) ;

	for( unsigned int i=0; i<count; i++ ) {
		DrawItem* handle = items[i] ;
		if( handle == 0 ) {
			FDrawX += tileXSize ;
			continue;
		}

		if( dItem==0 ) dItem = handle ;
		LDrawU1Map = handle->getU1() ;
		LDrawV1Map = handle->getV1() ;
		LDrawU2Map = handle->getU2() ;
		LDrawV2Map = handle->getV2() ;
		LDrawXSize = handle->getHalfWidth() ;
		LDrawYSize = handle->getHalfHeight() ;

		// add vertices in clockwise order
		int16_t v0 = surface.addVertex( FDrawX-LDrawXSize,FDrawY+LDrawYSize, diffuse, LDrawU1Map, LDrawV1Map ) ;
		int16_t v1 = surface.addVertex( FDrawX+LDrawXSize,FDrawY+LDrawYSize, diffuse, LDrawU2Map, LDrawV1Map ) ;
		int16_t v2 = surface.addVertex( FDrawX+LDrawXSize,FDrawY-LDrawYSize, diffuse, LDrawU2Map, LDrawV2Map ) ;
		int16_t v3 = surface.addVertex( FDrawX-LDrawXSize,FDrawY-LDrawYSize, diffuse, LDrawU1Map, LDrawV2Map ) ;
		surface.addQuadIndices(v0,v1,v2,v3) ;

		FDrawX += handle->getWidth() ;
	}

	if( dItem ) drawPrimitive(&surface,getMaterial(dItem), this->getSlotID(), IW_GX_QUAD_LIST ) ;
*/
}

void Draw3DManager::DrawTiledMap( DrawItem** items, int rows, int cols, float FDrawX, float FDrawY, int tileXSize, int tileYSize )
{
/*
	float LDrawU1Map=0;
	float LDrawV1Map=0;
	float LDrawU2Map=0;
	float LDrawV2Map=0;
	float LDrawXSize=0;
	float LDrawYSize=0;

	uint32_t diffuse = Draw3DManager::ColorG3D.getRgba() ;

	DrawItem* dItem = 0 ;

	Draw3DVertexBuffer surface(4*rows*cols, 4*rows*cols, true) ;

	for( int j=0; j<rows; j++ ) {
		for( int i=0; i<cols; i++ ) {
			DrawItem* handle = items[i+j*rows] ;
			if( handle == 0 ) {
				FDrawX += tileXSize ;
				continue;
			}

			if( dItem==0 ) dItem = handle ;
			float LDrawU1Map = handle->getU1() ;
			float LDrawV1Map = handle->getV1() ;
			float LDrawU2Map = handle->getU2() ;
			float LDrawV2Map = handle->getV2() ;
			float LDrawXSize = handle->getHalfWidth() ;
			float LDrawYSize = handle->getHalfHeight() ;

			// add vertices in clockwise order
			int16_t v0 = surface.addVertex( FDrawX-LDrawXSize,FDrawY+LDrawYSize, diffuse, LDrawU1Map, LDrawV1Map ) ;
			int16_t v1 = surface.addVertex( FDrawX+LDrawXSize,FDrawY+LDrawYSize, diffuse, LDrawU2Map, LDrawV1Map ) ;
			int16_t v2 = surface.addVertex( FDrawX+LDrawXSize,FDrawY-LDrawYSize, diffuse, LDrawU2Map, LDrawV2Map ) ;
			int16_t v3 = surface.addVertex( FDrawX-LDrawXSize,FDrawY-LDrawYSize, diffuse, LDrawU1Map, LDrawV2Map ) ;
			surface.addQuadIndices(v0,v1,v2,v3) ;

			FDrawX += handle->getWidth() ;
		}

		FDrawY += tileYSize ;
	}

	if( dItem ) drawPrimitive(&surface,getMaterial(dItem), this->getSlotID(), IW_GX_QUAD_LIST ) ;
*/
}

void Draw3DManager::Line3D( Texture2D* pTexture, float FDrawX1,float FDrawY1,float FDrawX2,float FDrawY2,float FDrawSize /*= 2*/ )
{
	DrawItem* item = DrawItem::create( pTexture ) ;
	Line3D( item, FDrawX1, FDrawY1, FDrawX2, FDrawY2, FDrawSize ) ;
	item = nullptr;
}


void Draw3DManager::Line3D( DrawItem* pItem, const Vector2& FDraw1, const Vector2& FDraw2, float FDrawSize /*= 2 */ )
{
	Line3D( pItem, FDraw1.x, FDraw1.y, FDraw2.x, FDraw2.y, FDrawSize ) ;
}


//Line3D( Handle, X-Start, Y-Start, X-End, Y-End, IDrawing-Size )
void Draw3DManager::Line3D( DrawItem* pItem, float FDrawX1,float FDrawY1,float FDrawX2,float FDrawY2,float FDrawSize /*= 2*/ )
{
/*
#ifndef JAM_LINE3D_DISABLED
	if (FDrawX1 != FDrawX2 || FDrawY1 != FDrawY2)
	{
		if (FDrawSize==0) FDrawSize=2;

		CIwFVec2 n = CIwFVec2( FDrawX2-FDrawX1, FDrawY2-FDrawY1 ) ;
		n.Normalise();
		float IDrawUTForm = n.x * 0.5f ;
		float IDrawVTForm = n.y * 0.5f ;

		float IDrawXTForm = IDrawUTForm * 2.0f * FDrawSize;
		float IDrawYTForm = IDrawVTForm * 2.0f * FDrawSize;

		//Vertex/Ploygon-Zuweisung/Berechnungen
		uint32_t diffuse = ColorN3D.getRgba();

		Draw3DVertexBuffer& LDrawFace = getVertexBuffer(pItem,4,6,this->getSlotID(),IW_GX_QUAD_LIST) ;
		short IDrawV0=LDrawFace.addVertex(FDrawX1+IDrawYTForm-0.5f, FDrawY1-IDrawXTForm+0.5f, diffuse, 0.5f+IDrawVTForm, 0.5f+IDrawUTForm);
		short IDrawV1=LDrawFace.addVertex(FDrawX1-IDrawYTForm-0.5f, FDrawY1+IDrawXTForm+0.5f, diffuse, 0.5f-IDrawVTForm, 0.5f-IDrawUTForm);
		short IDrawV2=LDrawFace.addVertex(FDrawX2-IDrawYTForm-0.5f, FDrawY2+IDrawXTForm+0.5f, diffuse, 0.5f-IDrawVTForm, 0.5f-IDrawUTForm);
		short IDrawV3=LDrawFace.addVertex(FDrawX2+IDrawYTForm-0.5f, FDrawY2-IDrawXTForm+0.5f, diffuse, 0.5f+IDrawVTForm, 0.5f+IDrawUTForm);

		LDrawFace.addQuadIndices(IDrawV0,IDrawV1,IDrawV2, IDrawV3);

		appendOrDraw() ;
	}
#endif
*/
}

//Plot3D( Handle, X-Center, Y-Center, IDrawing-Size )
void Draw3DManager::Plot3D( Texture2D* pTexture, float FDrawX, float FDrawY, float FDrawSize /*= 1*/ )
{
/*
	if (FDrawSize==0) FDrawSize=1;

	uint32_t diffuse = ColorN3D.getRgba();

	//Vertex/Ploygon-Zuweisung/Berechnungen
	DrawItem* it = DrawItem::create(pTexture) ;
	Draw3DVertexBuffer& LDrawFace = getVertexBuffer(it,4,6,this->getSlotID(),IW_GX_QUAD_LIST) ;
	int IDrawV0=LDrawFace.addVertex(FDrawX-FDrawSize-0.5f,FDrawY+FDrawSize+0.5f, diffuse, 0,0);
	int IDrawV1=LDrawFace.addVertex(FDrawX+FDrawSize-0.5f,FDrawY+FDrawSize+0.5f, diffuse, 1,0);
	int IDrawV2=LDrawFace.addVertex(FDrawX+FDrawSize-0.5f,FDrawY-FDrawSize+0.5f, diffuse, 1,1);
	int IDrawV3=LDrawFace.addVertex(FDrawX-FDrawSize-0.5f,FDrawY-FDrawSize+0.5f, diffuse, 0,1);

	LDrawFace.addQuadIndices(IDrawV0,IDrawV1,IDrawV2,IDrawV3);

	appendOrDraw() ;
	JAM_RELEASE(it) ;
*/
}

void Draw3DManager::Rect3D( Texture2D* pTexture, float FDrawX, float FDrawY, float FDrawXS, float FDrawYS, int FDrawFill /*=0*/, float FDrawSize /*=0*/ )
{
	// DrawBank Variable designation
	DrawItem* it = DrawItem::create(pTexture) ;
	StridedVertexBuffer& LDrawFace = GetGfx().getVertexBuffer(it,16,30) ;

	uint32_t diffuse = ColorN3D.getRgba();

	FDrawXS=fabsf(FDrawXS);
	FDrawYS=fabsf(FDrawYS);

	//Frame alone mode;
	if (FDrawFill==0) {
		if (FDrawSize==0) {FDrawSize=1;}

		int IDrawV0=LDrawFace.addVertex(FDrawX-FDrawXS-FDrawSize,FDrawY+FDrawYS+FDrawSize, diffuse, 0.5f,0);
		int IDrawV1=LDrawFace.addVertex(FDrawX+FDrawXS+FDrawSize,FDrawY+FDrawYS+FDrawSize, diffuse, 0.5f,0);
		int IDrawV2=LDrawFace.addVertex(FDrawX+FDrawXS-FDrawSize,FDrawY+FDrawYS-FDrawSize, diffuse, 0.5f,1);
		int IDrawV3=LDrawFace.addVertex(FDrawX-FDrawXS+FDrawSize,FDrawY+FDrawYS-FDrawSize, diffuse, 0.5f,1);
		int IDrawV4=LDrawFace.addVertex(FDrawX+FDrawXS+FDrawSize,FDrawY+FDrawYS+FDrawSize, diffuse, 1,0.5f);
		int IDrawV5=LDrawFace.addVertex(FDrawX+FDrawXS+FDrawSize,FDrawY-FDrawYS-FDrawSize, diffuse, 1,0.5f);
		int IDrawV6=LDrawFace.addVertex(FDrawX+FDrawXS-FDrawSize,FDrawY-FDrawYS+FDrawSize, diffuse, 0,0.5f);
		int IDrawV7=LDrawFace.addVertex(FDrawX+FDrawXS-FDrawSize,FDrawY+FDrawYS-FDrawSize, diffuse, 0,0.5f);
		int IDrawV8=LDrawFace.addVertex(FDrawX+FDrawXS+FDrawSize,FDrawY-FDrawYS-FDrawSize, diffuse, 0.5f,1);
		int IDrawV9=LDrawFace.addVertex(FDrawX-FDrawXS-FDrawSize,FDrawY-FDrawYS-FDrawSize, diffuse, 0.5f,1);
		int IDrawVA=LDrawFace.addVertex(FDrawX-FDrawXS+FDrawSize,FDrawY-FDrawYS+FDrawSize, diffuse, 0.5f,0);
		int IDrawVB=LDrawFace.addVertex(FDrawX+FDrawXS-FDrawSize,FDrawY-FDrawYS+FDrawSize, diffuse, 0.5f,0);
		int IDrawVC=LDrawFace.addVertex(FDrawX-FDrawXS-FDrawSize,FDrawY-FDrawYS-FDrawSize, diffuse, 0,0.5f);
		int IDrawVD=LDrawFace.addVertex(FDrawX-FDrawXS-FDrawSize,FDrawY+FDrawYS+FDrawSize, diffuse, 0,0.5f);
		int IDrawVE=LDrawFace.addVertex(FDrawX-FDrawXS+FDrawSize,FDrawY+FDrawYS-FDrawSize, diffuse, 1,0.5f);
		int IDrawVF=LDrawFace.addVertex(FDrawX-FDrawXS+FDrawSize,FDrawY-FDrawYS+FDrawSize, diffuse, 1,0.5f);
		LDrawFace.addTriIndices(IDrawV0,IDrawV1,IDrawV2);
		LDrawFace.addTriIndices(IDrawV2,IDrawV3,IDrawV0);
		LDrawFace.addTriIndices(IDrawV4,IDrawV5,IDrawV6);
		LDrawFace.addTriIndices(IDrawV6,IDrawV7,IDrawV4);
		LDrawFace.addTriIndices(IDrawV8,IDrawV9,IDrawVA);
		LDrawFace.addTriIndices(IDrawVA,IDrawVB,IDrawV8);
		LDrawFace.addTriIndices(IDrawVC,IDrawVD,IDrawVE);
		LDrawFace.addTriIndices(IDrawVE,IDrawVF,IDrawVC);
	}
	else
	{
		//Filled-single-mode
		int IDrawV0=LDrawFace.addVertex(FDrawX-FDrawXS,FDrawY+FDrawYS, diffuse, 0.5f-DRAWRECTOFFSET,0.5f-DRAWRECTOFFSET);
		int IDrawV1=LDrawFace.addVertex(FDrawX+FDrawXS,FDrawY+FDrawYS, diffuse, 0.5f+DRAWRECTOFFSET,0.5f-DRAWRECTOFFSET);
		int IDrawV2=LDrawFace.addVertex(FDrawX+FDrawXS,FDrawY-FDrawYS, diffuse, 0.5f+DRAWRECTOFFSET,0.5f+DRAWRECTOFFSET);
		int IDrawV3=LDrawFace.addVertex(FDrawX-FDrawXS,FDrawY-FDrawYS, diffuse, 0.5f-DRAWRECTOFFSET,0.5f+DRAWRECTOFFSET);
		LDrawFace.addTriIndices(IDrawV0,IDrawV1,IDrawV2);
		LDrawFace.addTriIndices(IDrawV2,IDrawV3,IDrawV0);
		//Add half-frame
		if (FDrawSize>0) {
			int IDrawV4=LDrawFace.addVertex(FDrawX-FDrawXS-FDrawSize,FDrawY+FDrawYS+FDrawSize, diffuse, 0.5f,0);
			int IDrawV5=LDrawFace.addVertex(FDrawX+FDrawXS+FDrawSize,FDrawY+FDrawYS+FDrawSize, diffuse, 0.5f,0);
			int IDrawV6=LDrawFace.addVertex(FDrawX+FDrawXS+FDrawSize,FDrawY+FDrawYS+FDrawSize, diffuse, 1,0.5f);
			int IDrawV7=LDrawFace.addVertex(FDrawX+FDrawXS+FDrawSize,FDrawY-FDrawYS-FDrawSize, diffuse, 1,0.5f);
			int IDrawV8=LDrawFace.addVertex(FDrawX+FDrawXS+FDrawSize,FDrawY-FDrawYS-FDrawSize, diffuse, 0.5f,1);
			int IDrawV9=LDrawFace.addVertex(FDrawX-FDrawXS-FDrawSize,FDrawY-FDrawYS-FDrawSize, diffuse, 0.5f,1);
			int IDrawVA=LDrawFace.addVertex(FDrawX-FDrawXS-FDrawSize,FDrawY-FDrawYS-FDrawSize, diffuse, 0,0.5f);
			int IDrawVB=LDrawFace.addVertex(FDrawX-FDrawXS-FDrawSize,FDrawY+FDrawYS+FDrawSize, diffuse, 0,0.5f);
			LDrawFace.addTriIndices(IDrawV4,IDrawV5,IDrawV1);
			LDrawFace.addTriIndices(IDrawV1,IDrawV0,IDrawV4);
			LDrawFace.addTriIndices(IDrawV6,IDrawV7,IDrawV2);
			LDrawFace.addTriIndices(IDrawV2,IDrawV1,IDrawV6);
			LDrawFace.addTriIndices(IDrawV8,IDrawV9,IDrawV3);
			LDrawFace.addTriIndices(IDrawV3,IDrawV2,IDrawV8);
			LDrawFace.addTriIndices(IDrawVA,IDrawVB,IDrawV0);
			LDrawFace.addTriIndices(IDrawV0,IDrawV3,IDrawVA);
		}
	}
	GetGfx().appendOrDraw() ;
	it = nullptr ;
}

//Poly3D( Handle, X1-Pos, Y1-Pos, X2-Pos, Y2-Pos, X3-Pos, Y3-Pos )
void Draw3DManager::Poly3D( Texture2D* pTexture, float FDrawX1, float FDrawY1, float FDrawX2, float FDrawY2, float FDrawX3, float FDrawY3 )
{
	DrawItem* it = DrawItem::create(pTexture) ;
	StridedVertexBuffer& LDrawFace = GetGfx().getVertexBuffer(it,3,3) ;

	uint32_t diffuse = ColorN3D.getRgba();

	//Vertex / Polygon Assignment / Calculations
	int IDrawV0=LDrawFace.addVertex(FDrawX1-0.5f,FDrawY1+0.5f, diffuse, 0.5f,0.5f);
	int IDrawV1=LDrawFace.addVertex(FDrawX2-0.5f,FDrawY2+0.5f, diffuse, 0.5f,0.5f);
	int IDrawV2=LDrawFace.addVertex(FDrawX3-0.5f,FDrawY3+0.5f, diffuse, 0.5f,0.5f);
	LDrawFace.addTriIndices(IDrawV0,IDrawV1,IDrawV2);

	GetGfx().appendOrDraw() ;
	it = nullptr ;
}

void Draw3DManager::Oval3D( Texture2D* pTexture, float FDrawX, float FDrawY, float FDrawXS, float FDrawYS, int FDrawFill /*=0*/, float FDrawSize /*=0*/ )
{
	//DrawBank Variable designation
	DrawItem* it = DrawItem::create(pTexture) ;
	StridedVertexBuffer& LDrawFace = GetGfx().getVertexBuffer(it,512,768) ;

	uint32_t diffuse = ColorN3D.getRgba();

	//Variable precomputation
	FDrawXS=fabsf(FDrawXS);
	FDrawYS=fabsf(FDrawYS);

	int IDrawSequences=8+((int)((FDrawXS+FDrawYS)/20));	//TODO:
	if (IDrawSequences>32) IDrawSequences=32;
	float IDrawAngle=90.0f/IDrawSequences;
	int IDrawLoop ;
	//Frame-alone mode
	if (FDrawFill==0) {
		if (FDrawSize==0) FDrawSize=2;

		float IDrawXOutside=FDrawXS+FDrawSize;
		float IDrawYOutside=FDrawYS+FDrawSize;
		float IDrawXInside=FDrawXS-FDrawSize;
		float IDrawYInside=FDrawYS-FDrawSize;
		for (IDrawLoop=1 ;IDrawLoop<= IDrawSequences; IDrawLoop++){
			//Vertex / Polygon Assignment / Calculations
			float IDrawXPos1=cosf(ToRadian(IDrawLoop*IDrawAngle));
			float IDrawYPos1=sinf(ToRadian(IDrawLoop*IDrawAngle));
			float IDrawXPos2=cosf(ToRadian(IDrawLoop*IDrawAngle-IDrawAngle));
			float IDrawYPos2=sinf(ToRadian(IDrawLoop*IDrawAngle-IDrawAngle));
			float IDrawUPos1=IDrawXPos1*0.5f;
			float IDrawVPos1=IDrawYPos1*0.5f;
			float IDrawUPos2=IDrawXPos2*0.5f;
			float IDrawVPos2=IDrawYPos2*0.5f;
			//Oval3D-Quartal-1/4-Mirror
			int IDrawV0=LDrawFace.addVertex(FDrawX+IDrawXPos1*IDrawXOutside,FDrawY+IDrawYPos1*IDrawYOutside, diffuse, 0.5f+IDrawUPos1,0.5f-IDrawVPos1);
			int IDrawV1=LDrawFace.addVertex(FDrawX+IDrawXPos2*IDrawXOutside,FDrawY+IDrawYPos2*IDrawYOutside, diffuse, 0.5f+IDrawUPos2,0.5f-IDrawVPos2);
			int IDrawV2=LDrawFace.addVertex(FDrawX+IDrawXPos2*IDrawXInside,FDrawY+IDrawYPos2*IDrawYInside, diffuse, 0.5f-IDrawUPos2,0.5f+IDrawVPos2);
			int IDrawV3=LDrawFace.addVertex(FDrawX+IDrawXPos1*IDrawXInside,FDrawY+IDrawYPos1*IDrawYInside, diffuse, 0.5f-IDrawUPos1,0.5f+IDrawVPos1);
			LDrawFace.addTriIndices(IDrawV0,IDrawV1,IDrawV2);
			LDrawFace.addTriIndices(IDrawV2,IDrawV3,IDrawV0);
			//Oval3D-Quartal-2/4-Mirror
			IDrawV0=LDrawFace.addVertex(FDrawX+IDrawXPos1*IDrawXOutside,FDrawY-IDrawYPos1*IDrawYOutside, diffuse, 0.5f+IDrawUPos1,0.5f+IDrawVPos1);
			IDrawV1=LDrawFace.addVertex(FDrawX+IDrawXPos2*IDrawXOutside,FDrawY-IDrawYPos2*IDrawYOutside, diffuse, 0.5f+IDrawUPos2,0.5f+IDrawVPos2);
			IDrawV2=LDrawFace.addVertex(FDrawX+IDrawXPos2*IDrawXInside,FDrawY-IDrawYPos2*IDrawYInside, diffuse, 0.5f-IDrawUPos2,0.5f-IDrawVPos2);
			IDrawV3=LDrawFace.addVertex(FDrawX+IDrawXPos1*IDrawXInside,FDrawY-IDrawYPos1*IDrawYInside, diffuse, 0.5f-IDrawUPos1,0.5f-IDrawVPos1);
			//LDrawFace.addTriangle(IDrawV0,IDrawV1,IDrawV2);
			//LDrawFace.addTriangle(IDrawV2,IDrawV3,IDrawV0);
			LDrawFace.addTriIndices(IDrawV0,IDrawV2,IDrawV1);
			LDrawFace.addTriIndices(IDrawV2,IDrawV0,IDrawV3);
			//Oval3D-Quartal-3/4-Mirror
			IDrawV0=LDrawFace.addVertex(FDrawX-IDrawXPos1*IDrawXOutside,FDrawY-IDrawYPos1*IDrawYOutside, diffuse, 0.5f-IDrawUPos1,0.5f+IDrawVPos1);
			IDrawV1=LDrawFace.addVertex(FDrawX-IDrawXPos2*IDrawXOutside,FDrawY-IDrawYPos2*IDrawYOutside, diffuse, 0.5f-IDrawUPos2,0.5f+IDrawVPos2);
			IDrawV2=LDrawFace.addVertex(FDrawX-IDrawXPos2*IDrawXInside,FDrawY-IDrawYPos2*IDrawYInside, diffuse, 0.5f+IDrawUPos2,0.5f-IDrawVPos2);
			IDrawV3=LDrawFace.addVertex(FDrawX-IDrawXPos1*IDrawXInside,FDrawY-IDrawYPos1*IDrawYInside, diffuse, 0.5f+IDrawUPos1,0.5f-IDrawVPos1);
			LDrawFace.addTriIndices(IDrawV0,IDrawV1,IDrawV2);
			LDrawFace.addTriIndices(IDrawV2,IDrawV3,IDrawV0);
			//Oval3D-Quartal-4/4-Mirror
			IDrawV0=LDrawFace.addVertex(FDrawX-IDrawXPos1*IDrawXOutside,FDrawY+IDrawYPos1*IDrawYOutside, diffuse, 0.5f-IDrawUPos1,0.5f-IDrawVPos1);
			IDrawV1=LDrawFace.addVertex(FDrawX-IDrawXPos2*IDrawXOutside,FDrawY+IDrawYPos2*IDrawYOutside, diffuse, 0.5f-IDrawUPos2,0.5f-IDrawVPos2);
			IDrawV2=LDrawFace.addVertex(FDrawX-IDrawXPos2*IDrawXInside,FDrawY+IDrawYPos2*IDrawYInside, diffuse, 0.5f+IDrawUPos2,0.5f+IDrawVPos2);
			IDrawV3=LDrawFace.addVertex(FDrawX-IDrawXPos1*IDrawXInside,FDrawY+IDrawYPos1*IDrawYInside, diffuse, 0.5f+IDrawUPos1,0.5f+IDrawVPos1);
			LDrawFace.addTriIndices(IDrawV0,IDrawV2,IDrawV1);
			LDrawFace.addTriIndices(IDrawV2,IDrawV0,IDrawV3);
		}
	}
	else
	{
		//Center vertex assignment / calculations
		int IDrawVC=LDrawFace.addVertex(FDrawX,FDrawY, diffuse, 0.5f,0.5f);
		//Half-filled + frame mode
		if (FDrawSize>0) {
			float IDrawXOutside=FDrawXS+FDrawSize;
			float IDrawYOutside=FDrawYS+FDrawSize;
			for (IDrawLoop=1;IDrawLoop<= IDrawSequences;IDrawLoop++){
				//Vertex/Ploygon-Zuweisung/Berechnungen
				float IDrawXPos1=cosf(ToRadian(IDrawLoop*IDrawAngle));
				float IDrawYPos1=sinf(ToRadian(IDrawLoop*IDrawAngle));
				float IDrawXPos2=cosf(ToRadian(IDrawLoop*IDrawAngle-IDrawAngle));
				float IDrawYPos2=sinf(ToRadian(IDrawLoop*IDrawAngle-IDrawAngle));
				float IDrawUPos1=IDrawXPos1*0.5f;
				float IDrawVPos1=IDrawYPos1*0.5f;
				float IDrawUPos2=IDrawXPos2*0.5f;
				float IDrawVPos2=IDrawYPos2*0.5f;
				//Oval3D-Quartal-1/4-Mirror
				int IDrawV0=LDrawFace.addVertex(FDrawX+IDrawXPos1*IDrawXOutside,FDrawY+IDrawYPos1*IDrawYOutside, diffuse, 0.5f+IDrawUPos1,0.5f-IDrawVPos1);
				int IDrawV1=LDrawFace.addVertex(FDrawX+IDrawXPos2*IDrawXOutside,FDrawY+IDrawYPos2*IDrawYOutside, diffuse, 0.5f+IDrawUPos2,0.5f-IDrawVPos2);
				int IDrawV2=LDrawFace.addVertex(FDrawX+IDrawXPos2*FDrawXS,FDrawY+IDrawYPos2*FDrawYS, diffuse, 0.5f,0.5f);
				int IDrawV3=LDrawFace.addVertex(FDrawX+IDrawXPos1*FDrawXS,FDrawY+IDrawYPos1*FDrawYS, diffuse, 0.5f,0.5f);
				LDrawFace.addTriIndices(IDrawV0,IDrawV1,IDrawV2);
				LDrawFace.addTriIndices(IDrawV2,IDrawV3,IDrawV0);
				LDrawFace.addTriIndices(IDrawVC,IDrawV3,IDrawV2);
				////Oval3D-Quartal-2/4-Mirror
				IDrawV0=LDrawFace.addVertex(FDrawX+IDrawXPos1*IDrawXOutside,FDrawY-IDrawYPos1*IDrawYOutside, diffuse, 0.5f+IDrawUPos1,0.5f+IDrawVPos1);
				IDrawV1=LDrawFace.addVertex(FDrawX+IDrawXPos2*IDrawXOutside,FDrawY-IDrawYPos2*IDrawYOutside, diffuse, 0.5f+IDrawUPos2,0.5f+IDrawVPos2);
				IDrawV2=LDrawFace.addVertex(FDrawX+IDrawXPos2*FDrawXS,FDrawY-IDrawYPos2*FDrawYS, diffuse, 0.5f,0.5f);
				IDrawV3=LDrawFace.addVertex(FDrawX+IDrawXPos1*FDrawXS,FDrawY-IDrawYPos1*FDrawYS, diffuse, 0.5f,0.5f);
				//LDrawFace.addTriangle(IDrawV0,IDrawV1,IDrawV2);
				//LDrawFace.addTriangle(IDrawV2,IDrawV3,IDrawV0);
				LDrawFace.addTriIndices(IDrawV0,IDrawV2,IDrawV1);
				LDrawFace.addTriIndices(IDrawV2,IDrawV0,IDrawV3);
				LDrawFace.addTriIndices(IDrawVC,IDrawV2,IDrawV3);
				//Oval3D-Quartal-3/4-Mirror
				IDrawV0=LDrawFace.addVertex(FDrawX-IDrawXPos1*IDrawXOutside,FDrawY-IDrawYPos1*IDrawYOutside, diffuse, 0.5f-IDrawUPos1,0.5f+IDrawVPos1);
				IDrawV1=LDrawFace.addVertex(FDrawX-IDrawXPos2*IDrawXOutside,FDrawY-IDrawYPos2*IDrawYOutside, diffuse, 0.5f-IDrawUPos2,0.5f+IDrawVPos2);
				IDrawV2=LDrawFace.addVertex(FDrawX-IDrawXPos2*FDrawXS,FDrawY-IDrawYPos2*FDrawYS, diffuse, 0.5f,0.5f);
				IDrawV3=LDrawFace.addVertex(FDrawX-IDrawXPos1*FDrawXS,FDrawY-IDrawYPos1*FDrawYS, diffuse, 0.5f,0.5f);
				LDrawFace.addTriIndices(IDrawV0,IDrawV1,IDrawV2);
				LDrawFace.addTriIndices(IDrawV2,IDrawV3,IDrawV0);
				LDrawFace.addTriIndices(IDrawVC,IDrawV3,IDrawV2);
				//Oval3D-Quartal-4/4-Mirror
				IDrawV0=LDrawFace.addVertex(FDrawX-IDrawXPos1*IDrawXOutside,FDrawY+IDrawYPos1*IDrawYOutside, diffuse, 0.5f-IDrawUPos1,0.5f-IDrawVPos1);
				IDrawV1=LDrawFace.addVertex(FDrawX-IDrawXPos2*IDrawXOutside,FDrawY+IDrawYPos2*IDrawYOutside, diffuse, 0.5f-IDrawUPos2,0.5f-IDrawVPos2);
				IDrawV2=LDrawFace.addVertex(FDrawX-IDrawXPos2*FDrawXS,FDrawY+IDrawYPos2*FDrawYS, diffuse, 0.5f,0.5f);
				IDrawV3=LDrawFace.addVertex(FDrawX-IDrawXPos1*FDrawXS,FDrawY+IDrawYPos1*FDrawYS, diffuse, 0.5f,0.5f);
				LDrawFace.addTriIndices(IDrawV0,IDrawV2,IDrawV1);
				LDrawFace.addTriIndices(IDrawV2,IDrawV0,IDrawV3);
				LDrawFace.addTriIndices(IDrawVC,IDrawV2,IDrawV3);
			}
		}
		else
		{
			//Filled single-mode
			for (IDrawLoop=1;IDrawLoop<= IDrawSequences;IDrawLoop++)
			{
				////Vertex / Polygon Assignment / Calculations
				float IDrawXPos1=cosf(ToRadian(IDrawLoop*IDrawAngle));
				float IDrawYPos1=sinf(ToRadian(IDrawLoop*IDrawAngle));
				float IDrawXPos2=cosf(ToRadian(IDrawLoop*IDrawAngle-IDrawAngle));
				float IDrawYPos2=sinf(ToRadian(IDrawLoop*IDrawAngle-IDrawAngle));
				//Oval3D-Quartal-1/4-Mirror
				int IDrawV0=LDrawFace.addVertex(FDrawX+IDrawXPos1*FDrawXS,FDrawY+IDrawYPos1*FDrawYS, diffuse, 0.5f,0.5f);
				int IDrawV1=LDrawFace.addVertex(FDrawX+IDrawXPos2*FDrawXS,FDrawY+IDrawYPos2*FDrawYS, diffuse, 0.5f,0.5f);
				LDrawFace.addTriIndices(IDrawVC,IDrawV0,IDrawV1);
				//Oval3D-Quartal-2/4-Mirror
				IDrawV0=LDrawFace.addVertex(FDrawX+IDrawXPos1*FDrawXS,FDrawY-IDrawYPos1*FDrawYS, diffuse, 0.5f,0.5f);
				IDrawV1=LDrawFace.addVertex(FDrawX+IDrawXPos2*FDrawXS,FDrawY-IDrawYPos2*FDrawYS, diffuse, 0.5f,0.5f);
				LDrawFace.addTriIndices(IDrawVC,IDrawV1,IDrawV0);
				//Oval3D-Quartal-3/4-Mirror
				IDrawV0=LDrawFace.addVertex(FDrawX-IDrawXPos1*FDrawXS,FDrawY-IDrawYPos1*FDrawYS, diffuse, 0.5f,0.5f);
				IDrawV1=LDrawFace.addVertex(FDrawX-IDrawXPos2*FDrawXS,FDrawY-IDrawYPos2*FDrawYS, diffuse, 0.5f,0.5f);
				LDrawFace.addTriIndices(IDrawVC,IDrawV0,IDrawV1);
				//Oval3D-Quartal-4/4-Mirror
				IDrawV0=LDrawFace.addVertex(FDrawX-IDrawXPos1*FDrawXS,FDrawY+IDrawYPos1*FDrawYS, diffuse, 0.5f,0.5f);
				IDrawV1=LDrawFace.addVertex(FDrawX-IDrawXPos2*FDrawXS,FDrawY+IDrawYPos2*FDrawYS, diffuse, 0.5f,0.5f);
				LDrawFace.addTriIndices(IDrawVC,IDrawV1,IDrawV0);
			}
		}
	}

	GetGfx().appendOrDraw() ;
	it = nullptr ;
}

void Draw3DManager::DrawPoly(const Polygon2f& poly)
{
/*
	int n = poly.getCount() ;
	CIwSVec2* vertices = IW_GX_ALLOC(CIwSVec2,n+1) ;
	CIwColour* cols = IW_GX_ALLOC(CIwColour,n+1) ;
	for(int i=0; i<n; i++) {
		vertices[i].x = (int16_t)(poly.getVertex(i).x * Draw3DManager::RatioX) ;
		vertices[i].y = (int16_t)(-poly.getVertex(i).y  * Draw3DManager::RatioY) ;
		cols[i] = Draw3DManager::ColorN3D ;
	}
	vertices[n].x = (int16_t)(poly.getVertex(0).x * Draw3DManager::RatioX) ;
	vertices[n].y = (int16_t)(-poly.getVertex(0).y  * Draw3DManager::RatioY) ;
	cols[n] = Draw3DManager::ColorN3D ;

	Material* pMaterial = IW_GX_ALLOC_MATERIAL() ;
	IwGxSetMaterial(pMaterial) ;
	IwGxSetVertStreamScreenSpace(vertices,n+1) ;
	IwGxSetColStream(cols) ;
	IwGxSetUVStream(0) ;
	IwGxSetNormStream(0) ;
	IwGxDrawPrims(IW_GX_LINE_STRIP,0,n+1) ;
*/
}

void Draw3DManager::DrawCircle(const Circle2f& c, float a, int segs, bool drawLineToCenter/*=false*/)
{
/*
	int additionalSegment = 1;
	if (drawLineToCenter) {
		++additionalSegment;
	}

	const float coef = 2.0f * (float) (M_PI) /segs;

	iwsfixed* vertices = IW_GX_ALLOC(iwsfixed,2*segs+2); //	float *vertices = (float *)malloc( sizeof(float)*2*(segs+2));
	CIwColour* cols = IW_GX_ALLOC(CIwColour,segs+2) ;
	if( ! vertices || ! cols) { return; }

	const Vector2& center = c.getCenter() ;
	float r = c.getRadius() ;

	for(int i=0;i<=segs;i++) {
		float rads = i*coef;
		float j = r * cosf(rads + a) + center.x;
		float k = r * sinf(rads + a) + center.y;

		vertices[i*2]   =  (iwsfixed)(j * Draw3DManager::RatioX);
		vertices[i*2+1] = (iwsfixed)(-k * Draw3DManager::RatioY);
		cols[i] = Draw3DManager::ColorN3D ;
	}

	vertices[(segs+1)*2] = (iwsfixed)(center.x * Draw3DManager::RatioX);
	vertices[(segs+1)*2+1] = (iwsfixed)(-center.y * Draw3DManager::RatioY);
	cols[segs+1] = Draw3DManager::ColorN3D ;

	Material* pMaterial = IW_GX_ALLOC_MATERIAL() ;
	IwGxSetMaterial(pMaterial) ;
	IwGxSetVertStreamScreenSpace((CIwSVec2*)vertices,segs+additionalSegment) ;
	IwGxSetColStream(cols) ;
	IwGxSetUVStream(0) ;
	IwGxSetNormStream(0) ;
	IwGxDrawPrims(IW_GX_LINE_STRIP,0,segs+additionalSegment) ;
*/
}


// ************************************************************************
//GZ TODO: optimize using triangle-strips
void Draw3DManager::renderAppendLine3D( DrawItem* handle,std::vector<jam::Pivot2D*> & trails, size_t first, size_t last, float FDrawSize, bool firsttime/*=false*/, float x/*=0.0f*/, float y/*=0.0f*/ )
{
/*
	//std::vector<jam::Pivot2D*> ;
	Texture2D* pTexture = handle->getTexture();
	//jam::Rect srcRect  = handle->getRect();
	// texture or cut X-size
	float LDrawXSize ;
	float IDrawXTForm ;
	float IDrawYTForm ;
	float LDrawU1Map ;
	float LDrawV1Map ;
	float LDrawU2Map ;
	float LDrawV2Map ;
	Draw3DVertexBuffer&	surface = getVertexBuffer(handle,4,4,this->getSlotID(),IW_GX_QUAD_LIST) ;
	uint32_t diffuse = Draw3DManager::ColorG3D.getRgba() ;
	LDrawXSize = (float)pTexture->getWidth() ;
	LDrawU1Map = handle->getU1() ;
	LDrawV1Map = handle->getV1() ;
	LDrawU2Map = handle->getU2() ;
	LDrawV2Map = handle->getV2() ;
	unsigned short  v0,v1,v2,v3;
	int j=first;
	float segments=(float)trails.size();
	float colFactor=255.0f/segments;
	float startSize=0.1f; float endSize=FDrawSize;
	float stepsize=(endSize-startSize)/segments;
	FDrawSize=startSize;
	float FDrawX1=0.0f;float FDrawY1=0.0f;float FDrawX2=99999;float FDrawY2=99999;
	size_t nt = trails.size() ;

 	float startFDrawX1 =0; //trails[first]->getWorldPos().x; 
 	float startFDrawY1 = 0;//trails[first]->getWorldPos().y; 

	for (size_t i=0; i<nt; i++)
	{
		diffuse = jam::Color(255,255,255,(uint8_t)(i*colFactor/2.0f)).getRgba() ;
		Draw3DManager::ColorG3D.setRgba(diffuse);

		if (i>0)	
		{
			FDrawX1=FDrawX2; FDrawY1=FDrawY2;	
		}
		else 
		{
			startFDrawX1 = FDrawX2; 
			startFDrawY1 = FDrawY2; 
		}

		FDrawX2=trails[j]->getWorldPos().x; FDrawY2=trails[j]->getWorldPos().y;

		if(i>0) 
		{
			if (FDrawX1 != FDrawX2 || FDrawY1 != FDrawY2)
			{

				Vector2 n = Vector2(FDrawX2-FDrawX1,FDrawY2-FDrawY1) ;	n.normalizeSafe();//TODO: Qui genera un errore
				IDrawXTForm = n.x * FDrawSize;
				IDrawYTForm = n.y * FDrawSize;

				// 		 //Modus-Rollen
				// 		 if (FDrawMode==1)//GS:TBD doesn't work!
				// 		 {
				// 			 float IDrawLineSize=sqrtf((FDrawX1-FDrawX2)*(FDrawX1-FDrawX2)+(FDrawY1-FDrawY2)*(FDrawY1-FDrawY2));
				// 			 LDrawU2Map=LDrawU1Map+float(IDrawLineSize/float(pTexture->GetWidth()))/2.0f;
				// 		 }
				// 		 else if (FDrawMode==2) 	//Modus-Absolut
				// 		 {
				// 			 FDrawX2 = FDrawX1 + n.x*LDrawXSize*2;
				// 			 FDrawY2 = FDrawY1 + n.y*LDrawXSize*2;
				// 		 }

				// bottom left
				v0 = surface.addVertex(FDrawX1-IDrawYTForm,FDrawY1+IDrawXTForm, diffuse, LDrawU1Map,LDrawV1Map);
				// bottom right
				v1 = surface.addVertex(FDrawX2-IDrawYTForm,FDrawY2+IDrawXTForm, diffuse, LDrawU2Map,LDrawV1Map);
				// top right
				v2 = surface.addVertex(FDrawX2+IDrawYTForm,FDrawY2-IDrawXTForm, diffuse, LDrawU2Map,LDrawV2Map);
				// top left
				v3 = surface.addVertex(FDrawX1+IDrawYTForm,FDrawY1-IDrawXTForm, diffuse, LDrawU1Map,LDrawV2Map);

				surface.addQuadIndices(v0,v1,v2,v3) ;

				//appendOrDraw() ;
			}
			FDrawSize+=stepsize;

			// 				startFDrawX1 = trails[j]->getWorldPos().x; 
			// 				startFDrawY1 = trails[j]->getWorldPos().y; 

			j++; if (j>=(int)trails.size()) j=0;
			if (j==last) 
			{
				break;	// LAST ELEMENT;
			}
		}


	}

	FDrawX1 = startFDrawX1 ;
	FDrawY1 = startFDrawY1 ;
*/
}

}