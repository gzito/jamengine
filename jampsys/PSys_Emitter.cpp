/**********************************************************************************
*
* PSys_Emitter.cpp
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

#include "stdafx.h"
#include <jam/Application.h>
#include <jam/Gfx.h>

using namespace jam;

// ******************************************************************************
PSysEmitter::PSysEmitter()
    : distance(0)
    , nrAlive(0)
    , windX(0)
    , windY(0)
    , windZ(0)
    , status(PSysStatus::PSYS_STOP)
    , pivot_ref(nullptr)
    , PSysParent_ref(nullptr)
    , m_ParticleTemplate(nullptr)
    , m_optimize_min_particles_emitted(MIN_PARTICLES_EMITTED)
    , m_optimize_max_particles_emitted(MAX_PARTICLES_EMITTED)
    , m_optimize_min_alpha_threshold(MIN_ALPHA_THRESHOLD)
    , m_optimize_min_size_threshold(MIN_SIZE_THRESHOLD)
    , m_optimize_min_framerate(0)
{
    //modelParticle = new Particle3D();
    m_ParticleTemplate = nullptr;
    //	drawingObject->init() ;
    Init();
};
void PSysEmitter::Init()
{
    distance = 0;
    groups = 0;
    nrAlive = 0;
    windX = 0;
    windY = 0;
    windZ = 0;
    //lastEmittedTime= iwge::Application::getSingleton().getTotalElapsed() ;
    m_optimize_min_framerate = 0;
    m_optimize_min_particles_emitted = MIN_PARTICLES_EMITTED;
    m_optimize_max_particles_emitted = MAX_PARTICLES_EMITTED;
    m_optimize_min_alpha_threshold = MIN_ALPHA_THRESHOLD;
    m_optimize_min_size_threshold = MIN_SIZE_THRESHOLD;

    for (int i = 0; i < PARTICLES_POOL; ++i)
    {
        removed.push(new PARTICLE());
    }

    //IWGE_DELETE(pivot_ref);
    //	removed.reserve(MAX_PARTICLES_POOL);
}

PSysEmitter::PSysEmitter(IParticleConfigurator* starterModel)
    : pivot_ref(nullptr)
    , m_ParticleTemplate(nullptr)
{
    pivot_ref = nullptr;
    m_ParticleTemplate = nullptr;
    SetConfigurator(starterModel);
    //	drawingObject->init() ;
    Init();
}

// ******************************************************************************
PSysEmitter::~PSysEmitter()
{
    DbgPrintf("PSysEmitter::~PSysEmitter");

    PSysEmitter::destroy(); // ***GS: Newer
    DbgPrintf("PSysEmitter::~PSysEmitter-end");
};

// ******************************************************************************

void PSysEmitter::destroy()
{
    // *** Destroy the Particle Emitter, called by PSYS
    // ** remove all particles
    DbgPrintf("PSysEmitter::destroy");
    JAM_DELETE(pivot_ref);

    for (auto& p : particles)
    {
        p->destroy();
        JAM_DELETE(p);
        PSysParent_ref->setAlives(PSysParent_ref->totalParticles() - 1);
    }
    particles.clear();


    while (!removed.empty())
    {
        PARTICLE* p = removed.front();
        removed.pop();
        p->destroy();

        JAM_DELETE(p);
    }


    DbgPrintf("PSysEmitter::destroy-end");
}

// ******************************************************************************
void PSysEmitter::SubscribeParticle(PARTICLE* pa)
{
    if (pa != nullptr) {
        pa->emitter = this;
        particles.emplace_back(pa);
        PSysParent_ref->setAlives(PSysParent_ref->totalParticles() + 1);
    }
}

// ******************************************************************************
void PSysEmitter::RemoveParticle(PARTICLE* pa, bool forceDelete)
{
    DbgPrintf("PSysEmitter::RemoveParticle");
    if (!particles.empty()) {
        if (pa != nullptr) {
            particles.erase(std::remove(particles.begin(), particles.end(), pa), particles.end());
            if (forceDelete) {
                JAM_DELETE(pa);
            }
            else {
                removed.push(pa);
                PSysParent_ref->setZombies(PSysParent_ref->totalRemoved() + 1);
            }

            pa = nullptr;
            PSysParent_ref->setAlives(PSysParent_ref->totalParticles() - 1);
        }
    }
}

bool PSysEmitter::updateRender()
{
    for (auto iter = particles.begin(); iter != particles.end(); ++iter)
    {
        (*iter)->updateRender();
    }
    //Draw3DManager::ColorG3D = Color::WHITE;
    return true;
}

void PSysEmitter::updateMovements()
{
    if (pivot_ref == nullptr)
        return;
    if (pivot_ref->movetime <= 0) {
        pivot_ref->movetime = 0;
        return;
    }

    const float timeSinceLastFrame = jam::Application::getSingleton().getElapsed() * 1000.0f;
    pivot_ref->x = pivot_ref->x + (((pivot_ref->destx - pivot_ref->x) * timeSinceLastFrame) / pivot_ref->movetime);
    pivot_ref->y = pivot_ref->y + (((pivot_ref->desty - pivot_ref->y) * timeSinceLastFrame) / pivot_ref->movetime);
    pivot_ref->movetime += timeSinceLastFrame;
}
// ******************************************************************************


void PSysEmitter::RemoveParticleFast(PARTICLE* p)
{
    removed.push(p);
    PSysParent_ref->setZombies(PSysParent_ref->totalRemoved() + 1);
    PSysParent_ref->setAlives(PSysParent_ref->totalParticles() - 1);
}

// ******************************************************************************
bool PSysEmitter::update()
{
    updateMovements();

    for (auto iter = particles.begin(); iter != particles.end();) {
        auto* p = *iter;
        p->render = false;
        if (p->status == PSysStatus::PSYS_END) {
            //p->SetRemovable();
            ++iter;
            continue;
        }

        
        if (p->status == PSysStatus::PSYS_REMOVE) {
           
            iter = particles.erase(iter);
            RemoveParticleFast(p);
            continue;
        }
        // for each particle
        bool isRemoved = false;
        if (!p->emitted) {
            if (--p->counter <= 0) { // *** if is zero then start lifecycle
                p->emitted = true; // ***	EMIT_PARTICLE:
                //PSysParent->EmitParticleAgain(p);
                //p->set();
                p->emit();
                p->counter = 0;

                //p->render=true;
                // ** calculate all initial components

            }
        }
        else { // *** MANAGING LIFECYCLE
            if (p->duration > 0) {
                p->duration -= jam::Application::getSingleton().getElapsed() * 1000.0f;
                if (p->duration <= 0) { // *** is the life ended
                    p->duration = 0;

                    if (p->loops != PSYS_INFINITE_LOOPS) {
                        p->loops--;
                        if (p->loops <= 0) { // *** Last loop, remove particle (p->loops--;)
                            nrAlive--;
                            p->loops = 0;
                            //p->destroy();	// remove particle	NB: destroy only unsubscribe form emitter
                            p->status = PSysStatus::PSYS_END;
                            iter = particles.erase(iter);
                            RemoveParticleFast(p);

                            isRemoved = true;
                            //RemoveParticle(p); // ** Remove from emitter's list
                            //if (nrAlive<=0)
                            //	PSysParent->RemoveEmitter(this); // ** Remove from PSYS's list
                            continue; // goto NEXT_PARTICLE
                        }

                    }
                    //p->render=true;
                    p->emit(); // ***again!
                }
                if (windY != 0.0f)
                    p->pos->dy += windY * (jam::Application::getSingleton().getElapsed());
                if (windX != 0.0f)
                    p->pos->dx += windX * (jam::Application::getSingleton().getElapsed());

                p->render = true;
                p->update(); // *** UPDATE PARTICLES

            }
        } // is emitted
        if (!isRemoved)
            ++iter;
    } // *** for

    // ******************************************************************************
    /*if (particles.size()==0)
    {
    PSysParent->RemoveEmitter(this);
    }#*/
    return true;
}

// ******************************************************************************
// *** Create an Emission
// ******************************************************************************
bool PSysEmitter::CreateEmission(int howmany, int _groups /*=1*/, int distance /*=1*/, float newDuration /*= PSYS_NULL*/)
{
    //PSYS* psys
    if (m_ParticleTemplate == nullptr)
        return false;
    if (newDuration != PSYS_NULL)
        m_ParticleTemplate->duration = newDuration; // *** a little bit dangerous, change all emitted duration

    // *** NOTE: all particles of an emitter use the same starting tempPart entity!
    // *** 1 emitter 1 entity
    int startCounter = 0;
    groups = LIMIT(_groups, 1, howmany);
    distance = LIMIT(distance, 1, 999999);
    float timeSinceFirstFrame = jam::Application::getSingleton().getTotalElapsed();
    float lastUpdate = timeSinceFirstFrame;
    float min_framerate = 0;
    auto& group = PSysParent_ref->groups[name];
    if (!name.empty()) {
        timeSinceFirstFrame = jam::Application::getSingleton().getTotalElapsed();
        lastUpdate = group.lastUpdate;
        min_framerate = group.min_framerate;
    }
    bool emitted = false;
    for (auto t = 1; t <= howmany; t += groups) {
        {
            for (auto g = 1; g <= groups; g++) {

                if (min_framerate == 0 || (timeSinceFirstFrame - lastUpdate) >= min_framerate) {
                    emitted = true;
                    PARTICLE* part;

                    if (!removed.empty()) {
                        part = removed.front();
                        removed.pop();
                        PSysParent_ref->setZombies(PSysParent_ref->totalRemoved() - 1);
                    }
                    else {
                        part = new PARTICLE();
                    }

                    part->emitter = this; // << Reverse Control (particle knows its emitter!)
                    part->set();

                    // **** Duration & Lifecycle ****
                    part->emitted = false;
                    part->startCounter = startCounter;
                    part->counter = startCounter;
                    part->status = PSysStatus::PSYS_GO;

                    SubscribeParticle(part);

                    nrAlive++;
                }
            } //g

            startCounter += distance;
        }
    } //t
    if (min_framerate > 0 && emitted) {
        group.lastUpdate = timeSinceFirstFrame;
        m_ParticleTemplate->setLastEmittedTime(timeSinceFirstFrame);
    }
    //PSysParent->SubscribeEmitter(this);

    return true;
}

void PSysEmitter::SetRemovable()
{
    if (PSysParent_ref != nullptr)
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
PSysEmitter* PSysSetEmitter(PSysEmitter* pPSS, int howmany, int distance, int groups, std::string name)
{
    pPSS->nrAlive = howmany;
    pPSS->groups = groups;
    pPSS->distance = distance;
    pPSS->name = name;
    return pPSS;
}

// ***********************************************************************
PSysEmitter* PSysSetWind(PSysEmitter* pPSS, float wx /*=PSYS_NULL*/, float wy /*=PSYS_NULL*/, float wz /*=PSYS_NULL */)
{
    if (wx != PSYS_NULL || wy != PSYS_NULL || wz != PSYS_NULL) {
        //		if (pPSS==NULL)	pPSS=new PSysEmitter();
        if (wx == PSYS_NULL)
            pPSS->windX = 0;
        else
            pPSS->windX = wx;
        if (wy == PSYS_NULL)
            pPSS->windY = 0;
        else
            pPSS->windY = wy;
        if (wz == PSYS_NULL)
            pPSS->windZ = 0;
        else
            pPSS->windZ = wz;
    }
    else {
        pPSS->windX = 0;
        pPSS->windY = 0;
        pPSS->windZ = 0;
    }
    return pPSS;
}

PSysEmitter* PSysSetPivotMovements(PSysEmitter* pPSS, float fromX, float fromY, float toX, float toY, float duration /*=PSYS_NULL */)
{
    if (pPSS->pivot_ref == nullptr)
        pPSS->pivot_ref = new PSysPivot();

    pPSS->pivot_ref->x = fromX;
    pPSS->pivot_ref->destx = toX;
    pPSS->pivot_ref->y = fromY;
    pPSS->pivot_ref->desty = toY;
    pPSS->pivot_ref->movetime = duration;

    return pPSS;
}
PSysEmitter* PSysSetOptimizationFrameRate(PSysEmitter* pPSS, const std::string& name /*=""*/, float minimal /*=PSYS_NULL*/)
{
    if (minimal == PSYS_NULL)
        minimal = 0;

    if (!name.empty())
        pPSS->name = name;

    pPSS->m_optimize_min_framerate = (minimal) / 1000.0f; //min_framerate
    pPSS->PSysParent_ref->groups[pPSS->name].min_framerate = pPSS->m_optimize_min_framerate;

    return pPSS;
}

PSysEmitter* PSysSetOptimizationPool(PSysEmitter* pPSS, int minimal /*=PSYS_NULL*/, int maximal /*=PSYS_NULL*/)
{
    
   /* if (minimal == PSYS_NULL)
        minimal = MAX_PARTICLES_EMITTED;
    if (maximal == PSYS_NULL)
        maximal = MAX_PARTICLES_EMITTED;
    pPSS->m_optimize_min_particles_emitted = minimal;
    pPSS->m_optimize_max_particles_emitted = maximal;*/
    return pPSS;
}
