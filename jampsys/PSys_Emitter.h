#ifndef PSysEmitter_h__
#define PSysEmitter_h__

#include <queue>
#include <vector>

class PSYS;
class PSysIParticle;
class PARTICLE;
class IParticleConfigurator;

struct PSysPivot
{
	float x,y;
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
	void RemoveParticle(PARTICLE* pa,  bool forceDelete=false);
	virtual void SetRemovable();
	bool update();
	virtual void destroy();
	
	//Particle3D* modelParticle;						// A model for the particle re-emitted
	IParticleConfigurator* GetConfigurator() const { return m_ParticleTemplate; }
	void SetConfigurator(IParticleConfigurator* starterModel) { m_ParticleTemplate = starterModel; }
	bool CreateEmission( int howmany, int _groups=1, int distance=1, int newDuration = PSYS_NULL);
	void Init();
	bool updateRender();
	void updateMovements();

	// Attribs
	int	distance;
	int	groups;										// how many emitted together
	int	nrAlive; 										// Number of particles still alive.	
	float windX, windY, windZ;						// wind direction for all the particles
	
	int	status;
	std::string name;

	float GetPivotX() const { return (pivot_ref) ? pivot_ref->x : 0; }
	float GetPivotY() const { return (pivot_ref) ? pivot_ref->y : 0; }
	
	PSysPivot*	pivot_ref;								// Followed entity, attach the emitter to it
	PSYS* PSysParent_ref; 	

	IParticleConfigurator*	m_ParticleTemplate;
	std::vector<PARTICLE*> particles;
	std::queue<PARTICLE*> removed;

	int m_optimize_min_particles_emitted;
	int m_optimize_max_particles_emitted;
	float m_optimize_min_alpha_threshold;
	float m_optimize_min_size_threshold;
	float m_optimize_min_framerate;

private:

	size_t idPa;
};

// ******************************************************************************
PSysEmitter* PSysSetEmitter(PSysEmitter* pPSS, int howmany=1 ,int  distance = 1, int groups=1,std::string name ="");
PSysEmitter* PSysSetWind(PSysEmitter* pPSS, float wx=PSYS_NULL, float wy=PSYS_NULL, float wz=PSYS_NULL );
PSysEmitter* PSysSetPivotMovements( PSysEmitter* pPSS, float fromX, float fromY, float toX, float toY, float duration/*=PSYS_NULL */ );
PSysEmitter* PSysSetOptimizationPool( PSysEmitter* pPSS, int minimal=MIN_PARTICLES_EMITTED, int maximal=MAX_PARTICLES_EMITTED);
PSysEmitter* PSysSetOptimizationFrameRate( PSysEmitter* pPSS,const std::string& name="", float minimal=PSYS_NULL);
#endif // PSysEmitter_h__
