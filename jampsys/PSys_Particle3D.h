#ifndef PSysParticle3D_h__
#define PSysParticle3D_h__

#include <jam/DrawItem.h>
//#include <IwMaterial.h>

class PSysIParticle;
// ******************************************************************************
class Particle3D : public PSysIParticle
{
public:                  
	Particle3D();
	virtual ~Particle3D();

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
	jam::Texture2D*	entity;
	jam::BlendMode	blendMode;
	jam::DrawItem* item;

};

#endif // PSysParticle3D_h__
