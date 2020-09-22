/**********************************************************************************
*
* PSys_Emitter.h
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
#pragma once

#include <PSys_Globals.h>

#include <queue>
#include <vector>

#include "PSys_Particle3D.h"

enum class PSysStatus;
class PSYS;
class PSysIParticle;
class IParticleConfigurator;

struct PSysPivot
{
	float x, y;
	float destx, desty;
	int movetime;
};

// ******************************************************************************
class PSysEmitter
{
public:
	PSysEmitter();
	virtual ~PSysEmitter();
	PSysEmitter(IParticleConfigurator* starterModel);

	// Methods
	void SubscribeParticle(PARTICLE* pa);
	void RemoveParticle(PARTICLE* pa, bool forceDelete = false);
	virtual void SetRemovable();
	bool update();
	virtual void destroy();

	//Particle3D* modelParticle;						// A model for the particle re-emitted
	IParticleConfigurator* GetConfigurator() const { return m_ParticleTemplate; }
	void SetConfigurator(IParticleConfigurator* starterModel) { m_ParticleTemplate = starterModel; }
	bool CreateEmission(int howmany, int _groups = 1, int distance = 1, float newDuration = PSYS_NULL);
	void Init();
	bool updateRender();
	void updateMovements();
	
	// Attribs
	int	distance;
	int	groups;										// how many emitted together
	int	nrAlive; 										// Number of particles still alive.	
	float windX, windY, windZ;						// wind direction for all the particles

	PSysStatus	status;
	std::string name;

	float GetPivotX() const { return (pivot_ref) ? pivot_ref->x : 0; }
	float GetPivotY() const { return (pivot_ref) ? pivot_ref->y : 0; }

	PSysPivot* pivot_ref;								// Followed entity, attach the emitter to it
	PSYS* PSysParent_ref;

	IParticleConfigurator* m_ParticleTemplate;
	std::list<PARTICLE*> particles;
	std::queue<PARTICLE*> removed;

	int m_optimize_min_particles_emitted;
	int m_optimize_max_particles_emitted;
	float m_optimize_min_alpha_threshold;
	float m_optimize_min_size_threshold;
	float m_optimize_min_framerate;

private:
	void RemoveParticleFast(PARTICLE* pa);

	size_t idPa;
};

// ******************************************************************************
PSysEmitter* PSysSetEmitter(PSysEmitter* pPSS, int howmany = 1, int  distance = 1, int groups = 1, std::string name = "");
PSysEmitter* PSysSetWind(PSysEmitter* pPSS, float wx = PSYS_NULL, float wy = PSYS_NULL, float wz = PSYS_NULL);
PSysEmitter* PSysSetPivotMovements(PSysEmitter* pPSS, float fromX, float fromY, float toX, float toY, float duration/*=PSYS_NULL */);
PSysEmitter* PSysSetOptimizationPool(PSysEmitter* pPSS, int minimal = MIN_PARTICLES_EMITTED, int maximal = MAX_PARTICLES_EMITTED);
PSysEmitter* PSysSetOptimizationFrameRate(PSysEmitter* pPSS, const std::string& name = "", float minimal = PSYS_NULL);
