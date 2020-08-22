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
		size_t length() const { return numOfElements; }

		// zeroes the array
		void setZero() { memset( m_data, 0, byteSize() ) ; }

		// sets with given value
		void set( const T& fill ) { for(int i=0; i<length(); i++) { m_data[i] = fill ; } }

		void copyFrom( const T* buff ) { memcpy( m_data, buff, byteSize() ) ; }
		void copyFrom( const T* buff, size_t bytesCount ) { memcpy( m_data, buff, bytesCount ) ; }

		// the number of bytes occupied by the array
		size_t byteSize() const { return sizeof(T)*numOfElements; }

		// array operators
		const T&		operator[]( size_t idx ) const { JAM_ASSERT( idx < numOfElements ); return m_data[idx]; }
		T&				operator[]( size_t idx ) { JAM_ASSERT( idx < numOfElements ); return m_data[idx]; }

		// returns a pointer to the underlying native array
		const T*		data() const { return m_data; }
		T*				data() { return m_data; }

	private:
		T				m_data[numOfElements] ;
	};			


	/*!
		\class HeapArray

		Wraps dynamically-allocated native array
	*/
	template<typename T> class HeapArray
	{
	public:
		HeapArray(int size = 0 ) : m_numOfElements(0), m_data(0) { if(size) { create(size); } }
		~HeapArray() { destroy(); }

		void create(size_t size) { if(m_data) { destroy(); } m_numOfElements = size; m_data = new T[size]; }
		void destroy() { JAM_DELETE(m_data); }

		// the number of elements into the array
		size_t length() const { return m_numOfElements; }

		// zeroes the array
		void setZero() { memset( m_data, 0, byteSize() ) ; }

		// sets with given value
		void set( const T& fill ) { for(int i=0; i<length(); i++) { m_data[i] = fill ; } }

		void copyFrom( const T* buff ) { memcpy( m_data, buff, byteSize() ) ; }
		void copyFrom( const T* buff, size_t bytesCount ) { memcpy( m_data, buff, bytesCount ) ; }

		// the number of bytes occupied by the array
		size_t byteSize() const { return sizeof(T)*m_numOfElements; }

		// array operators
		const T&		operator[]( size_t idx ) const { JAM_ASSERT( idx < m_numOfElements ); return m_data[idx]; }
		T&				operator[]( size_t idx ) { JAM_ASSERT( idx < m_numOfElements ); return m_data[idx]; }

		// returns a pointer to the underlying native array
		const T*		data() const { return m_data; }
		T*				data() { return m_data; }

	private:
		T*				m_data ;
		size_t			m_numOfElements ;
	};			
}

#endif // __JAM_ARRAY_H__

