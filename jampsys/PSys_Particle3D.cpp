/**********************************************************************************
*
* PSys_Particle3D.cpp
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

#include <jam/Draw3dManager.h>
#include <jam/Gfx.h>

using namespace jam;



Particle3D::~Particle3D()
{
	DbgPrintf("Particle3D::~Particle3D");

	JAM_RELEASE_NULL(item) ;

	DbgPrintf("Particle3D::~Particle3D-end");
}


void Particle3D::copy(Particle3D* pModel)
{
	memcpy(this, pModel, sizeof(&pModel));	//SAFE_CLONE(pDest,pModel, Particle3D)
}

Particle3D::Particle3D() : status(PSysStatus::PSYS_GO), render(false), pos(nullptr), scale(nullptr), alpha(nullptr), angle(nullptr), color(nullptr), frame(0), entity(nullptr), item(nullptr)
{	// ** creates empty components
	pos = std::make_unique<PSysPosition>();
	scale = std::make_unique< PSysSize>();
	alpha = std::make_unique< PSysAlpha>();
	angle = std::make_unique< PSysAngle>();
	color = std::make_unique< PSysColor>();
	item = DrawItem::create();
};

void Particle3D::update()
{
	if (status == PSysStatus::PSYS_GO)
	{
		if (jam::Application::getSingleton().getElapsed() > 0.0f)
		{
			if (pos != nullptr)		pos->update();
			if (scale != nullptr)	scale->update();
			if (alpha != nullptr)	alpha->update();
			if (angle != nullptr)	angle->update();
			if (color != nullptr)	color->update();
		}
	}
}

void Particle3D::SetRemovable()
{
	if (emitter != nullptr)
	{
		status = PSysStatus::PSYS_REMOVE;
		//emitter->RemoveParticle(this);
	}
}

void Particle3D::destroy()
{
	DbgPrintf("Particle3D::destroy");

/*
	JAM_DELETE(pos);
	JAM_DELETE(scale);
	JAM_DELETE(alpha);
	JAM_DELETE(angle);
	JAM_DELETE(color);*/

	DbgPrintf("Particle3D::destroy-end");
}

void Particle3D::emit()
{
	if (!emitter)
		return;
	if (!emitter->GetConfigurator())
		return;

	Particle3DConfigurator* pModel = static_cast<Particle3DConfigurator*>(emitter->GetConfigurator());

	set();
	status = PSysStatus::PSYS_GO;
	counter = pModel->startCounter;
	emitted = true;

}


void Particle3D::set()
{
	status = PSysStatus::PSYS_STOP;
	emitted = false;

	if (emitter == nullptr)
		return;//error
	Particle3DConfigurator* pModel = static_cast<Particle3DConfigurator*>(emitter->GetConfigurator());

	// 	/*	pModel->duration=duration;
	// 	pModel->durationRange=durationRange;
	// 	pModel->calculate(duration);	// ** get the duration from model*/

	duration = pModel->duration;
	durationRange = pModel->durationRange;
	pModel->calculateInto(duration);	// ** get the duration from model*/
	if (pos)
	{
		pModel->positionX.calculateInto(pos->x, pos->endX, pos->dx, static_cast<int>(duration));
		pModel->positionY.calculateInto(pos->y, pos->endY, pos->dy, static_cast<int>(duration));
		pModel->positionZ.calculateInto(pos->z, pos->endZ, pos->dz, static_cast<int>(duration));
	}
	if (angle)
	{
		pModel->rotation.calculateInto(angle->alfa, angle->maxAlfa, angle->rotation, static_cast<int>(duration));
	}
	if (scale)
	{
		pModel->scale.calculateInto(scale->Scale, scale->maxScale, scale->dScale, static_cast<int>(duration));
	}
	if (alpha)
	{
		pModel->alpha.calculateInto(alpha->alpha, alpha->endAlpha, alpha->stepAlpha, static_cast<int>(duration));
	}

	JAM_RELEASE_NULL(item) ;

	if (pModel->entityItem != nullptr)
	{
		item = DrawItem::create(pModel->entityItem->getTexture(), pModel->entityItem->getRect());
	}
	else
	{
		entity = pModel->entity;
		item = DrawItem::create(entity, nullptr);
	}
	if (color)
	{
		pModel->R.calculateInto(color->R, color->endR, color->stepR, static_cast<int>(duration));
		pModel->G.calculateInto(color->G, color->endG, color->stepG, static_cast<int>(duration));
		pModel->B.calculateInto(color->B, color->endB, color->stepB, static_cast<int>(duration));
	}
	
	blendMode = pModel->blendMode;
	item->setBlendingMode(blendMode);

	loops = pModel->loops;
	frame = 0;
}

void Particle3D::updateRender()
{
	if (status == PSysStatus::PSYS_GO)
	{
		if (render)
		{

			if (emitter->PSysParent_ref->totalParticles() > emitter->m_optimize_max_particles_emitted)
			{
				SetRemovable();
				emitter->PSysParent_ref->setOptimized(emitter->PSysParent_ref->getOptimized() + 1);
				return;
			}
			else if (emitter->PSysParent_ref->totalParticles() > emitter->m_optimize_min_particles_emitted)	// *** Do optimizations
			{

				if ((alpha->alpha < emitter->m_optimize_min_alpha_threshold && alpha->stepAlpha <= 0)
					|| (scale->Scale < emitter->m_optimize_min_size_threshold && scale->dScale <= 0))
				{
					SetRemovable();
					emitter->PSysParent_ref->setOptimized(emitter->PSysParent_ref->getOptimized() + 1);
					return;
				}

				if ((pos->x < -Draw3DManager::ScaledHalfVPWidth && pos->dx <= 0.0f)
					|| (pos->x > Draw3DManager::ScaledHalfVPWidth && pos->dx >= 0.0f))
				{
					SetRemovable();
					emitter->PSysParent_ref->setOptimized(emitter->PSysParent_ref->getOptimized() + 1);
					return;
				}
				else if ((pos->y < -Draw3DManager::ScaledHalfVPHeight && pos->dy <= 0.0f)
					|| (pos->y > Draw3DManager::ScaledHalfVPHeight && pos->dy >= 0.0f))
				{
					SetRemovable();
					emitter->PSysParent_ref->setOptimized(emitter->PSysParent_ref->getOptimized() + 1);
					return;
				}
			}
			item->setBlendingMode(blendMode);
			item->getMaterial()->setBlendEnabled(true);
			Draw3DManager::ColorG3D = Color(color->R, color->G, color->B, alpha->alpha);

			// TODO GZ
			GetGfx().setRenderLevel(emitter->PSysParent_ref->getLevel());
			GetDraw3DMgr().DrawImage3D(item, pos->x + emitter->GetPivotX(), pos->y + emitter->GetPivotY(), 0, angle->alfa, scale->Scale);
			Draw3DManager::ColorG3D = Color::WHITE;

		}

	}
}