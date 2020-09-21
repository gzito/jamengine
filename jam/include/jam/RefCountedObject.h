/**********************************************************************************
* 
* RefCountedObject.h
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

#ifndef __JAM_REFCOUNTEDOBJECT_H__
#define __JAM_REFCOUNTEDOBJECT_H__

#include <jam/jam.h>
#include <jam/String.h>

#include <atomic>


namespace jam
{

/**
*
* \remark A RefCountedObject when created has a reference counter of 1
*/
class JAM_API RefCountedObject
{
//	friend class AutoReleasePool ;
	template<typename> friend class Ref ;

public:
	/** Returns the value of reference counter */
	int32_t					getRefCount() const ;

	/** Increments the reference counter */
	void					addRef();

	/**
		Decrements the reference counter
		\remark The object will be freed when its reference counter become 0
	*/
	void					release();

#ifdef JAM_DEBUG
	/** Get object debug info */
	virtual String			getDebugInfo(bool typeInfo=true) ;
	void					setFileLine(char* file, int line) { m_file=file; m_line=line; }
#endif

protected:
							RefCountedObject();
	virtual					~RefCountedObject();

	std::atomic_int32_t		m_refCount ;

#ifdef JAM_DEBUG
private:
	String					m_file ;
	int						m_line ;
#endif

private:
	// forbids copy-construction and assignment
							RefCountedObject( const RefCountedObject& ) = delete ;
	RefCountedObject&		operator=(const RefCountedObject&) = delete ;
};

}

#endif // __JAM_REFCOUNTEDOBJECT_H__
