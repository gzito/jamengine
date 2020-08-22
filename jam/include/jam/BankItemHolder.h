/**********************************************************************************
* 
* BankItemHolder.h
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
#ifndef __JAM_BANKITEMHOLDER_H__
#define __JAM_BANKITEMHOLDER_H__

#include <jam/jam.h>
#include <jam/BankItem.h>
#include <jam/String.h>

#include <vector>
#include <map>

#ifdef JAM_DEBUG
#define JAM_BANK_ITERATION_START				m_iteratingBank=true ;
#define JAM_BANK_ITERATION_END					m_iteratingBank=false ;
#else
#define JAM_BANK_ITERATION_START				
#define JAM_BANK_ITERATION_END					
#endif

namespace jam
{

class JAM_API BankItemHolder
{
	friend class BankItem ;

public:
	typedef BankItem*									BankItemPtr ;
	typedef std::vector<BankItemPtr>					BankItemList ;
	typedef std::map<String,BankItemPtr>				BankItemMap ;
	typedef std::map<TagType,BankItemList>				BankItemTagList ;

	int						getFirstFreeId() ;
	int						getLastFreeId() ;

	bool					existsById( int id ) ;
	bool					existsByName(const String& name) ;

	void					removeBankItem(BankItemPtr item) ;
	void					removeAllBankItems( bool forceReserved = false ) ;

	virtual void			dump(const char* msg);
	int						countItems() {return	(int)m_objectsMap.size(); }
protected:
	BankItemHolder() ;
	virtual ~BankItemHolder() ;

	void					addBankItemById( BankItemPtr item, int id ) ;
	// overwrite item name (if item already has name)
	void					addBankItemByName( BankItemPtr item, const String& name ) ;

	void					removeBankItemById( int id ) ;
	void					removeBankItemByName( const String& name ) ;

	BankItem*				removeBankItemWithoutReleasing(BankItemPtr item) ;

	BankItemList&			getBankItemsByTag( const TagType& tag ) ;
	
	// add the given item to "m_tags" map
	void					setTags(BankItemPtr item) ;
	// remove the given item from the "m_tags" map
	void					deleteTags(BankItemPtr item) ;

	void					changeBankItemName( BankItemPtr item, const String& name ) ;

protected:
	BankItemList			m_objectsArray;
	BankItemMap				m_objectsMap;
	BankItemTagList			m_tags ;			// it is a map<tag,list-of-items>: each tag is associated to a list of item having this tag

#ifdef JAM_DEBUG
	bool					m_iteratingBank ;
#endif
};

}

#endif // __JAM_BANKITEMHOLDER_H__
