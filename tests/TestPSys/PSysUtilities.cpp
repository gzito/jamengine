#include "PSysUtilities.h"
#include <jam/Draw3dManager.h>
#include <jam/DrawItemManager.h>
#include <jam/core/bmkextras.hpp>
#include <PSys.h>

#define PIGRECO			3.14159265358979323846
#define RADK			0.017453292519943295769222222222222f
#define KRAD			57.295779513082320876846364344191f
#define SIN(X)		  sin((X) * (RADK))
#define COS(X)		  cos((X) * (RADK))


using namespace jam ;

void PSysHelper::Init()
{
	//GetParticleSystem()=PSYS();
	//	SetupPSysResources();
	//*** create Particle template
	//, BubblePartEmitter(0), StarPartEmitter(0),IcePartEmitter(0),SparklePartEmitter(0)
	IcePoolIndex=0;
	StarPoolIndex=0;
	BubblePoolIndex=0;
	SparklePoolIndex=0;

	// GZ TODO
//	GetDraw3DMgr().setRenderLevel(0);
}

//***********************************************************************************************
void PSysHelper::SetupPSysResources( const String& path )
{
	int PSYS_ID=500;
	if (!StarParticleImage) StarParticleImage=GetDrawItemMgr().loadTexture(path + "star3.png","psys")->getTexture();			// 500
	if (!BubbleParticleImage) BubbleParticleImage=GetDrawItemMgr().loadTexture(path + "bubble.png","psys")->getTexture();		// 501
	if (!IceParticleImage) IceParticleImage=GetDrawItemMgr().loadTexture(path + "ice.png","psys")->getTexture();				// 502
	if (!SparkleParticleImage) SparkleParticleImage=GetDrawItemMgr().loadTexture(path + "Sparkle.bmp","psys")->getTexture();	// 503
	if (!GlowParticleImage) GlowParticleImage=GetDrawItemMgr().loadTexture(path + "glow.bmp","psys")->getTexture();				// 504
	if (!BlastParticleImage) BlastParticleImage=GetDrawItemMgr().loadTexture(path + "Blast.png","psys")->getTexture();			// 505
	if (!DustParticleImage) DustParticleImage=GetDrawItemMgr().loadTexture(path + "Debris.png","psys")->getTexture();			// 506
	if (!RippleParticleImage) RippleParticleImage=GetDrawItemMgr().loadTexture(path + "ripple2.png","psys")->getTexture();		// 507
	if (!RainParticleImage) RainParticleImage=GetDrawItemMgr().loadTexture(path + "rain.png","psys")->getTexture();				// 508
	if (!ThunderParticleImage) ThunderParticleImage=GetDrawItemMgr().loadTexture(path + "thunder.png","psys")->getTexture();	// 509
	if (!StarSparkleParticleImage) StarSparkleParticleImage=GetDrawItemMgr().loadTexture(path + "star.bmp","psys")->getTexture();	// 510
	if (!BigDustParticleImage) BigDustParticleImage=GetDrawItemMgr().loadTexture(path + "Smoke2.png","psys")->getTexture();		// 511
	if (!JellySprite) JellySprite=GetDrawItemMgr().loadTexture(path + "jelly.png","psys")->getTexture();						// 512
	if (!StainParticleImage) StainParticleImage=GetDrawItemMgr().loadTexture(path + "stain.png","psys")->getTexture();			// 513
	if (!DragonSprite) DragonSprite=GetDrawItemMgr().loadTexture(path + "Dragon.png","psys")->getTexture();						// 514

	for (int t=0; t<=MAX_MARKED; t++)
	{
		StarPartEmitter[t]=new Particle3DConfigurator();
		StarPartEmitter[t]->duration=50; StarPartEmitter[t]->loops=1; StarPartEmitter[t]->durationRange=50;		
		StarPartEmitter[t]->entity=StarParticleImage;
	}
	for (int t=0; t<=MAX_BUBBLES; t++)
	{
		BubblePartEmitter[t]=new Particle3DConfigurator();
		BubblePartEmitter[t]->duration=50; BubblePartEmitter[t]->loops=1; BubblePartEmitter[t]->durationRange=50;
		BubblePartEmitter[t]->entity=BubbleParticleImage;
	}
	for (int t=0; t<=MAX_ICES; t++)
	{
		IcePartEmitter[t]=new Particle3DConfigurator();
		IcePartEmitter[t]->duration=190; IcePartEmitter[t]->loops=1; IcePartEmitter[t]->durationRange=90;
		IcePartEmitter[t]->entity=IceParticleImage;
	}
	for (int t=0; t<=MAX_SPARKLES; t++)
	{
		SparklePartEmitter[t]=new Particle3DConfigurator();
		SparklePartEmitter[t]->duration=190; SparklePartEmitter[t]->loops=1; SparklePartEmitter[t]->durationRange=90;
		SparklePartEmitter[t]->entity=SparkleParticleImage;
	}
	
}
void PSysHelper::ClearPSys()
{
	GetParticleSystem().clearAll();
}

void PSysHelper::DestroyPSys()
{
	for (int t=0; t<=MAX_MARKED; t++){	JAM_DELETE(StarPartEmitter[t]);	}
	for (int t=0; t<=MAX_BUBBLES; t++){	JAM_DELETE(BubblePartEmitter[t]);}
	for (int t=0; t<=MAX_ICES; t++){	JAM_DELETE(IcePartEmitter[t]);	}
	for (int t=0; t<=MAX_SPARKLES; t++){	JAM_DELETE(SparklePartEmitter[t]);	}

	/*	delete[](BubblePartEmitter);
	delete[]( StarPartEmitter);
	delete[]( IcePartEmitter);
	delete[]( SparklePartEmitter);*/

	//	GetParticleSystem().clearAll();

	GetParticleSystem().destroySingleton();

}

PSysHelper::PSysHelper(): 
	StarPoolIndex(0),BubblePoolIndex(0),SparklePoolIndex(0),
	StarParticleImage(0),BubbleParticleImage(0),IceParticleImage(0),StarSparkleParticleImage(0),
	SparkleParticleImage(0),GlowParticleImage(0),BlastParticleImage(0),BigDustParticleImage(0),
	DustParticleImage(0),RippleParticleImage(0),RainParticleImage(0), ThunderParticleImage(0),
	JellySprite(0),StainParticleImage(0),DragonSprite(0)
{
	Init();
}

PSysHelper::~PSysHelper()
{
	DestroyPSys();
	//	ClearPSys();
}

void PSysHelper::Begin()
{
	ClearPSys();
	IcePoolIndex=0;
	StarPoolIndex=0;
	BubblePoolIndex=0;
	SparklePoolIndex=0;
}


//***********************************************************************************************
void PSysHelper::CreateBubbleEmission(float x, float y)
{
	CreateJellyEmission(x,y);
	//*** Create Bubble particle fx 
	BubblePartEmitter[BubblePoolIndex]->duration=2000; 
	BubblePartEmitter[BubblePoolIndex]->loops=1; BubblePartEmitter[BubblePoolIndex]->durationRange=90;
	BubblePartEmitter[BubblePoolIndex]->setPosition(paramRange(x,0,0),paramRange(x,-50,50), paramRange(y,-10,10),paramRange(y,00,-60), paramRange(0,0,0),paramRange(0,0,0));
	BubblePartEmitter[BubblePoolIndex]->setZoom(paramRange(1.2,0.1,0.1),paramRange(1.8,0,0));
	BubblePartEmitter[BubblePoolIndex]->setBlend(BlendMode::AlphaBlend);
	BubblePartEmitter[BubblePoolIndex]->setTransparency(paramRange(1,0,0),paramRange(0,0,0));
	BubblePartEmitter[BubblePoolIndex]->setRotation(paramRange(0,-20,20),paramRange(0,-360,360));
		
	PSysEmitter * pe = GetParticleSystem().CreateEmitter(BubblePartEmitter[BubblePoolIndex],"CreateBubbleEmission");
	
	PSysSetWind(pe,PSYS_NULL,100.0f,PSYS_NULL);
	
	PSysSetOptimizationPool(pe,40,100);
	pe->CreateEmission(1,1);
	BubblePoolIndex++; if (BubblePoolIndex>MAX_BUBBLES) BubblePoolIndex=0;	
}

//***********************************************************************************************
void PSysHelper::CreateBigBubbleEmission(float x, float y)
{
	//*** Create Bubble particle fx 
	BubblePartEmitter[BubblePoolIndex]->duration=1450; 
	BubblePartEmitter[BubblePoolIndex]->setPosition(paramRange(x,-20,20),paramRange(x,-80,80), paramRange(y,-20,20),paramRange(y,00,-330), paramRange(0,0,0),paramRange(0,0,0));
	BubblePartEmitter[BubblePoolIndex]->setZoom(paramRange(1.5,0.1,0.1),paramRange(0.5,.1,.2));
	BubblePartEmitter[BubblePoolIndex]->setBlend(BlendMode::Additive);
	BubblePartEmitter[BubblePoolIndex]->setTransparency(paramRange(1,0,0),paramRange(0,0,0));
	BubblePartEmitter[BubblePoolIndex]->setRotation(paramRange(0,-20,20),paramRange(0,-360,360));

	PSysEmitter * pe = GetParticleSystem().CreateEmitter(BubblePartEmitter[BubblePoolIndex],"CreateBigBubbleEmission");
	PSysSetWind(pe,PSYS_NULL,-100.0f,PSYS_NULL);
	PSysSetOptimizationPool(pe,240,320);
	PSysSetOptimizationFrameRate(pe,"CreateBigBubbleEmission",10);
	pe->CreateEmission(10,1);
	BubblePoolIndex++; if (BubblePoolIndex>MAX_BUBBLES) BubblePoolIndex=0;	
}

//***********************************************************************************************
void PSysHelper::CreateIceEmission(float x, float y)
{
	CreateSingleDustEmission(x,y);
	//*** Create Bubble particle fx 
	IcePartEmitter[IcePoolIndex]->setPosition(paramRange(x,-64,64),paramRange(x,-128,128), paramRange(y,-40,40), paramRange(y,-100,250),paramRange(0,0,0),paramRange(0,0,0));
	IcePartEmitter[IcePoolIndex]->setZoom(paramRange(1,0.04,0.04),paramRange(.25,0,0));
	IcePartEmitter[IcePoolIndex]->duration=1900; 
	IcePartEmitter[IcePoolIndex]->loops=1; IcePartEmitter[IcePoolIndex]->durationRange=90;
	IcePartEmitter[IcePoolIndex]->setBlend(BlendMode::AlphaBlend);
	IcePartEmitter[IcePoolIndex]->setColor(paramSettingsColor(1,1),paramSettingsColor(1,1), paramSettingsColor(1,1));
	IcePartEmitter[IcePoolIndex]->setTransparency(paramRange(.4,-.4,.4),paramRange(.1,0,0));
	IcePartEmitter[IcePoolIndex]->setRotation(paramRange(0,-60,60),paramRange(0,-360,360));
	IcePartEmitter[IcePoolIndex]->entity=IceParticleImage;
	PSysEmitter * pe = GetParticleSystem().CreateEmitter(IcePartEmitter[IcePoolIndex],"CreateIceEmission");
	PSysSetWind(pe,PSYS_NULL,-100.0f,PSYS_NULL);
	//PSysSetOptimizationPool(pe,120,350);
	PSysSetOptimizationFrameRate(pe,"CreateIceEmission",200);
	//BubblePartEmitter[BubblePoolIndex]->setName("CreateBubbleEmission");
	pe->CreateEmission(8,1);
	IcePoolIndex++; if (IcePoolIndex>MAX_ICES) IcePoolIndex=0;	
}
//***********************************************************************************************
void PSysHelper::CreateIceEmissionMini( float x, float y, float sz/*=2.0f*/, bool bRandom/*=true*/ )
{
	float dx1=16;
	float dx2=256;
	float dy1=16;
	float dy2=256;

	//*** Create Bubble particle fx 
	IcePartEmitter[IcePoolIndex]->setPosition(paramRange(x,-dx1,dx1),paramRange(x,-dx2,dx2), paramRange(y,-dy1,dy1), paramRange(y,-100,dy2),paramRange(0,0,0),paramRange(0,0,0));
	IcePartEmitter[IcePoolIndex]->setZoom(paramRange(sz,.5,.5),paramRange(sz/8.0f,0,0));
	IcePartEmitter[IcePoolIndex]->duration=1400; IcePartEmitter[IcePoolIndex]->loops=1; IcePartEmitter[IcePoolIndex]->durationRange=600;
	IcePartEmitter[IcePoolIndex]->setBlend(BlendMode::Additive);
	IcePartEmitter[IcePoolIndex]->setTransparency(paramRange(1,-.4,.4),paramRange(.1,0,0));
	IcePartEmitter[IcePoolIndex]->setRotation(paramRange(0,-60,60),paramRange(0,-360,360));
	IcePartEmitter[IcePoolIndex]->entity=DustParticleImage;
	
	int i=0;
	if (bRandom)
	{
		int i = RANGERAND(0,4);
		if (i==0)
			IcePartEmitter[IcePoolIndex]->setColor(paramSettingsColor(0,1),paramSettingsColor(0,1), paramSettingsColor(1,1));
		else if (i==1)
			IcePartEmitter[IcePoolIndex]->setColor(paramSettingsColor(1,1),paramSettingsColor(0,1), paramSettingsColor(0,1));
		else if (i==2)
			IcePartEmitter[IcePoolIndex]->setColor(paramSettingsColor(0,1),paramSettingsColor(1,1), paramSettingsColor(0,1));
		else if (i==3)
			IcePartEmitter[IcePoolIndex]->setColor(paramSettingsColor(1,1),paramSettingsColor(1,1), paramSettingsColor(0,1));
	}
	else
	{
		IcePartEmitter[IcePoolIndex]->setColor(paramSettingsColor(1,1),paramSettingsColor(1,0), paramSettingsColor(0,0));	
	}

	PSysEmitter * pe = GetParticleSystem().CreateEmitter(IcePartEmitter[IcePoolIndex],"CreateIceEmissionMini");
	PSysSetWind(pe,PSYS_NULL,-100,PSYS_NULL);


	pe->CreateEmission(3,1);
	IcePoolIndex++; if (IcePoolIndex>MAX_ICES) IcePoolIndex=0;	
}

//***********************************************************************************************
void PSysHelper::CreateDustEmission( float x, float y ,int howmany )
{
	//*** Create star particle fx 
	StarPartEmitter[StarPoolIndex]->duration=1500; 
	StarPartEmitter[StarPoolIndex]->setPosition(paramRange(x,-16,16),paramRange(x,-64,64), paramRange(y,-2,2),paramRange(y+64,-2,2), paramRange(0,0,0),paramRange(0,0,0));
	StarPartEmitter[StarPoolIndex]->setZoom(paramRange(0.5,-.01,.01),paramRange(4, -.1,.1));
	StarPartEmitter[StarPoolIndex]->setBlend(BlendMode::Additive);
	StarPartEmitter[StarPoolIndex]->setColor(paramSettingsColor(1,1),paramSettingsColor(1,1), paramSettingsColor(1,1));
	StarPartEmitter[StarPoolIndex]->setTransparency(paramRange(1,0,0),paramRange(0,0,0));
	StarPartEmitter[StarPoolIndex]->setRotation(paramRange(0,-20,20),paramRange(0,-360,360));
	StarPartEmitter[StarPoolIndex]->entity=DustParticleImage;

	PSysEmitter * pe = GetParticleSystem().CreateEmitter(StarPartEmitter[StarPoolIndex],"CreateDustEmission");
	PSysSetWind(pe,PSYS_NULL,-90.0f,PSYS_NULL);
	pe->CreateEmission(6,1);
	StarPoolIndex++; if (StarPoolIndex>MAX_MARKED) StarPoolIndex=0;
}
void PSysHelper::CreateSingleDustEmission( float x, float y  )
{
	//*** Create star particle fx 
	float a=8.0f;
	StarPartEmitter[StarPoolIndex]->duration=3700; 
	//StarPartEmitter[StarPoolIndex]->setPosition(paramRange(x,-a,a),PSYS_NULL, paramRange(y,-a,a),PSYS_NULL, paramRange(0,0,0),paramRange(0,0,0));
	StarPartEmitter[StarPoolIndex]->setPosition(paramRange(x,-a,a),paramRange(x,-a,a), paramRange(y,-a,a),paramRange(y,-a,a), paramRange(0,0,0),paramRange(0,0,0));
	//StarPartEmitter[StarPoolIndex]->setPosition(paramRange(x,-30,30),paramRange(x,-200,200), paramRange(y,-30,30),paramRange(y,-100,250), paramRange(0,0,0),paramRange(0,0,0));
	StarPartEmitter[StarPoolIndex]->setZoom(paramRange(2.0,-.01,.01),paramRange(4.5, -.1,.1));
	StarPartEmitter[StarPoolIndex]->setBlend(BlendMode::AlphaBlend);
	StarPartEmitter[StarPoolIndex]->setColor(paramSettingsColor(.6,1),paramSettingsColor(.6,1), paramSettingsColor(.6,1));
	StarPartEmitter[StarPoolIndex]->setTransparency(paramRange(.3,0,0),paramRange(0,0,0));
	StarPartEmitter[StarPoolIndex]->setRotation(paramRange(0,-360,360), PSYS_NULL);
	StarPartEmitter[StarPoolIndex]->entity=BigDustParticleImage;

	PSysEmitter * pe = GetParticleSystem().CreateEmitter(StarPartEmitter[StarPoolIndex],"CreateSingleDustEmission");
	PSysSetWind(pe,PSYS_NULL,PSYS_NULL,PSYS_NULL);
	pe->CreateEmission(1,1);
	StarPoolIndex++; if (StarPoolIndex>MAX_MARKED) StarPoolIndex=0;
}
//***********************************************************************************************
void PSysHelper::CreatePieceStarEmission( float x, float y, float zz/*=1.0f*/,int howmany )
{
	//*** Create star particle fx 
	StarPartEmitter[StarPoolIndex]->duration=2000; 
	StarPartEmitter[StarPoolIndex]->setPosition(paramRange(x,-30,30),paramRange(x,-200,200), paramRange(y,-30,30),paramRange(y,-100,250), paramRange(0,0,0),paramRange(0,0,0));
	StarPartEmitter[StarPoolIndex]->setZoom(paramRange(0.1*zz,-.01*zz,.01*zz),paramRange(2*zz, -.1*zz,.1*zz));
	StarPartEmitter[StarPoolIndex]->setBlend(BlendMode::Additive);
	StarPartEmitter[StarPoolIndex]->setColor(paramSettingsColor(1,1),paramSettingsColor(1,1), paramSettingsColor(1,1));
	StarPartEmitter[StarPoolIndex]->setTransparency(paramRange(1,0,0),paramRange(0,0,0));
	StarPartEmitter[StarPoolIndex]->setRotation(paramRange(0,-20,20),paramRange(0,-360,360));
	StarPartEmitter[StarPoolIndex]->entity=StarParticleImage;

	PSysEmitter * pe = GetParticleSystem().CreateEmitter(StarPartEmitter[StarPoolIndex],"CreatePieceStarEmission");
	PSysSetWind(pe,PSYS_NULL,-200.0f,PSYS_NULL);
	PSysSetOptimizationPool(pe,200,500);
	
	pe->CreateEmission(howmany,2);
	StarPoolIndex++; if (StarPoolIndex>MAX_MARKED) StarPoolIndex=0;
}

//***********************************************************************************************
void PSysHelper::CreatePieceStarEmission2( float x, float y, float zz/*=1.0f*/,int howmany )
{
	//*** Create star particle fx 
	StarPartEmitter[StarPoolIndex]->duration=2000; 
	StarPartEmitter[StarPoolIndex]->setPosition(paramRange(x,-30,30),paramRange(x,-200,200), paramRange(y,-30,30),paramRange(y,-300,450), paramRange(0,0,0),paramRange(0,0,0));
	StarPartEmitter[StarPoolIndex]->setZoom(paramRange(0.3*zz,-.03*zz,.03*zz),paramRange(2*zz, -.1*zz,.1*zz));
	StarPartEmitter[StarPoolIndex]->setBlend(BlendMode::Additive);
	StarPartEmitter[StarPoolIndex]->setTransparency(paramRange(1,0,0),paramRange(0,0,0));
	StarPartEmitter[StarPoolIndex]->setColor(paramSettingsColor(1,1),paramSettingsColor(1,1), paramSettingsColor(1,0));
	StarPartEmitter[StarPoolIndex]->setRotation(paramRange(0,-20,20),paramRange(0,-360,360));
	StarPartEmitter[StarPoolIndex]->entity=StarSparkleParticleImage;

	PSysEmitter * pe = GetParticleSystem().CreateEmitter(StarPartEmitter[StarPoolIndex],"CreatePieceStarEmission2");
	PSysSetWind(pe,PSYS_NULL,-100.0f,PSYS_NULL);
	CreateIceEmissionMini(x,y,.5,false);
	PSysSetOptimizationPool(pe,150,500);
	pe->CreateEmission(3,3,40);
	StarPoolIndex++; if (StarPoolIndex>MAX_MARKED) StarPoolIndex=0;
}



//***********************************************************************************************
void PSysHelper::LineCircleSparkleEmission( float x, float y )
{
	static float CreateCircleSparkleCounter2=0;

	float timeSinceLastFrame = GetAppMgr().getElapsed();

	{
		float dd=80;
		CreateCircleSparkleCounter2 += timeSinceLastFrame*1000.0f;
		float dist=SIN(CreateCircleSparkleCounter2/10);
		dd=dd*dist;
		float xx=SIN(CreateCircleSparkleCounter2*2)*dd;
		float yy=COS(CreateCircleSparkleCounter2*2)*dd;
		x=x+xx; y=y+yy;
		float x0=200; float y0=-200;
		//for (int i=0; i<4; i++)
		{
			//*** Create star particle fx 
			SparklePartEmitter[SparklePoolIndex]->duration=2000; 
			//SparklePartEmitter[SparklePoolIndex]->setPosition(paramRange(x,0,0),paramRange(x,0,0), paramRange(y,0,0),paramRange(y,0,0), paramRange(0,0,0),paramRange(0,0,0));
			SparklePartEmitter[SparklePoolIndex]->setPosition(paramRange(0,0,0),paramRange(0,0,0), paramRange(0,0,0),paramRange(0,0,0), paramRange(0,0,0),paramRange(0,0,0));
			SparklePartEmitter[SparklePoolIndex]->setZoom(paramRange(1.8,-.01,.01),paramRange(1.2, 0,0));
			SparklePartEmitter[SparklePoolIndex]->entity=SparkleParticleImage;
			SparklePartEmitter[SparklePoolIndex]->setBlend(BlendMode::Additive);
			SparklePartEmitter[SparklePoolIndex]->setTransparency(paramRange(1,0,0),paramRange(0.3,0,0));
			SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(1,0),paramSettingsColor(0,1), paramSettingsColor(1,0));
			SparklePartEmitter[SparklePoolIndex]->setRotation(paramRange(0,-60,60),paramRange(0,-360,360));

			int r=LIMIT(RANGERAND(0,2),0,1);
			int g=LIMIT(RANGERAND(0,2),0,1);
			int b=LIMIT(RANGERAND(0,2),0,1);
			int r1=LIMIT(RANGERAND(0,2),0,1);
			int g1=LIMIT(RANGERAND(0,2),0,1);
			int b1=LIMIT(RANGERAND(0,2),0,1);
			SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(r,r1),paramSettingsColor(g,g1), paramSettingsColor(b,b1));
			PSysEmitter * pe = GetParticleSystem().CreateEmitter(SparklePartEmitter[SparklePoolIndex],"LineCircleSparkleEmission");
			PSysSetWind(pe,PSYS_NULL,PSYS_NULL,PSYS_NULL);
			PSysSetPivotMovements(pe,x0,y0,x,y,100);
			pe->CreateEmission(1,1);
			SparklePoolIndex++; if (SparklePoolIndex>MAX_SPARKLES) SparklePoolIndex=0;
		}
	}

}

//***********************************************************************************************
void PSysHelper::CreateCircleSparkleEmission( float x, float y )
{
	static float CreateCircleSparkleCounter=0;
	float timeSinceLastFrame = GetAppMgr().getElapsed();
	{
		float dd=60;
		CreateCircleSparkleCounter +=timeSinceLastFrame*1000.0f;
		float dist=SIN(CreateCircleSparkleCounter/10);
		dd=dd*dist;
		float xx=SIN(CreateCircleSparkleCounter)*dd;
		float yy=COS(CreateCircleSparkleCounter)*dd;

		int i=int(CreateCircleSparkleCounter) % 4;
		x=x+xx; y=y+yy;
		//for (int i=0; i<4; i++)
		{
			//*** Create star particle fx 
			SparklePartEmitter[SparklePoolIndex]->duration=2000; 
			SparklePartEmitter[SparklePoolIndex]->setPosition(paramRange(x,0,0),paramRange(x,0,0), paramRange(y,0,0),paramRange(y,0,0), paramRange(0,0,0),paramRange(0,0,0));
			SparklePartEmitter[SparklePoolIndex]->setZoom(paramRange(2.8,-.01,.01),paramRange(0.2, 0,0));
			SparklePartEmitter[SparklePoolIndex]->entity=RippleParticleImage;
			SparklePartEmitter[SparklePoolIndex]->setBlend(BlendMode::Additive);
			SparklePartEmitter[SparklePoolIndex]->setTransparency(paramRange(1,0,0),paramRange(0.3,0,0));
			SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(1,0),paramSettingsColor(0,1), paramSettingsColor(1,0));
			SparklePartEmitter[SparklePoolIndex]->setRotation(paramRange(0,-60,60),paramRange(0,-360,360));

			if (i==0)
				SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(0,1),paramSettingsColor(0,1), paramSettingsColor(1,1));
			else if (i==1)
				SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(1,1),paramSettingsColor(0,1), paramSettingsColor(0,1));
			else if (i==2)
				SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(0,1),paramSettingsColor(1,1), paramSettingsColor(0,1));
			else if (i==3)
				SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(1,1),paramSettingsColor(1,1), paramSettingsColor(0,1));

			PSysEmitter * pe = GetParticleSystem().CreateEmitter(SparklePartEmitter[SparklePoolIndex],"CreateCircleSparkleEmission");
			PSysSetWind(pe,PSYS_NULL,5.0f,PSYS_NULL);
			pe->CreateEmission(1,1);
			SparklePoolIndex++; if (SparklePoolIndex>MAX_SPARKLES) SparklePoolIndex=0;
		}
	}
}
//***********************************************************************************************
void PSysHelper::CreateMultiSparkleEmission( float x, float y, float zz/*=1.0f*/,int howmany )
{
	for (int i=0; i<4; i++)
	{
		//*** Create star particle fx 
		SparklePartEmitter[SparklePoolIndex]->duration=3000; 
		SparklePartEmitter[SparklePoolIndex]->setPosition(paramRange(x,-20,20),paramRange(x,-50,50), paramRange(y,-30,30),paramRange(y,-100,250), paramRange(0,0,0),paramRange(0,0,0));
		SparklePartEmitter[SparklePoolIndex]->setZoom(paramRange(2.5,-.01,.01),paramRange(0.1*zz, 0,0));
		SparklePartEmitter[SparklePoolIndex]->entity=SparkleParticleImage;
		SparklePartEmitter[SparklePoolIndex]->setBlend(BlendMode::Additive);
		SparklePartEmitter[SparklePoolIndex]->setTransparency(paramRange(1,0,0),paramRange(0.6,0,0));
		SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(0,0),paramSettingsColor(0,1), paramSettingsColor(1,0));
		SparklePartEmitter[SparklePoolIndex]->setRotation(paramRange(0,-60,60),paramRange(0,-360,360));

		if (i==0)
			SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(0,1),paramSettingsColor(0,1), paramSettingsColor(1,1));
		else if (i==1)
			SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(1,1),paramSettingsColor(0,1), paramSettingsColor(0,1));
		else if (i==2)
			SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(0,1),paramSettingsColor(1,1), paramSettingsColor(0,1));
		else if (i==3)
			SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(1,1),paramSettingsColor(1,1), paramSettingsColor(0,1));

		PSysEmitter * pe = GetParticleSystem().CreateEmitter(SparklePartEmitter[SparklePoolIndex],"CreateMultiSparkleEmission");
		//PSysSetOptimizationFrameRate(pe,"CreateMultiSparkleEmission",3);
		PSysSetWind(pe,RANGERAND(-250,250),5.0f,PSYS_NULL);
		

		pe->CreateEmission(2,1);
		SparklePoolIndex++; if (SparklePoolIndex>MAX_SPARKLES) SparklePoolIndex=0;
	}
}
//***********************************************************************************************
void PSysHelper::CreateSparkleEmission( float x, float y, float zz/*=1.0f*/,int howmany )
{
	//*** Create star particle fx 
	SparklePartEmitter[SparklePoolIndex]->duration=3000; 
	SparklePartEmitter[SparklePoolIndex]->setPosition(paramRange(x,-20,20),paramRange(x,-50,50), paramRange(y,-30,30),paramRange(y,-100,250), paramRange(0,0,0),paramRange(0,0,0));
	SparklePartEmitter[SparklePoolIndex]->setZoom(paramRange(1,-.01,.01),paramRange(0.1*zz, 0,0));
	SparklePartEmitter[SparklePoolIndex]->entity=SparkleParticleImage;
	SparklePartEmitter[SparklePoolIndex]->setBlend(BlendMode::Additive);
	SparklePartEmitter[SparklePoolIndex]->setTransparency(paramRange(1,0,0),paramRange(0,0,0));
	SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(0,0),paramSettingsColor(0,1), paramSettingsColor(1,0));
	SparklePartEmitter[SparklePoolIndex]->setRotation(paramRange(0,-60,60),paramRange(0,-360,360));
	PSysEmitter * pe = GetParticleSystem().CreateEmitter(SparklePartEmitter[SparklePoolIndex],"CreateSparkleEmission");
	PSysSetWind(pe,PSYS_NULL,20.0f,PSYS_NULL);
	//PSysSetOptimizationPool(pe,40,200);
	pe->CreateEmission(howmany,2);
	SparklePoolIndex++; if (SparklePoolIndex>MAX_SPARKLES) SparklePoolIndex=0;
}

//***********************************************************************************************
void PSysHelper::CreateGlowEmission( float x, float y, int howmanys/*=1.0f*/)
{
	//*** Create star particle fx 
	SparklePartEmitter[SparklePoolIndex]->duration=3000; 
	SparklePartEmitter[SparklePoolIndex]->setPosition(paramRange(x,-2,2),paramRange(x,0,0), paramRange(y,0,0),paramRange(y,0,0), paramRange(0,0,0),paramRange(0,0,0));
	SparklePartEmitter[SparklePoolIndex]->setZoom(paramRange(1,-.01,.01),paramRange(4, -.5,.5));
	SparklePartEmitter[SparklePoolIndex]->setBlend(BlendMode::Additive);
	SparklePartEmitter[SparklePoolIndex]->setTransparency(paramRange(1,0,0),paramRange(1,0,0));
	SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(1,1),paramSettingsColor(0,1), paramSettingsColor(0,0));
	SparklePartEmitter[SparklePoolIndex]->entity=GlowParticleImage;
	//SparklePartEmitter[SparklePoolIndex]->se
	PSysEmitter * pe = GetParticleSystem().CreateEmitter(SparklePartEmitter[SparklePoolIndex],"CreateGlowEmission");
	PSysSetWind(pe,PSYS_NULL,PSYS_NULL,PSYS_NULL);
	PSysSetOptimizationPool(pe,140,300);
	PSysSetOptimizationFrameRate(pe,"CreateGlowEmission",20);
	pe->CreateEmission(howmanys,1);
	SparklePoolIndex++; if (SparklePoolIndex>MAX_SPARKLES) SparklePoolIndex=0;
}
//***********************************************************************************************
void PSysHelper::CreateBlastEmission( float x, float y)
{
	//*** Create star particle fx 
	SparklePartEmitter[SparklePoolIndex]->duration=3000; 
	SparklePartEmitter[SparklePoolIndex]->setPosition(paramRange(x,-2,2),paramRange(x,0,0), paramRange(y,0,0),paramRange(y,0,0), paramRange(0,0,0),paramRange(0,0,0));
	SparklePartEmitter[SparklePoolIndex]->setZoom(paramRange(1,-1,1),paramRange(8, -2,2));
	SparklePartEmitter[SparklePoolIndex]->setBlend(BlendMode::Additive);
	SparklePartEmitter[SparklePoolIndex]->setTransparency(paramRange(1,0,0),paramRange(.5,0,0));
	SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(1,0),paramSettingsColor(1,1), paramSettingsColor(1,0));
	SparklePartEmitter[SparklePoolIndex]->setRotation(paramRange(0,-60,60),paramRange(0,-360,360));
	SparklePartEmitter[SparklePoolIndex]->entity=BlastParticleImage;
	//SparklePartEmitter[SparklePoolIndex]->se
	PSysEmitter * pe = GetParticleSystem().CreateEmitter(SparklePartEmitter[SparklePoolIndex],"CreateBlastEmission");
	PSysSetWind(pe,PSYS_NULL,PSYS_NULL,PSYS_NULL);
	PSysSetOptimizationPool(pe,20,40);
	PSysSetOptimizationFrameRate(pe,"CreateBlastEmission",100);

	pe->CreateEmission(1,1);
	SparklePoolIndex++; if (SparklePoolIndex>MAX_SPARKLES) SparklePoolIndex=0;
}

//***********************************************************************************************
void PSysHelper::CreateSparkleImplosionEmission(float x, float y)
{
	for (int i=0; i<10; i++)
	{

		//*** Create Bubble particle fx 
		SparklePartEmitter[SparklePoolIndex]->duration=500; 
		SparklePartEmitter[SparklePoolIndex]->loops=1; SparklePartEmitter[SparklePoolIndex]->durationRange=90;
		//float angle=atan2(MouseX3D-BildX,MouseY3D-BildY)
		float dx=300;
		SparklePartEmitter[SparklePoolIndex]->setPosition(paramRange(x,-dx,dx),paramRange(x,-0,0), paramRange(y,-dx,dx),paramRange(y,-0,0), paramRange(0,0,0),paramRange(0,0,0));
		SparklePartEmitter[SparklePoolIndex]->setZoom(paramRange(3.5,0.1,0.1),paramRange(0.2,0,0));
		SparklePartEmitter[SparklePoolIndex]->setBlend(BlendMode::Additive);
		SparklePartEmitter[SparklePoolIndex]->setTransparency(paramRange(0.1,0,0),paramRange(1,0,0));
		SparklePartEmitter[SparklePoolIndex]->setRotation(paramRange(0,-20,20),paramRange(0,-360,360));
		SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(1,1),paramSettingsColor(1,0), paramSettingsColor(1,1));
		PSysEmitter * pe = GetParticleSystem().CreateEmitter(SparklePartEmitter[SparklePoolIndex],"CreateSparkleImplosionEmission");
		SparklePartEmitter[SparklePoolIndex]->entity=SparkleParticleImage;
		PSysSetWind(pe,PSYS_NULL,PSYS_NULL,PSYS_NULL);
		PSysSetOptimizationPool(pe,140,200);
		pe->CreateEmission(1,3);
		SparklePoolIndex++; if (SparklePoolIndex>MAX_SPARKLES) SparklePoolIndex=0;	
	}
}

//***********************************************************************************************
void PSysHelper::CreateThunderEmission(float x, float y)
{
	for (int i=0; i<1; i++)
	{

		//*** Create Bubble particle fx 
		SparklePartEmitter[SparklePoolIndex]->duration=50; 
		SparklePartEmitter[SparklePoolIndex]->loops=1; SparklePartEmitter[SparklePoolIndex]->durationRange=50;
		//float angle=atan2(MouseX3D-BildX,MouseY3D-BildY)
		float dx=25;
		SparklePartEmitter[SparklePoolIndex]->setPosition(paramRange(x,-dx,dx),PSYS_NULL, paramRange(y/10.0f,-dx*8,dx*8),PSYS_NULL, paramRange(0,0,0),paramRange(0,0,0));
		SparklePartEmitter[SparklePoolIndex]->setZoom(paramRange(5,-2,1),paramRange(5,-1,1));
		SparklePartEmitter[SparklePoolIndex]->setBlend(BlendMode::Additive);
		SparklePartEmitter[SparklePoolIndex]->setTransparency(paramRange(1,0,0),paramRange(.1,0,0));
		SparklePartEmitter[SparklePoolIndex]->setRotation(paramRange(0,-10,10),PSYS_NULL);
		SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(1,0),paramSettingsColor(1,1), paramSettingsColor(1,1));
		PSysEmitter * pe = GetParticleSystem().CreateEmitter(SparklePartEmitter[SparklePoolIndex],"CreateThunderEmission");
		SparklePartEmitter[SparklePoolIndex]->entity=ThunderParticleImage;
		PSysSetWind(pe,PSYS_NULL,PSYS_NULL,PSYS_NULL);
		PSysSetOptimizationPool(pe,100,200);
		pe->CreateEmission(1,1);
		SparklePoolIndex++; if (SparklePoolIndex>MAX_SPARKLES) SparklePoolIndex=0;	
	}
}
//***********************************************************************************************
void PSysHelper::CreateRainEmission( float x, float y, float zz/*=1.0f*/,int howmany )
{
	//*** Create star particle fx 
	StarPartEmitter[StarPoolIndex]->duration=5000; 
	StarPartEmitter[StarPoolIndex]->setPosition(paramRange(0,-300,300),PSYS_NULL, paramRange(700,-30,80),paramRange(-700,-50,200), paramRange(0,0,0),paramRange(0,0,0));
	StarPartEmitter[StarPoolIndex]->setZoom(paramRange(1,0,0),PSYS_NULL);
	StarPartEmitter[StarPoolIndex]->setBlend(BlendMode::Additive);
	StarPartEmitter[StarPoolIndex]->setTransparency(paramRange(1,0,0),paramRange(0.1,0,0));
	StarPartEmitter[StarPoolIndex]->setColor(paramSettingsColor(.8,1),paramSettingsColor(.8,1), paramSettingsColor(.8,1));

	StarPartEmitter[StarPoolIndex]->entity=RainParticleImage;
	//float direction = sin( iwge::Application::getSingleton().getElapsed() )*200;
	float direction = x/16.0f;
	StarPartEmitter[StarPoolIndex]->setRotation(paramRange(-direction/2.0f,0,0),PSYS_NULL);

	PSysEmitter * pe = GetParticleSystem().CreateEmitter(StarPartEmitter[StarPoolIndex],"CreateRainEmission");
	PSysSetWind(pe,direction,-1000.0f,PSYS_NULL);
	CreateThunderEmission(x,y);

	PSysSetOptimizationPool(pe,100,250);
	pe->CreateEmission(1,1,400);
	StarPoolIndex++; if (StarPoolIndex>MAX_MARKED) StarPoolIndex=0;

}


//***********************************************************************************************
void PSysHelper::CreateRainbowEmission( float x, float y, float dx, float dy)
{
	
	//float dy=20;
	float yy=0;float xx=0;
	dx=LIMIT(dx/10,-4,4);
	dy=LIMIT(dy/10,-4,4);
	for (int i=0; i<8; i++)
	{


		SparklePartEmitter[SparklePoolIndex]->duration=2000; 
		SparklePartEmitter[SparklePoolIndex]->setPosition(paramRange(x,-2,2),paramRange(x,0,0), paramRange(y,0,0),paramRange(y,0,0), paramRange(0,0,0),paramRange(0,0,0));
		SparklePartEmitter[SparklePoolIndex]->setZoom(paramRange(1.5,-.01,.01),paramRange(2, -.5,.5));
		SparklePartEmitter[SparklePoolIndex]->setBlend(BlendMode::Additive);
		SparklePartEmitter[SparklePoolIndex]->setTransparency(paramRange(1,0,0),paramRange(0.1,0,0));

		switch (i)
		{
		case 0:
			SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(1,1),paramSettingsColor(0,0), paramSettingsColor(1,1));
			break;

		case 1: //PURPLE
			SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(.5,.5),paramSettingsColor(0,0), paramSettingsColor(1,1));
			break;

		case 2: //BLUE
			SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(0,0),paramSettingsColor(0,0), paramSettingsColor(1,1));
			break;

		case 3: //CYAN
			SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(1,1),paramSettingsColor(1,1), paramSettingsColor(.5,.5));
			break;

		case 4: //GREEN
			SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(0,0),paramSettingsColor(1,1), paramSettingsColor(0,0));
			break;

		case 5: // YELLOW
			SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(1,1),paramSettingsColor(1,1), paramSettingsColor(0,0));
			break;

		case 6:	//ORANGE
			SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(1,1),paramSettingsColor(.5,.5), paramSettingsColor(0,0));
			break;

		case 7:	//RED
			SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(1,1),paramSettingsColor(0,0), paramSettingsColor(0,0));
			break;


		}
		
		yy=y+(i-4)*dy*10;
		xx=x+(i-4)*dx*10;
		SparklePartEmitter[SparklePoolIndex]->setPosition(paramRange(xx,0,0),paramRange(xx,0,0), paramRange(yy,0,0),paramRange(yy/2,0,0), paramRange(0,0,0),paramRange(0,0,0));
		SparklePartEmitter[SparklePoolIndex]->entity=GlowParticleImage;
		//SparklePartEmitter[SparklePoolIndex]->se
		PSysEmitter * pe = GetParticleSystem().CreateEmitter(SparklePartEmitter[SparklePoolIndex],"CreateRainbowEmission");
		PSysSetWind(pe,PSYS_NULL,-10,PSYS_NULL);

		pe->CreateEmission(1,1);
		SparklePoolIndex++; if (SparklePoolIndex>MAX_SPARKLES) SparklePoolIndex=0;
	}
}


//***********************************************************************************************
void PSysHelper::CreateJellyEmission( float x, float y )
{
	//*** Create star particle fx 
	StarPartEmitter[StarPoolIndex]->duration=20000; 
	//StarPartEmitter[StarPoolIndex]->setPosition(paramRange(-300,-30,30),paramRange(300,-30,30), paramRange(-700,-30,80),paramRange(700,-50,20), paramRange(0,0,0),paramRange(0,0,0));
	StarPartEmitter[StarPoolIndex]->setPosition(paramRange(x,-100,100),paramRange(x,-150,150), paramRange(-700,-100,100),paramRange(700,00,-60), paramRange(0,0,0),paramRange(0,0,0));
	StarPartEmitter[StarPoolIndex]->setZoom(paramRange(1,0,0),PSYS_NULL);
	StarPartEmitter[StarPoolIndex]->setBlend(BlendMode::AlphaBlend);
	StarPartEmitter[StarPoolIndex]->setTransparency(paramRange(0.1,0,0),paramRange(0.8,0,0));
	StarPartEmitter[StarPoolIndex]->setColor(paramSettingsColor(.8,1),paramSettingsColor(.8,1), paramSettingsColor(.8,1));

	StarPartEmitter[StarPoolIndex]->entity=JellySprite;
	//float direction = sin( iwge::Application::getSingleton().getElapsed() )*200;
	float direction = x/8.0f;
	StarPartEmitter[StarPoolIndex]->setRotation(paramRange(-direction,0,0),0);

	PSysEmitter * pe = GetParticleSystem().CreateEmitter(StarPartEmitter[StarPoolIndex],"CreateJellyEmission");
//	PSysSetWind(pe,direction,1000.0f,PSYS_NULL);
	PSysSetOptimizationPool(pe,10,80);
	PSysSetOptimizationFrameRate(pe,"CreateJellyEmission",2000);
	pe->CreateEmission(1,1);
	
	StarPoolIndex++; if (StarPoolIndex>MAX_MARKED) StarPoolIndex=0;

}

//***********************************************************************************************
void PSysHelper::CreateStainEmission( float x, float y, float size, int duration)
{
	//*** Create star particle fx 
	SparklePartEmitter[SparklePoolIndex]->duration=duration; 
	SparklePartEmitter[SparklePoolIndex]->setPosition(paramRange(x,-2,2),paramRange(x,0,0), paramRange(y,0,0),paramRange(y,0,0), paramRange(0,0,0),paramRange(0,0,0));
	SparklePartEmitter[SparklePoolIndex]->setZoom(paramRange(size,-.1,.1),paramRange(size*1.3,-.1,.1));
	SparklePartEmitter[SparklePoolIndex]->setBlend(BlendMode::AlphaBlend);
	SparklePartEmitter[SparklePoolIndex]->setTransparency(paramRange(1,0,0),paramRange(0,0,0));
	SparklePartEmitter[SparklePoolIndex]->setColor(paramSettingsColor(1,1),paramSettingsColor(1,1), paramSettingsColor(1,1));
	SparklePartEmitter[SparklePoolIndex]->setRotation(paramRange(0,-180,180),paramRange(0,-180,180));//6paramRange(PSYS_NULL));
	SparklePartEmitter[SparklePoolIndex]->entity=StainParticleImage;
	//SparklePartEmitter[SparklePoolIndex]->se
	PSysEmitter * pe = GetParticleSystem().CreateEmitter(SparklePartEmitter[SparklePoolIndex],"CreateStainEmission");
	PSysSetWind(pe,PSYS_NULL,PSYS_NULL,PSYS_NULL);
	PSysSetOptimizationPool(pe,20,200);
	//PSysSetOptimizationFrameRate(pe,"CreateStainEmission",1000);
	pe->CreateEmission(1,1);
	
	//GetParticleSystem().getImageManager()
	GetDraw3DMgr().DrawImage3D(GetDrawItemMgr().getObject("Dragon"),x,	y ,0) ; 
	SparklePoolIndex++; if (SparklePoolIndex>MAX_SPARKLES) SparklePoolIndex=0;
}
