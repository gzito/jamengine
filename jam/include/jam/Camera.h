/**********************************************************************************
* 
* Camera.h
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

#ifndef __JAM_CAMERA_H__
#define __JAM_CAMERA_H__

#include <jam/jam.h>
#include <jam/GameObject.h>
#include <jam/core/geom.h>

namespace jam
{

class JAM_API Camera : public GameObject
{
public:
	static const Vector3	m_defaultPosition ;

	enum class ProjectionType {
		Perspective,
		Orthographic
	};

							Camera() ;

	void					setPerspectiveProjection( float fovy, float aspect, float znear = -1, float zfar = -1 ) ;
	void					setOrthographicProjection( float left, float right, float bottom, float top, float znear = -1, float zfar = -1 ) ;

	void					setPosition( const Vector3& pos ) ;
	void					translate( float tx, float ty, float tz ) ;
	void					translate( const Vector3& offset ) ;

	void					lookAt( const Vector3& eye, const Vector3& center, const Vector3& up ) ;
	void					yawPitchRoll( float yaw, float pitch, float roll ) ;
	void					yawPitchRoll( const Vector3& eulerAngles ) ;

	void					setFovY( float fovY ) ;
	void					setAspectRatio( float aspect ) ;

	Vector3					getForward() const ;
	Vector3					getRight() const ;
	Vector3					getUp() const ;
	Vector3					getPosition() const ;
	Vector3					getEulerAngles() const ;

	/// get rotation about y
	float					getYaw() const ;
	/// get rotation about x
	float					getPitch() const ;
	/// get rotation about z
	float					getRoll() const ;

	float					getFovY() const { return m_fovy; }
	float					getAspectRatio() const { return m_aspect; }
	float					getZNear() const { return m_znear; }
	float					getZFar() const { return m_zfar; }
	ProjectionType			getProjectionType() const { return m_projType; }

	void					update() ;
	void					setActive() ;

	Matrix4					getViewMatrix() const ;
	Matrix4					getProjectionMatrix() const ;

private:
	void					updateProjection() ;
	void					updateView();

private:
	float					m_isViewDirty ;
	float					m_isProjDirty ;

	//
	// view params
	//

	// position
	Vector3					m_position ;

	// yaw, pitch and roll in radian ;
	Vector3					m_eulerAngles ;


	//
	// projection params
	//
	ProjectionType			m_projType ;

	// perspective projection params
	float					m_fovy ;		// fov y in radian
	float					m_aspect ;

	// ortho projection params
	float					m_left ;
	float					m_right ;
	float					m_bottom ;
	float					m_top ;

	// common projection params
	float					m_znear ;
	float					m_zfar ;

	// view and proj matrix
	Matrix4					m_viewMatrix ;
	Matrix4					m_projMatrix ;
};

JAM_INLINE	Vector3		Camera::getPosition() const { return m_position; }
JAM_INLINE	Matrix4		Camera::getViewMatrix() const { return m_viewMatrix ; }
JAM_INLINE	Matrix4		Camera::getProjectionMatrix() const { return m_projMatrix ; }

}

#endif // __JAM_CAMERA_H__
