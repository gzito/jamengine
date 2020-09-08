#include "stdafx.h"
#include <algorithm>

void PSYS::RemoveEmitter(PSysEmitter* em, bool forceDelete)
{
	DbgPrintf("PSYS::RemoveEmitter");

	if (emitters.size())
	{
		if (em!=NULL)
		{
			emitters.erase(std::remove(emitters.begin(), emitters.end(), em), emitters.end());

			if (forceDelete) {JAM_DELETE(em);}
			else {eremoved.push(em);}
		}
	}

	DbgPrintf("PSYS::RemoveEmitter-end");
}
// ******************************************************************************


void PSYS::destroy()
{
	DbgPrintf("PSYS::destroy");
	clearAll();
}

// ******************************************************************************
PSYS::~PSYS()
{
	// ***
	DbgPrintf("PSYS::~PSYS");

	destroy();

	DbgPrintf("PSYS::~PSYS-end");

}
// ******************************************************************************
bool PSYS::clearAll()
{
	PSysEmitter *pem=0; //	
	for (size_t t=0; t<emitters.size(); t++)
	{
		pem = emitters[t];
		pem->destroy();
		JAM_DELETE(pem);
	}
	emitters.clear();


	while( !eremoved.empty() )
	{
		pem = eremoved.front();
		eremoved.pop();
		pem->destroy();
		JAM_DELETE(pem);
	}
	optimized=0;
	groups.clear();
	return true;
}

// ******************************************************************************

void PSYS::SubscribeEmitter( PSysEmitter* em )
{
	if (em!=NULL)	
	{
		em->PSysParent_ref=this;
		emitters.push_back(em);
		groups[em->name].name=em->name;
		groups[em->name].min_framerate=em->m_optimize_min_framerate;
		groups[em->name].lastUpdate=0;
	}
}

// ******************************************************************************

PSysEmitter* PSYS::CreateEmitter( IParticleConfigurator* starterModel, std::string name )
{

	PSysEmitter *pem=0; //
	if (eremoved.size())
	{
		pem = eremoved.front();
		eremoved.pop();
		if (pem)
		{
			pem->SetConfigurator(starterModel);
		}
	}
	else
	{
		pem = new PSysEmitter(starterModel);
	}
	if (pem) 
	{

		if (name.empty())
			pem->name=name;
		else
			pem->name=starterModel->name;

		pem->status=PSYS_GO	;
		SubscribeEmitter(pem);
		PSysSetPivotMovements(pem,0,0,0,0,0);
		PSysSetOptimizationPool(pem,PSYS_NULL,PSYS_NULL);
		PSysSetWind(pem,PSYS_NULL,PSYS_NULL,PSYS_NULL);
		PSysSetOptimizationFrameRate(pem,"",PSYS_NULL);

	}

	return (pem);
}

// ******************************************************************************
void PSYS::update()
{
	//DbgPrintf("PSYS::update Remove");
	for (idEm=0; idEm<emitters.size(); idEm++)
	{
		if (emitters[idEm]->particles.size()==0)
		{
			RemoveEmitter(emitters[idEm]);
			//emitters[idEm]->SetRemovable();	// ***GS: Newer (NON AZZERA LE PARTICELLE!!!)
		}
		else if (emitters[idEm]->status==PSYS_GO  )	
		{
			emitters[idEm]->update();
		}

	}

	//DbgPrintf("PSYS::update END");

}
void PSYS::updateRender()
{


	//DbgPrintf("PSYS::update Remove");
	for (idEm=0; idEm<emitters.size(); idEm++)
	{
		if (emitters[idEm]->status==PSYS_GO  )	
		{
			emitters[idEm]->updateRender();
		}

	}

	//DbgPrintf("PSYS::update END");

}


PSYS::PSYS():idEm(0), alives(0), zombies(0), optimized(0), slot(1)
{
	emitters.reserve(MAX_EMITTERS_POOL);
	groups.clear();
	//eremoved.reserve(MAX_EMITTERS_POOL);
}

// ******************************************************************************
Particle3D* PSysSetRotation( Particle3D* pDest, float angle, float rotationSpeed)
{
	if ( pDest->angle==NULL) pDest->angle = new PSysAngle();
	pDest->angle->handle=true;
	pDest->angle->alfa=angle;
	pDest->angle->minAlfa=angle;
	pDest->angle->maxAlfa=angle+360;
	pDest->angle->rotation=rotationSpeed;
	return pDest;
}

// ******************************************************************************
Particle3D* PSysSetAlpha(Particle3D* pDest,float alpha, float endAlpha, float autoFadeNext, float autoFadeFar)
{
	if (pDest->alpha==NULL) pDest->alpha = new PSysAlpha();
	pDest->alpha->startAlpha= alpha;
	pDest->alpha->handle=true;
	if ( endAlpha!=PSYS_NULL ) 
	{
		pDest->alpha->endAlpha = endAlpha;
		if ( (pDest->duration) ) 
		{
			pDest->alpha->stepAlpha= (pDest->alpha->endAlpha - pDest->alpha->startAlpha) / float(pDest->duration);
		}
		else
		{
			pDest->alpha->stepAlpha = Sgn(pDest->alpha->endAlpha - pDest->alpha->startAlpha) * 0.1f;
			/*
			if ( autoFadeNext<>PSYS_NULL && autoFadeFar<>PSYS_NULL )  
			{
			pDest->alpha.fadeNext = autoFadeNext;
			pDest->alpha.fadeFar = autoFadeFar;
			}	
			*/
		}
	}
	else
	{
		pDest->alpha->endAlpha = alpha;
		pDest->alpha->stepAlpha = 0;
	}
	return pDest;
}


