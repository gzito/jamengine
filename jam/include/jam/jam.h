/**********************************************************************************
* 
* jam.h
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

#ifndef __JAM_H__
#define __JAM_H__

#include <jam/jam-config.h>
#include <jam/core/types.h>

#include <cassert>
#include <windows.h>

// When building jam as dll the JAM_EXPORTS symbol is defined in the corresponding dll project
// Applications linking jam as dll have to define symbol JAM_DLL
#if defined(WIN32) && defined(JAM_DLL)
	#ifdef JAM_EXPORTS
		#define JAM_API __declspec(dllexport)
	#else
		#define JAM_API __declspec(dllimport)
	#endif

	#pragma warning (disable : 4251)
	#pragma warning (disable : 4275)
#else
	#define JAM_API
#endif


#ifdef _MSC_VER
	#define JAM_FUNCTION_NAME		__FUNCTION__
#elif defined(__GNUC__)
	#define JAM_FUNCTION_NAME		__func__
#else
	#error "Unsupported compiler"
#endif

#define JAM_PROFILE(x)				
#define JAM_INLINE					inline

#define JAM_DELETE(x)				if( (x) ) { delete (x); (x)=NULL; }
#define JAM_DELETE_ARRAY(x)			if( (x) ) { delete[] (x); (x)=NULL; }
/*
#define JAM_RELEASE(x)				if( (x) ) { (x)->release(); }
#define JAM_RELEASE_NULL(x)			if( (x) ) { (x)->release(); (x)=NULL; }
#define JAM_ADDREF(x)				if( (x) ) { (x)->addRef(); }
*/
#define JAM_DESTROY(x)				if( (x) ) { (x)->destroy(); }
#define JAM_DESTROY_NULL(x)			if( (x) ) { (x)->destroy(); (x)=NULL; }

#ifdef _DEBUG
#define JAM_BREAK(x)				DebugBreak() ;
#else
#define JAM_BREAK(x)				throw std::runtime_error(x);
#endif

//
// assertions, debugging, trace, errors
//
#define JAM_ASSERT(expr)			assert((expr))

#ifdef _DEBUG
#define JAM_ASSERT_MSG(expr, ... )	\
    do { \
		static char buff[256] ; \
        if( !(expr) ) { \
			sprintf( buff, "Assertion failed: " ) ; \
			sprintf( buff+strlen(buff), ##__VA_ARGS__ ) ; \
			sprintf( buff+strlen(buff), "\nat FILE: %s LINE: %d\n", __FILE__, __LINE__ ) ; \
			OutputDebugStringA( buff ) ; \
			MessageBoxA( NULL, buff, "Jam Engine", MB_OK | MB_ICONWARNING) ; \
			JAM_BREAK( buff ) ; \
        } \
    } while (false)
#else
#define JAM_ASSERT_MSG(expr,  ... ) \
	do { } while (false)
#endif


#define JAM_ERROR(args, ... )	\
	do { \
		static char buff[256] ; \
		sprintf( buff, "Error issued: " ) ; \
		sprintf( buff+strlen(buff), args, ##__VA_ARGS__ ) ; \
		sprintf( buff+strlen(buff), "\nat FILE: %s LINE: %d\n", __FILE__, __LINE__ ) ; \
		OutputDebugStringA( buff ) ; \
		MessageBoxA( NULL, buff, "Jam Engine", MB_OK | MB_ICONERROR ) ; \
		JAM_BREAK( buff ) ; \
	} while(false)


#define JAM_TRACE(args, ... )	\
	do { \
		static char buff[256] ; \
		sprintf( buff, args, ##__VA_ARGS__ ) ; \
		OutputDebugStringA( buff ) ; \
	} while(false)

#endif	// __JAM_H__
