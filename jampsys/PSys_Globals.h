#ifndef PSysGlobals_h__
#define PSysGlobals_h__

#if !defined(IW_PLATFORM_S3E) && !defined(IW_PLATFORM_S3E_LIB)
#pragma warning (disable:4244)
#pragma warning (disable:4018)
#pragma warning (disable:4305)
#pragma warning (disable:4800)
#endif

#include <algorithm>
#include <string>


#define DbgPrintf	//
#define dbgprintf


#define PSYS_INFINITE_LOOPS		0
#define PSYS_STOP				0
#define PSYS_GO					1
#define PSYS_END				-1
#define PSYS_NULL			 -99999

#define PARTICLE	Particle3D

#define PARENT_PSYS		emitter->PSysParent_ref


#define MAX_EMITTERS_POOL	2000
#define MAX_PARTICLES_POOL	2000
#define MIN_ALPHA_THRESHOLD	48.0f
#define MIN_SIZE_THRESHOLD	0.2f
#define MAX_PARTICLES_EMITTED 800
#define MIN_PARTICLES_EMITTED 256



#endif // PSysGlobals_h__

