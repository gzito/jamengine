#ifndef __PsysUtilities_h__
#define __PsysUtilities_h__

#include <jam/DrawItem.h>
#include <PSys.h>

#define MAX_MARKED	128
#define MAX_BUBBLES	264
#define MAX_ICES	64
#define MAX_SPARKLES 256

class PSysHelper
{
public:
	PSysHelper() ;
	~PSysHelper();

	//	void CreatePieceStarEmission(Piece* p, float zz=1.0f,int howmany=12);
	//	void CreatePieceStarEmission(float x, float y, float zz=1.0f, int howmany=12);
	void Init();
	void Begin();
	void DestroyPSys();
	void ClearPSys();
	void SetupPSysResources( const jam::String& path );

	void CreateIceEmission(float x, float y);
	void CreateBigBubbleEmission(float x, float y);
	void CreateIceEmissionMini(float x, float y, float sz=2.0f, bool bRandom=true);
	void CreatePieceStarEmission( float x, float y, float zz=1.0f,int howmany=12 );
	void CreatePieceStarEmission2( float x, float y, float zz=1.0f,int howmany=4 );
	void CreateBubbleEmission(float x, float y);
	void CreateSparkleEmission( float x, float y, float zz=1.0f,int howmany=12 );
	void CreateGlowEmission( float x, float y, int howmany = 1 );
	void CreateBlastEmission( float x, float y);
	void CreateMultiSparkleEmission( float x, float y, float zz=1.0f,int howmany=12 );
	void CreateDustEmission( float x, float y ,int howmany=12 );
	void CreateCircleSparkleEmission( float x, float y );
	void LineCircleSparkleEmission( float x, float y );
	void CreateSparkleImplosionEmission(float x, float y);
	void CreateRainEmission( float x, float y, float zz=1.0f,int howmany=12 );
	void CreateRainbowEmission( float x, float y, float dx, float dy);
	void CreateThunderEmission(float x, float y);
	void CreateSingleDustEmission( float x, float y );
	void CreateJellyEmission( float x, float y );
	void CreateStainEmission( float x, float y, float size, int duration);
	//PSYS& psysMgr() {return GetParticleSystem(); }
	//PSYS psysMgr;

protected:

	jam::Texture2D*			StarParticleImage;
	jam::Texture2D*			StarSparkleParticleImage;
	jam::Texture2D*			BubbleParticleImage;
	jam::Texture2D*			IceParticleImage;
	jam::Texture2D*			SparkleParticleImage;
	jam::Texture2D*			GlowParticleImage;
	jam::Texture2D*			BlastParticleImage;
	jam::Texture2D*			DustParticleImage;
	jam::Texture2D*			RippleParticleImage;
	jam::Texture2D*			RainParticleImage;
	jam::Texture2D*			ThunderParticleImage;
	jam::Texture2D*			BigDustParticleImage;
	jam::Texture2D*			JellySprite;
	jam::Texture2D*			StainParticleImage;
	jam::Texture2D*			DragonSprite;

	int StarPoolIndex;
	int BubblePoolIndex;
	int IcePoolIndex;
	int SparklePoolIndex;

	Particle3DConfigurator *BubblePartEmitter[MAX_BUBBLES+1];
	Particle3DConfigurator *StarPartEmitter[MAX_MARKED+1];
	Particle3DConfigurator *IcePartEmitter[MAX_ICES+1];
	Particle3DConfigurator *SparklePartEmitter[MAX_SPARKLES+1];

};

#endif
