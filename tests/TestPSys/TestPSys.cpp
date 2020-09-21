/**********************************************************************************
* 
* TestPSys.cpp
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

#ifdef __i386__
#pragma warning (disable:4244)
#pragma warning (disable:4018)
#endif

#include "PSysUtilities.h"

#include <jam/core/bmkextras.hpp>
#include <jam/Application.h>
#include <jam/Draw3dManager.h>
#include <jam/Draw3dBatch.h>
#include <jam/DrawItemManager.h>
#include <jam/InputManager.h>
#include <jam/ButtonNode.h>
#include <jam/TextNode.h>
#include <jam/Layer.h>
#include <jam/Scene.h>
#include <jam/Camera.h>
#include <jam/Gfx.h>
#include <jam/Event.h>

#define Iw2DGetSurfaceWidth		IwGxGetScreenWidth
#define Iw2DGetSurfaceHeight	IwGxGetScreenHeight

using namespace jam;




class PsysTestApp : public Application
{
public:
	PsysTestApp() ;

protected:
	virtual void handleInput() ;
	virtual void afterSceneUpdate() ;
	virtual void render() ;

	virtual bool init() ;
	virtual void destroy() ;

	virtual void onButtonReleased( TouchEventArgs& args, IEventSource& src ) ;
	virtual void onBatchPressed( TouchEventArgs& args, IEventSource& src ) ;

private:
	float getFps( float rate = 1.0f ) ;

	//	long UpdateScreenOrientation(void*);
	bool UpdateScreenOrientation();
	PSysHelper*					ps ;
	//Text3DManager				m_txtMgr ;
	//Draw3DImageManager		m_backMgr;
	//Draw3DImageManager		m_uiMgr;
	//Draw3DManager				m_mouseMgr ;
	//UserInterfaceManager	m_GUI ;

	std::string					m_fontname;
	std::string					m_backgroundImageName;

	Ref<Texture2D>				m_pFontTexture ;

	Draw3DBatch*				m_pBatch ;

	int							m_partType;
	std::vector<std::string>	m_partname;
	int lastOptimized;

	bool  m_showtime;

#ifdef GZ_DISABLED_CODE
	CIwMat s_OrientationCompensation;
	IwGxScreenOrient lastScreenOrient;
#endif
};


//*************************************************************************

PsysTestApp::PsysTestApp() :
	m_pFontTexture(0),
	m_fontname("media/ui/Standard.png"),m_backgroundImageName("media/psys/Background1.png"),
	//m_backgroundImageName("LionDoor"),
	m_partType(0),
	ps(0),
	m_pBatch(0)
{
}

bool PsysTestApp::init()
{
	Draw3DManager::Origin3D(960,540) ;

	// loaded inside SetupPSysResources
	//IwGetResManager()->LoadGroup("media/psys.group"); 
	ps = new PSysHelper;
	ps->Init();
	ps->SetupPSysResources("media/psys/");

	m_pFontTexture = Ref<Texture2D>( GetDraw3DMgr().LoadFont3D(m_fontname), true ) ;

	// GZ TODO
	GetGfx().setRenderLevel(1) ;

	GetDrawItemMgr().loadTexture(m_backgroundImageName,"psys") ;		// 50
	//DrawItemManager::getSingleton().LoadTexture(m_backgroundImageName,"psys",60) ;

	GetDrawItemMgr().loadTexture("media/ui/previous-track.png","psys") ;		// 51
	GetDrawItemMgr().loadTexture("media/ui/next-track.png","psys") ;			// 52
	GetDrawItemMgr().loadTexture("media/ui/mouse2.png","psys") ;				// 53


	m_partname.clear();
	m_partname.push_back("BubbleEmission");
	m_partname.push_back("BigBubbleEmission");
	m_partname.push_back("IceEmission");
	m_partname.push_back("IceEmissionMini");
	m_partname.push_back("PieceStarEmission");
	m_partname.push_back("PieceStarEmission2");
	m_partname.push_back("SparkleEmission");
	m_partname.push_back("GlowEmission");
	m_partname.push_back("BlastEmission");
	m_partname.push_back("DustEmission");
	m_partname.push_back("MultiSparkle");
	m_partname.push_back("CircleSparkle");
	m_partname.push_back("LineCircleSparkle");
	m_partname.push_back("SparkleImplosion");
	m_partname.push_back("RainEmission");
	m_partname.push_back("RainbowEmission");
	m_partname.push_back("StainEmission");

	GetGfx().setClearColor( Color(0,0,0,0) ) ;
	lastOptimized=0;
	m_showtime=true;
	//m_txtMgr.addText("iPieces",0,0,4000,2,1 ) ;
	//m_txtMgr.addText("by IntensiveWorks 2010/2011",0,-50,3600,1,1 ) ;
	//m_txtMgr.addText("Version 1.3.1",0,-90,3000,.5,1 ) ;


// 	float wi=(DrawItemManager::getSingleton().getById(51).getRect().getWidth())*Draw3DManager::RatioX;
// 	float he=(DrawItemManager::getSingleton().getById(51).getRect().getHeight())*Draw3DManager::RatioY;
// 
// 	float x1= LEFT_SCREEN(wi/2); float y1= CY3D_BOTTOM_SCREEN(he/2);
// 	float x2= CX3D_RIGHT_SCREEN(wi/2); float y2= CY3D_BOTTOM_SCREEN(he/2);

	float x1 = RIGHT_SCREEN(220),  y1 = BOTTOM_SCREEN(64) ;
	float x2 = RIGHT_SCREEN(96), y2 = BOTTOM_SCREEN(64) ;

	Layer* uiLayer = new Layer() ;

	ButtonNode* b1 = new ButtonNode( DrawItemManager::getSingleton().getObject("previous-track") ) ;
	b1->setPos(Vector2(x1,y1)) ;
	b1->getTouchReleasedEvent() += EventHandler<TouchEventArgs>::create<PsysTestApp,&PsysTestApp::onButtonReleased>(this) ;
	b1->setTag("left") ;
	b1->setButtonScale(2.0f) ;
	uiLayer->addChild(b1) ;

	ButtonNode* b2 = new ButtonNode( DrawItemManager::getSingleton().getObject("next-track") ) ;
	b2->setPos(Vector2(x2,y2)) ;
	b2->getTouchReleasedEvent() += EventHandler<TouchEventArgs>::create<PsysTestApp,&PsysTestApp::onButtonReleased>(this) ;
	b2->setTag("right") ;
	b2->setButtonScale(2.0f) ;
	uiLayer->addChild(b2) ;

	TextNode* tn = TextNode::create();
	tn->setPos( Vector2(CX3D(0),CY3D(150)) ) ;
	tn->setText("Standard","Change batch mode",Draw3DManager::DRAW_ALIGN_LEFT,1) ;
	tn->getTouchPressedEvent() += EventHandler<TouchEventArgs>::create<PsysTestApp,&PsysTestApp::onBatchPressed>(this) ;
	//tn->enableCheckQuad(true) ;
	tn->setTag("batch") ;
	uiLayer->addChild(tn) ;

	getScene()->addChild(uiLayer,5) ;

	m_pBatch = GetGfx().getBatch() ;
	GetGfx().setBatch(nullptr) ;

	// set camera
	Camera* pCamera = new Camera() ;
	pCamera->setOrthographicProjection(	-Draw3DManager::VPWidth/2, Draw3DManager::VPWidth/2,
										-Draw3DManager::VPHeight/2, Draw3DManager::VPHeight/2,
										1.f, 50.f ) ;
	pCamera->lookAt( Vector3(0,0,30), Vector3(0,0,0), Vector3(0,1,0) ) ;
	getScene()->setCamera(pCamera) ;

	return true ;
}


void PsysTestApp::destroy() 
{
	//SAFE_DELETE(m_pFontTexture);
	//IWGE_DELETE(m_pFontTexture) ;
//	m_txtMgr.end();
	ps->DestroyPSys();
	JAM_DELETE(ps) ;
	//IwGetResManager()->DestroyGroup("psys") ;

}


void PsysTestApp::handleInput()
{
	if( GetInputMgr().keyPressed(SDL_SCANCODE_SPACE) ) {
		Draw3DBatch* batch = GetGfx().getBatch() ;
		if( batch ) {
			GetGfx().setBatch(nullptr) ;
		}
		else {
			GetGfx().setBatch(m_pBatch) ;
		}
	}
}

void PsysTestApp::onButtonReleased( TouchEventArgs& args, IEventSource& src )
{
	Node& n = dynamic_cast<Node&>(src) ;
	
	if( n.getTag() == "left" )
	{
		m_partType-- ; 
		if (m_partType<0) m_partType=m_partname.size()-1 ;
	}
	else if( n.getTag() == "right" )
	{
		m_partType++ ; 
		if (m_partType>(m_partname.size()-1)) m_partType=0 ;	
	}
}

void PsysTestApp::afterSceneUpdate()
{
	char buffer[256];
	Draw3DManager::ColorG3D = Color::WHITE ;

	float size = 1.0f ;

	if (m_showtime)
	{
		const std::string name=m_partname[m_partType];
		const float x1 = LEFT_SCREEN(16);
		const float y1 = BOTTOM_SCREEN(128) ;

		GetDraw3DMgr().Text3D( m_pFontTexture,x1,y1, name+" "+ std::to_string(m_partType+1)+"/"+std::to_string((int)m_partname.size()) ) ;

		if (lastOptimized<GetParticleSystem().getOptimized())
		{
			//m_txtMgr.addText("Optimized "+itos(GetParticleSystem().getOptimized()),CX3D(0),CY3D_BOTTOM_SCREEN(he*2),1000,1 ) ;
			lastOptimized=GetParticleSystem().getOptimized();
		}

		GetDraw3DMgr().ColorT3D = Color::WHITE;
		int sy = 16 ;
		sprintf(buffer, "FPS: %0.2f", GetAppMgr().getFps());
		GetDraw3DMgr().Text3D( m_pFontTexture,CX3D(0),CY3D(sy),buffer) ;
		sy += 24 ;
		//IwGxPrintFrameRate(0,0) ;
		sprintf(buffer, "Particles: %d", GetParticleSystem().totalParticles());
		GetDraw3DMgr().Text3D(m_pFontTexture,CX3D(0),CY3D(sy),buffer, 0,0,0,0) ;
		sy += 24 ;

		sprintf(buffer, "Emitters: %d" , GetParticleSystem().totalEmitters());
		GetDraw3DMgr().Text3D(m_pFontTexture,CX3D(0),CY3D(sy),buffer, 0,0,0,0) ;
		sy += 24 ;

		sprintf(buffer,"Coords: %d,%d",Draw3DManager::MouseX3D,Draw3DManager::MouseY3D) ;
		GetDraw3DMgr().Text3D(m_pFontTexture,CX3D(0),CY3D(sy), buffer, 0,0,0,0) ;
		sy += 24 ;

		sprintf(buffer, "Batching: %s", GetGfx().getBatch() != nullptr ? "ON" : "OFF");
		GetDraw3DMgr().Text3D(m_pFontTexture,CX3D(0),CY3D(sy), buffer, 0,0,0,0) ;
		sy += 24 ;

		//float x3= CX3D_RIGHT_SCREEN(wi/2); float y3= TOP_SCREEN(he/2);

		//Button3DItem bMenu(m_uiMgr,&DrawItemManager::getSingleton().getById(51),x3,y3,0,size);
		//if (bMenu.isReleased())
		//{
		//	m_showtime=false;
		//}
	}
	else
	{
		const float wi=(GetDrawItemMgr().getObject("next-track")->getRect().getWidth())*Draw3DManager::RatioX;
		const float he=(GetDrawItemMgr().getSingleton().getObject("next-track")->getRect().getHeight())*Draw3DManager::RatioY;

		float x1= LEFT_SCREEN(wi/2); float y1= TOP_SCREEN(he/2);
	}

	if( !Draw3DManager::MouseDown3DEventDetected ) {
		for(U32 i = 0; i < JAM_MAX_TOUCHES; i++)
		{
			// For each active touch
			int touchState = GetInputMgr().getTouchState(i) ;

			if( touchState == InputManager::KeyStatus::DOWN )
			{
				float mx = GetInputMgr().getTouchX(i);
				float my = GetInputMgr().getTouchY(i);
				switch (m_partType)
				{
				case 0:
					ps->CreateBubbleEmission(mx,my) ;
					break;
				case 1:
					ps->CreateBigBubbleEmission(mx,my) ;
					break;
				case 2:
					ps->CreateIceEmission(mx,my) ;
					break;
				case 3:
					ps->CreateIceEmissionMini(mx,my) ;
					break;
				case 4:
					ps->CreatePieceStarEmission( mx,my) ;
					break;
				case 5:
					ps->CreatePieceStarEmission2( mx,my) ;
					break;
				case 6:
					ps->CreateSparkleEmission( mx,my) ;
					break;
				case 7:
					ps->CreateGlowEmission( mx,my) ;
					break;
				case 8:
					ps->CreateBlastEmission( mx,my) ;
					break;
				case 9:
					ps->CreateDustEmission( mx,my) ;
					break;
				case 10:
					ps->CreateMultiSparkleEmission( mx,my) ;
					break;
				case 11:
					ps->CreateCircleSparkleEmission( mx,my) ;
					break;
				case 12:
					ps->LineCircleSparkleEmission( mx,my) ;
					break;
				case 13:
					ps->CreateSparkleImplosionEmission( mx,my) ;
					break;
				case 14:
					ps->CreateRainEmission( mx,my) ;
					break;	
				case 15:
					ps->CreateRainbowEmission( mx,my,GetInputMgr().getTouchSpeedY(i), GetInputMgr().getTouchSpeedX(i)) ;
					break;
				case 16:
					ps->CreateStainEmission( mx,my, 1,2000) ;
					break;
				}
			} // if( g_Touches[i].active )
		}  // for MAX_TOUCHES
	}

	// TODO setSlot for psys
	GetParticleSystem().update();
	
}

void PsysTestApp::render()
{
	//if (UpdateScreenOrientation())
	//{
	//	Draw3DManager::Origin3D(Draw3DManager::Original3DWidth,Draw3DManager::Original3DHeight) ;
	//}

	GetGfx().setRenderLevel( 1 ) ;
	GetDraw3DMgr().DrawImage3D(DrawItemManager::getSingleton().getObject("background1"),0,0);

	GetGfx().setRenderLevel( 0 ) ;
	Draw3DBatch* pBatch = GetGfx().getBatch() ;
	if( pBatch ) pBatch->begin() ;
	GetParticleSystem().updateRender();
	if( pBatch ) pBatch->end() ;
}


float PsysTestApp::getFps( float rate /*= 1.0f*/ )
{
	static U16 framesCount  = 0 ;
	static float totalElapsed = 0.0f ;
	static float result = 0.0f ;

	float elapsed = getElapsed();

	// Update the frame rate
	if( totalElapsed > rate ) {
		result = framesCount / totalElapsed ;
		framesCount = 0 ;
		totalElapsed = 0.0f ;
	}
	else {
		totalElapsed += elapsed ;
		framesCount++ ;
	}

	return result ;
}


//*************************************************************************
bool PsysTestApp::UpdateScreenOrientation()
{
	return false ;
}

void PsysTestApp::onBatchPressed( TouchEventArgs& args, IEventSource& src )
{
	Draw3DBatch* batch = GetGfx().getBatch() ;
	if( batch ) {
		GetGfx().setBatch(nullptr) ;
	}
	else {
		GetGfx().setBatch(m_pBatch) ;
	}
}


//*************************************************************************
int main(int argc, char* argv[] ) 
{
	// Create application object
	jam::runEngine<PsysTestApp>() ;
	return 0 ;
}
