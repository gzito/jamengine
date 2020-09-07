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

#ifdef JAM_EXCLUDED_BLOCK

#include <jam/jam.h>
#include <jam/String.h>


namespace jam
{

/**
*
* \remark RefCountedObjects created on the stack cannot be managed. DO NOT CALL addRef, release or autorelease on them
*/
class JAM_API RefCountedObject
{
//	friend class AutoReleasePool ;
	template<typename> friend class Ref ;

public:
	virtual ~RefCountedObject();

	/** Returns the value of reference counter */
	int32_t					getRefCount() const ;

#ifdef JAM_DEBUG
	/** Get object debug info */
	virtual String			getDebugInfo(bool typeInfo=true) ;
	void					setFileLine(char* file, int line) { m_file=file; m_line=line; }
#endif

protected:
	RefCountedObject();

	int32_t					m_refCount ;

private:
	/** Increments the reference counter */
	void					addRef();

	/**
		Decrements the reference counter
		\remark The object will be destroyed when its reference counter become 0
	*/
	void					release();

#ifdef JAM_DEBUG
private:
	String					m_file ;
	int						m_line ;
#endif

private:
	// to prevent the use
							RefCountedObject( const RefCountedObject& ) = delete ;
	RefCountedObject&		operator=(const RefCountedObject&) = delete ;
};


template <typename T=RefCountedObject>
class Ref
{
public:
							Ref() ;
	explicit				Ref( T* rawptr ) ;

							~Ref() ;

							Ref( const Ref& other ) ;
	Ref&					operator=( const Ref& other ) ;

							Ref( Ref&& other ) noexcept ;
	Ref&					operator=( Ref&& other ) noexcept ;

	T*						get() const ;
	void					reset( T* rawptr = nullptr ) ;

	T*						operator->() const ;
	T&						operator*() const ;

	bool					operator==( const T* rawptr ) const ;
	bool					operator==( const Ref& other ) const ;
	bool					operator!=( const T* rawptr ) const ;
	bool					operator!=( const Ref& other ) const ;

	bool					operator!() const ;
	explicit				operator bool() const ;

private:
	T*						m_pReferenceCountedObject ;
};

template <typename T>
Ref<T>::Ref() : m_pReferenceCountedObject( nullptr ) {
}

template <typename T>
Ref<T>::Ref( T* rawptr ) : m_pReferenceCountedObject( rawptr ) {
}

template <typename T>
Ref<T>::~Ref() {
	if( m_pReferenceCountedObject != nullptr ) {
		m_pReferenceCountedObject->release() ;
	}
}

template <typename T>
Ref<T>::Ref( const Ref& other ) {
	m_pReferenceCountedObject = other.m_pReferenceCountedObject ;
	if( m_pReferenceCountedObject != nullptr ) {
		m_pReferenceCountedObject->addRef() ;
	}
}

template <typename T>
Ref<T>& Ref<T>::operator=( const Ref<T>& other ) {
	if( this != &other ) {
		if( m_pReferenceCountedObject != nullptr ) {
			m_pReferenceCountedObject->release() ;
		}

		m_pReferenceCountedObject = other.m_pReferenceCountedObject ;

		if( m_pReferenceCountedObject != nullptr ) {
			m_pReferenceCountedObject->addRef() ;
		}
	}
	return *this ;
}

template <typename T>
Ref<T>::Ref( Ref&& other ) noexcept {
	m_pReferenceCountedObject = other.m_pReferenceCountedObject ;
	other.m_pReferenceCountedObject = nullptr ;
}

template <typename T>
Ref<T>& Ref<T>::operator=( Ref<T>&& other ) noexcept {
	if( this != &other ) {
		m_pReferenceCountedObject = other.m_pReferenceCountedObject ;
		other.m_pReferenceCountedObject = nullptr ;
	}
	return *this ;
}

template <typename T>
T* Ref<T>::get() const {
	return m_pReferenceCountedObject ;
}

template <typename T>
void Ref<T>::reset( T* rawptr /* = nullptr */ ) {
	if( m_pReferenceCountedObject != nullptr ) {
		m_pReferenceCountedObject->release() ;
	}

	m_pReferenceCountedObject = rawptr ;

	if( m_pReferenceCountedObject != nullptr ) {
		m_pReferenceCountedObject->addRef() ;
	}	
}


template <typename T>
T* Ref<T>::operator->() const {
	return m_pReferenceCountedObject ;
}

template <typename T>
T& Ref<T>::operator*() const {
	return *m_pReferenceCountedObject ;
}

template <typename T>
bool Ref<T>::operator==( const T* rawptr ) const {
	return m_pReferenceCountedObject == rawptr ;
}

template <typename T>
bool Ref<T>::operator==( const Ref& other ) const {
	return m_pReferenceCountedObject == other.m_pReferenceCountedObject ;
}

template <typename T>
bool Ref<T>::operator!=( const T* rawptr ) const {
	return m_pReferenceCountedObject != rawptr ;
}

template <typename T>
bool Ref<T>::operator!=( const Ref& other ) const {
	return m_pReferenceCountedObject != other.m_pReferenceCountedObject ;
}

template<typename T>
bool Ref<T>::operator!() const {
	return !m_pReferenceCountedObject;
}

template <typename T>
Ref<T>::operator bool() const {
	return m_pReferenceCountedObject != nullptr ;
}

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

#endif // JAM_EXCLUDED_BLOCK

#endif // __JAM_REFCOUNTEDOBJECT_H__
