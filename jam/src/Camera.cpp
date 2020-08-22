/**********************************************************************************
* 
* Camera.cpp
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

#include <jam/Camera.h>
#include <jam/core/bmkextras.hpp>
#include <jam/core/math.h>
#include <jam/core/geom.h>
#include <jam/DeviceManager.h>

#include <cmath>

namespace jam
{
	const Vector3 Camera::m_defaultPosition = Vector3(0.0f,0.0f,10.0f) ;
		
	Camera::Camera() : 
		m_isViewDirty(true),
		m_isProjDirty(true),
		m_projType(ProjectionType::Perspective),
		m_fovy(),
		m_aspect(),
		m_left(),
		m_right(), 
		m_bottom(),
		m_top(),
		m_znear(),
		m_zfar(),
		m_position(0.0f),
		m_eulerAngles(0.0f),
		m_viewMatrix(1.0f),
		m_projMatrix(1.0f)
	{
		float aspect = GetDeviceMgr().getNativeDisplayWidth() / (float)GetDeviceMgr().getNativeDisplayHeight() ;
		setPerspectiveProjection(60.0f, aspect, 0.1f, 1000) ;

 		lookAt( m_defaultPosition, Vector3(0.0f,0.0f,0.0f), Vector3(0.0f,1.0f,0.0f) ) ;
// it is equivalent to:
// 		setPosition( m_defaultPosition ) ;
// 		setRotation( Vector3(0.0f, 0.0f, 0.0f ) ) ;
	}


	void Camera::setPerspectiveProjection( float fovy, float aspect, float znear, float zfar )
	{
		m_projType = ProjectionType::Perspective ;
		m_znear = znear ;
		m_zfar = zfar ;
		m_fovy = ToRadian(fovy) ;
		m_aspect = aspect ;
		m_isProjDirty = true ;
	}

	void Camera::setOrthographicProjection( float left, float right, float bottom, float top, float znear, float zfar )
	{
		m_projType = ProjectionType::Orthographic ;
		m_znear = znear ;
		m_zfar = zfar ;
		m_left = left ;
		m_right = right ;
		m_bottom = bottom ;
		m_top = top ;
		m_isProjDirty = true ;
	}

	void Camera::setPosition( const Vector3& pos )
	{
		m_position = pos ;
		m_isViewDirty = true ;
	}

	void Camera::translate( float tx, float ty, float tz )
	{
		translate( Vector3(tx,ty,tz) ) ;
	}

	void Camera::translate(const Vector3 & offset)
	{
		m_position += offset ;
		m_isViewDirty = true ;
	}

	/// Build a look at view matrix based on the default handedness.
	///
	/// @param eye Position of the camera
	/// @param center Position where the camera is looking at
	/// @param up Normalized up vector, how the camera is oriented.
	void Camera::lookAt( const Vector3& eye, const Vector3& center, const Vector3& up )
	{
		Matrix4 m = glm::lookAt( eye, center, up ) ;
		m_position = eye ;
		glm::quat orientation = quat_cast(m) ;
		m_eulerAngles = glm::eulerAngles( orientation ) ;
		m_isViewDirty = true ;
	}

	/// yaw=y, pitch=x, roll=z
	void Camera::yawPitchRoll( float yaw, float pitch, float roll )
	{
		yawPitchRoll( Vector3(pitch,yaw,roll) );
	}

	void Camera::yawPitchRoll( const Vector3& eulerAngles )
	{
		m_eulerAngles = ToRadian(eulerAngles) ;
		m_isViewDirty = true ;
	}

	Vector3 Camera::getForward() const
	{
		if( m_isViewDirty ) {
			const_cast<Camera*>(this)->updateView() ;
		}

		Vector3 forward( m_viewMatrix[0][2], m_viewMatrix[1][2], m_viewMatrix[2][2] ) ;
		return glm::normalize(forward) ;
	}

	Vector3 Camera::getRight() const
	{
		if( m_isViewDirty ) {
			const_cast<Camera*>(this)->updateView() ;
		}

		Vector3 right( m_viewMatrix[0][0], m_viewMatrix[1][0], m_viewMatrix[2][0] ) ;
		return glm::normalize(right) ;
	}

	Vector3 Camera::getUp() const
	{
		Matrix4 m = mat4_cast(glm::quat(m_eulerAngles)) ;
		return glm::normalize(m[0]) ;
	}

	Vector3 Camera::getEulerAngles() const
	{
		return ToDegree(m_eulerAngles) ;
	}

	float Camera::getYaw() const
	{
		return ToDegree(m_eulerAngles.y) ;
	}

	float Camera::getPitch() const
	{
		return ToDegree(m_eulerAngles.x) ;
	}

	float Camera::getRoll() const
	{
		return ToDegree(m_eulerAngles.z);
	}

	void Camera::setFovY( float fovY )
	{
		m_fovy = ToRadian(Limit(fovY,1,170)) ;
		m_isProjDirty = true ;
	}

	void Camera::setAspectRatio(float aspect)
	{
		m_aspect = aspect ;
		m_isProjDirty = true ;
	}


	void Camera::update()
	{
		if( m_isProjDirty ) {
			updateProjection() ;
		}

		if( m_isViewDirty ) {
			updateView() ;
		}
	}


	void Camera::setActive()
	{
		m_isViewDirty = m_isProjDirty = true ;
		update() ;
	}


	void Camera::updateProjection()
	{
		if( m_projType == ProjectionType::Perspective ) {
			m_projMatrix = glm::perspective(m_fovy,m_aspect,m_znear,m_zfar) ;
		}
		else {
			m_projMatrix = glm::ortho(m_left,m_right,m_bottom,m_top,m_znear,m_zfar) ;
		}
		m_isProjDirty = false ;
	}

	void Camera::updateView()
	{
		Matrix4 matrixRotate(1.0f) ;
		Matrix4 matrixTranslate(1.0f) ;

		// rotation matrix from euler angles
		matrixRotate = glm::yawPitchRoll( m_eulerAngles.y, m_eulerAngles.x, m_eulerAngles.z ) ;

		// rotation matrix from quaternion
		//matrixRotate = mat4_cast(glm::quat(m_eulerAngles)) ;
		
		matrixTranslate = glm::translate(matrixTranslate,-m_position) ;

		m_viewMatrix = matrixRotate * matrixTranslate;
		m_isViewDirty = false ;
	}

}
