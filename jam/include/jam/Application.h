/**********************************************************************************
* 
* Application.h
* 
* This file is part of Jam
* 
* Copyright (c) 2014-2019 Giovanni Zito.
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

#ifndef __JAM_APPLICATION_H__
#define __JAM_APPLICATION_H__

#include <jam/jam.h>

#include <jam/Singleton.h>
#include <jam/Color.h>
#include <jam/ResourceManager.h>

#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/glew.h>
//#include <GLFW/glfw3.h>
#include <SDL.h>

#include <string>
#include <set>
#include <vector>
#include <list>
#include <map>

namespace jam
{

class Scene ;
class Node ;
class Timer ;

#ifdef JAM_PHYSIC_ENABLED
class b2World ;
#endif
#ifdef JAM_TRACE_ACTIVE_NODES
class TimeExpiredEventArgs ;
class RefCountedObject ;
typedef RefCountedObject IEventSource ;
#endif

#define JAM_APP_MSPERFRAME_SAMPLES			60

/** Base class which manages the standard startup, the main loop and the cleanup of an application
    Designed to be subclassed to implement the wanted behaviour.
*/
class JAM_API Application : public jam::Singleton<Application>
{
	friend class Singleton<Application> ;
	friend class Scene ;

public:
	/// Returns time (in seconds) since last frame, i.e. how many seconds the previous frame lasted
	jam::time				getElapsed() const { return m_secsElapsed; }

	/// Returns total time (in seconds) since application started. It is refreshed every enter frame
	jam::time				getTotalElapsed() const { return m_secsTotalElapsed; }

	/// Returns time (in ms) since last frame, i.e. how many milliseconds the previous frame lasted
	uint64_t				getElapsedMs() const { return m_elapsedMs; }
	
	/// Returns total time (in ms) since application started. It is refreshed every enter frame
	uint64_t				getTotalElapsedMs() const { return m_totalElapsedMs; }

	/// Returns remaining time (in ms) since the current frame started
	uint64_t				getRemainingFrameMs() const;

	/// Gets the pointer to the scene node
	Scene*					getScene();

	ResourceManager&		getResourceManager() ;

#ifdef JAM_PHYSIC_ENABLED
	static b2World*			getB2World() ;
	void					enablePhysics(bool enable = true) ;

	float					getPtmRatio() const { return m_ptmRatio; }
	void					setPtmRatio(float ptmRatio) ;
#endif

	/// Returns frames per second value
	float					getFps() const ;

	/// Returns minimum frames per second value, since the application started
	float					getFpsMin() const ;

	/// Returns maximum frames per second value, since the application started
	float					getFpsMax() const ;

	/// Returns average frames per second value, calculated every 60 frames
	float					getFpsAvg() const ;

	/// Sets the requested duration of a single frame (in secs). Defaults to 1/60.0 (16.667 ms)
	void					setAnimationInterval(jam::time interval) ;

	/// Sets the requested duration of a single frame (in ms). Defaults to 16.667 ms (1/60.0)
	void					setAnimationIntervalMs(uint64_t interval) ;

	/// Returns the requested duration of a single frame (in secs)
	jam::time				getAnimationInterval() const;

	/// Returns the requested duration of a single frame (in ms)
	uint64_t				getAnimationIntervalMs() const { return m_animationIntervalMs; }

	/// Sets the actions speed. Defaults to 1
	void					setActionsSpeedUpdate(float interval) {m_actionSpeed=interval;}
	float					getActionsSpeedUpdate() const { return m_actionSpeed; }

	/// use this method to signal the application exit
	void					signalExit();

	void					enableClearingColorBuffer(bool val) { m_clearColorBuffer = val; }
	void					setClearColor( const Color& val );

	// Starts the application and enters the main loop
	// called from main, so it must be public
	void					start() ;

	void					pause();
	void					resume();
	void					togglePause() ;
	bool					isPaused() const { return m_paused; }

	static SDL_Window*		getWindowPtr() ;

	static void				engineInitialize() ;
	static void				engineTerminate() ;

#ifdef JAM_TRACE_ACTIVE_NODES
	void					traceActiveNodes(TimeExpiredEventArgs& args, IEventSource& source) ;
#endif

protected:
							Application() ;
	virtual					~Application() ;

	/** Runs endless main game loop, until exit conditions are met */
	void					gameLoop();

	/**	Called when the appication starts */
    virtual bool			init();

	/** Called when the current frame is entered. */
	virtual void			enterFrame();

	/**	Called just after entering the frame. */
	virtual void			handleInput();

	/**	Called before the scene is updated. */
	virtual void			beforeSceneUpdate();

	/** Called after the scene is updated. */
	virtual void			afterSceneUpdate();

	/**	Called before the scene is rendered. */
	virtual void			render();

	virtual void			extraRender() ;

	/**	Called before the current frame ends, just before the frame buffer is flipped */
	virtual void			exitFrame();

	/** Called when the application is exited. */
	virtual void			destroy() ;

	/** Called when the application is idle. */
	virtual void			idle() ;

	float					getCollisionCheckFactor() const { return m_collisionCheckFactor; }
	void					setCollisionCheckFactor(float val);

	float					getAudioCheckFactor() const { return m_audioCheckFactor; }
	void					setAudioCheckFactor(float val);

	float					getInputCheckFactor() const { return m_inputCheckFactor; }
	void					setInputCheckFactor(float val);

private:
	void					setup() ;
	void					cleanup() ;
	void					doFrame();
	void					refreshTime() ;
	void					updateCollisions();
	void					updateSounds() ;
	void					updateMsPerFrame();

	static void				framebufferSizeCallback(SDL_Window* window, int width, int height) ;

private:
	static bool				m_engineInited ;

	// note: time is stored in milliseconds
	uint64_t				m_totalElapsedMs ;
	uint64_t				m_elapsedMs ;
	uint64_t				m_lastTimeMs ;
	uint64_t				m_animationIntervalMs ;
	float					m_actionSpeed;
		
	// time in seconds, it's refreshed every frame
	jam::time				m_secsElapsed ;
	jam::time				m_secsTotalElapsed ;

	Scene*					m_sceneNode ;
	bool					m_exitFromMainLoop ;
	bool					m_callAppHandlers ;

	// fps
	uint64_t				m_msPerFrame ;
	uint64_t				m_minMsPerFrame ;
	uint64_t				m_maxMsPerFrame ;

	uint64_t				m_avgMsPerFrame ;
	uint64_t				m_avgMsPerFrameBuffer[JAM_APP_MSPERFRAME_SAMPLES] ;
	size_t					m_avgMsPerFrameIdx ;

	bool					m_clearColorBuffer ;
	Color					m_clearColor ;
	bool					m_bClearColorChanged ;

	bool					m_paused ;

#ifdef JAM_PHYSIC_ENABLED
	// physix
	static b2World*			m_pPhysWorld ;
	bool					m_physicsEnabled ;
	float					m_ptmRatio ;		// pixel to meter ratio for Box2D
#endif

	float					m_collisionCheckFactor ;
	float					m_audioCheckFactor ;
	float					m_inputCheckFactor ;

#ifdef JAM_TRACE_ACTIVE_NODES
	Timer*					m_traceActiveNodesTimer ;
#endif

	static SDL_Window*		m_pWindow;
			
	uptr<ResourceManager>	m_resourceManager ;
};	// class Application

JAM_INLINE Application& GetAppMgr() { return Application::getSingleton(); }

JAM_INLINE SDL_Window* Application::getWindowPtr() { return m_pWindow ; }




template <class T> void runEngine()
{
	try
	{
		Application::engineInitialize() ;

		T* app = new T();
		app->start();
		JAM_DELETE( app ) ;

		Application::engineTerminate() ;
	}
	catch( std::exception& ex )
	{
		JAM_TRACE( "Exception thrown:\n%s\n", ex.what() ) ;
		printf( "Exception thrown:\n%s\n", ex.what() ) ; 
	}
	catch(...)
	{
		JAM_TRACE( "Unknown exception thrown!\n" ) ;
		printf( "Unknown exception thrown!\n" ) ; 
	}
};

}	// namespace jam

#endif // __JAM_APPLICATION_H__
