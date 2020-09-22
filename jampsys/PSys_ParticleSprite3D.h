#ifdef DISABLED_CODE

/**********************************************************************************
*
* PSys_PaticleSprite3D.h
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

#include <jam/Sprite.h>
#include <jam/DrawItem.h>
#include <jam/Material.h>

#include "PSys_Particle.hpp"
#include "PSys_Particle3D.h"

// ******************************************************************************
class  ParticleSprite3D : public Particle3D
{
public:
	ParticleSprite3D();
	virtual ~ParticleSprite3D();

	// TODO chiedere a GS - function "Particle3D::copy(Particle3D *)" is hidden by "ParticleSprite3D::copy" -- virtual function override intended?
	void copy(Particle3D* pModel) override;
	void update() override;
	void updateRender() override;
	void emit() override;
	void destroy() override;
	void set() override;
	void SetRemovable() override;

	PSysStatus status;
	bool render;
	std::unique_ptr<PSysPosition> pos;
	std::unique_ptr<PSysSize> scale;
	std::unique_ptr<PSysAlpha> alpha;
	std::unique_ptr<PSysAngle> angle;
	std::unique_ptr<PSysColor> color;

	int frame;
	//CIwTexture*	entity;
	jam::Sprite entity;
	jam::BlendMode	blendMode;
	jam::DrawItem* item;
	//	iwge::SpriteManager*		m_pSprMgr;
private:
	size_t ic;
};

#endif
