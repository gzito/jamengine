/**********************************************************************************
* 
* BankItem.cpp
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

#include "jam/BankItem.h"
#include "jam/BankItemHolder.h"

#include <algorithm>
#include <sstream>

using namespace std ;

namespace jam
{

BankItem::BankItem() :
	m_id(-1), m_name(), m_tags(), m_bank(0), m_isReserved(false)
{

}


BankItem::~BankItem()
{
	removeFromBank() ;
}


 void BankItem::setName( const String& val )
 {
	 // already in the bank?
 	if( m_bank != 0 ) {
		m_bank->changeBankItemName(this,val) ;
 	}
	else {
 		m_name = val;
	}
 }

void BankItem::removeFromBank()
{
	if( m_bank ) {
		m_bank->removeBankItem(this);
	}
}

void BankItem::setTag( const TagType& tag )
{
	m_tags.insert(tag);
	if( m_bank ) {
		m_bank->deleteTags(this) ;
		m_bank->setTags(this) ;
	}
}

bool BankItem::hasTag( const TagType& tag ) const
{
	return m_tags.find(tag) != m_tags.end() ;
}

void BankItem::deleteAllTags()
{
	if( m_bank ) {
		m_bank->deleteTags(this) ;
	}
	m_tags.clear() ;
}


void BankItem::destroy()
{
	if( !m_markedAsDestroyed ) {
 		//this->deleteAllTags() ;
 		this->removeFromBank();	// Modificata per evitare di restituire un item appena distrutto con la getPtrByName();
 		RefCountedObject::destroy() ;
	}
}


#ifdef JAM_DEBUG
String BankItem::getDebugInfo(bool typeInfo/*=true*/)
{
	stringstream oss ;
	oss << RefCountedObject::getDebugInfo(typeInfo) ;
	oss << " id=" << m_id ;
	oss << " name=" << m_name ;
	return oss.str() ;
}
#endif

}
