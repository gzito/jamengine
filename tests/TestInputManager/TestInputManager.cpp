/**********************************************************************************
* 
* TestInputManager.cpp
* 
* This file is part of Jam
* 
* Copyright (c) 2014-2020 Giovanni Zito, Gianluca Sclano
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

#include <jam/Application.h>
#include <jam/Draw3dManager.h>
#include <jam/InputManager.h>
#include <jam/Camera.h>
#include <jam/Texture2D.h>
#include <jam/Scene.h>
#include <jam/SpriteBatch.h>
#include <jam/DrawItemManager.h>

#include <SDL_ttf.h>

#include <stdio.h>

using namespace jam;


class TestInputManagerApp : public jam::Application
{
public:
							TestInputManagerApp() = default ;
	virtual					~TestInputManagerApp() = default ;

protected:
	virtual bool			init() ;
	virtual void			render() ;
	virtual void			destroy() ;

private:
	void					loadFont() ;

private:
	TTF_Font*				m_pFont = nullptr;
	Texture2D*				m_pTexture = nullptr;
	Texture2D*				m_pFontTex = nullptr ;
	SpriteBatch*			m_pSpriteBatch = nullptr ;

};


void TestInputManagerApp::loadFont()
{
	TTF_Init() ;
	m_pFont = TTF_OpenFont( "konsystem.ttf", 12 );

	SDL_Color text_color = { 255, 255, 255, 255 };
	SDL_Surface* pSurface = TTF_RenderText_Blended( m_pFont, "Hello World!", text_color );	// ARGB8888
	m_pTexture = new (GC) Texture2D() ;
	m_pTexture->createFromSDLSurface(pSurface) ;
	SDL_FreeSurface(pSurface) ;
}

//*************************************************************************
bool TestInputManagerApp::init()
{
	Draw3DManager::Origin3D(960,540);

	m_pFontTex = GetDraw3DMgr().LoadFont3D( "media/Babylon1.png" ) ;

	m_pTexture = GetDrawItemMgr().loadTexture("media/star.bmp","psys")->getTexture() ;
	m_pSpriteBatch = new (GC) SpriteBatch(10) ;

	// set camera
	Camera* pCamera = new (GC) Camera() ;
	pCamera->setOrthographicProjection(	-Draw3DManager::VPWidth/2, Draw3DManager::VPWidth/2,
										-Draw3DManager::VPHeight/2, Draw3DManager::VPHeight/2,
										1.f, 50.f ) ;
	pCamera->lookAt( Vector3(0,0,30), Vector3(0,0,0), Vector3(0,1,0) ) ;
	getScene()->setCamera(pCamera) ;
	
	return true ;
}


void TestInputManagerApp::render()
{

/*
	m_pSpriteBatch->Begin() ;
	m_pSpriteBatch->Draw( m_pTexture, Vector2(0,0), nullptr, Color::RED, 0, Vector2(0,0), Vector2(1.0f,-1.0f), SpriteEffects::None, 0 ) ;
	m_pSpriteBatch->End(); 
*/

	static char lastEvent[50] = {0} ;

	char buf[256] = {0};
	I32 sx = -470;
	I32 sy = 250;

	float x = GetInputMgr().getPointerX() ;
	float y = GetInputMgr().getPointerY() ;
	InputManager::KeyStatus stateSelect = GetInputMgr().getPointerState(SDL_BUTTON_LEFT);
	InputManager::KeyStatus stateMiddle = GetInputMgr().getPointerState(SDL_BUTTON_MIDDLE);
	InputManager::KeyStatus stateRight = GetInputMgr().getPointerState(SDL_BUTTON_RIGHT);
	InputManager::KeyStatus stateA = GetInputMgr().getKeyState(SDL_SCANCODE_RETURN);
/*
	s3ePointerState stateWheelup = GetInputMgr().getPointerState(S3E_POINTER_BUTTON_MOUSEWHEELUP);
	s3ePointerState stateWheeldown = GetInputMgr().getPointerState(S3E_POINTER_BUTTON_MOUSEWHEELDOWN);
	s3ePointerState stateBtnMax = GetInputMgr().getPointerState(S3E_POINTER_BUTTON_MAX);
*/

	int stateTouch0 = GetInputMgr().getTouchState(0) ;
	I32 xt0 = GetInputMgr().getTouchX(0) ;
	I32 yt0 = GetInputMgr().getTouchY(0) ;

/*
	IwGxPrintSetColour(255,255,255) ;

	sprintf(buf,"Pointer type: %d - %d",GetInputMgr().getPointerType(),GetInputMgr().getPointerStylusType());
	IwGxPrintString(sx,sy,buf) ;
	sy+=16;
*/

//	sprintf(buf,"Pointer: %d %d %d %d %d %d %d %d",x,y,stateSelect, stateRight, stateMiddle, stateWheelup, stateWheeldown, stateBtnMax);
	sprintf( buf,"Pointer: %.0f %.0f",x,y );
	GetDraw3DMgr().Text3D( m_pFontTex, sx, sy, buf) ;
	sy-=32;

	sprintf( buf,"Left: %s", InputManager::keyStatusString(stateSelect).c_str() );
	GetDraw3DMgr().Text3D( m_pFontTex, sx, sy, buf) ;
	sy-=32;

	sprintf( buf,"Middle: %s", InputManager::keyStatusString(stateMiddle).c_str() );
	GetDraw3DMgr().Text3D( m_pFontTex, sx, sy, buf) ;
	sy-=32;

	sprintf( buf,"Right: %s", InputManager::keyStatusString(stateRight).c_str() );
	GetDraw3DMgr().Text3D( m_pFontTex, sx, sy, buf) ;
	sy-=64;

	if( GetInputMgr().isTouchPressed(0) ) {
		if( GetInputMgr().isTouchDoublePressed(0) ) {
			strcpy( lastEvent, " was double pressed" ) ;
		}
		else {
			strcpy( lastEvent, " was pressed" ) ;
		}
	}
	if( GetInputMgr().isTouchReleased(0) ) {
		strcpy( lastEvent, " was released" ) ;
	}
	sprintf(buf,"Touch #0: %d %d %s %s",xt0, yt0,  GetInputMgr().isTouchDown(0) ? "down" : "up", lastEvent );
	GetDraw3DMgr().Text3D( m_pFontTex, sx, sy, buf) ;
	sy-=32;

	sprintf(buf,"Key ENTER: %s", GetInputMgr().keyStatusString(stateA).c_str() );
	GetDraw3DMgr().Text3D( m_pFontTex, sx, sy, buf) ;
	sy-=32;

/*
	sprintf(buf,"Pointer coordinates (native coords): %d %d", s3ePointerGetTouchX(0), s3ePointerGetTouchY(0)) ;
	IwGxPrintString(sx,sy,buf) ;
	sy+=16;
*/
}


void TestInputManagerApp::destroy()
{
	if( m_pFont ) 
		TTF_CloseFont( m_pFont ); m_pFont = NULL;

	if( TTF_WasInit() )
		TTF_Quit();
}


//*************************************************************************

int main( int argc, char* argv[])
{
	jam::runEngine<TestInputManagerApp>() ;
	return 0 ;
}
