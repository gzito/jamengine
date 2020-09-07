/**********************************************************************************
* 
* Object.h
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

#ifndef __JAM_OBJECT_H__
#define __JAM_OBJECT_H__

#include <jam/jam.h>
#include <jam/String.h>

#include <gc_cpp.h>
#include <sstream>

namespace jam
{

/**
	This class represents TODO
*/
class JAM_API INamedObject
{
public:
							INamedObject() = default ;
	virtual					~INamedObject() = default ;
	virtual String			getName() const = 0 ;
	virtual void			setName( const String& name ) = 0 ;
};

class JAM_API ITaggedObject
{
public:
							ITaggedObject() = default ;
	virtual					~ITaggedObject() = default ;
	virtual String			getTag() const = 0 ;
	virtual void			setTag( const String& tag ) = 0 ;
};

class JAM_API NamedObject : public Collectible, public INamedObject
{
public:
							NamedObject() ;
	virtual					~NamedObject() = default ;

	// Inherited via INamedObject
	virtual String			getName() const override;
	virtual void			setName(const String& name) override;

private:
	String					m_name ;
};

class JAM_API TaggedObject : public Collectible, public ITaggedObject
{
public:
							TaggedObject() ;
	virtual					~TaggedObject() = default ;

private:
	String					m_tag ;

	// Inherited via ITaggedObject
	virtual String			getTag() const override;
	virtual void			setTag(const String& tag) override;
};

class JAM_API NamedTaggedObject : public Collectible, public INamedObject, public ITaggedObject
{
public:
							NamedTaggedObject() ;
	virtual					~NamedTaggedObject() = default ;

	virtual String			getName() const override;
	virtual void			setName(const String& name) override;
	virtual String			getTag() const override;
	virtual void			setTag(const String& tag) override;

private:
	String					m_name ;
	String					m_tag ;
};

template <typename T>
String generateID( T* p ) {
	JAM_ASSERT( p!=0 ) ;
	std::stringstream s ;
	s << typeid(*p).name() << "@" << p ;
	return s.str() ;
}

}

#endif // __JAM_OBJECT_H__
