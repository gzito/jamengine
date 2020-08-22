/**********************************************************************************
* 
* Bank.h
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
#ifndef __JAM_BANK_H__
#define __JAM_BANK_H__

#include <jam/jam.h>
#include <jam/BankItemHolder.h>
#include <jam/String.h>

#include <cassert>
#include <vector>
#include <map>

namespace jam
{
template <class T>
class Bank : public BankItemHolder
{
public:
	/// Adds a new item to bank, assigning id and name to the item, returning its id
	int						add(T* item, bool incRef=true) ;
	/// Adds a new item to bank, assigning id and name to the item
	void					addById( T* item, int id, bool incRef=true) ;
	/// Adds a new item to bank, assigning id and name to the item
	void					addByName( T* item, const String& name, bool incRef=true) ;
	/// Adds a new item to bank, assigning id and name to the item
	void					addByIdAndName( T* item, int id, const String& name, bool incRef=true) ;

	T&						getById( int id ) { return *((T*)getPtrById(id)); }
	T&						getByName( const String& name ) { return *((T*)getPtrByName(name)); }
	T*						getPtrById( int id ) ;
	T*						getPtrByName( const String& name ) ;
	T*						findPtrById( int id ) ;
	T*						findPtrByName( const String& name ) ;

	std::vector<T*>			getByTag( const TagType& tag ) ;
	size_t					queryByTag( const TagType& tag, std::vector<T*>& result ) ;
	const std::vector<T*>&	queryByTagFast( const TagType& tag ) ;

	void					remove( T* item ) { removeBankItem(item) ; }
	void					removeById( int id ) { removeBankItemById(id) ; }
	void					removeByName( const String& name ) { removeBankItemByName(name) ; }
	void					removeByTag( const TagType& tag );

	size_t					getCountByTag( const TagType& tag ) ;
};

//
// class implementation
//

template <class T>
int Bank<T>::add( T* item, bool incRef/*=true*/)
{
	int id = getLastFreeId() ;
	addBankItemById(item,id) ;
	String name = item->getName() ;
	if( name.empty() ) {
		name = "item_" + std::to_string(id) ;
	}
	addBankItemByName(item,name) ;
	if(incRef) item->addRef() ;
	setTags(item);
	return id ;
}

template <class T>
void Bank<T>::addById( T* item, int id, bool incRef/*=true*/)
{
	addBankItemById(item,id) ;
	String name = item->getName() ;
	if( name.empty() ) {
		name = "item_" + std::to_string(id) ;
	}
	addBankItemByName(item,name) ;
	if(incRef) item->addRef() ;
	setTags(item);
}

template <class T>
void Bank<T>::addByName( T* item, const String& name, bool incRef/*=true*/)
{
	addBankItemByName(item,name) ;
	addBankItemById(item,getFirstFreeId()) ;
	if(incRef) item->addRef() ;
	setTags(item);
}

template <class T>
void Bank<T>::addByIdAndName( T* item, int id, const String& name, bool incRef/*=true*/)
{
	addBankItemById(item,id) ;
	addBankItemByName(item,name) ;
	if(incRef) item->addRef() ;
	setTags(item);
}

template <class T>
T* Bank<T>::getPtrById(int id)
{
	BankItemPtr bi = 0 ;
	if( (size_t)id < m_objectsArray.size() ) {
		bi = m_objectsArray[id] ;
	}
	JAM_ASSERT_MSG(bi,"getById(%d) returned null ptr",id) ;

	return (T*)bi ;
}

template <class T>
T* Bank<T>::getPtrByName(const String& name)
{
	BankItemPtr bi = 0 ;
	std::map<String,BankItem*>::iterator it = m_objectsMap.find(name) ;
	if( it != m_objectsMap.end() ) {
		bi = it->second ;
	}
	JAM_ASSERT_MSG(bi,("getPtrByName('%s') returned null ptr",name.c_str())) ;

	return (T*)bi ;
}

template <class T>
T* Bank<T>::findPtrByName(const String& name)
{
	BankItemPtr bi = 0 ;
	std::map<String,BankItem*>::iterator it = m_objectsMap.find(name) ;
	if( it != m_objectsMap.end() ) {	bi = it->second ;	}

	return (T*)bi ;
}

template <class T>
T* Bank<T>::findPtrById(int id)
{
	BankItemPtr bi = 0 ;
	if( (size_t)id < m_objectsArray.size() ) { bi = m_objectsArray[id] ;	}
	return (T*)bi ;
}

template <class T>
std::vector<T*> Bank<T>::getByTag( const TagType& tag )
{
	std::vector<T*> appo ;
	std::map<TagType,std::vector<BankItem*> >::const_iterator it = m_tags.find(tag) ;
	if( it != m_tags.end() ) {
		appo.reserve(it->second.size()) ;
		for( size_t i=0; i<it->second.size(); i++ ) {
			appo.push_back( dynamic_cast<T*>(it->second[i]) ) ;
		}
	}
	return appo ;
}

template <class T>
size_t Bank<T>::queryByTag( const TagType& tag, std::vector<T*>& result )
{
	std::map<TagType,std::vector<BankItem*> >::const_iterator it = m_tags.find(tag) ;
	if( it != m_tags.end() ) {
		result.reserve( result.size() + it->second.size() ) ;
		for( size_t i=0; i<it->second.size(); i++ ) {
			result.push_back( dynamic_cast<T*>(it->second[i]) ) ;
		}
	}

	return it != m_tags.end() ? it->second.size() : 0 ;
}

template <class T>
const std::vector<T*>& Bank<T>::queryByTagFast( const TagType& tag )
{
	return (const std::vector<T*>&)m_tags[tag] ;
}

template <class T>
void jam::Bank<T>::removeByTag( const TagType& tag )
{
	std::vector<T*> allItems ;
	size_t res = queryByTag(tag,allItems);
	for (size_t i=0; i<allItems.size(); i++){		removeBankItem(allItems[i]);	}
}

template <class T>
size_t Bank<T>::getCountByTag( const TagType& tag )
{		
	BankItemTagList::const_iterator it = m_tags.find(tag) ;
	if( it != m_tags.end() )
		return it->second.size() ;
	else
		return 0 ;
}

}

#endif // __JAM_BANK_H__