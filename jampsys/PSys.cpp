/**********************************************************************************
*
* PSys.cpp
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

void PSYS::RemoveEmitter(PSysEmitter* em, bool forceDelete)
{
	DbgPrintf("PSYS::RemoveEmitter");

	if (!emitters.empty() && em != nullptr)
	{
		emitters.erase(std::remove(emitters.begin(), emitters.end(), em), emitters.end());	//Not optimal O(n) with vector

		if (forceDelete) { JAM_DELETE(em); }
		else { emitremoved.push(em); }
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

	PSYS::destroy();

	DbgPrintf("PSYS::~PSYS-end");

}
// ******************************************************************************
bool PSYS::clearAll()
{
	
	for (auto& pem : emitters)
	{
		pem->destroy();
		JAM_DELETE(pem);
	}
	emitters.clear();


	while (!emitremoved.empty())
	{
		PSysEmitter* pem = emitremoved.front();
		emitremoved.pop();
		pem->destroy();
		JAM_DELETE(pem);
	}

	optimized = 0;
	groups.clear();
	return true;
}

// ******************************************************************************

void PSYS::SubscribeEmitter(PSysEmitter* em)
{
	if (em != nullptr)
	{
		em->PSysParent_ref = this;
		emitters.emplace_back(em);
		auto& group = groups[em->name];
		group.name = em->name;
		group.min_framerate = em->m_optimize_min_framerate;
		group.lastUpdate = 0;
		
	}
}

// ******************************************************************************

PSysEmitter* PSYS::CreateEmitter(IParticleConfigurator* starterModel, std::string name)
{

	PSysEmitter* pem; // Recycle from the pool
	if (!emitremoved.empty())
	{
		pem = emitremoved.front();
		emitremoved.pop();
		if (pem)
		{
			pem->SetConfigurator(starterModel);
		}
	}
	else
	{
		pem = new PSysEmitter(starterModel);
	}

	//Set values
	if (pem)
	{
		if (name.empty())
			pem->name = name;
		else
			pem->name = starterModel->name;

		pem->status = PSysStatus::PSYS_GO;
		SubscribeEmitter(pem);
		PSysSetPivotMovements(pem, 0, 0, 0, 0, 0);
		PSysSetOptimizationPool(pem, PSYS_NULL, PSYS_NULL);
		PSysSetWind(pem, PSYS_NULL, PSYS_NULL, PSYS_NULL);
		PSysSetOptimizationFrameRate(pem, "", PSYS_NULL);
	}

	return (pem);
}

// ******************************************************************************
void PSYS::update()
{
	//DbgPrintf("PSYS::update Remove");
	for (auto iter = emitters.begin(); iter != emitters.end(); )
	{
		auto* emitter = *iter;
		if (emitter->particles.empty())
		{
				iter = emitters.erase(iter);	//RemoveEmitter(emitter); Not optimal O(n) with vector, changed into list
				emitremoved.push(emitter);
			//emitters[idEm]->SetRemovable();	// ***GS: Newer (NON AZZERA LE PARTICELLE!!!)
		}
		else
		{
			++iter;
			if (emitter->status == PSysStatus::PSYS_GO)
			{
				emitter->update();
			}
		}
	}

	//DbgPrintf("PSYS::update END");

}
void PSYS::updateRender()
{
	for (auto emitter : emitters)
	{
		if (emitter->status == PSysStatus::PSYS_GO)
		{
			emitter->updateRender();
		}
	}

}


PSYS::PSYS() :idEm(0), alives(0), zombies(0), optimized(0), slot(0)
{
	//emitters.reserve(MAX_EMITTERS_POOL);
	for (int i=0; i< EMITTERS_POOL; ++i)
	{
		emitremoved.push(new PSysEmitter());
	}
	groups.clear();

	m_pSpriteBatch = new jam::SpriteBatch( 500 ) ;
}

// ******************************************************************************
Particle3D* PSysSetRotation(Particle3D* pDest, float angle, float rotationSpeed)
{
	if (pDest->angle == nullptr)
	{
		pDest->angle = std::make_unique<PSysAngle>();
	}
	pDest->angle->handle = true;
	pDest->angle->alfa = angle;
	pDest->angle->minAlfa = angle;
	pDest->angle->maxAlfa = angle + 360;
	pDest->angle->rotation = rotationSpeed;
	return pDest;
}

// ******************************************************************************
Particle3D* PSysSetAlpha(Particle3D* pDest, float alpha, float endAlpha, float autoFadeNext, float autoFadeFar)
{
	if (pDest->alpha == nullptr) pDest->alpha = std::make_unique<PSysAlpha>();
	pDest->alpha->startAlpha = alpha;
	pDest->alpha->handle = true;
	if (endAlpha != PSYS_NULL)
	{
		pDest->alpha->endAlpha = endAlpha;
		if (pDest->duration != 0.0f)
		{
			pDest->alpha->stepAlpha = (pDest->alpha->endAlpha - pDest->alpha->startAlpha) / static_cast<float>(pDest->duration);
		}
		else
		{
			pDest->alpha->stepAlpha = Sgn(pDest->alpha->endAlpha - pDest->alpha->startAlpha) * 0.1f;
		}
	}
	else
	{
		pDest->alpha->endAlpha = alpha;
		pDest->alpha->stepAlpha = 0;
	}
	return pDest;
}


