#ifndef PSysParticleSprite3D_h__
#define PSysParticleSprite3D_h__

#include <jam/Sprite.h>
#include <jam/DrawItem.h>
#include <jam/Material.h>


class PSysIParticle;
// ******************************************************************************
class  ParticleSprite3D: public Particle3D
{
public:                  
	ParticleSprite3D();
	virtual ~ParticleSprite3D();

	// TODO chiedere a GS - function "Particle3D::copy(Particle3D *)" is hidden by "ParticleSprite3D::copy" -- virtual function override intended?
	virtual void copy(Particle3D* pModel);
	virtual void update();	
	virtual void updateRender();	
	virtual void emit();
	virtual void destroy();
	virtual void set();
	virtual void SetRemovable();

	int status;
	bool render;

	PSysPosition	*pos;
	PSysSize		*scale;
	PSysAlpha		*alpha;
	PSysAngle		*angle;
	PSysColor		*color;

	int frame;
	//CIwTexture*	entity;
	jam::Sprite entity;
	jam::BlendMode	blendMode;
	jam::DrawItem* item;
//	iwge::SpriteManager*		m_pSprMgr;
private:
	size_t ic;
};

#endif // PSysParticleSprite3D_h__
