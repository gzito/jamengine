/**********************************************************************************
* 
* geom.h
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

#ifndef __JAM_GEOM_H__
#define __JAM_GEOM_H__

#include <jam/jam.h>
#include <jam/core/math.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>


namespace jam
{

using Vector2 = glm::vec2 ;
using Vector3 = glm::vec3 ;
using Vector4 = glm::vec4 ;
using Matrix2 = glm::mat2 ;
using Matrix3 = glm::mat3 ;
using Matrix4 = glm::mat4 ;
using Quaternion = glm::quat ;

JAM_API Vector2				transform(const Matrix3& m, const Vector2& v) ;
JAM_API Vector2				transformNormal(const Matrix3& m, const Vector2& v) ;

JAM_API Vector2				getScale(const Matrix3& m) ;
JAM_API Matrix3				getRotate(const Matrix3& m) ;
JAM_API Vector2				getTranslate(const Matrix3& m) ;

JAM_API void				setTranslate(Matrix3& m, const Vector2& t) ;

JAM_API Matrix3				createScaleMatrix2D(const Vector2& v) ;
JAM_API Matrix3				createRotationMatrix2D(float angle) ;
JAM_API Matrix3				createTranslationMatrix2D(const Vector2& v) ;

JAM_API Vector3				transform(const Matrix4& m, const Vector3& v) ;
JAM_API void				setTranslate(Matrix4& m, const Vector3& t) ;
JAM_API Vector3				getTranslate(const Matrix4& m) ;
JAM_API Matrix4				createScaleMatrix3D(const Vector3& v) ;
JAM_API Matrix4				createRotationMatrix3D( float yaw, float pitch, float roll ) ;
JAM_API Matrix4				createTranslationMatrix3D(const Vector3& v) ;

}
#endif	// __JAM_GEOM_H__
