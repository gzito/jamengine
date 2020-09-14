/**********************************************************************************
*
* PSys_Particle.h
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

#include <jam/Application.h>
#include <PSys_Status.h>

class Particle3D;
class PSysEmitter;


// ******************************************************************************
// *** PARTICLE COMPONENTS
// ******************************************************************************

// ******************************************************************************
class PSysIComponent
{
public:
	PSysIComponent() :status(0), handle(false) { }
	virtual ~PSysIComponent() = default;
	virtual void update() = 0;
	int status;
	bool handle;

};

// ******************************************************************************
class PSysPosition final : public PSysIComponent
{
public:
	PSysPosition() : x(0), y(0), z(0), dx(0), dy(0), dz(0), xMin(0), yMin(0), zMin(0), endX(0), endY(0), endZ(0) {  }

	virtual ~PSysPosition() = default;

	void setPositionX(float _x, float _dx = 0, float _xMax = PSYS_NULL)
	{
		x = _x;
		xMin = _x;
		endX = _xMax == PSYS_NULL ? _x : _xMax;
		dx = _dx;
	}
	void setPositionY(float _y, float _dy = 0, float _yMax = PSYS_NULL)
	{
		y = _y;
		yMin = _y;
		endY = (_yMax == PSYS_NULL) ? _y : _yMax;
		dy = _dy;
	}
	void setPositionZ(float _z, float _dz = 0, float _zMax = PSYS_NULL)
	{
		z = _z;
		zMin = _z;
		endZ = (_zMax == PSYS_NULL) ? _z : _zMax;
		dz = _dz;
	}

	void setPosition(float _x, float _y, float _z)
	{
		x = _x; y = _y; z = _z;
	}

	void setSpeed(float _dx = 0, float _dy = 0, float _dz = 0)
	{
		dx = _dx; dy = _dy; dz = _dz;
	}



	void update() override
	{
		x += dx * jam::Application::getSingleton().getElapsed();
		y += dy * jam::Application::getSingleton().getElapsed();
		z += dz * jam::Application::getSingleton().getElapsed();
		/*x=LIMIT(x+dx,xMin,xMax);
		y=LIMIT(y+dy,yMin,yMax);
		z=LIMIT(z+dz,zMin,zMax);*/
	}
	float x, y, z;
	float dx, dy, dz;
	float xMin, yMin, zMin;
	float endX, endY, endZ;

};
// ******************************************************************************
class PSysSize final : public PSysIComponent
{
public:
	PSysSize() : Scale(1), dScale(0), minScale(1), maxScale(1) {  }
	virtual ~PSysSize() = default;

	void setZoom(float _scale, float _dScale = 0, float _endScale = PSYS_NULL)
	{
		Scale = _scale;
		minScale = _scale;
		maxScale = (_endScale != PSYS_NULL) ? _endScale : _scale;
		dScale = _dScale;
	}

	void update() override
	{
		Scale = Scale + dScale * jam::Application::getSingleton().getElapsed();
		/*Scale=LIMIT(Scale+dScale,minScale,maxScale);*/
	}

	float Scale, dScale;
	float minScale, maxScale;
};
// ******************************************************************************
class PSysAlpha : public PSysIComponent
{
public:
	PSysAlpha() : alpha(1), stepAlpha(0), startAlpha(0), endAlpha(1)
	{
	}
	virtual ~PSysAlpha() {};

	void setTransparency(float _alpha, float _dAlpha = 0, float _alphaMax = PSYS_NULL)
	{
		alpha = _alpha;
		startAlpha = _alpha;
		endAlpha = (_alphaMax == PSYS_NULL) ? alpha : _alphaMax;
		stepAlpha = _dAlpha;
	}

	void update() override
	{
		alpha += stepAlpha * jam::Application::getSingleton().getElapsed();
		/*alpha=LIMIT(alpha+stepAlpha,startAlpha,endAlpha);*/
	}

	float alpha, stepAlpha;
	float startAlpha, endAlpha;
	//	float oldA;
		//float fadeNext;  float fadeFar;
};
// ******************************************************************************
class PSysAngle final : public PSysIComponent
{
public:
	PSysAngle() :alfa(1), rotation(0), minAlfa(0), maxAlfa(0) {  }
	virtual ~PSysAngle() {}

	void setRotation(float _alfa, float _dAlfa = 0, float _alfaMax = PSYS_NULL)
	{
		alfa = _alfa;
		minAlfa = _alfa;
		maxAlfa = (_alfaMax == PSYS_NULL) ? _alfa : _alfaMax;
		rotation = _dAlfa;
	}

	void update() override
	{
		alfa = alfa + rotation * jam::Application::getSingleton().getElapsed();
		/*alfa=WRAPf(alfa+rotation,minAlfa,maxAlfa);*/
	}

	float alfa, rotation, minAlfa, maxAlfa;
};
// ******************************************************************************
class PSysColor final : public PSysIComponent
{
public:
	PSysColor() : R(1), stepR(0), endR(1),
		G(1), stepG(0), endG(1),
		B(1), stepB(0), endB(1)
	{  }
	virtual ~PSysColor() {}

	void setColor(	const float _R, const float _G, const float _B, 
					const float _dR = 0, const float _dG = 0, const float _dB = 0, 
					const float _eR = 1, const float _eG = 1, const float _eB = 1)
	{
		R = _R;
		endR = _eR;
		stepR = _dR;

		G = _G;
		endG = _eG;
		stepG = _dG;

		B = _B;
		endB = _eB;
		stepB = _dB;
	}

	void update() override
	{
		R += stepR * jam::Application::getSingleton().getElapsed();
		G += stepG * jam::Application::getSingleton().getElapsed();
		B += stepB * jam::Application::getSingleton().getElapsed();
		/*alpha=LIMIT(alpha+stepAlpha,startAlpha,endAlpha);*/

	}

	float R, stepR, endR;
	float G, stepG, endG;
	float B, stepB, endB;

};
// ******************************************************************************
// *** PARTICLE INTERFACE
// ******************************************************************************
class PSysIParticle
{
public:
	PSysIParticle() : status(PSysStatus::PSYS_GO), duration(0), durationRange(0), counter(0), startCounter(0), loops(0),
		emitted(false), emitter(nullptr)
	{
	}
	virtual ~PSysIParticle()
	{
		DbgPrintf("PSysIParticle::~PSysIParticle");
		DbgPrintf("PSysIParticle::~PSysIParticle-end");
	}
	virtual void update() = 0;
	virtual void emit() = 0;
	virtual void destroy() = 0;
	virtual void set() = 0;

	PSysEmitter* setEmitter() const { return emitter; }
	void getEmitter(PSysEmitter* val) { emitter = val; }

	PSysStatus status;
	float duration;
	float durationRange;
	int counter;
	int startCounter;				// dead or alive status
	int loops;						// Regenerations loop [0=infinite,n]

	bool emitted;
	PSysEmitter* emitter;


};

// ******************************************************************************

class PSysSimpleParticle final : public PSysIParticle
{
public:
	PSysSimpleParticle() : ic(0)
	{
	}

	virtual ~PSysSimpleParticle()
	{
		destroy();
	}

	void update() override
	{
		if (status==PSysStatus::PSYS_GO)
		{
			for (ic = 0; ic < component.size(); ic++)
			{
				component[ic]->update();
			}
		}
	}

	void set() override
	{
		//todo
	}

	void emit() override
	{
		//todo
	}

	void destroy() override
	{
		//todo
		/*for (ic = 0; ic < component.size(); ic++)
		{
			JAM_DELETE(component[ic]);
		}*/
		component.clear();
		//emitter->RemoveParticle(*this);
	}

	/*int status;*/
	std::vector<std::unique_ptr<PSysIComponent>> component;
	/*int duration;
	int durationRange;
	int counter;
	int startCounter;				// dead or alive status
	int loops;						// Regenerations loop [0=infinite,n]
	bool emitted;
	PSysEmitter* emitter;*/

private:
	size_t ic;
};
