/**********************************************************************************
* 
* Transform.h
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
 
#ifndef __JAM_TRANSFORM_H__
#define __JAM_TRANSFORM_H__

#include <GL/glew.h>

#include <jam/jam.h>
#include <jam/core/geom.h>
#include <jam/Component.h>
#include <jam/RefCountedObject.h>
#include <vector>

namespace jam
{

class GameObject ;

/*!
	\class Transform
*/
class JAM_API Transform : public Component
{
public:
	enum class Space
	{
		World,
		Local
	};

							Transform( GameObject* gameObject ) ;
	virtual					~Transform() ;

	Vector3					getLocalPosition() const ;
	Quaternion				getLocalRotation() const ;
	Vector3					getLocalScale() const ;
	Matrix4					getLocalTformMatrix() const ;
	Vector3					getLocalEulerAngles() const ;

	Vector3					getWorldPosition() const ;
	Quaternion				getWorldRotation() const ;
	Vector3					getWorldScale() const ;
	Matrix4					getWorldTformMatrix() const ;
	Vector3					getWorldEulerAngles() const ;

	void					setLocalPosition(const Vector3& v) ;
	void					setLocalRotation(const Quaternion& q) ;
	void					setLocalScale(const Vector3& v) ;
	void					setLocalTformMatrix(const Matrix4& m) ;

	void					setWorldPosition(const Vector3& v) ;
	void					setWorldRotation(const Quaternion& q) ;
	void					setWorldScale(const Vector3& v) ;
	void					setWorldTformMatrix(const Matrix4& m) ;

	Vector3					getUp() const ;
	Vector3					getForward() const ;
	Vector3					getRight() const ;

	size_t					getChildCount() const ;
	Transform*				getChild( size_t index ) const ;
	Transform*				getParent() const ;

	void					setParent( Transform* parent, bool worldPositionStays = true ) ;

	void					setPositionAndOrientation( const Vector3& position, const Quaternion rotation) ;
	void					translate( const Vector3& translation, const Space& relativeTo = Space::Local ) ;
	void					rotate( const Vector3& eulerAngles, const Space& relativeTo = Space::Local ) ;

	void					setAsFirstSibling() ;
	void					setAsLastSibling() ;
	void					setSiblingIndex(size_t index) ;
	size_t					getSiblingIndex() const ;

protected:
	void					invalidateLocal() ;
	void					invalidateWorld() ;

private:
	mutable bool			m_localIsDirty ;
	mutable bool			m_worldIsDirty ;

	Vector3					m_localPosition ;
	Vector3					m_localScale ;
	Quaternion				m_localRotationQ ;
	mutable Matrix4			m_localTformM ;

	mutable Vector3			m_worldPosition ;
	mutable Vector3			m_worldScale ;
	mutable Quaternion		m_worldRotationQ ;
	mutable Matrix4			m_worldTformM ;

	Transform*				m_parent ;
	std::vector<Ref<Transform>>	m_children ;
};

JAM_INLINE size_t			Transform::getChildCount() const { return m_children.size(); }
JAM_INLINE Transform*		Transform::getParent() const { return m_parent; }
}

#endif // __JAM_TRANSFORM_H__
