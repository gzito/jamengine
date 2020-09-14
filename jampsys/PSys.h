/**********************************************************************************
*
* PSys.h
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

#include "PSys_Emitter.h"
#include "PSys_ParticleSprite3D.h"
#include <jam/Singleton.h>

struct emitterGroup
{
	emitterGroup() { name = ""; min_framerate = 0; lastUpdate = 0; };
	std::string name;
	float min_framerate;
	float lastUpdate;
};

class PSysEmitter;
class IParticleConfigurator;

// The Particle System Engine
class PSYS : public  jam::Singleton<PSYS>
{
	friend class jam::Singleton<PSYS>;
public:

	virtual void destroy();
	void update();
	void updateRender();

	void RemoveEmitter(PSysEmitter* em, bool forceDelete = false);
	void SubscribeEmitter(PSysEmitter* em);
	int totalParticles() const { return alives; }
	int totalRemoved() const { return zombies; }
	int totalEmitters() const { return static_cast<int>(emitters.size()); }
	void setAlives(int val) { alives = val; }
	void setZombies(int val) { zombies = val; }
	int getOptimized() const { return optimized; }
	void setOptimized(int val) { optimized = val; }

	int getLevel() const { return slot; }

	bool clearAll();
	PSysEmitter* CreateEmitter(IParticleConfigurator* starterModel, std::string name = "");

	std::queue<PSysEmitter*> eremoved;
	std::list<PSysEmitter*> emitters;
	std::unordered_map<std::string, emitterGroup> groups;

private:

	PSYS();
	virtual ~PSYS();

	size_t idEm;
	int alives;
	int zombies;
	int optimized;
	int slot;
};

inline PSYS& GetParticleSystem() { return PSYS::getSingleton(); }

