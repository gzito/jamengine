/**********************************************************************************
* 
* Ref.hpp
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

#ifndef __JAM_REF_HPP__
#define __JAM_REF_HPP__

#include <jam/jam.h>


namespace jam
{

template <typename T>
class Ref
{
public:
	Ref(): m_pReferenceCountedObject(nullptr) {
	}

	explicit Ref(T* rawptr): m_pReferenceCountedObject(rawptr) {
	}

	Ref(T* rawptr,bool shared): m_pReferenceCountedObject(rawptr)
	{
		if(shared && rawptr) {
			rawptr->addRef() ;
		}
	}

	Ref(const Ref& other) {
		m_pReferenceCountedObject = other.m_pReferenceCountedObject ;
		if(m_pReferenceCountedObject != nullptr) {
			m_pReferenceCountedObject->addRef() ;
		}
	}

	~Ref() {
		if(m_pReferenceCountedObject != nullptr) {
			m_pReferenceCountedObject->release() ;
		}
	}

	Ref& assign(const Ref& other) {
		if(this != &other) {
			if(m_pReferenceCountedObject != nullptr) {
				m_pReferenceCountedObject->release() ;
			}

			m_pReferenceCountedObject = other.m_pReferenceCountedObject ;

			if(m_pReferenceCountedObject != nullptr) {
				m_pReferenceCountedObject->addRef() ;
			}
		}
		return *this ;
	}
	Ref& assign(const T* rawptr,bool shared) {
		if(m_pReferenceCountedObject != rawptr) {
			if(m_pReferenceCountedObject != nullptr) {
				m_pReferenceCountedObject->release() ;
			}

			m_pReferenceCountedObject = const_cast<T*>(rawptr) ;

			if(shared && rawptr) {
				m_pReferenceCountedObject->addRef() ;
			}
		}
		return *this ;
	}

	Ref& operator=( const Ref& other ) {
		return assign(other) ;
	}
	
	Ref& operator=( const T* rawptr ) {
		return assign(rawptr,false) ;
	}

	Ref(Ref&& other) noexcept {
		m_pReferenceCountedObject = other.m_pReferenceCountedObject ;
		other.m_pReferenceCountedObject = nullptr ;
	}

	Ref& operator=( Ref&& other ) noexcept 	{
		if( this != &other ) {
			m_pReferenceCountedObject = other.m_pReferenceCountedObject ;
			other.m_pReferenceCountedObject = nullptr ;
		}
		return *this ;
	}

	T* get() {
		return m_pReferenceCountedObject ;
	}
	
	const T* get() const {
		return m_pReferenceCountedObject ;
	}

	operator T* () {
		return m_pReferenceCountedObject;
	}
	
	operator const T* () const {
		return m_pReferenceCountedObject;
	}

	void reset(T* rawptr = nullptr) {
		if(m_pReferenceCountedObject != nullptr) {
			m_pReferenceCountedObject->release() ;
		}

		m_pReferenceCountedObject = rawptr ;

		if(m_pReferenceCountedObject != nullptr) {
			m_pReferenceCountedObject->addRef() ;
		}
	}

	//T* addRef() {
	//	if(m_pReferenceCountedObject) {
	//		m_pReferenceCountedObject->addRef() ;
	//	}
	//	return m_pReferenceCountedObject ;
	//}

	T* operator->() {
		return m_pReferenceCountedObject ;
	}
	
	const T* operator->() const {
		return m_pReferenceCountedObject ;
	}

	T& operator*() {
		if( !m_pReferenceCountedObject ) { 
			JAM_ERROR("Null pointer");
		}
		return *m_pReferenceCountedObject ;
	}
	
	const T& operator*() const {
		if( !m_pReferenceCountedObject ) { 
			JAM_ERROR("Null pointer");
		}
		return *m_pReferenceCountedObject ;
	}

	bool operator==( const Ref& other ) const {
		return m_pReferenceCountedObject == other.m_pReferenceCountedObject ;
	}

	bool operator==( T* rawptr ) const {
		return m_pReferenceCountedObject == rawptr ;
	}
	
	bool operator==( const T* rawptr ) const {
		return m_pReferenceCountedObject == rawptr ;
	}
	
	bool operator==( std::nullptr_t rawptr ) const {
		return m_pReferenceCountedObject == rawptr ;
	}
	
	bool operator!=( const Ref& other ) const {
		return m_pReferenceCountedObject != other.m_pReferenceCountedObject ;
	}

	bool operator!=( T* rawptr ) const {
		return m_pReferenceCountedObject != rawptr ;
	}

	bool operator!=( const T* rawptr ) const {
		return m_pReferenceCountedObject != rawptr ;
	}
	
	bool operator!=( std::nullptr_t rawptr ) const {
		return m_pReferenceCountedObject != rawptr ;
	}
	
	bool isNull() const {
		return m_pReferenceCountedObject == nullptr ;
	}

	bool operator!() const {
		return !m_pReferenceCountedObject;
	}
	
	explicit operator bool() const {
		return m_pReferenceCountedObject != nullptr ;
	}

private:
	T*						m_pReferenceCountedObject ;
};

template <typename S,typename T>
Ref<S> dynamic_ref_cast( const Ref<T>& r ) {
    if( auto p = dynamic_cast<S*>(r.get()) ) {
        return Ref<S>(p);
    } else {
        return Ref<S>(nullptr);
    }
}

template <typename S,typename T>
Ref<S> static_ref_cast( const Ref<T>& r ) {
    if( auto p = static_cast<S*>(r.get()) ) {
        return Ref<S>(p);
    } else {
        return Ref<S>(nullptr);
    }
}

template <typename S,typename T>
Ref<S> const_ref_cast( const Ref<T>& r ) {
    if( auto p = const_cast<S*>(r.get()) ) {
        return Ref<S>(p);
    } else {
        return Ref<S>(nullptr);
    }
}

template <typename T, typename... Args>
Ref<T> make_ref(Args&&... args) {
	return Ref<T>(new T(args...)) ;
}

}

#endif // __JAM_REF_HPP__
