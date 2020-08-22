/**********************************************************************************
* 
* Draw3dManager.h
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

#ifndef __JAM_DRAW3DMANAGER_H__
#define __JAM_DRAW3DMANAGER_H__


#include <jam/jam.h>
#include <jam/Draw2d.h>
#include <jam/DrawItem.h>
#include <jam/Polygon2f.h>
#include <jam/Circle2f.h>
#include <jam/Color.h>
#include <jam/Draw3dBatch.h>
#include <jam/Singleton.h>
#include <jam/Pivot2d.h>
#include <jam/String.h>
//#include <jam/GPUProgram.h>

#include <map>


namespace jam
{

//
// Coordinate system
//	The default coordinate system in OpenGL is right-handed:
//	the positive x and y axes point right and up, 
//	and the negative z axis points forward (i.e. inside the screen).
//	Positive rotation is counterclockwise about the axis of rotation.
//

// ***************************************************************************
// ** Da piv-centrato a cartexiano top/left
#ifdef RESIZEBYANIG
#define CX_HALF_WIDTH		jam::Draw3DManager::HalfVPWidth
#define CX_HALF_HEIGHT		jam::Draw3DManager::HalfVPHeight
#else
#define CX_HALF_WIDTH		jam::Draw3DManager::HalfOriginal3DWidth
#define CX_HALF_HEIGHT		jam::Draw3DManager::HalfOriginal3DHeight
#endif


#define OX3D(Cx)			((Cx) + CX_HALF_WIDTH)
#define OY3D(Cy)			(-(Cy) + CX_HALF_HEIGHT)

// ** Da cartexiano top/left a piv-centrato
#define CX3D(Ox)			((Ox)-CX_HALF_WIDTH)
#define CY3D(Oy)			(-(Oy)+CX_HALF_HEIGHT)
// aliases
#define CX2PIV				CX3D
#define CY2PIV				CY3D
#define PIV2CX				OX3D
#define PIV2CY				OY3D

// ** Da cartexiano top/left a piv-centrato scalato (per il mouse)
#define CX2PIV_SCALED(Ox)	(((Ox)-jam::Draw3DManager::HalfVPWidth)/Draw3DManager::RatioX)
#define CY2PIV_SCALED(Oy)	((-(Oy)+jam::Draw3DManager::HalfVPHeight)/Draw3DManager::RatioY)


//			O(0,0) C(-512,384)
//			+---+--->
//			|   | C(0,0)
//			<---+--->
//			|	|	
//			V	V	O(800,600)
// ***************************************************************************

//** The next are deprecated
#define CX3D_LEFT_SCREEN(Oxx)		(((Oxx)/Draw3DManager::RatioX)-jam::Draw3DManager::HalfVPWidth)
#define CY3D_TOP_SCREEN(Oyy)		CY3D((Oyy))
#define CX3D_RIGHT_SCREEN(Oxx)		((CX_HALF_WIDTH)-(Oxx))
#define CY3D_BOTTOM_SCREEN(Oyy)		(Oyy)-(CX_HALF_HEIGHT)
//** Use these instead
#define LEFT_SCREEN(Oxx)	(((Oxx))-jam::Draw3DManager::ScaledHalfVPWidth)
#define TOP_SCREEN(Oyy)		((-(Oyy))+jam::Draw3DManager::ScaledHalfVPHeight)
#define RIGHT_SCREEN(Oxx)	((jam::Draw3DManager::ScaledHalfVPWidth)-(Oxx))
#define BOTTOM_SCREEN(Oyy)	((Oyy)-(jam::Draw3DManager::ScaledHalfVPHeight))

//class Draw3DVertexBuffer ;

class JAM_API Draw3DManager : public Singleton<Draw3DManager>
{
	friend class Singleton<Draw3DManager>;
	friend class Application ;
	friend class GridBase ;
	friend class Node ;

	using TFontsTilesMap = std::map<Texture2D*,std::vector<std::pair<int,int> > > ;
	
public:
	static int				MouseSpeedX3D;
	static int				MouseSpeedY3D;
	static int				LastMouseX3D;
	static int				LastMouseY3D;
	static int				MouseDown3D;	// Tasto del mouse tenuto premuto sull'oggetto
	static int				MouseOver3D;	// Puntatore del mouse sopra l'oggetto
	static int				MouseHit3D;		// Tasto del mouse premuto sull'oggetto. Coincide con il primo evento di MouseDown3D, poi viene subito azzerato al frame successivo
	static int				MousePit3D;		// Puntatore del mouse entrato sull'oggetto (corrisponde ad un evento di OnMouseEnter)
	static int				MouseUp3D;		// Non funziona/non usato
	static int				MouseX3D ;		// contains mouse x coordinate in the range -HalfScreenWidth <= x <= +HalfScreenWidth
	static int				MouseY3D ;		// contains mouse y coordinate in the range +HalfScreenWidth <= y <= -HalfScreenWidth
	static String			MouseText3D() { return m_mouseText3D; }
	static void				MouseText3D(const String& mt3d) { strcpy(m_mouseText3D,mt3d.c_str()); }

	static bool				MouseDown3DEventDetected ;
	static bool				MouseOver3DEventDetected ;

	static int				GDrawXSize ; //X-Skallierungsfaktor-Für-MouseX3D
	static int				GDrawYSize ; //Y-Skallierungsfaktor-Für-MouseY3D
	static int				GDrawMouseHit ; //Für-Eigene-MouseHit-Berechnung
	static String			GDrawMousePit() { return m_drawMousePit; };//Für-Eigene-MousePit-Berechnung
	static void				GDrawMousePit(const String& dmp ) { strcpy(m_drawMousePit,dmp.c_str()); }

	static float			VPWidth;
	static float			VPHeight ;
	static float			HalfVPWidth;
	static float			HalfVPHeight ;

	static float			ScaledHalfVPWidth ;
	static float			ScaledHalfVPHeight;
	static float			ScaledVPWidth ;
	static float			ScaledVPHeight;

	static float			RatioX ;
	static float			RatioY ;

	static bool				UseRatioScale ;			// when drawing use ratio scale, can be disable if set to false 

	static int				Original3DWidth ;
	static int				Original3DHeight ;
	static int				HalfOriginal3DWidth ;
	static int				HalfOriginal3DHeight ;

	static Color			ColorT3D ;
	static Color			ColorN3D ;
	static Color			ColorG3D ;

	static float			GDrawSFont; // Size scaling
	static float			GDrawHFont; // Scaling heights only
	static float			GDrawPFont; // Character spacing in pixels
	static float			GDrawIFont; // Italic in degrees (signed)

	static char				m_mouseText3D[255] ;
	static char				m_drawMousePit[255] ;

	// DrawAlign: specifies where to apply (FDrawX,FDrawY) on the text
	//	0 - The text start at (FDrawX,FDrawY)
	//	1 - The text will be centered on (FDrawX, FDrawY)
	//	2 - The text will end at (FDrawX, FDrawY)
	enum DrawAlign
	{
		DRAW_ALIGN_LEFT = 0,
		DRAW_ALIGN_CENTER = 1,
		DRAW_ALIGN_RIGHT = 2
	};

	//
	// Init routines
	//

	static void				Clear3D() ;
	static void				Origin3D(int w=-1, int h=-1);

	//
	// IMAGES / GRAPHICS
	// 

	void					VerifyQuad3D(DrawItem* handle, float FDrawX, float FDrawY, float FDrawAngle = 0.0f, float FDrawScale = 1.0f, float LHotSpotX = 0.0f, float LHotSpotY = 0.0f) ;

	void					DrawImage3D(const Ref<Texture2D>& pTexture, jam::Rect* pSrcRect, float FDrawX, float FDrawY,int FDrawButton = 0, float FDrawAngle = 0.0f,
											float FDrawScale = 1.0f, float LHotSpotX = 0.0f, float LHotSpotY = 0.0f, Color* color = 0, bool SwapU = false,	bool SwapV = false );

	void					DrawImage3D(DrawItem* handle, float FDrawX, float FDrawY,	int FDrawButton = 0, float FDrawAngle = 0.0f, float FDrawScale = 1.0f,
											float LHotSpotX = 0.0f, float LHotSpotY = 0.0f, Color* color = 0, bool SwapU = false, bool SwapV = false );

	void					DrawImage3D(DrawItem* handle, const Vector2& position, float FDrawAngle, const Vector2& scale, const Color& color );

	void					DrawQuad3D( const Polygon2f& poly, Color* color = 0 ) ;
	void					DrawQuad3D( Material* pMat, const jam::Rect& dstRect, Color* color = 0 ) ;
	void					DrawTexture( const Ref<Texture2D>& pTexture, const jam::Rect& dstRect, Color* color = 0, Shader* shader = 0 ) ;

	void					DrawImageTransform3D(DrawItem* handle, const Matrix3& transform = Matrix3(1.0f), int FDrawButton = 0,
											const Vector2& hotSpot = Vector2(0.0f),	Color* color = 0, bool SwapU = false, bool SwapV = false );

	void					DrawTransformedQuad3D(DrawItem* handle, const Polygon2f& poly, int FDrawButton = 0, Color* color = 0, bool SwapU = false, bool SwapV = false) ;

	/**	
		This command draws a texture as a line between two points, with different modes can be selected.
		The start and end coordinates work on the same principle as with the command line,
		only that they be counted in the Draw3D coordinate system.
		The line width determines how wide should be the texture in Draw3D coordinate system. 

		Mode =''0''means that the texture is stretched along the two coordinates,
		so that it always fits perfectly between the start and end point.
		Mode =''1''means that the texture between the start and end point 'rolled out' is.
		It always fits between the two points, but it is not stretched, but possibly drawn repeatedly.
		Mode =''2''means that the texture is shown in its full length, but will be targeted based on the line.
		So it begins at the starting point and will be drawn at the same angle as that of the line.
	*/
	void					ImageLine3D( DrawItem* handle,float FDrawX1,float FDrawY1,float FDrawX2,float FDrawY2,float FDrawSize,int FDrawMode );

	void					DrawTiledMapRow( DrawItem** items, unsigned int count, float FDrawX, float FDrawY, int tileXSize ) ;
	void					DrawTiledMap( DrawItem** items, int rows, int cols, float FDrawX, float FDrawY, int tileXSize, int tileYSize ) ;

	void					renderAppendLine3D( DrawItem* handle,std::vector<jam::Pivot2D*> & trails, size_t first, size_t last, float FDrawSize, bool firsttime=false, float x=0.0f, float y=0.0f);


	//
	// NATIVES
	//


	/**
		Draws a dot on the screen.

		This method draws a dot. It can be resized by 'FDrawSize' parameter
	
		\param pTexture - texture pointer or NULL to don't use texturing
		\param FDrawX - X position of the point
		\param FDrawY - Y position of the point
		\param FDrawX - Size of the point
		\param FDrawSize - Sign thickness of the point
	*/	 
	void					Plot3D( const Ref<Texture2D>& pTexture, float FDrawX, float FDrawY, float FDrawSize = 1 ) ;
	
	/**
		Draws a line on the screen.
	
		This method draws a line between the specified positions. The line width is determined by 'FDrawSize' parameter.

		\param pTexture - texture pointer or NULL to don't use texturing
		\param FDrawX1 - x-start coordinate of the line 
		\param FDrawY1 - y-start coordinate of the line 
		\param FDrawX2 - x-end coordinate of the line 
		\param FDrawY2 - y-end coordinate of the line 
		\param FDrawSize - Sign thickness of the line
	*/
	void					Line3D( const Ref<Texture2D>& pTexture, float FDrawX1,float FDrawY1,float FDrawX2,float FDrawY2,float FDrawSize = 2 ) ;
	void					Line3D( DrawItem* pItem, const Vector2& FDraw1, const Vector2& FDraw2, float FDrawSize = 2 ) ;
	void					Line3D( DrawItem* pItem, float FDrawX1,float FDrawY1,float FDrawX2,float FDrawY2,float FDrawSize = 2 ) ;
	
	/**
		Draws a rectangle on the screen.

		This method draws a rectangle.

		\param pTexture - texture pointer or NULL to don't use texturing
		\param FDrawX - x-center of the rectangle 
		\param FDrawY - y-center of the rectangle 
		\param FDrawXS - Half-width of the rectangle 
		\param FDrawYS - Half-height of the rectangle 
		\param FDrawFill - Fill mode: 0 - No fill, 1 - Filled
		\param FDrawSize - Frame Thickness. If (FDrawFill==0 and FDrawSize==0) then FDrawSize=1

		\remark It draw axis aligned rectangle only, i.e. it doesn't work for rotated rectangles
	*/
	void					Rect3D( const Ref<Texture2D>& pTexture, float FDrawX, float FDrawY, float FDrawXS, float FDrawYS, int FDrawFill=0, float FDrawSize=0 ) ;
	
	/**
		Draws a triangle on the screen.

		\param pTexture - texture pointer or NULL to don't use texturing
		\param FDrawX1 - First X-corner coordinates of the triangle 
		\param FDrawY1 - First Y-corner coordinates of the triangle 
		\param FDrawX2 - Second X-corner coordinates of the triangle 
		\param FDrawY2 - Second Y-corner coordinates of the triangle 
		\param FDrawX3 - Third X-corner coordinates of the triangle 
		\param FDrawY3 - Third Y-corner coordinates of the triangle 
	*/
	void					Poly3D( const Ref<Texture2D>& pTexture, float FDrawX1, float FDrawY1, float FDrawX2, float FDrawY2, float FDrawX3, float FDrawY3) ;
	
	/**
		Draws an oval on the screen.

		\param pTexture - texture pointer or NULL to don't use texturing
		\param FDrawX - X-center of the oval  
		\param FDrawY - Y-center of the oval  
		\param FDrawXS - Width of the oval  
		\param FDrawYS - Height of the oval  
		\param FDrawFill - Fill mode: 0 - No fill, 1 - Filled
		\param FDrawSize - Frame Thickness. If (FDrawFill==0 and FDrawSize==0) then FDrawSize=2
	*/
	void					Oval3D( const Ref<Texture2D>& pTexture, float FDrawX, float FDrawY, float FDrawXS, float FDrawYS, int FDrawFill=0, float FDrawSize=0 ) ;

	void					DrawPoly(const Polygon2f& poly) ;
	void					DrawCircle(const Circle2f& c, float a, int segs, bool drawLineToCenter=false) ;


	//
	// FONTS
	//

	uint32_t				getDraw3DTextTollerance() const { return m_draw3DTextTollerance; }
	void					setDraw3DTextTollerance(uint32_t val) { m_draw3DTextTollerance = val; }

	Ref<Texture2D>			LoadFont3D( const String& FDrawFile, int charstart = 0 , uint32_t tollerance=0, bool fixed=false, int width_min=0) ;

	int						Text3D(const String& drawItemName, float FDrawX, float FDrawY, const String& FDrastring, float FDrawAlign=0.0f, int FDrawButton=0, float FDrawAngle=0.0f, float FKerningWidth=0.0f, float FZoom=1.0f, float FZoomY=0, bool fastParse=true, float FKerningHeight=1.0f);
	int						Text3D(const Ref<Texture2D>& pTexture , float FDrawX, float FDrawY, const String& FDrastring, float FDrawAlign=0.0f, int FDrawButton=0, float FDrawAngle=0.0f, float FKerningWidth=0.0f, float FZoom=1.0f, float FZoomY=0, bool fastParse=true, float FKerningHeight=1.0f);
	int						Text3D(DrawItem* handle,float FDrawX, float FDrawY, const String& FDrastring, float FDrawAlign=0.0f, int FDrawButton=0, float FDrawAngle=0.0f, float FKerningWidth=0.0f, float FZoom=1.0f, float FZoomY=0, bool fastParse=true, float FKerningHeight=1.0f);

	int						StringWidth3D(const Ref<Texture2D>& pTexture ,const String& FDrastring) ;
	int						StringWidth3D(const String& drawItemName,const String& FDrastring) ;

protected:
	void					CheckQuad3D(float FDrawX1,float FDrawY1,float FDrawX2,float FDrawY2,float FDrawX3,float FDrawY3,float FDrawX4,float FDrawY4,
										int FDrawButton = 0, const String& FDrawOver="not set");

private:
							Draw3DManager();
	virtual					~Draw3DManager();

	int						getAscAtPos(const String& FDrastring,size_t* IDrawLoop, bool* isComplex);
	bool					XDrawTT3D(const Ref<Texture2D>& FDrawHandle,int FDrawXT ,int FDrawYT ,int FDrawXP ,int FDrawYP, int&, int& );

private:
	static TFontsTilesMap*	m_pFontsSizeMap ;
	uint32_t				m_draw3DTextTollerance;
};

JAM_INLINE Draw3DManager& GetDraw3DMgr() { return Draw3DManager::getSingleton(); }

}

#endif // __JAM_DRAW3DMANAGER_H__
