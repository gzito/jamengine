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

	Texture2D*					m_pFontTexture ;

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
	//Draw3DManager::Origin3D(800,600) ;
	//Draw3DManager::Origin3D(320*2,480*2) ;
	Draw3DManager::Origin3D(768,1024) ;

	// loaded inside SetupPSysResources
	//IwGetResManager()->LoadGroup("media/psys.group"); 
	ps = new PSysHelper;
	ps->Init();
	ps->SetupPSysResources("media/psys/");

	// slot 0 is for mouse
	//m_uiMgr.setSlot(2) ;
	//m_backMgr.setSlot(-1) ;

	m_pFontTexture = GetDraw3DMgr().LoadFont3D(m_fontname) ;

	// GZ TODO
//	GetDraw3DMgr().setRenderLevel(1) ;

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

	/*
	s3eSurfaceRegister(S3E_SURFACE_SCREENSIZE, (s3eCallback) UpdateScreenOrientation, NULL);
	UpdateScreenOrientation(0);
	*/
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
	Camera* pCamera = new (GC) Camera() ;
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
	/*
	int32 x = s3eAccelerometerGetX();
	int32 y = s3eAccelerometerGetY();
	int32 z = s3eAccelerometerGetZ();

	CIwVec3 r = s_OrientationCompensation.TransposeRotateVec(CIwVec3(x,y,z));

	//dWorldSetGravity (world,r.x/2.0f,-r.y/2.0f,-r.z/2.0f);
	*/


	Draw3DManager::ColorG3D = Color::WHITE ;
	// GZ TODO
//	GetDraw3DMgr().setRenderLevel(-4) ;
	GetDraw3DMgr().DrawImage3D(DrawItemManager::getSingleton().getObject("background1"),0,0);

	//*** Button Print


	//m_uiMgr.DrawImage3D(DrawItemManager::getSingleton().getItemById(51),x1,y1, -1, 0) ;

	float size = 1.0f ;

	if (m_showtime)
	{
		std::string name=m_partname[m_partType];
		float x1 = LEFT_SCREEN(16),  y1 = BOTTOM_SCREEN(128) ;

		GetDraw3DMgr().Text3D( m_pFontTexture,x1,y1, name+" "+ std::to_string(m_partType+1)+"/"+std::to_string((int)m_partname.size()) ) ;

		//m_txtMgr.addText(name,CX3D(64),CY3D(448),0,1,1);
		//m_txtMgr.addText("info1",name+std::string(" ")+toString(m_partType+1)+"/"+toString((int)listsize),LEFT_SCREEN(wi+4),y2,2000,1) ;
		//m_txtMgr.Text3D(m_pFontTexture,CX3D(0),CY3D(12),std::string("FPS: ") + ftos(getFps(),0), 0,0,0,0,1.5) ;
		//m_txtMgr.Update();

		if (lastOptimized<GetParticleSystem().getOptimized())
		{
			//m_txtMgr.addText("Optimized "+itos(GetParticleSystem().getOptimized()),CX3D(0),CY3D_BOTTOM_SCREEN(he*2),1000,1 ) ;
			lastOptimized=GetParticleSystem().getOptimized();
		}

		GetDraw3DMgr().ColorT3D = Color::WHITE;
		int sy = 16 ;
		GetDraw3DMgr().Text3D( m_pFontTexture,CX3D(0),CY3D(sy),"FPS: " + std::to_string(GetAppMgr().getFps()) ) ;
		sy += 24 ;
		//IwGxPrintFrameRate(0,0) ;
		GetDraw3DMgr().Text3D(m_pFontTexture,CX3D(0),CY3D(sy),"Particles: " + std::to_string( GetParticleSystem().totalParticles() ), 0,0,0,0) ;
		sy += 24 ;
		GetDraw3DMgr().Text3D(m_pFontTexture,CX3D(0),CY3D(sy),"Emitters: " + std::to_string( GetParticleSystem().totalEmitters() ), 0,0,0,0) ;
		sy += 24 ;

		char coors[256] = {0} ;
		sprintf(coors,"Coords: %d,%d",Draw3DManager::MouseX3D,Draw3DManager::MouseY3D) ;
		GetDraw3DMgr().Text3D(m_pFontTexture,CX3D(0),CY3D(sy), coors, 0,0,0,0) ;
		sy += 24 ;

		GetDraw3DMgr().Text3D(m_pFontTexture,CX3D(0),CY3D(sy), std::string("Batching: ") + ((GetGfx().getBatch()!=0) ?"ON":"OFF" ), 0,0,0,0) ;
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
		float wi=(GetDrawItemMgr().getObject("next-track")->getRect().getWidth())*Draw3DManager::RatioX;
		float he=(GetDrawItemMgr().getSingleton().getObject("next-track")->getRect().getHeight())*Draw3DManager::RatioY;

		float x1= LEFT_SCREEN(wi/2); float y1= TOP_SCREEN(he/2);

		//Button3DItem bMenu(m_uiMgr,&DrawItemManager::getSingleton().getById(52),x1,y1,0,size);
		//if (bMenu.isReleased())
		//{
		//	m_showtime=true;
		//}

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
	/*	CIwMat view;
	view = s_OrientationCompensation;
	view.t.z = -200;
	IwGxSetViewMatrix(&view);*/

	if (UpdateScreenOrientation())
	{
		Draw3DManager::Origin3D(Draw3DManager::Original3DWidth,Draw3DManager::Original3DHeight) ;
	}

	// GZ TODO
//	GetDraw3DMgr().setRenderLevel( 0 ) ;
	
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
#ifdef GZ_DISABLED_CODE
	//IwGxGetScreenOrient
	if (IwGxGetScreenOrient() != lastScreenOrient)
	{
		lastScreenOrient=IwGxGetScreenOrient() ;
		return true;
	}
	else
	{
		lastScreenOrient=IwGxGetScreenOrient() ;
		return false;
	}
#endif

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

/*
//... On initialization:

s3eSurfaceRegister(S3E_SURFACE_SCREENSIZE, (s3eCallback) UpdateScreenOrientation, NULL);
UpdateScreenOrientation(0,0);

//... On render:

CIwMat view;
view = s_OrientationCompensation;
view.t.z = -200;
IwGxSetViewMatrix(&view);

//... On each frame:

int32 x = s3eAccelerometerGetX();
int32 y = s3eAccelerometerGetY();
int32 z = s3eAccelerometerGetZ();

CIwVec3 r = s_OrientationCompensation.TransposeRotateVec(CIwVec3(x,y,z));

dWorldSetGravity (world,r.x/2.0f,-r.y/2.0f,-r.z/2.0f);

*/


//*************************************************************************
int main(int argc, char* argv[] ) 
{
	// Create application object
	jam::runEngine<PsysTestApp>() ;
	return 0 ;
}
