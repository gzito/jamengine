/**********************************************************************************
* 
* InputManager.cpp
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

#include "jam/InputManager.h"
#include "jam/Application.h"
//#include "jam/Utilities.h"
#include "jam/Draw3dManager.h"
#include "jam/Timer.h"
#include "jam/SysTimer.h"


namespace jam
{

// static initialization
const uint64_t InputManager::DefaultDoubleTapMaxDelayMs = 300 ;


void InputManager::keyboardCallback( SDL_KeyboardEvent& e )
{
	SDL_Scancode key = e.keysym.scancode;
	Uint8 action = e.state;
	Uint8 repeat = e.repeat ;
	if( key != SDL_SCANCODE_UNKNOWN ) {

		// manage ESC key to quit the app
		if( action == SDL_PRESSED && !repeat && key == SDL_SCANCODE_ESCAPE ) {
			GetAppMgr().signalExit() ;
		}

		if( action == SDL_PRESSED && !repeat) {
			m_keyMap[key] = PRESSED ;
		}
		else if( action == SDL_RELEASED && !repeat) {
			m_keyMap[key] = RELEASED ;
		}
		else if( repeat ) {
			m_keyMap[key] = DOWN ;
		}
	}
}

void InputManager::mouseButtonCallback( SDL_MouseButtonEvent& e )
{
	Uint8 action = e.state;
	Uint8 button = e.button ;
	if( action == SDL_PRESSED ) {
		m_mouseButtonsMap[button] = DOWN ;
	}
	else if( action == SDL_RELEASED ) {
		m_mouseButtonsMap[button] = UP ;
	}
}

void InputManager::mouseMoveCallback( SDL_MouseMotionEvent& e )
{
	m_pointerX = e.x ;
	m_pointerY = e.y ;
}

void InputManager::mouseScrollCallback( SDL_MouseWheelEvent& e )
{
	m_pointerScrollX = e.x ;
	m_pointerScrollY = e.y ;
}

InputManager::InputManager():
	m_maxNumberOfTouches(JAM_MAX_TOUCHES), m_doubleTapDelay(DefaultDoubleTapMaxDelayMs),
	m_pointerY(0.f), m_pointerX(0.f), m_pointerScrollX(0.f), m_pointerScrollY(0.f)
{
	m_pUpdateTimer = Timer::create() ;
	if( !isMultiTouchAvailable() ) {
		m_maxNumberOfTouches = 1 ;
	}

	for( uint16_t touchId=0; touchId<m_maxNumberOfTouches; touchId++ ) {
		m_touchX[touchId] = -1.0f ;
		m_touchY[touchId] = -1.0f ;
		m_lastTapTime[touchId] = GetSysTimer().getTime() ;
		m_doubleTap[touchId] = false ;
	}

	memset( m_keyMap, 0, sizeof(m_keyMap) ) ;
	memset( m_mouseButtonsMap, 0, sizeof(m_mouseButtonsMap) ) ;
}

InputManager::~InputManager()
{
}

bool InputManager::keyUp(jam::key key) const
{
	return m_keyMap[key] == UP || m_keyMap[key] == RELEASED ;
}

bool InputManager::keyDown(jam::key key) const
{
	return m_keyMap[key] == DOWN || m_keyMap[key] == PRESSED ;
}

bool InputManager::keyPressed(jam::key key) const
{
	return m_keyMap[key] == PRESSED ;
}

bool InputManager::keyReleased(jam::key key) const
{
	return m_keyMap[key] == RELEASED ;	
}

bool InputManager::isPointerAvailable() const
{
	return true ;
}

int InputManager::getPointerState(int pb) const
{
	return m_mouseButtonsMap[pb] ;
}

void InputManager::pointerUpdate()
{
}

float InputManager::getPointerX() const
{
	return CX2PIV_SCALED((float)m_pointerX);
}

float InputManager::getPointerY() const
{
	return CY2PIV_SCALED((float)m_pointerY);
}

void InputManager::disableCursor()
{
// TODO in SDL2
//	glfwSetInputMode(Application::getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void InputManager::hideCursor()
{
	SDL_ShowCursor(SDL_DISABLE);
}

void InputManager::showCursor()
{
	SDL_ShowCursor(SDL_ENABLE);
}

bool InputManager::isCursorVisible() const
{
	int im = SDL_ShowCursor(SDL_QUERY) ;
	return (im == SDL_ENABLE) ;
}

bool InputManager::isMultiTouchAvailable() const
{
	return false ;
}


int InputManager::getTouchState(uint32_t touchId) const
{
	// ignore touchId on PC
	return getPointerState(SDL_BUTTON_LEFT) ;
}

void InputManager::touchUpdate()
{
	for( uint16_t touchId=0; touchId<m_maxNumberOfTouches; touchId++ ) {
		if( m_touchX[touchId] == -1 ) {
			// this is the very first frame
			m_touchLastX[touchId] = getPointerX() ;
			m_touchLastY[touchId] = getPointerY() ;
		}
		else {
			m_touchLastX[touchId] = m_touchX[touchId] ;
			m_touchLastY[touchId] = m_touchY[touchId] ;
		}

		m_touchX[touchId] = getPointerX() ;
		m_touchY[touchId] = getPointerY() ;

		if( getTouchState(touchId) == PRESSED )
		{
			uint64_t n = GetSysTimer().getTime() - m_lastTapTime[touchId] ;
			m_doubleTap[touchId] = n < m_doubleTapDelay ;
			m_lastTapTime[touchId] = GetSysTimer().getTime() ;
		}
	}
}

float InputManager::getTouchSpeedX( uint32_t touchId ) const
{
	return m_touchX[touchId] - m_touchLastX[touchId] ;
}

float InputManager::getTouchSpeedY( uint32_t touchId ) const
{
	return m_touchY[touchId] - m_touchLastY[touchId] ;
}

bool InputManager::isTouchUp(uint32_t touchId)
{
	return (getTouchState(touchId) == UP) ;
}

bool InputManager::isTouchDown(uint32_t touchId)
{
	return (getTouchState(touchId) == DOWN) ;
}

bool InputManager::isTouchPressed(uint32_t touchId)
{
	return (getTouchState(touchId)  == PRESSED) ;
}

bool InputManager::isTouchReleased(uint32_t touchId)
{
	return (getTouchState(touchId) == RELEASED) ;
}

void InputManager::keyUpdate()
{
}

void InputManager::update()
{
	touchUpdate() ;
}

}
