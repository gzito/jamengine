/**********************************************************************************
* 
* geom.cpp
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

#include "jam/core/geom.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/euler_angles.hpp>


namespace jam
{

Vector2 fromPolar(float angle, float magnitude)
{
	float angleRad = ToRadian(angle) ;
    return magnitude * Vector2((float)cosf(angleRad), sinf(angleRad));
}

Vector2 transform(const Matrix3& m,const Vector2& v)
{
	Vector3 tform = m * Vector3(v,1.0f) ;
	return Vector2( tform ) ;
}

Vector2 transformNormal(const Matrix3& m,const Vector2 & v)
{
	return jam::transform( glm::transpose( glm::inverse(m) ), v ) ;
}

Vector2 getScale(const Matrix3 & m)
{
	return Vector2( glm::length(Vector2(m[0][0],m[0][1])), glm::length(Vector2(m[1][0],m[1][1])) ) ;
}

Matrix3 getRotate(const Matrix3& m)
{
	Matrix3 rotateMat(1.0f) ;
	return glm::rotate(rotateMat,-atan2f(m[1][0],m[1][1])) ;
}

Vector2 getTranslate(const Matrix3& m)
{
	return Vector2(m[2]) ;
}

Vector3 getTranslate(const Matrix4& m)
{
	return Vector3(m[3]) ;
}

void setTranslate(Matrix3& m, const Vector2& t)
{
	m[2] = Vector3(t,1.0f); 
}

Matrix3 createScaleMatrix2D(const Vector2& v)
{
	Matrix3 m(1.0f) ;
	return glm::scale( m, v ) ;
}

Matrix3 createRotationMatrix2D(float angle)
{
	Matrix3 m(1.0f) ;
	return glm::rotate( m, ToRadian(angle) ) ;
}

Matrix3 createTranslationMatrix2D(const Vector2& v)
{
	Matrix3 m(1.0f) ;
	return glm::translate( m, v ) ;
}

void setTranslate(Matrix4& m,const Vector3& t)
{
	m[3] = glm::vec4(t,1.0f); 
}

Vector3 transform(const Matrix4& m,const Vector3& v)
{
	Vector4 tform = m * Vector4(v,1.0f) ;
	return Vector4( tform ) ;
}

Matrix4 createScaleMatrix3D(const Vector3 & v)
{
	Matrix4 m(1.0f) ;
	return glm::scale( m, v ) ;
}

Matrix4 createRotationMatrix3D( float yaw, float pitch, float roll )
{
	Matrix4 m(1.0f) ;
	return glm::yawPitchRoll( ToRadian(yaw), ToRadian(pitch), ToRadian(roll) ) ;
}

Matrix4 createTranslationMatrix3D(const Vector3 & v)
{
	Matrix4 m(1.0f) ;
	return glm::translate( m, v ) ;
}

}

