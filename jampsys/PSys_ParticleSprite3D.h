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
