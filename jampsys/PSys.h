#ifndef PSys_h__
#define PSys_h__

#include "PSys_Globals.h"
#include "PSys_Emitter.h"
#include "PSys_Particle.hpp"
#include "PSys_Particle3D.h"
#include "PSys_ParticleSprite3D.h"
#include "PSys_ParticleConfigurator.h"

#include <map>
#include <jam/Singleton.h>

struct emitterGroup
{
	emitterGroup(){name=""; min_framerate=0; lastUpdate=0;};
	std::string name;
	float min_framerate;
	float lastUpdate;
};

class PSysEmitter;
class IParticleConfigurator;

// The Particle System Engine
class PSYS : public  jam::Singleton<PSYS>
{
	friend class jam::Singleton<PSYS> ;
public:

	virtual void destroy();
	void update();
	void updateRender();

	void RemoveEmitter(PSysEmitter* em,  bool forceDelete=false);
	void SubscribeEmitter(PSysEmitter* em);
	int totalParticles() { return alives;}
	int totalRemoved() { return zombies;}
	int totalEmitters() { return (int)emitters.size();}
	void setAlives(int val) { alives = val; }
	void setZombies(int val) { zombies = val; }
	int getOptimized() const { return optimized; }
	void setOptimized(int val) { optimized = val; }

	int getLevel() const { return slot; }

	bool clearAll();
	PSysEmitter* CreateEmitter( IParticleConfigurator* starterModel, std::string name="" );
	
	std::queue<PSysEmitter*> eremoved;
	std::vector<PSysEmitter*> emitters;
	std::map<std::string, emitterGroup> groups;

private:
	PSYS();
	virtual ~PSYS();

	size_t idEm;
	int alives;
	int zombies;
	int optimized;
	int slot ;
};

inline PSYS& GetParticleSystem() { return PSYS::getSingleton(); }

#endif // PSys_h__

