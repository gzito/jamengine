	/**********************************************************************************
* 
* BaseManager.hpp
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

#ifndef __JAM_BASEMANAGER_H__
#define __JAM_BASEMANAGER_H__

#include <jam/jam.h>
#include <jam/Object.h>

#include <unordered_map>
#include <vector>

namespace jam
{

template <typename T>
class NamedObjectManager
{
public:
	using ObjectsMap = std::unordered_map<String,T*> ;

public:
	virtual void			addObject( T* object ) ;
	T*						getObject( const String& name ) const ;
	virtual void			eraseObject( const String& name ) ;
	virtual void			clearAll() ;
	size_t					size() const ;

	ObjectsMap&				getManagerMap() ;
	const ObjectsMap&		getManagerMap() const ;

	virtual	void			dump(const char* msg) ;

protected:
							NamedObjectManager() ;
	virtual					~NamedObjectManager() = default ;

protected:
	ObjectsMap				m_objectsMap ;
};

template<typename T>
NamedObjectManager<T>::NamedObjectManager()
{
}

template<typename T>
void NamedObjectManager<T>::addObject( T* object )
{
	m_objectsMap.emplace( std::make_pair(object->getName(),object) ) ;
}

template<typename T>
T* NamedObjectManager<T>::getObject( const String& name ) const
{
	// std::unordered_map::at
	// Returns a reference to the mapped value of the element with key k in the unordered_map.
	// If k does not match the key of any element in the container, the function throws an out_of_range exception.
	try {
		return m_objectsMap.at(name);
	}
	catch( std::out_of_range& ex ) {
		JAM_ERROR( "Cannot get managed object named \"%s\"", name.c_str() ) ;
	}
}

template<typename T>
void NamedObjectManager<T>::eraseObject( const String& name )
{
	auto it = m_objectsMap.find(name) ;
	if( it != m_objectsMap.end() ) {
		m_objectsMap.erase(it) ;
	}
}

template<typename T>
void NamedObjectManager<T>::clearAll()
{
	m_objectsMap.clear();
}

template<typename T>
size_t NamedObjectManager<T>::size() const
{
	return  m_objectsMap.size();
}

template<typename T>
typename NamedObjectManager<T>::ObjectsMap& NamedObjectManager<T>::getManagerMap()
{
	return m_objectsMap ;
}

template<typename T>
typename const NamedObjectManager<T>::ObjectsMap& NamedObjectManager<T>::getManagerMap() const
{
	return m_objectsMap ;
}

template<typename T>
void NamedObjectManager<T>::dump(const char* msg)
{
	// TODO
}

template <typename T>
class NamedTaggedObjectManager : public NamedObjectManager<T>
{
public:
	using TagsMap = std::unordered_multimap<String,T*> ;
	using RangeTags = std::pair<typename NamedTaggedObjectManager<T>::TagsMap::iterator,typename NamedTaggedObjectManager<T>::TagsMap::iterator> ;

public:
	virtual void			addObject( T* object ) override ;
	T*						findObjectByTag( const String& tag ) const ;
	RangeTags				findObjectsByTag( const String& tag ) ;
	size_t					countObjectsByTag( const String& tag ) ;
	virtual void			eraseObject( const String& name ) override ;
	virtual void			clearAll() override ;

	virtual	void			dump(const char* msg) override ;

protected:
							NamedTaggedObjectManager() ;

private:
	TagsMap					m_tagsMap ;
};

template<typename T>
NamedTaggedObjectManager<T>::NamedTaggedObjectManager()
{
}

template<typename T>
void NamedTaggedObjectManager<T>::addObject( T* object )
{
	m_objectsMap.emplace( std::make_pair(object->getName(),object) ) ;
	if( std::is_base_of<ITaggedObject,T>::value ) {
		m_tagsMap.emplace( std::make_pair(object->getTag(),object) ) ;
	}
}

template<typename T>
T* NamedTaggedObjectManager<T>::findObjectByTag(const String& tag) const
{
	return NULL;
}

template<typename T>
typename NamedTaggedObjectManager<T>::RangeTags NamedTaggedObjectManager<T>::findObjectsByTag(const String& tag)
{
	return m_tagsMap.equal_range( tag ) ;
}

template<typename T>
size_t NamedTaggedObjectManager<T>::countObjectsByTag(const String& tag)
{
	return m_tagsMap.count(tag);
}

template<typename T>
void NamedTaggedObjectManager<T>::eraseObject( const String& name )
{
	auto it = m_objectsMap.find(name) ;
	if( it != m_objectsMap.end() ) {
		ITaggedObject* tObj = (*it).second ;
		if (std::is_base_of<ITaggedObject,T>::value && !tObj->getTag().empty() ) {
			auto range = m_tagsMap.equal_range( tObj->getTag() ) ;
			for( auto rIt = range.first; rIt!= range.second; rIt++ ) { 
				if ( (*rIt).second == tObj ) {
					m_tagsMap.erase(rIt) ;
					break ;
				}
			}
		}

		m_objectsMap.erase(it) ;
	}
}

template<typename T>
void NamedTaggedObjectManager<T>::clearAll()
{
	m_objectsMap.clear();
	m_tagsMap.clear() ;
}

template<typename T>
void NamedTaggedObjectManager<T>::dump(const char* msg)
{
	// TODO
}

}

#endif	// __JAM_BASEMANAGER_H__
