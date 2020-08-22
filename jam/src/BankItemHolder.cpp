/**********************************************************************************
* 
* BankItemHolder.cpp
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

#include "jam/BankItemHolder.h"
//#include "jam/Utilities.h"

#include <typeinfo>

#define JAM_BANK_RESIZE_STEP		20
#define JAM_BANK_ASSERT_CHANNEL	"Bank"

using namespace std;

namespace jam
{

//
// class implementation
//

BankItemHolder::BankItemHolder() 
#ifdef JAM_DEBUG
	: m_iteratingBank(false)
#endif
{
	m_objectsArray.resize(JAM_BANK_RESIZE_STEP);
}


BankItemHolder::~BankItemHolder()
{
	removeAllBankItems(true) ;
}


int BankItemHolder::getFirstFreeId()
{
	size_t i = 0 ;
	for( ; i<m_objectsArray.size(); i++ )
	{
		if( m_objectsArray[i] == 0 ) {
			break ;
		}
	}

	// no free position found
	if( i == m_objectsArray.size()-1 ) {
		m_objectsArray.resize( m_objectsArray.size() + JAM_BANK_RESIZE_STEP, 0 ) ;
		i++ ;
	}
	
	return i ;
}


int BankItemHolder::getLastFreeId()
{
	int i = m_objectsArray.size()-1 ;
	for( ; i>=0; i-- )
	{
		if( m_objectsArray[i] == 0 ) {
			break ;
		}
	}

	// no free position found
	if( i < 0 ) {
		i = m_objectsArray.size() ;
		m_objectsArray.resize( m_objectsArray.size() + JAM_BANK_RESIZE_STEP, 0 ) ;
	}

	return i ;
}


bool BankItemHolder::existsById( int id )
{
	BankItemPtr bi = 0 ;
	if( (size_t)id < m_objectsArray.size() ) {
		bi = m_objectsArray[id] ;
	}

	return bi != 0 ;
}

bool BankItemHolder::existsByName( const String& name )
{
	BankItemPtr bi = 0 ;
	std::map<String,BankItem*>::iterator it = m_objectsMap.find(name) ;
	if( it != m_objectsMap.end() ) {
		bi = it->second ;
	}

	return bi != 0 ;
}

void BankItemHolder::removeBankItem( BankItemPtr item )
{
#ifdef JAM_DEBUG
	JAM_ASSERT_MSG( (m_iteratingBank==false),("Removing bankitem while iterating bank is not recommended") ) ;
#endif
	removeBankItemWithoutReleasing( item ) ;
	item->release() ;
}


void BankItemHolder::removeBankItemByName( const String& name )
{
	BankItemMap::iterator it = m_objectsMap.find(name) ;
	JAM_ASSERT_MSG(it != m_objectsMap.end(),("removeBankItemByName() : item with name '%s' not found", name.c_str())) ;
	BankItemPtr bi = it->second ;
	removeBankItemWithoutReleasing(bi) ;
	bi->release() ;
}


void BankItemHolder::removeBankItemById( int id )
{
	BankItemPtr bi = m_objectsArray[id] ;
	JAM_ASSERT_MSG(bi, "removeBankItemById() : item with id '%d' not found", id ) ;
	removeBankItemWithoutReleasing(bi) ;
	bi->release() ;
}



void BankItemHolder::removeAllBankItems( bool forceReserved /*= false*/  )
{
#ifdef JAM_DEBUG
	dump("removeAllBankItems()");
#endif
	for ( size_t i=0; i<m_objectsArray.size(); i++ ) {
		if( m_objectsArray[i] != 0 && (!m_objectsArray[i]->isReserved() || forceReserved) ) {
			removeBankItem(m_objectsArray[i]) ;
		}
	}
	m_objectsArray.clear() ;
	m_objectsMap.clear() ;
}


void BankItemHolder::dump(const char* msg)
{
	JAM_TRACE( "%s : bank '%s' contains %zu items\n", msg, typeid(*this).name(), m_objectsMap.size() );
}


void BankItemHolder::addBankItemById( BankItemPtr item, int id )
{
	while( id >= (int)m_objectsArray.size() ) {
		m_objectsArray.push_back(0);
	}

	JAM_ASSERT_MSG(m_objectsArray[id]==0, "addBankItemById() : id %d already in use",id );
	item->setId(id) ;
	item->setBank(this);
	m_objectsArray[id] = item ;
}


void BankItemHolder::addBankItemByName( BankItemPtr item, const String& name )
{
	JAM_ASSERT_MSG(!name.empty(),("addBankItemByName() : name is empty")) ;
	JAM_ASSERT_MSG( m_objectsMap.find(name)==m_objectsMap.end(), ("addBankItemByName() : name %s already in use",name.c_str()) ) ;
	m_objectsMap[name] = item ;
	item->setName(name) ;
	item->setBank(this);
}


BankItem* BankItemHolder::removeBankItemWithoutReleasing( BankItemPtr item )
{
	JAM_ASSERT_MSG(item!=0,("removeBankItemWithoutReleasing() : null pointer")) ;
	JAM_ASSERT_MSG(m_objectsArray.size()!=0,("removeBankItemWithoutReleasing() : bank is empty")) ;

	if( item->m_bank ) {
		int id = item->getId() ;
		BankItemPtr bi = m_objectsArray[id] ;
		JAM_ASSERT_MSG(bi, "removeBankItem() : item with address '%p' not found", item ) ;
		String name = bi->getName() ;
		m_objectsArray[id] = 0 ;

		m_objectsMap.erase(name) ;

		deleteTags(bi);
		bi->setBank(0);
		bi->setId(-1) ;
		bi->setName("") ;
	}
	return item ;
}


void BankItemHolder::setTags( BankItemPtr item )
{
	TagSet& itemTags = item->m_tags ;
	for( TagSet::const_iterator i=itemTags.begin(); i != itemTags.end(); i++ ) {
		const TagType& tag = *i ;
		BankItemTagList::iterator it = m_tags.find(tag) ;
		if( it == m_tags.end() ) {
			m_tags[tag].push_back(item);
		}
		else {
			it->second.push_back(item) ;
		}
	}
}


void BankItemHolder::deleteTags( BankItemPtr item )
{
	TagSet& itemTags = item->m_tags ;
	for( TagSet::const_iterator i=item->getTags().begin(); i != itemTags.end(); i++ ) {
		const TagType& tag = *i ;
		BankItemList& v = m_tags[tag] ;
		BankItemList::iterator it = std::find(v.begin(), v.end(), item ) ;
		if( it!=v.end() ) {
			v.erase(it) ;
		}
	}
}


BankItemHolder::BankItemList& BankItemHolder::getBankItemsByTag( const TagType& tag )
{
	return m_tags[tag] ;
}

void BankItemHolder::changeBankItemName( BankItemPtr item, const String& name )
{
//	JAM_ASSERT_MSG( m_objectsMap.find(name)==m_objectsMap.end(), ("addBankItemByName() : name %s already in use",name.c_str()) ) ;

	BankItemMap::iterator it = m_objectsMap.find(item->getName()) ;
	if( it!=m_objectsMap.end() ) {
		m_objectsMap.erase(it) ;
	}
	m_objectsMap[name] = item ;
	item->setBank(0) ;
	item->setName(name) ;
	item->setBank(this) ;
}

}

