#include "stdafx.h"
#include <jam/Draw3dManager.h>

using namespace jam ;

ParticleSprite3D::~ParticleSprite3D()
{
	DbgPrintf("ParticleSprite3D::~ParticleSprite3D");
	destroy();
	DbgPrintf("ParticleSprite3D::~ParticleSprite3D-end");
}

void ParticleSprite3D::copy(Particle3D* pModel)
{
	memcpy(this,pModel,sizeof(&pModel));	//SAFE_CLONE(pDest,pModel, ParticleSprite3D)
}

ParticleSprite3D::ParticleSprite3D(): status(1), color(0), pos(0),scale(0),alpha(0),angle(0), frame(0), render(false), item(0)
{	// ** creates empty components
	//quad(0)
	//entity(0),
	pos = new PSysPosition(); 
	scale = new PSysSize(); 
	alpha = new PSysAlpha(); 
	angle = new PSysAngle(); 
	color = new PSysColor(); 
	item = DrawItem::create();
};

void ParticleSprite3D::update()
{
	if (status==PSYS_GO)
	{
		if (pos!=NULL)		pos->update();
		if (scale!=NULL)	scale->update();
		if (alpha!=NULL)	alpha->update();
		if (angle!=NULL)	angle->update();
		if (color!=NULL)	color->update();
	} 
}

void ParticleSprite3D::SetRemovable()
{
	if (emitter!=NULL)
		emitter->RemoveParticle(this);
}

void ParticleSprite3D::destroy()
{
	DbgPrintf("ParticleSprite3D::destroy");

	JAM_DELETE(pos);
	JAM_DELETE(scale);
	JAM_DELETE(alpha);
	JAM_DELETE(angle);
	JAM_DELETE(color);

	DbgPrintf("ParticleSprite3D::destroy-end");
}

void ParticleSprite3D::emit()
{
	if (!emitter)
		return;
	if (!emitter->GetConfigurator())
		return;

	Particle3DConfigurator* pModel= (Particle3DConfigurator*) emitter->GetConfigurator();

	set();	
	status=PSYS_GO;
	counter = pModel->startCounter;
	emitted = true;

}


void ParticleSprite3D::set()
{
	status=PSYS_STOP;
	emitted = false;

	if (emitter==NULL) 
		return ;//error
	Particle3DConfigurator* pModel= (Particle3DConfigurator*) emitter->GetConfigurator();

	duration=pModel->duration;
	durationRange=pModel->durationRange;
	pModel->calculateInto(duration);	// ** get the duration from model*/

	pModel->positionX.calculateInto(pos->x,pos->endX, pos->dx,(int)duration);
	pModel->positionY.calculateInto(pos->y,pos->endY, pos->dy,(int)duration);
	pModel->positionZ.calculateInto(pos->z,pos->endZ,pos->dz,(int)duration);

	pModel->rotation.calculateInto(angle->alfa,angle->maxAlfa,angle->rotation,(int)duration);
	pModel->scale.calculateInto(scale->Scale,scale->maxScale, scale->dScale,(int)duration);
	pModel->alpha.calculateInto(alpha->alpha,alpha->endAlpha, alpha->stepAlpha,(int)duration);


	pModel->R.calculateInto(color->R, color->endR, color->stepR, (int)duration);
	pModel->G.calculateInto(color->G, color->endG, color->stepG, (int)duration);
	pModel->B.calculateInto(color->B, color->endB, color->stepB,(int) duration);

	loops=pModel->loops;

	frame=0;
}

void ParticleSprite3D::updateRender()
{
	if (status==PSYS_GO)
	{
		if (render)
		{
			// TODO GZ
//			GetDraw3DMgr().setRenderLevel( PARENT_PSYS->getLevel() ) ;
			if (PARENT_PSYS->totalParticles()>emitter->m_optimize_max_particles_emitted)
			{
				SetRemovable();
				PARENT_PSYS->setOptimized(PARENT_PSYS->getOptimized()+1);
				return;
			}
			else if (PARENT_PSYS->totalParticles()>emitter->m_optimize_min_particles_emitted)	// *** Do optimizations
			{

				if ( (alpha->alpha<emitter->m_optimize_min_alpha_threshold && alpha->stepAlpha<=0) 
					|| (scale->Scale<emitter->m_optimize_min_size_threshold && scale->dScale<=0))				
				{
					SetRemovable();
					PARENT_PSYS->setOptimized(PARENT_PSYS->getOptimized()+1);
					return;
				}

				if ( (pos->x<-Draw3DManager::ScaledHalfVPWidth && pos->dx<=0.0f)
					|| (pos->x>Draw3DManager::ScaledHalfVPWidth && pos->dx>=0.0f))
				{					
					SetRemovable();
					PARENT_PSYS->setOptimized(PARENT_PSYS->getOptimized()+1);
					return;
				}
				else if ((pos->y<-Draw3DManager::ScaledHalfVPHeight && pos->dy<=0.0f)
					|| (pos->y>Draw3DManager::ScaledHalfVPHeight && pos->dy>=0.0f))
				{
					SetRemovable();
					PARENT_PSYS->setOptimized(PARENT_PSYS->getOptimized()+1);
					return;
				}
			}
			item->setBlendingMode(blendMode);
			Draw3DManager::ColorG3D = Color(color->R,color->G,color->B,alpha->alpha) ;
			GetDraw3DMgr().DrawImage3D(item,pos->x + emitter->GetPivotX() ,pos->y + emitter->GetPivotY(),0,angle->alfa,scale->Scale) ; 			
		} 

	}
	Draw3DManager::ColorG3D = Color::WHITE;

}