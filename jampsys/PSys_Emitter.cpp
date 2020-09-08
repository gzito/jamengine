#include "stdafx.h"
#include <jam/Application.h>

using namespace jam ;

// ******************************************************************************
PSysEmitter::PSysEmitter() : 
	m_ParticleTemplate(0), PSysParent_ref(0), status(0), distance(0), nrAlive(0),windX(0),windY(0),windZ(0), pivot_ref(0),
	m_optimize_min_particles_emitted(MIN_PARTICLES_EMITTED),m_optimize_max_particles_emitted(MAX_PARTICLES_EMITTED),
	m_optimize_min_alpha_threshold(MIN_ALPHA_THRESHOLD),m_optimize_min_size_threshold(MIN_SIZE_THRESHOLD),
	m_optimize_min_framerate(0)
{
	//modelParticle = new Particle3D();
	m_ParticleTemplate=NULL;
	//	drawingObject->init() ;
	Init();
};
void PSysEmitter::Init()
{
	distance=0;
	groups=0;
	nrAlive=0;
	windX=0; windY=0; windZ=0;	
	//lastEmittedTime= iwge::Application::getSingleton().getTotalElapsed() ;
	m_optimize_min_framerate=0;
	m_optimize_min_particles_emitted=MIN_PARTICLES_EMITTED;
	m_optimize_max_particles_emitted=MAX_PARTICLES_EMITTED;
	m_optimize_min_alpha_threshold=MIN_ALPHA_THRESHOLD;
	m_optimize_min_size_threshold=MIN_SIZE_THRESHOLD;
	particles.reserve(MAX_PARTICLES_POOL);
	//IWGE_DELETE(pivot_ref);
	//	removed.reserve(MAX_PARTICLES_POOL);
}

PSysEmitter::PSysEmitter( IParticleConfigurator* starterModel ) :
	pivot_ref(0), m_ParticleTemplate(0)
{
	pivot_ref=NULL;
	m_ParticleTemplate=NULL;
	SetConfigurator(starterModel);
	//	drawingObject->init() ;
	Init();
}

// ******************************************************************************
PSysEmitter::~PSysEmitter()
{
	DbgPrintf("PSysEmitter::~PSysEmitter");

	destroy();		  // ***GS: Newer
	DbgPrintf("PSysEmitter::~PSysEmitter-end");
};

// ******************************************************************************

void PSysEmitter::destroy()
{
	// *** Destroy the Particle Emitter, called by PSYS
	// ** remove all particles
	DbgPrintf("PSysEmitter::destroy");
	JAM_DELETE(pivot_ref);

	PARTICLE *p=0;
	for (size_t t=0; t<particles.size(); t++)
	{
		p=particles[t];
		p->destroy();
		JAM_DELETE(p);
		PSysParent_ref->setAlives(PSysParent_ref->totalParticles()-1);
	}
	particles.clear();


	for (size_t t=0; t<removed.size(); t++)
	{
		p = removed.front();
		removed.pop();
		p->destroy();

		JAM_DELETE(p);
	}

	DbgPrintf("PSysEmitter::destroy-end");
}


// ******************************************************************************
void PSysEmitter::SubscribeParticle( PARTICLE* pa )
{
	if (pa!=NULL) 
	{	
		pa->emitter=this;	
		particles.push_back (pa);
		PSysParent_ref->setAlives(PSysParent_ref->totalParticles()+1);
	}

}

// ******************************************************************************
void PSysEmitter::RemoveParticle( PARTICLE* pa,  bool forceDelete )
{
	DbgPrintf("PSysEmitter::RemoveParticle");
	if (particles.size())
	{
		if (pa!=NULL)
		{
			particles.erase(std::remove(particles.begin(), particles.end(), pa), particles.end());
			if (forceDelete) 	
			{ 
				JAM_DELETE(pa);
			}
			else 
			{
				removed.push(pa);PSysParent_ref->setZombies(PSysParent_ref->totalRemoved()+1);
			}

			pa=NULL;
			PSysParent_ref->setAlives(PSysParent_ref->totalParticles()-1);
		}
	}
}

bool PSysEmitter::updateRender()
{
	PARTICLE *p;
	bool ret=false;
//  Originally commented 
//	GetDraw3DMgr().setRenderLevel( PARENT_PSYS->getLevel() ) ;
	for (idPa=0; idPa<particles.size(); idPa++) 
	{
		p=particles[idPa];
		p->updateRender();

	}
	//Draw3DManager::ColorG3D = Color::WHITE;
	return true;
}


void PSysEmitter::updateMovements()
{
	if (pivot_ref==0) return;
	if (pivot_ref->movetime<=0)
	{
		pivot_ref->movetime=0; return;
	}

	float timeSinceLastFrame   = jam::Application::getSingleton().getElapsed()*1000.0f ;
	pivot_ref->x = pivot_ref->x+(((pivot_ref->destx-pivot_ref->x)*timeSinceLastFrame)/pivot_ref->movetime);
	pivot_ref->y = pivot_ref->y+(((pivot_ref->desty-pivot_ref->y)*timeSinceLastFrame)/pivot_ref->movetime);
	pivot_ref->movetime += timeSinceLastFrame;

}
// ******************************************************************************
bool PSysEmitter::update()
{
	PARTICLE *p;

	updateMovements();

	float timeSinceLastFrame   = jam::Application::getSingleton().getElapsed() ;
	for (idPa=0; idPa<particles.size(); idPa++)
	{
		p=particles[idPa];
		p->render=false;
		if (p->status==PSYS_END)
		{
			//p->SetRemovable();
			continue;
		}
		// for each particle 
		

		if (!p->emitted) {
			p->counter--;
			if ( p->counter<=0) {	// *** if is zero then start lifecycle
				p->emitted=true;		// ***	EMIT_PARTICLE:				
				//PSysParent->EmitParticleAgain(p);
				//p->set();				
				p->emit();
				p->counter=0;
				
				//p->render=true;
				// ** calculate all initial components
				continue;
			}
		}
		else { // *** MANAGING LIFECYCLE	
			if (p->duration>0)
			{
				p->duration -= timeSinceLastFrame*1000.0f;
				if (p->duration<=0) {	// *** is the life ended
					p->duration=0;
					if (p->loops!=PSYS_INFINITE_LOOPS) 
					{

						p->loops--;
						if (p->loops<=0) 
						{	// *** Last loop, remove particle (p->loops--;)		
							nrAlive--;	
							p->loops=0;
							//p->destroy();	// remove particle	NB: destroy only unsubscribe form emitter
							p->status=PSYS_END;
							RemoveParticle(p); // ** Remove from emitter's list							
							//if (nrAlive<=0)
							//	PSysParent->RemoveEmitter(this); // ** Remove from PSYS's list
							continue; // goto NEXT_PARTICLE
						}
					}	
					//p->render=true;					
					p->emit();	// ***again!
				}				
				if (windY) 
					p->pos->dy+=windY*( timeSinceLastFrame / 1.0f );
				if (windX) 
					p->pos->dx+=windX*( timeSinceLastFrame / 1.0f );

				p->render=true;
				p->update();// *** UPDATE PARTICLES
			}
		}	// is emitted
	}	// *** for

	// ******************************************************************************
	/*if (particles.size()==0)
	{
	PSysParent->RemoveEmitter(this);
	}*/
	return true;
}

// ******************************************************************************
// *** Create an Emission
// ******************************************************************************
bool PSysEmitter::CreateEmission( int howmany, int _groups/*=1*/, int distance/*=1*/, int newDuration /*= PSYS_NULL*/ )
{
	//PSYS* psys
	if (m_ParticleTemplate==NULL) return false;	
	if (newDuration!=PSYS_NULL) m_ParticleTemplate->duration = newDuration;	// *** a little bit dangerous, change all emitted duration

	// *** NOTE: all particles of an emitter use the same starting tempPart entity!
	// *** 1 emitter 1 entity
	int startCounter=0;
	groups=LIMIT(_groups,1,howmany);
	distance = LIMIT(distance,1,999999);
	float timeSinceFirstFrame   = jam::Application::getSingleton().getTotalElapsed() ;
	float lastUpdate=timeSinceFirstFrame;
	float min_framerate=0;


	if (!name.empty())
	{
		timeSinceFirstFrame   = jam::Application::getSingleton().getTotalElapsed() ;
		lastUpdate=PSysParent_ref->groups[name].lastUpdate;
		min_framerate=PSysParent_ref->groups[name].min_framerate;
	}
	bool emitted=false;
	for (int t=1; t<=howmany; t+=groups )
	{
		{
		for (int g=1; g<=groups; g++ )
		{

			if (min_framerate==0 || (timeSinceFirstFrame-lastUpdate) >= min_framerate)
			{
				emitted=true;
				PARTICLE* part=0;
				if (removed.size())
				{
					part=removed.front();
					removed.pop();
					PSysParent_ref->setZombies(PSysParent_ref->totalRemoved()-1);
				}
				else
				{
					part = new PARTICLE();
				}
				part->emitter = this;					// << Reverse Control (particle knows its emitter!)	
				part->set();

				// **** Duration & Lifecycle ****								
				part->emitted = false;
				part->startCounter = startCounter;
				part->counter = startCounter;
				part->status = PSYS_GO;

				SubscribeParticle(part);
				

				nrAlive++;
			}
		}//g		

		startCounter += distance;
		}
	} //t
	if ( min_framerate>0 && emitted) 
	{	
		PSysParent_ref->groups[name].lastUpdate=timeSinceFirstFrame;
		m_ParticleTemplate->setLastEmittedTime(timeSinceFirstFrame);
	}
	//PSysParent->SubscribeEmitter(this);

	
	return true;
}

void PSysEmitter::SetRemovable()
{
	if (PSysParent_ref!=NULL)
		PSysParent_ref->RemoveEmitter(this);
}
// ***********************************************************************
//Type particleEmitterStruct
//	Field int distance
//	Field int howMany												// how many emissions?			
//	Field int groups												// how many emitted together	
//	
//	Field wind.vector											// constant wind direction
//	Field int systemType 										// identification type (*)
//	Field int status 												// status of the emitter
//	Field endStatus											// die || stay freeze?	(*)
//	Field name$						
//End Type
// ***********************************************************************
PSysEmitter* PSysSetEmitter(PSysEmitter* pPSS, int howmany ,int  distance , int groups,std::string name )
{
	pPSS->nrAlive = howmany;
	pPSS->groups = groups;
	pPSS->distance= distance;
	pPSS->name = name;
	return pPSS;
}

// ***********************************************************************
PSysEmitter* PSysSetWind( PSysEmitter* pPSS, float wx/*=PSYS_NULL*/, float wy/*=PSYS_NULL*/, float wz/*=PSYS_NULL */ )
{
	if (wx!=PSYS_NULL || wy!=PSYS_NULL || wz!=PSYS_NULL)
	{
		//		if (pPSS==NULL)	pPSS=new PSysEmitter();
		if (wx==PSYS_NULL)  pPSS->windX = 0; else pPSS->windX = wx;
		if (wy==PSYS_NULL)  pPSS->windY = 0; else pPSS->windY = wy;
		if (wz==PSYS_NULL)  pPSS->windZ = 0; else pPSS->windZ = wz;
	}
	else 
	{
		pPSS->windX = 0; pPSS->windY=0; pPSS->windZ = 0;
	}
	return pPSS;
}


PSysEmitter* PSysSetPivotMovements( PSysEmitter* pPSS, float fromX, float fromY, float toX, float toY, float duration/*=PSYS_NULL */ )
{
	if (pPSS->pivot_ref==NULL)
		pPSS->pivot_ref=new PSysPivot();

	pPSS->pivot_ref->x=fromX;
	pPSS->pivot_ref->destx=toX;
	pPSS->pivot_ref->y=fromY;
	pPSS->pivot_ref->desty=toY;
	pPSS->pivot_ref->movetime=duration;

	return pPSS;
}
PSysEmitter* PSysSetOptimizationFrameRate( PSysEmitter* pPSS,const std::string& name/*=""*/, float minimal/*=PSYS_NULL*/ )
{
	if (minimal==PSYS_NULL) minimal=0;
	
	if (!name.empty())
		pPSS->name=name;

	pPSS->m_optimize_min_framerate=(minimal)/1000.0f;//min_framerate
	pPSS->PSysParent_ref->groups[pPSS->name].min_framerate=pPSS->m_optimize_min_framerate;
	
	return pPSS;
}

PSysEmitter* PSysSetOptimizationPool( PSysEmitter* pPSS, int minimal/*=PSYS_NULL*/, int maximal/*=PSYS_NULL*/)
{
	if (minimal==PSYS_NULL) minimal=MAX_PARTICLES_EMITTED;
	if (maximal==PSYS_NULL) maximal=MAX_PARTICLES_EMITTED;
	pPSS->m_optimize_min_particles_emitted=minimal;
	pPSS->m_optimize_max_particles_emitted=maximal;
	return pPSS;
}
