/**********************************************************************************
* 
* GameObject.cpp
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
#include <jam/GameObject.h>
#include <jam/Transform.h>

namespace jam
{

	GameObject::GameObject() :
		m_transform(0),
		m_components(),
		m_active(true)
	{
		// each GameObject has a Transform component
		m_transform = new (GC) Transform(this) ;
		addComponent(*m_transform) ;
	}

	GameObject::~GameObject()
	{
	}

	Transform& GameObject::getTransform()
	{
		return *m_transform ;
	}

	void GameObject::addComponent( Component& component )
	{
		m_components.push_back( &component ) ;
	}


	bool GameObject::getActiveSelf() const
	{
		return m_active;
	}

	bool GameObject::activeInHierarchy() const
	{
		const GameObject* gameObject = this ;
		Transform* parentTransform = m_transform->getParent() ;
		while( parentTransform != 0 ) {
			gameObject = &(parentTransform->getGameObject()) ;
			if( gameObject->getActiveSelf() ) {
				parentTransform = parentTransform->getParent() ;
			}
			else {
				return false ;
			}
		}

		return gameObject->getActiveSelf() ;
	}

	void GameObject::setActive(bool value)
	{
		m_active = value ;
	}

}
