/**********************************************************************************
* 
* Application.cpp
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

#include "stdafx.h"

#include <iostream>

#include "jam/Application.h"
#include "jam/core/bmkextras.hpp"
#include "jam/StateMachine.h"
#include "jam/GameManager.h"
#include "jam/DrawItemManager.h"
#include "jam/Animation2dManager.h"
#include "jam/InputManager.h"
#include "jam/AudioManager.h"
#include "jam/ActionManager.h"
#include "jam/CollisionManager.h"
#include "jam/Timer.h"
#include "jam/Draw3dManager.h"
#include "jam/TextNode.h"
#include "jam/DeviceManager.h"
#include "jam/AutoReleasePool.h"
#include "jam/Event.h"
#include "jam/Scene.h"
#include "jam/Achievement.h"
#include "jam/SysTimer.h"
#include "jam/Gfx.h"
#include "jam/Camera.h"

#ifdef JAM_PHYSIC_ENABLED
#include <Box2D/Dynamics/b2World.h>
#endif

#include <stdexcept>
#include <typeinfo>

//////////////////////////////////////////////////////////////////////////
// Debug Menu
//

// Attempt to lock to 60 frames per second
#define JAM_APP_DEFAULT_MS_PER_FRAME (1000UL / 60UL)

namespace jam
{

Application::Application() :
	m_engineInited(false),
	m_elapsedMs(0),
	m_totalElapsedMs(0),
	m_secsElapsed(0.0f),
	m_secsTotalElapsed(0.0f),
	m_lastTimeMs(0),
	m_sceneNode(0),
	m_exitFromMainLoop(false),
	m_callAppHandlers(true),
	m_minMsPerFrame(0),
	m_maxMsPerFrame(0),
	m_msPerFrame(0),
	m_avgMsPerFrameIdx(0),
#ifdef JAM_PHYSIC_ENABLED
	m_pPhysWorld(nullptr)
	m_physicsEnabled(false),
	m_ptmRatio(0),
#endif		
	m_animationIntervalMs(JAM_APP_DEFAULT_MS_PER_FRAME),
	m_actionSpeed(1.0f),
	m_clearColorBuffer(true),
	m_clearColor(Color::BLACK),
	m_bClearColorChanged(true),
	m_paused(false),
#ifdef JAM_TRACE_ACTIVE_NODES
	m_traceActiveNodesTimer(nullptr),
#endif
	m_collisionCheckFactor(1.0f),
	m_audioCheckFactor(1.0f),
	m_inputCheckFactor(1.0f),
	m_sysTimerManager(nullptr),
	m_resourceManager(nullptr),
	m_pWindow(nullptr),
	m_GLContext(nullptr)
{
	setup() ;
}

Application::~Application()
{
	cleanup() ;
}

void Application::start()
{
	if( !m_engineInited ) {
		JAM_ERROR( ("jam::initialize() must be called before to start application") ) ;
	}

	// default setting
	m_animationIntervalMs = SysTimer().getUnitsPerSecond() / 60UL ;

	m_totalElapsedMs = GetSysTimer().getTime() ;
	refreshTime() ;

	m_sceneNode = new (GC) Scene() ;

//		IwGxLightingOff() ;
//		IwGxMipMappingOff() ;
//		IwGxSetNormStream(0) ;

	//these are the default IwGx sort mode settings
	//IwGxSetSortMode(IW_GX_SORT_BY_SLOT) ;
	//IwGxSetSortModeScreenSpace(IW_GX_SORT_BY_MATERIAL) ;
	GetGfx().setDepthTest(true) ;

#ifndef JAM_DRAW3DBATCH_DISABLED
	Draw3DBatch* batch = new Draw3DBatch();
	GetGfx().setBatch(batch) ;
#endif

#ifdef JAM_PHYSIC_ENABLED
	m_ptmRatio = GetDeviceMgr().getNativeDisplayWidth() / 10.0f ;
#endif
	// create default resource cache
	IResourceFile* pResourceFile = new FileSystemResourceFile(ShaderManager::DEFAULT_SHADERS_PATH) ;
	m_resourceManager = new (GC) ResourceManager(10,pResourceFile) ;
	m_resourceManager->init() ;
	m_resourceManager->registerLoader( new (GC) ShaderFileResourceLoader() ) ;

	// create default shaders
	GetShaderMgr().createDefaultLit() ;
	GetShaderMgr().createSkinningLit() ;
	GetShaderMgr().createSkyBox() ;
	GetShaderMgr().createNormalMapping() ;
	GetShaderMgr().createScreen() ;
	GetShaderMgr().createDefaultUnlit() ;

	if( game::GetStateMachine().isStarted() ) {
		game::GetStateMachine().checkNewState() ;
	}

	m_minMsPerFrame = GetSysTimer().getUnitsPerSecond() ;
	m_maxMsPerFrame = 0 ;

	if( !init() ) // virtual call
	{
		return ;
	}

	m_sceneNode->init() ;
	m_sceneNode->onEnter() ;

	m_sysTimerManager = new (GC) TimerManager() ;

#ifndef JAM_COLLISIONS_DISABLED
	GetCollMgr().getUpdateTimer().setSweep((uint64_t)(m_animationIntervalMs*m_collisionCheckFactor)) ;
	GetCollMgr().getUpdateTimer().setRepeatSweep(-1) ;
	GetCollMgr().getUpdateTimer().setName("CollMgr.updateTimer") ;
	GetCollMgr().getUpdateTimer().start() ;

	getSysTimerManager().addObject(&GetCollMgr().getUpdateTimer()) ;
#endif
	GetAudioMgr().getUpdateTimer().setSweep((uint64_t)(m_animationIntervalMs*m_audioCheckFactor)) ;
	GetAudioMgr().getUpdateTimer().setRepeatSweep(-1) ;
	GetAudioMgr().getUpdateTimer().setName("AudioMgr.updateTimer") ;
	GetAudioMgr().getUpdateTimer().start() ;
	getSysTimerManager().addObject(&GetAudioMgr().getUpdateTimer()) ;

	GetInputMgr().getUpdateTimer().setSweep((uint64_t)(m_animationIntervalMs*m_inputCheckFactor)) ;
	GetInputMgr().getUpdateTimer().setRepeatSweep(-1) ;
	GetInputMgr().getUpdateTimer().setName("InputMgr.updateTimer") ;
	GetInputMgr().getUpdateTimer().start() ;
	getSysTimerManager().addObject(&GetInputMgr().getUpdateTimer()) ;

#ifdef JAM_TRACE_ACTIVE_NODES
	m_traceActiveNodesTimer = Timer::create() ;
	m_traceActiveNodesTimer->setReserved() ;
	m_traceActiveNodesTimer->getTimeExpiredEvent() += JAM_EVT_HANDLER(Application,TimeExpiredEventArgs,this,&Application::traceActiveNodes) ;
	m_traceActiveNodesTimer->setRepeatSweep(-1) ;
	m_traceActiveNodesTimer->setRepeatSweepTime(5000) ;
	m_traceActiveNodesTimer->start() ;
	GetTimerMgr().addByName(m_traceActiveNodesTimer,"Application::traceActiveNodesTimer") ;
#endif

	// ------------------------
	// end of game initialization sequence
	// ------------------------

	// enters main game infinite loop, until exit conditions are met
	gameLoop();

	// ------------------------
	// game exit sequence
	// ------------------------

	// ------------------------
	// game exit sequence
	// ------------------------
	if( game::GetStateMachine().isStarted() ) 
	{
		game::GetStateMachine().getCurrentState()->end();
		game::GetStateMachine().getCurrentState()->destroy() ;
	}
	destroy() ; // virtual call

#ifdef JAM_TRACE_ACTIVE_NODES
	JAM_RELEASE(m_traceActiveNodesTimer) ;
#endif

#ifdef JAM_DEBUG_MENU_ENABLED
	destroyDebugMenu() ;
#endif

#ifdef IW_USE_PROFILE
	destroyProfileMgr() ;
#endif

#ifndef JAM_DRAW3DBATCH_DISABLED
	JAM_DELETE(batch) ;
#endif

	m_sceneNode->onExit() ;
	m_sceneNode->destroy() ;

	m_sceneNode = nullptr ;
}

void Application::gameLoop()
{
	int64_t yieldMs = 0 ;
		
	SDL_Event e;
	while( !m_exitFromMainLoop )
	{
		m_totalElapsedMs = GetSysTimer().getTime() ;
		refreshTime() ;

		while( SDL_PollEvent( &e ) != 0 )
		{
			if( e.type == SDL_QUIT ) {
				m_exitFromMainLoop = true;
			}
			else if( e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP ) {
				GetInputMgr().mouseButtonCallback( (SDL_MouseButtonEvent&)e ) ;
			}
			else if( e.type == SDL_MOUSEMOTION ) {
				GetInputMgr().mouseMoveCallback( (SDL_MouseMotionEvent&)e ) ;
			}
			else if( e.type == SDL_MOUSEWHEEL ) {
				GetInputMgr().mouseScrollCallback( (SDL_MouseWheelEvent&)e ) ;
			}
			else if( e.type == SDL_KEYDOWN || e.type == SDL_KEYUP ) {
				GetInputMgr().keyboardCallback( (SDL_KeyboardEvent&)e ) ;
			}
			else if( e.type == SDL_WINDOWEVENT ) {
				switch (e.window.event) {
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					framebufferSizeCallback( e.window.data1, e.window.data2 ) ;
					break;
				case SDL_WINDOWEVENT_CLOSE:
					break;
				}
			}
		}

		// handle machine change state
		if( game::GetStateMachine().isStarted() ) {
			game::GetStateMachine().checkNewState() ;
		}

		doFrame();

		// handle machine change state
		if( game::GetStateMachine().isStarted() ) {
			game::GetStateMachine().update() ;
		}

		while( true ) {
 			yieldMs = (int64_t)getRemainingFrameMs() ;
			if (yieldMs<=0) break ;
			SDL_Delay((DWORD)yieldMs) ;
		}

		if( getTotalElapsed() > 1.0f ) {
			updateMsPerFrame() ;
		}

	}	// main game loop
}

uint64_t Application::getRemainingFrameMs() const
{
	uint64_t elapsedInThisFrameMs = (uint64_t)(GetSysTimer().getTime() - m_totalElapsedMs) ;
	return (elapsedInThisFrameMs < m_animationIntervalMs) ? (m_animationIntervalMs - elapsedInThisFrameMs) : 0 ;
}

Scene* Application::getScene()
{
	return m_sceneNode;
}

TimerManager& Application::getSysTimerManager()
{
	return *m_sysTimerManager ;
}

void Application::setup()
{
	m_singleton = this ;
}

void Application::cleanup()
{
}

/**
	Sequence of private/overrides methods called in a frame

	clear frame buffer
	calculate and refresh previuos frame duration (elapsed) and current frame time
	read engine input
	enterFrame (application and state)
	handleInput (application and state)
	update timers (timeexpired event are fired, not queued)
	update actions
	beforeSceneUpdate (application and state)
	update collisions
	queued events dispatch (touches, collisions, achievement completed)
	update physics
	custom render (application and state)
	render nodes
	afterSceneUpdate (application and state)
	gfx flush
	update touchable nodes
	sound update
	exitFrame (application and state)
	swap frame buffers
*/
void Application::doFrame()
{
	/* TODO
	s3eDeviceYield(0);
	*/

	Draw3DBatch* pBatch = 0 ;

	// Clear the screen
	GLbitfield clearFlags = GL_DEPTH_BUFFER_BIT ;
	if( m_clearColorBuffer ) {
		clearFlags |= GL_COLOR_BUFFER_BIT ;
		if( m_bClearColorChanged ) { 
			GetGfx().setClearColor( m_clearColor ) ;
			m_bClearColorChanged = false ;
		}
	}
	GetGfx().clear( clearFlags ) ;

	// handle input update
	GetInputMgr().update() ;

	Draw3DManager::Origin3D() ;
	Draw3DManager::Clear3D();

	pBatch = GetGfx().getBatch() ;
	if( pBatch ) {
		pBatch->reset() ;
	}

	game::State* gState = game::GetStateMachine().isStarted() ? game::GetStateMachine().getCurrentState() : 0 ;

	// enterFrame virtual calls
	if( m_callAppHandlers && !isPaused() ) {
		enterFrame() ;
	}

	if( gState && !isPaused() ) {
		gState->enterFrame() ;
	}

	// handle handleInput
//		if( bInputMgrSweep )
	{
		if( m_callAppHandlers ) handleInput() ;
		if( gState ) gState->handleInput() ;
	}

	// update registered timers
	if( !isPaused() ) {
		getSysTimerManager().update() ;
	}

	// update actions

	if( !isPaused() ) {
		GetActionMgr().update( getElapsed() ) ;
	}

	if( m_callAppHandlers && !isPaused() ) {
		beforeSceneUpdate() ;			// virtual call
	}
	if( gState != 0 && !isPaused() ) {
		gState->beforeSceneUpdate() ;
	}

#ifdef JAM_TRACE_ACTIVE_NODES
	Node::clearDebugNodeInfo() ;
#endif

#ifdef JAM_PHYSIC_ENABLED
	// update physics
	if( m_physicsEnabled && !isPaused() ) {
		JAM_PROFILE("Box2d.step") ;
		m_pPhysWorld->Step( 1.0f/60.0f, 10, 8 ) ;
		// By default, forces will be automatically cleared, so you don't need to call this function.
		// m_pPhysWorld->ClearForces();
	}
#endif

	// handle collisions
	if( m_callAppHandlers && !isPaused() ) {
		updateCollisions() ;
	}

	GetEventDispatcher().dispatch() ;

	// default unlit to draw 2d scene
	Shader* pCurrentShader = GetShaderMgr().getDefaultUnlit() ;
	pCurrentShader->use() ;

	// handle render
	Camera* pCamera = getScene()->getCamera() ;
	if( pCamera ) {
		pCamera->setActive() ;
		pCurrentShader->setModelMatrix( Matrix4(1.0f) ) ;
		pCurrentShader->setViewMatrix( pCamera->getViewMatrix() ) ;
		pCurrentShader->setProjectionMatrix( pCamera->getProjectionMatrix() ) ;
		pCurrentShader->setViewPosition( pCamera->getPosition() ) ;	//  for specular light
		Node::setCurrentCamera(pCamera) ;
	}

	if( m_callAppHandlers ) render() ;			// virtual call
	if( gState != 0 ) gState->render() ;

	// render nodes
#ifdef JAM_TRACE_INVIEW_SPRITES
	Sprite::setTotalInView(0) ;
#endif
	getScene()->clearTouchableNodes() ;
	getScene()->visitGraph( pBatch ) ;

#ifdef JAM_TRACE_INVIEW_SPRITES
	JAM_TRACE( ("Total sprites %d, in view: %d", Sprite::getTotalSprites(), Sprite::getTotalInView()) ) ;
#endif

	if( m_callAppHandlers && !isPaused() ) {
		afterSceneUpdate() ;			// virtual call
	}
	if( gState != 0 && !isPaused() ) {
		gState->afterSceneUpdate() ;
	}

	pCurrentShader->stopUsing();

	// end drawing
	glFlush() ;

	m_sceneNode->updateTouchableNodes() ;

	// handle sound update
	updateSounds() ;

	if( m_callAppHandlers && !isPaused() ) {
		exitFrame() ;
	}
	if( gState && !isPaused() ) {
		gState->exitFrame() ;
	}

	// swap buffers
	SDL_GL_SwapWindow(m_pWindow) ;
}

void Application::refreshTime()
{
	m_elapsedMs = (m_totalElapsedMs - m_lastTimeMs);
	m_lastTimeMs = m_totalElapsedMs ;
	m_elapsedMs = (uint64_t)(m_elapsedMs * m_actionSpeed) ;		// scales elapsed

	m_secsElapsed = m_elapsedMs / ((float)GetSysTimer().getUnitsPerSecond()) ;
	m_secsTotalElapsed = m_totalElapsedMs / ((float)GetSysTimer().getUnitsPerSecond()) ;
}


#ifdef JAM_PHYSIC_ENABLED
void Application::enablePhysics( bool enable /*= true*/ )
{
	m_physicsEnabled = enable ;
}

b2World* Application::getB2World()
{
	return m_pPhysWorld ;
}

void Application::setPtmRatio( float ptmRatio )
{
	m_ptmRatio = ptmRatio;
}
#endif


// ****************************************************************
// *** Concrete application class should override these methods ***
//

bool Application::init()
{
	return true ;
}

void Application::destroy()
{
}

void Application::enterFrame()
{
}

void Application::handleInput()
{
}

void Application::beforeSceneUpdate()
{
}

void Application::afterSceneUpdate()
{
}

void Application::render()
{
}

void Application::extraRender()
{

}

void Application::exitFrame()
{
}

void Application::idle()
{
}

float Application::getFps() const
{
	return ((float)GetSysTimer().getUnitsPerSecond()) / m_msPerFrame ;
}

float Application::getFpsMin() const
{
	return ((float)GetSysTimer().getUnitsPerSecond()) / m_maxMsPerFrame ;
}

float Application::getFpsMax() const
{
	return ((float)GetSysTimer().getUnitsPerSecond()) / m_minMsPerFrame ;
}

float Application::getFpsAvg() const
{
	return ((float)GetSysTimer().getUnitsPerSecond()) / m_avgMsPerFrame ;
}

void Application::setAnimationInterval( jam::time interval )
{
	setAnimationIntervalMs((uint64_t)(interval * 1000)) ;
}


void Application::setAnimationIntervalMs( uint64_t interval )
{
	m_animationIntervalMs = interval ;
	setCollisionCheckFactor( m_collisionCheckFactor) ;
	setAudioCheckFactor( m_audioCheckFactor ) ;
	setInputCheckFactor( m_inputCheckFactor ) ;
}


jam::time Application::getAnimationInterval() const
{
	return ((float)m_animationIntervalMs) / GetSysTimer().getUnitsPerSecond();
}

void Application::signalExit()
{
	m_exitFromMainLoop = true ;
}


void Application::setClearColor( const Color& val )
{
	m_clearColor = val;
	m_bClearColorChanged = true ;
}

void Application::updateCollisions()
{
	const Timer& timer = GetCollMgr().getUpdateTimer() ;
	if( timer.isSweep() ) {
		GetCollMgr().update( getElapsed()/**m_actionSpeed*/);
	}
}

void Application::updateSounds()
{
	const Timer& timer = GetAudioMgr().getUpdateTimer() ;
	if( timer.isSweep() ) {
		GetAudioMgr().update( getElapsed() );
	}
}

void Application::updateMsPerFrame()
{
	// this is the elapsed time since the start of frame
	m_msPerFrame = GetSysTimer().getTime() - m_totalElapsedMs ;
	m_minMsPerFrame = Min( m_minMsPerFrame, m_msPerFrame ) ;
	m_maxMsPerFrame = Max( m_maxMsPerFrame, m_msPerFrame ) ;
	m_avgMsPerFrameBuffer[m_avgMsPerFrameIdx++] = m_msPerFrame ;

	if( m_avgMsPerFrameIdx >= JAM_APP_MSPERFRAME_SAMPLES) {
		m_avgMsPerFrame = 0 ;
		for( size_t i=0; i<JAM_APP_MSPERFRAME_SAMPLES; i++ ) 
			m_avgMsPerFrame += m_avgMsPerFrameBuffer[i] ;

		m_avgMsPerFrame /= JAM_APP_MSPERFRAME_SAMPLES ;
		m_avgMsPerFrameIdx = 0 ;
	}
}

void Application::framebufferSizeCallback(int width,int height)
{
	glViewport( 0,0, width,height ) ;
	float aspect = width / (float)height ;
	GetAppMgr().getScene()->getCamera()->setAspectRatio( aspect ) ;
}

void Application::pause()
{
	m_paused = true;
	getSysTimerManager().pauseAll();
}

void Application::resume()
{
	m_paused = false;
	getSysTimerManager().resumeAll();
}

void Application::togglePause()
{
	m_paused = !m_paused ;
	if (m_paused) getSysTimerManager().pauseAll();
	else getSysTimerManager().resumeAll();
}

#ifdef JAM_DEBUG
void APIENTRY glDebugOutput(GLenum source, 
							GLenum type, 
							GLuint id, 
							GLenum severity, 
							GLsizei length, 
							const GLchar *message, 
							const void *userParam)
{
	// ignore non-significant error/warning codes
	if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " <<  message << std::endl;

	switch (source)
	{
		case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
		case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;
    
	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}
#endif

void Application::engineInitialize()
{
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		JAM_ERROR( "Failed to initialize SDL" );
	}		

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 ) ;
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 2 ) ;
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, 0 ) ;	// leaves the choice of profile up to SDL
	SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4) ;
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 ) ;	// depth buffer is 24-bit by default
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 ) ;	// enable double buffer

#ifdef JAM_DEBUG
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG ) ;
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		// initialize debug output 
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
#endif

	// Open a window and create its OpenGL context
	m_pWindow = SDL_CreateWindow( "Jam Engine", 
								 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
								 JAM_WINDOWS_WIDTH, JAM_WINDOWS_HEIGHT,
								 SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
	if( m_pWindow == NULL ){
		SDL_Quit();
		JAM_ERROR("Failed to create window") ;
	}
	
	if( ! (m_GLContext = SDL_GL_CreateContext( m_pWindow )) ) {
		SDL_DestroyWindow( m_pWindow );
		m_pWindow = NULL;
		SDL_Quit();
		JAM_ERROR("Failed to create GL context: %s", SDL_GetError() ) ;
	}

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		SDL_GL_DeleteContext(m_GLContext);
		SDL_DestroyWindow( m_pWindow );
		m_pWindow = NULL;
		SDL_Quit();
		JAM_ERROR("Failed to initialize GLEW") ;
	}

	// Use Vsync
	if( SDL_GL_SetSwapInterval( 1 ) < 0 ) {
		SDL_GL_DeleteContext(m_GLContext);
		SDL_DestroyWindow( m_pWindow );
		m_pWindow = NULL;
		SDL_Quit();
		JAM_ERROR( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
	}

	glViewport( 0, 0, JAM_WINDOWS_WIDTH, JAM_WINDOWS_HEIGHT ) ;

#ifdef JAM_PHYSIC_ENABLED
	m_pPhysWorld = new b2World( b2Vec2(0,0), true ) ;
#endif

	m_engineInited = true ;
}

void Application::engineTerminate()
{
	// Cleanup banks
	GetActionMgr().removeAllActions() ;
/*
	GetAliveSpriteFactoryMgr().removeAllBankItems(true) ;
	GetTextNodeMgr().removeAllBankItems(true) ;
	GetAnim2DMgr().removeAllBankItems(true) ;
	GetDrawItemMgr().removeAllBankItems(true) ;
	GetAudioMgr().removeAllBankItems(true) ;
*/
	getSysTimerManager().clearAll() ;
/*
	GetTimerMgr().removeAllBankItems(true) ;
	GetAchievementMgr().removeAllBankItems(true) ;
	GetMaterialMgr().removeAllBankItems(true) ;
	GetTextureMgr().removeAllBankItems(true) ;

	GetAutoReleasePoolMgr().collect(true) ;
*/
	// delete singletons
//	AliveSpriteFactoryManager::destroySingleton() ;
//	NodeStateManager::destroySingleton() ;
	CollisionManager::destroySingleton() ;
	Animation2DManager::destroySingleton() ;
	DrawItemManager::destroySingleton() ;
	game::StateMachine::destroySingleton() ;
	game::GameManager::destroySingleton() ;
	InputManager::destroySingleton() ;
	AudioManager::destroySingleton() ;
	DeviceManager::destroySingleton() ;
	TextNodeManager::destroySingleton() ;
	ActionManager::destroySingleton() ;
	Draw3DManager::destroySingleton() ;
	EventDispatcher::destroySingleton() ;	
//	AchievementManager::destroySingleton() ;
	SysTimer::destroySingleton() ;
	Gfx::destroySingleton() ;
	MaterialManager::destroySingleton() ;
//	TextureManager::destroySingleton() ;


	// cleanup and delete autorelease pool
//	AutoReleasePool::destroySingleton() ;

#ifdef JAM_PHYSIC_ENABLED
	JAM_DELETE(m_pPhysWorld) ;
#endif

#ifdef OLDCODE
	ObjectManager::destroySingleton() ;
#endif
	
	if( m_GLContext ) {
		SDL_GL_DeleteContext(m_GLContext);
		m_GLContext = 0 ;
	}

	if( m_pWindow ) {
		SDL_DestroyWindow( m_pWindow );
		m_pWindow = 0;
	}

	SDL_Quit();
}

#ifdef JAM_TRACE_ACTIVE_NODES
void Application::traceActiveNodes( TimeExpiredEventArgs& args, IEventSource& source )
{
	Node::dumpDebugNodeInfo() ;
}
#endif

#ifndef JAM_COLLISIONS_DISABLED
void Application::setCollisionCheckFactor( float val )
{
	m_collisionCheckFactor = val;
	GetCollMgr().getUpdateTimer().setSweep((uint64_t)(m_animationIntervalMs*m_collisionCheckFactor)) ;
	GetCollMgr().getUpdateTimer().stop() ;
	GetCollMgr().getUpdateTimer().start() ;
}
#endif

void Application::setAudioCheckFactor( float val )
{
	m_audioCheckFactor = val;
	GetAudioMgr().getUpdateTimer().setSweep((uint64_t)(m_animationIntervalMs*m_audioCheckFactor)) ;
	GetAudioMgr().getUpdateTimer().stop() ;
	GetAudioMgr().getUpdateTimer().start() ;
}

void Application::setInputCheckFactor( float val )
{
	m_inputCheckFactor = val;
	GetInputMgr().getUpdateTimer().setSweep((uint64_t)(m_animationIntervalMs*m_inputCheckFactor)) ;
	GetInputMgr().getUpdateTimer().stop() ;
	GetInputMgr().getUpdateTimer().start() ;
}

ResourceManager& Application::getResourceManager()
{
	return *m_resourceManager ;
}

}	// namespace jam

