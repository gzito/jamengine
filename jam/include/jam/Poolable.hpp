/**********************************************************************************
* 
* Poolable.h
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

#ifndef __JAM_Poolable_H__
#define __JAM_Poolable_H__

#include <jam/jam.h>


namespace jam
{


// Ensure that the objects can only be created by the pool. 
// A simple way to do this is to make the pool class a friend of the object class,
// and then make the object�s constructor private.

// interface
class Poolable
{
	template<class T> friend class ObjectPool;

protected:
							Poolable() : m_pNext(0), m_inuse(false) {}
	bool					isInUse() { return m_inuse; }
private:
	void					setInUse(bool val) { m_inuse = val; }


	void					setNext(Poolable* p) { m_pNext = p; }
	Poolable*				getNext() { return m_pNext; }

	// overridables
	virtual void			extractedFromPool() {}
	virtual void			returnedToPool() {}

private:
	bool					m_inuse ;
	Poolable*				m_pNext ;
};


}

#endif // __JAM_Poolable_H__
