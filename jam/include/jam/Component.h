/**********************************************************************************
* 
* Component.h
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
 
#ifndef __JAM_COMPONENT_H__
#define __JAM_COMPONENT_H__

#include <GL/glew.h>

#include <jam/jam.h>
#include <jam/String.h>
#include <jam/RefCountedObject.h>

namespace jam
{
class GameObject ;
class Transform ;

/*!
	\class Component
*/
class JAM_API Component : public RefCountedObject
{
public:
							Component( GameObject& gameObject ) ;
	virtual					~Component() ;

	GameObject&				getGameObject() ;
	Transform&				getTransform() ;

	String					getTag() const ;
	void					setTag( const String& tag ) ;

//	virtual void			update( jam::time deltaTime ) = 0 ;

private:
	Ref<GameObject>			m_gameObject ;
	String					m_tag ;
};

JAM_INLINE GameObject&		Component::getGameObject() { return *(m_gameObject.get()); }

JAM_INLINE String			Component::getTag() const { return m_tag; }
JAM_INLINE void				Component::setTag( const String& tag ) { m_tag = tag ; }
}

#endif // __JAM_COMPONENT_H__

