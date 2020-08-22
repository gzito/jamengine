/**********************************************************************************
* 
* BankItem.h
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

#ifndef __JAM_BANKITEM_H__
#define __JAM_BANKITEM_H__

#include <jam/jam.h>
#include <jam/RefCountedObject.h>
#include <jam/String.h>

#include <set>

namespace jam
{

using	TagType = String ;
using  	TagSet = std::set<TagType> ;

// forward reference
class BankItemHolder ;

//
// interface
//
class JAM_API BankItem : public RefCountedObject
{
	friend class BankItemHolder ;

public:
	BankItem();
	virtual ~BankItem() ;

	/// Returns the id of this BankItem
	int						getId() const { return m_id; }

	/// Returns the name of this BankItem
	String					getName() const { return m_name; }
	void					setName(const String& val) ;

	/// Returns the bank this BankItem is contained in (0 if it was never inserted into a bank)
	BankItemHolder*			getBank() { return m_bank; }

	/// Adds a new tag to this BankItem
	void					setTag(const TagType& tag);
	const TagSet&			getTags() const { return m_tags; }
	void					deleteAllTags() ;

	/// Returns true if the BankItem has the specified tag, otherwise returns false
	bool					hasTag(const TagType& tag) const ;

	/// Removes this BankItem from the Bank containing it
	void					removeFromBank() ;

	/// Returns if this object is "reserved", e.g. used internally by the engine
	bool					isReserved() const { return m_isReserved; }
	
	/// Set this object as "reserved", e.g. used internally by the engine
	void					setReserved( bool val = true ) { m_isReserved = val; }


	/**
		Marks the object as destroyed.
		Overrides RefCountedObject::destroy()
		\remark Object destruction will be delayed at the end of current frame
	*/
	virtual void			destroy();

#ifdef JAM_DEBUG
	virtual String			getDebugInfo(bool typeInfo=true) ;
#endif

private:
	// these method are private and they'll be used by BankItemHolder
	void					setId(int val) { m_id = val; }
	void					setBank(BankItemHolder* val) { m_bank = val; }

private:
	int						m_id ;
	String					m_name ;
	TagSet					m_tags ;
	BankItemHolder*			m_bank ;
	bool					m_isReserved ;
//	U8						pad1_, pad2_, pad3_ ;
};

}

#endif // __JAM_BANKITEM_H__
