/**********************************************************************************
*
* PSys_ParticleConfigurator.h
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

#include "jam\DrawItem.h"

struct paramColor
{
	paramColor() :R(1), G(1), B(1), endR(1), endG(1), endB(1) {}
	paramColor(float r, float g, float b, float er = 1, float eg = 1, float eb = 1) { R = r; G = g; B = b; endR = er; endG = eg; endB = eb; }
	float R; float G; float B;
	float endR; float endG; float endB;

};

struct paramRange
{
	paramRange() :aValue(0), minOffset(0), maxOffset(0) {}
	paramRange(float _v, float _min = 0, float _max = 0) { aValue = _v; minOffset = _min; maxOffset = _max; }
	float aValue; float minOffset; float maxOffset;
};

struct paramSettings
{
	paramSettings() :start(0), end(0) {}
	paramRange start;
	paramRange end;
	void calculateInto(float& from, float& to, float& delta, int duration) const
	{
		from = RANGERANDF(start.aValue + start.minOffset, start.aValue + start.maxOffset);
		if (to == PSYS_NULL)
			to = from;
		else
			to = RANGERANDF(end.aValue + end.minOffset, end.aValue + end.maxOffset);
		delta = 1000.0f * (static_cast<float>(to - from) / static_cast<float>(duration));
		//delta=float(duration)/(float(to-from)/(float(duration)));
		//delta=(float(to-from));
	}
};

struct paramSettingsColor
{
	paramSettingsColor() :start(255.0f), end(255.0f) {}
	paramSettingsColor(float _start, float _end = 1) { start = _start * 255.0f; end = _end * 255.0f; }
	float start;	float end;
	void calculateInto(float& from, float& to, float& delta, int duration) const
	{
		from = start;
		to = end;
		delta = 1000.0f * (static_cast<float>(to - from) / static_cast<float>(duration));
	}
};

class IParticleConfigurator
{
public:
	float duration;
	float endDuration;
	float durationRange;
	int startCounter;				// dead or alive status
	int loops;						// Regenerations loop [0=infinite,n]
	std::string name;

	IParticleConfigurator() :duration(0), endDuration(0), durationRange(0), startCounter(0), loops(0), lastEmittedTime(0) { name = ""; }

	virtual ~IParticleConfigurator()
	{
		duration = 0;
	}
	float getLastEmittedTime() const { return lastEmittedTime; }
	void setLastEmittedTime(float val) { lastEmittedTime = val; }

	void setName(std::string _name) { name = _name; }
	std::string getName() const { return name; }

protected:
	float lastEmittedTime;
};

class Particle3DConfigurator final : public IParticleConfigurator
{
public:
	Particle3DConfigurator() : entity(nullptr), entityItem(nullptr) {}
	paramSettings positionX;
	paramSettings positionY;
	paramSettings positionZ;
	paramSettings alpha;
	paramSettings rotation;
	paramSettings scale;
	paramSettingsColor R;
	paramSettingsColor G;
	paramSettingsColor B;

	jam::Texture2D* entity;
	jam::DrawItem* entityItem;
	jam::BlendMode blendMode;
	virtual ~Particle3DConfigurator();

	void calculateInto(float& _newduration) const
	{
		_newduration = RANGERANDF(duration - durationRange, duration + durationRange);
	}
	void setBlend(jam::BlendMode blmode)
	{
		blendMode = blmode;
	}
	void setRotation(const paramRange& start, const paramRange& end)
	{
		rotation.start = start; rotation.end = end;
	}
	void setTransparency(const paramRange& start, const paramRange& end)
	{
		alpha.start = start; alpha.end = end;
		alpha.start.aValue *= 255.0f;
		alpha.start.maxOffset *= 255.0f;
		alpha.start.minOffset *= 255.0f;

		alpha.end.aValue *= 255.0f;
		alpha.end.maxOffset *= 255.0f;
		alpha.end.minOffset *= 255.0f;
	}
	void setPosition(const paramRange& startX, const paramRange& endX, const paramRange& startY, const paramRange& endY, const paramRange& startZ, const paramRange& endZ)
	{
		positionX.start = startX;	positionX.end = endX;
		positionY.start = startY;	positionY.end = endY;
		positionZ.start = startZ;	positionZ.end = endZ;
	}
	void setZoom(const paramRange& start, const paramRange& end)
	{
		scale.start = start; scale.end = end;
	}
	void setColor(const paramSettingsColor& r, const paramSettingsColor& g, const paramSettingsColor& b)
	{
		R = r; G = g; B = b;
	}
};

inline Particle3DConfigurator::~Particle3DConfigurator()
{
}
