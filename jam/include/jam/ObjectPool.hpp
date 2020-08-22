/**********************************************************************************
* 
* ObjectPool.h
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

#ifndef __JAM_ObjectPool_H__
#define __JAM_ObjectPool_H__

#include <jam/jam.h>
#include <jam/Poolable.hpp>
#include <jam/Singleton.h>
#include <vector>

namespace jam
{

/**
	Pool for game objects

	\remark Objects in the pool MUST inherit from Poolable AND RefCountedObject
*/
template<class T=Poolable>
class ObjectPool : public Singleton< ObjectPool<T> >
{
	friend class Singleton<ObjectPool<T> > ;
	
public:
	void					initialize( size_t initialSize, bool canGrow = true ) ;

	T*						getObject() ;
	void					releaseObject( T* pObj ) ;

	void					dump() const ;

private:
							ObjectPool() ;
							~ObjectPool() ;

	std::vector<T*>			m_pool ;
	bool					m_canGrow ;
	T*						m_pNextFreeObject ;

	int32					m_numOfInUse ;
};


template<class T>
void ObjectPool<T>::initialize( size_t initialSize, bool canGrow /*= true*/ )
{
	m_pool.resize( initialSize ) ;

	for( size_t i=0; i<initialSize; i++ ) {
		m_pool[i] = new T() ;
		m_pool[i]->setInUse( false ) ;
		// links the previous element to the current one
		if( i > 0 ) m_pool[i-1]->setNext( m_pool[i] ) ;
	}

	// last array element 
	m_pool[initialSize-1]->setNext(0) ;

	// points to the first element
	m_pNextFreeObject = m_pool[0] ;

//	dump() ;
}


template<class T>
T* ObjectPool<T>::getObject()
{
	T* pObject = 0 ;

	JAM_ASSERT_MSG( (m_pool.size() != 0 ), ("ObjectPool not initialized") ) ;

//	dump() ;

	if( m_pNextFreeObject != 0 ) {
		// pop front
		pObject = m_pNextFreeObject ;
		pObject->setInUse(true) ;
		m_pNextFreeObject = (T*)pObject->getNext() ;
		pObject->setNext(0) ;
		pObject->extractedFromPool() ;
		m_numOfInUse++ ;
	}
	else if( m_canGrow ) {
		JAM_ASSERT( m_numOfInUse == m_pool.size() ) ;
		pObject = new T() ;
		pObject->setInUse( true ) ;
		pObject->setNext( 0 ) ;
		pObject->extractedFromPool() ;
		// not neeeded, already 0
		//m_pNextFreeObject = 0 ;
		m_pool.push_back(pObject) ;
		m_numOfInUse++ ;
		JAM_TRACE( ("ObjectPool grow (size is %d, numOfInUse is %d)",m_pool.size(),m_numOfInUse) ) ;
	}
// else {
// 	pObject = 0 ;
// }
//	IWGE_TRACE(("getObject: %p",pObject)) ;
	return pObject ;
}


template<class T>
void ObjectPool<T>::releaseObject( T* pObj )
{
//	IWGE_TRACE(("releaseObject %p",pObj)) ;
	JAM_ASSERT_MSG( (pObj->isInUse()), ("Already released object") ) ;
	if( pObj->isInUse() ) {
		pObj->returnedToPool() ;
		pObj->setInUse( false ) ;
		pObj->setNext(m_pNextFreeObject) ;	// push front
		m_pNextFreeObject = pObj ;
		m_numOfInUse-- ;
	}
//	dump() ;
}


template<class T>
ObjectPool<T>::ObjectPool() :
	m_pool(), m_pNextFreeObject(0), m_canGrow(true), m_numOfInUse(0)
{
}


template<class T>
ObjectPool<T>::~ObjectPool()
{
	for( size_t i=0; i<m_pool.size(); i++ ) {
		JAM_RELEASE(m_pool[i]) ;
	}

	m_pNextFreeObject = 0 ;
}

template<class T>
void ObjectPool<T>::dump() const
{
	JAM_TRACE(("--------- dump ---------")) ;
	JAM_TRACE(("poolsize: %d", m_pool.size() ) );
	JAM_TRACE(("numOfInUse: %d", m_numOfInUse )) ;
	JAM_TRACE(("m_pNextFreeObject: %p",m_pNextFreeObject)) ;
	for(size_t i=0; i<m_pool.size(); i++) {
		T* pObject = m_pool[i] ;
		JAM_TRACE( ("--- Item #%d: %p ---",i, pObject) ) ;
		JAM_TRACE( ("isInUse: %d", pObject->isInUse()) ) ;
		JAM_TRACE( ("getNext():   %p", (T*)pObject->getNext()) ) ;
	}
}


} // end namespace iwge ;

#endif // __JAM_ObjectPool_H__
