/**********************************************************************************
* 
* array.hpp
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

#ifndef __JAM_ARRAY_H__
#define __JAM_ARRAY_H__

#include <jam/jam.h>
#include <memory.h>

namespace jam
{

/*!
	\class Array

	Wraps statically-allocated native arrays
*/
template<typename T, size_t numOfElements> class Array
{
public:
							Array() {}
							~Array() {}

	// the number of elements into the array
	size_t					length() const { return numOfElements; }

	// zeroes the array
	void					setZero() { memset( m_data, 0, byteSize() ) ; }

	// sets with given value
	void					set( const T& fill ) { for(int i=0; i<length(); i++) { m_data[i] = fill ; } }

	void					copyFrom( const T* buff ) { memcpy( m_data, buff, byteSize() ) ; }
	void					copyFrom( const T* buff, size_t bytesCount ) { memcpy( m_data, buff, bytesCount ) ; }

	// the number of bytes occupied by the array
	size_t					byteSize() const { return sizeof(T)*numOfElements; }

	// array operators
	const T&				operator[]( size_t idx ) const { JAM_ASSERT( idx < numOfElements ); return m_data[idx]; }
	T&						operator[]( size_t idx ) { JAM_ASSERT( idx < numOfElements ); return m_data[idx]; }

	// returns a pointer to the underlying native array
	const T*				data() const { return m_data; }
	T*						data() { return m_data; }

private:
	T						m_data[numOfElements] ;
};			


/*!
	\class HeapArray

	Wraps dynamically-allocated native array
*/
template<typename T> class HeapArray
{
public:
							HeapArray(int size = 0 ) ;
							~HeapArray() { destroy(); }

							HeapArray( const HeapArray<T>& other ) ;
	HeapArray<T>&			operator=( const HeapArray<T>& other ) ;

							HeapArray( HeapArray<T>&& other ) ;
	HeapArray<T>&			operator=( HeapArray<T>&& other ) ;

	void					create(size_t size) ;
	void					destroy() ;

	// the number of elements into the array
	size_t					length() const { return m_numOfElements; }

	// the number of bytes occupied by the array
	size_t					byteSize() const { return sizeof(T)*m_numOfElements; }

	// zeroes the array
	void					setZero() ;

	// resize the array, trying to keep the existing content
	void					resize( int newSize ) ;

	// sets with given value
	void					set( const T& fill ) ;

	static void				copy( const HeapArray<T>& sourceArray, HeapArray<T>& destinationArray, long length ) ;

	// Copies all the elements of the current array to the specified array starting at the specified destination array index. 
	void					copyTo( HeapArray<T>& array, size_t index ) ;

	void					copyFrom( const T* buff ) ;
	void					copyFrom( const T* buff, size_t bytesCount ) ;

	// array operators
	const T&				operator[]( size_t idx ) const { JAM_ASSERT( idx < m_numOfElements ); return m_data[idx]; }
	T&						operator[]( size_t idx ) { JAM_ASSERT( idx < m_numOfElements ); return m_data[idx]; }

	// returns a pointer to the underlying native array
	const T*				data() const { return m_data; }
	T*						data() { return m_data; }

private:
	T*						m_data ;
	size_t					m_numOfElements ;
};			

template<typename T>
inline HeapArray<T>::HeapArray(int size /* = 0 */ )
	: m_numOfElements(0), m_data(0)
{
	if( size ) { 
		create(size);
	}
}

template<typename T>
inline HeapArray<T>::HeapArray(const HeapArray<T>& other)
{
	*this = other ;
}

template<typename T>
inline HeapArray<T>& HeapArray<T>::operator=(const HeapArray<T>& other)
{
	if( this != &other ) {
		create(other.length()) ;
		copy( other, *this, other.length() ) ;
		m_numOfElements = other.m_numOfElements ;
	}

	return *this ;
}

template<typename T>
inline HeapArray<T>::HeapArray(HeapArray<T>&& other) : m_data(nullptr), m_numOfElements(0)
{
	m_data = other.m_data ;
	m_numOfElements = other.m_numOfElements ;
	other.m_data = nullptr ;
	other-m_numOfElements = 0 ;
}

template<typename T>
inline HeapArray<T>& HeapArray<T>::operator=(HeapArray<T>&& other)
{
	if( this != &other ) {
		JAM_DELETE_ARRAY(m_data) ;
		m_data = other.m_data ;
		m_numOfElements = other.m_numOfElements ;
		other.m_data = nullptr ;
		other-m_numOfElements = 0 ;
	}

	return *this ;
}

template<typename T>
inline void HeapArray<T>::create(size_t size)
{
	if(m_data) { 
		destroy();
	} 
	m_numOfElements = size; 
	m_data = new T[size];
}

template<typename T>
inline void HeapArray<T>::destroy()
{
	JAM_DELETE_ARRAY(m_data);
	m_data = nullptr ;
	m_numOfElements = 0 ;
}

template<typename T>
inline void HeapArray<T>::setZero()
{
	memset( m_data, 0, byteSize() ) ;
}

template<typename T>
inline void HeapArray<T>::resize(int newSize)
{
	T* newArray = new T[newSize] ;
	memcpy( newArray, m_data, byteSize() ) ;
	destroy() ;

	m_data = newArray ;
	m_numOfElements = newSize ;
}

template<typename T>
inline void HeapArray<T>::set(const T& fill)
{
	for(int i=0; i<length(); i++) { 
		m_data[i] = fill ;
	}
}

template<typename T>
inline void HeapArray<T>::copy(const HeapArray<T>& sourceArray,HeapArray<T>& destinationArray,long length)
{
	memcpy( destinationArray.data(), sourceArray.data(), length*sizeof(T) );
}

template<typename T>
inline void HeapArray<T>::copyTo(HeapArray<T>& array,size_t index)
{
	memcpy( &array[index], m_data, sizeof(T)*(array.length() - index) ) ;
}

template<typename T>
inline void HeapArray<T>::copyFrom(const T* buff)
{
	copyFrom( buff, byteSize() ) ;
}

template<typename T>
inline void HeapArray<T>::copyFrom(const T* buff,size_t bytesCount)
{
	memcpy( m_data, buff, bytesCount ) ;
}

}

#endif // __JAM_ARRAY_H__

