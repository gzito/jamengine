/**********************************************************************************
* 
* Transform.cpp
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

#include <jam/Transform.h>
#include <jam/GameObject.h>
#include <jam/core/math.h>
#include <jam/core/geom.h>

namespace jam
{

	//*******************
	//
	// Class Transform
	//
	//*******************

	Transform::Transform( GameObject* gameObject ) :
		Component( *gameObject ),
		m_localIsDirty(false),
		m_worldIsDirty(false),
		m_localPosition(0.0f),
		m_localScale(1.0f),
		m_localRotationQ(),
		m_worldPosition(0.0f),
		m_worldScale(1.0f),
		m_worldRotationQ(),
		m_parent(0),
		m_children()
	{
	}

	Transform::~Transform()
	{
	}

	Vector3 Transform::getLocalPosition() const
	{
		return m_localPosition ;
	}

	Quaternion Transform::getLocalRotation() const
	{
		return m_localRotationQ ;
	}

	Vector3 Transform::getLocalScale() const
	{
		return m_localScale ;
	}

	Matrix4 Transform::getLocalTformMatrix() const
	{
		if( m_localIsDirty ) {
			Matrix4 rot = glm::mat4_cast(m_localRotationQ) ;
			Matrix4 scl = createScaleMatrix3D( m_localScale ) ; 
			Matrix4 tra = createTranslationMatrix3D( m_localPosition ) ;
			m_localTformM = tra * rot * scl ;
			m_localIsDirty = false ;
		}

		return m_localTformM ;
	}
	
	Vector3 Transform::getLocalEulerAngles() const
	{
		return ToDegree( glm::eulerAngles( getLocalRotation() ) );
	}

	Vector3 Transform::getWorldPosition() const
	{
		m_worldScale = m_parent ? m_parent->getWorldPosition() * m_localPosition : m_localPosition;
		return m_worldScale;
	}

	Quaternion Transform::getWorldRotation() const
	{
		m_worldRotationQ = m_parent ? m_parent->getWorldRotation() * m_localRotationQ : m_localRotationQ ;
		return m_worldRotationQ ;
	}

	Vector3 Transform::getWorldScale() const
	{
		m_worldScale = m_parent ? m_parent->getWorldScale() * m_localScale : m_localScale;
		return m_worldScale;
	}

	Matrix4 Transform::getWorldTformMatrix() const
	{
		if( m_worldIsDirty ) {
			m_worldTformM = m_parent ? getWorldTformMatrix() * getLocalTformMatrix() : getLocalTformMatrix() ;
			m_worldIsDirty = false ;
		}
		return m_worldTformM ;
	}
	
	Vector3 Transform::getWorldEulerAngles() const
	{
		return ToDegree( glm::eulerAngles( getWorldRotation() ) ) ;
	}

	void Transform::setLocalPosition(const Vector3 & v)
	{
		m_localPosition = v ;
		invalidateLocal() ;
	}

	void Transform::setLocalRotation(const Quaternion & q)
	{
		m_localRotationQ = glm::normalize(q) ;
		invalidateLocal() ;
	}

	void Transform::setLocalScale(const Vector3 & v)
	{
		m_localScale = v ;
		invalidateLocal() ;
	}

	void Transform::setLocalTformMatrix(const Matrix4 & m)
	{
		m_localPosition = Vector3(m[3]) ;
		m_localScale = Vector3( m[0].length(), m[1].length(), m[2].length() ) ;
		m_localRotationQ = Quaternion(m) ;
		invalidateLocal();
	}

	void Transform::setWorldPosition(const Vector3 & v)
	{
		setLocalPosition( m_parent ? Vector3( glm::inverse(m_parent->getWorldTformMatrix()) * glm::vec4(v,1.0)) : v );
	}

	void Transform::setWorldRotation(const Quaternion & q)
	{
		setLocalRotation( m_parent ? glm::inverse(m_parent->getWorldRotation()) * q : q );
	}

	void Transform::setWorldScale(const Vector3 & v)
	{
		setLocalScale( m_parent ? v / m_parent->getWorldScale() : v );
	}

	void Transform::setWorldTformMatrix(const Matrix4 & m)
	{
		setLocalTformMatrix( m_parent ? glm::inverse(m_parent->getWorldTformMatrix()) * m : m ) ;
	}

	Vector3 Transform::getUp() const
	{
		return getWorldRotation() * Vector3(0,1,0);
	}

	Vector3 Transform::getForward() const
	{
		return getWorldRotation() * Vector3(0,0,1);
	}

	Vector3 Transform::getRight() const
	{
		return getWorldRotation() * Vector3(1,0,0);
	}

	Transform* Transform::getChild(size_t index) const
	{
		return const_cast<Transform*>( m_children[index].get() ) ;
	}

	/**
	* Set the parent of the transform.
	*
	* \param parent - The parent Transform to use.
	* \param worldPositionStays - If true, the parent-relative position, scale and rotation are modified such that
	*                             the object keeps the same world space position, rotation and scale as before.
	*                             Default value is true
	*/
	void Transform::setParent(Transform * parent, bool worldPositionStays)
	{
		m_parent = parent ;
		if( worldPositionStays == false ) {
			setLocalTformMatrix( Matrix4(1.0f) ) ;
		}
	}

	/**
	* Sets the world space position and rotation of the Transform component.
	*/
	void Transform::setPositionAndOrientation(const Vector3 & position,const Quaternion rotation)
	{
		setWorldPosition(position) ;
		setWorldRotation(rotation) ;
	}

	/**
	* Moves the transform in the direction and distance of translation.
	*
	* If relativeTo is left out or set to Space::Local the movement is applied relative to the transform's local axes.
	* If relativeTo is Space.World the movement is applied relative to the world coordinate system. 
	*/
	void Transform::translate(const Vector3 & translation,const Space & relativeTo)
	{
		if( relativeTo == Space::Local ) {
			setLocalPosition( getLocalPosition() + translation) ;
		}
		else {
			setWorldPosition( getWorldPosition() + translation) ;
		}
	}

	/**
	* Applies a rotation of eulerAngles.z degrees around the z axis, eulerAngles.x degrees around the x axis,
	* and eulerAngles.y degrees around the y axis (in that order).
	*
	* If relativeTo is not specified or set to Space::Local the rotation is applied around the transform's local axes.
	* If relativeTo is set to Space.World the rotation is applied around the world x, y, z axes.
	*/
	void Transform::rotate(const Vector3 & eulerAngles, const Space& relativeTo)
	{
		Quaternion rhs(ToRadian(eulerAngles)) ;
		if( relativeTo == Space::Local ) {
			setLocalRotation( getLocalRotation() * rhs) ;
		}
		else {
			setWorldRotation( getWorldRotation() * rhs) ;
		}
	}

	void Transform::setAsFirstSibling()
	{
		if( m_parent ) {
			setSiblingIndex(0) ;
		}
	}

	void Transform::setAsLastSibling()
	{
		if( m_parent ) {
			setSiblingIndex( m_parent->getChildCount() ) ;
		}
	} 

	void Transform::setSiblingIndex(size_t index)
	{
		if( m_parent ) {
			size_t currentIdx = getSiblingIndex() ;
			if( currentIdx != index ) {
				m_parent->m_children.erase( m_children.begin()+currentIdx ) ;
				if( index > 0 ) { index--; }
				m_parent->m_children.insert( m_children.begin()+index, Ref<Transform>(this,true) ) ;
			}
		}
	}

	size_t Transform::getSiblingIndex() const
	{
		size_t sIdx = 0 ;

		if( m_parent ) {
			for( size_t i = 0; i < m_parent->getChildCount(); i++ ) {
				if( m_parent->getChild(i) == this ) {
					sIdx = i ;
					break ;
				}
			}
		}

		return sIdx ;
	}

	void Transform::invalidateLocal()
	{
		m_localIsDirty = true ;
		invalidateWorld() ;
	}

	void Transform::invalidateWorld()
	{
		if( !m_worldIsDirty ) {
			m_worldIsDirty = true ;
			for( Transform* t : m_children ) {
				t->invalidateWorld() ;
			}
		}
	}

}
