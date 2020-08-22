/**********************************************************************************
* 
* Singleton.h
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

#ifndef __JAM_SINGLETON_H__
#define __JAM_SINGLETON_H__

#include <jam/jam.h>


namespace jam
{

/**
	Template class for creating single-instance global classes.
*/
template <class T>
class Singleton
{
public:
	static T&				getSingleton();
	static void				destroySingleton() ;

protected:
							Singleton() = default ;
	virtual					~Singleton() = default ;

	static T*				m_singleton ;

private:
	// to prevent the use
							Singleton( const Singleton& ) = delete ;
	Singleton&				operator=( const Singleton& ) = delete ;
};




//
// class implementation
//

template <class T>
T* Singleton<T>::m_singleton = 0 ;

template <class T>
T& Singleton<T>::getSingleton()
{
	if( m_singleton == 0 ) {
		m_singleton = new T() ;
	}
	return *m_singleton ;
}

template <class T>
void Singleton<T>::destroySingleton()
{
	JAM_DELETE(m_singleton) ;
}

}

#endif // __JAM_SINGLETON_H__
