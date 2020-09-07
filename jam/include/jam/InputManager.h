/**********************************************************************************
* 
* InputManager.h
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

#ifndef __JAM_INPUTMANAGER_H__
#define __JAM_INPUTMANAGER_H__

#include <jam/jam.h>
#include <jam/Singleton.h>

#include <SDL.h>

// todo : method to choose the maximum number of handled touches
#define JAM_MAX_TOUCHES   1

namespace jam\
{
class Timer ;

typedef SDL_Scancode key ;

class JAM_API InputManager : public Singleton<InputManager>
{
	friend class Singleton<InputManager> ;
	friend class Application ;

public:
	enum KeyStatus {
		UP  = 0,
		DOWN,
		PRESSED,
		RELEASED
	};

public:
	static const uint64_t	DefaultDoubleTapMaxDelayMs ;

	KeyStatus				m_keyMap[SDL_NUM_SCANCODES+1] ;
	KeyStatus				m_mouseButtonsMap[SDL_BUTTON_X2+1] ;
	double					m_pointerX ;
	double					m_pointerY ;
	double					m_pointerScrollX ;
	double					m_pointerScrollY ;
	
public:
	/** Returns true if the given key is currently down. */
	bool					keyDown(jam::key key) const ;

	/** Returns true if the given key is currently depressed. */
	bool					keyUp(jam::key  key) const ;

	/** Returns true if the given key is currently down and was up in the last frame */
	bool					keyPressed(jam::key  key) const ;

	/** Returns true if the given key is currently up and was down in the last frame */
	bool					keyReleased(jam::key  key) const ;

	/** Return true if pointer (i.e. mouse cursor) is available on device */
	bool					isPointerAvailable() const ;

	/** Return the pointer state.
	 \param pb One of GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_MIDDLE, GLFW_MOUSE_BUTTON_RIGHT
	 \remark Pointer state can be one of UP, DOWN, PRESSED, RELEASED
	 \sa s3ePointerState
	*/
	int						getPointerState(int pb) const ;

	/** Returns pointer's x-coordinate (piv-centered). */
	float					getPointerX() const;

	/** Returns pointer's y-coordinate (piv-centered). */
	float					getPointerY() const;

	/** Hides the mouse pointer and also locks it to the application window */
	void					disableCursor() ;

	/** Hides the mouse pointer */
	void					hideCursor() ;

	/** Shows the mouse pointer */
	void					showCursor() ;

	/** Returns true if the mosue pointer is visible */
	bool					isCursorVisible() const ;

	/** Returns true if multitouch is available. */
	bool					isMultiTouchAvailable() const ;
	
	uint32_t				getMaxNumberOfTouches() const ;
	void					setMaxNumberOfTouches(uint32_t val) ;

	/**
	 Gets the current pointer state for a given touchId.
	 Calling this with touchID 0 is the equivalent of calling getPointerState(S3E_POINTER_BUTTON_SELECT), even if multitouch is not supported.
	 \sa getPointerState
	*/
	int						getTouchState(uint32_t touchId) const ;
	
	/** Returns touch's x-coordinate (piv-centered) for a given touchId. */
	float					getTouchX(uint32_t touchId) const ;
	/** Returns touch's y-coordinate (piv-centered) for a given touchId. */
	float					getTouchY(uint32_t touchId) const ;

	/** Gets the difference from current x-coordinate and the one reported in the last frame, for a giver touchId */
	float					getTouchSpeedX(uint32_t touchId) const ; 
	/** Gets the difference from current y-coordinate and the one reported in the last frame, for a giver touchId */
	float					getTouchSpeedY(uint32_t touchId) const ; 

	
	/** Returns true if the touch state is currently up, for a given touchId */
	bool					isTouchUp(uint32_t touchId) ;

	/** Returns true if the touch state is currently down, for a given touchId */
	bool					isTouchDown(uint32_t touchId) ;

	/** Returns true if the touch state is currently down and it was up in the last frame, for a given touchId */
	bool					isTouchPressed(uint32_t touchId) ;

	/** Returns true if the touch state is currently up and it was down in the last frame, for a given touchId */
	bool					isTouchReleased(uint32_t touchId) ;

	/** Sets the maximum interval time used to consider a second click as doubleclick */
	void					setDoubleTapMaxDelay(uint64_t maxDelay) ;

	/** Gets the maximum interval time used to consider a second click as doubleclick */
	uint64_t				getDoubleTapMaxDelay() const ;

	/** Returns true if the touch state is a doubleclick */
	bool					isTouchDoublePressed(int32_t touchId) ;

	Timer&					getUpdateTimer() ;
	const Timer&			getUpdateTimer() const ;

protected:
	void					update() ;
	void					touchUpdate() ;
	void					keyUpdate() ;
	void					pointerUpdate();

private:
	void					keyboardCallback( SDL_KeyboardEvent& e ) ;
	void					mouseButtonCallback( SDL_MouseButtonEvent& e ) ;
	void					mouseMoveCallback( SDL_MouseMotionEvent& e ) ;
	void					mouseScrollCallback( SDL_MouseWheelEvent& e );

private:
	int32_t					m_maxNumberOfTouches ;

	float					m_touchX[JAM_MAX_TOUCHES] ;
	float					m_touchY[JAM_MAX_TOUCHES] ;
	float					m_touchLastX[JAM_MAX_TOUCHES] ;
	float					m_touchLastY[JAM_MAX_TOUCHES] ;

	bool					m_doubleTap[JAM_MAX_TOUCHES] ;
	uint64_t				m_lastTapTime[JAM_MAX_TOUCHES] ;
	uint64_t				m_doubleTapDelay ;

	InputManager() ;
	virtual ~InputManager() ;

	Timer*					m_pUpdateTimer ;
};

JAM_INLINE InputManager& GetInputMgr() { return InputManager::getSingleton(); }

// inline methods
JAM_INLINE uint32_t			InputManager::getMaxNumberOfTouches() const { return m_maxNumberOfTouches; }
JAM_INLINE void				InputManager::setMaxNumberOfTouches(uint32_t val) { m_maxNumberOfTouches = val; }
JAM_INLINE float			InputManager::getTouchX(uint32_t touchId) const { return m_touchX[touchId]; }
JAM_INLINE float			InputManager::getTouchY(uint32_t touchId) const { return m_touchY[touchId]; }
JAM_INLINE void				InputManager::setDoubleTapMaxDelay(uint64_t maxDelay) { m_doubleTapDelay = maxDelay; }
JAM_INLINE uint64_t			InputManager::getDoubleTapMaxDelay() const { return m_doubleTapDelay; }
JAM_INLINE bool				InputManager::isTouchDoublePressed(int32_t touchId) { return m_doubleTap[touchId] ; }
JAM_INLINE Timer&			InputManager::getUpdateTimer() { return *m_pUpdateTimer; }
JAM_INLINE const Timer&		InputManager::getUpdateTimer() const { return *m_pUpdateTimer; }

}

#endif // __JAM_INPUTMANAGER_H__
