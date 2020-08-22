/**********************************************************************************
* 
* filesystem.h
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

#ifndef __JAM_FILESYSTEM_H__
#define __JAM_FILESYSTEM_H__

#include <jam/jam.h>
#include <jam/String.h>

// for gcc compiler they're already defined as POSIX standard
#ifdef _MSC_VER
	#if !defined(S_ISREG)
		#define S_ISREG(m)		(((m) & _S_IFREG) != 0)
	#endif
	#if !defined(S_ISDIR)
		#define S_ISDIR(m)		(((m) & _S_IFDIR) != 0)
	#endif
#endif

#if defined( WIN32 ) || defined(WIN64)
#define SYSTEM_DIRECTORY_SEPARATOR_CHAR		'\\'
#else
#define SYSTEM_DIRECTORY_SEPARATOR_CHAR		'/'
#endif

namespace jam
{

JAM_API bool				wildcardMatch(const char *pat, const char *str) ;

/// Returns the given filename's extension
JAM_API String				getFileNameExtension(const String& pathName) ;
JAM_API void				getFileNameExtension(const String& pathName, char* ext) ;

/// Returns the given filename without its extension
JAM_API String				getFileNameWithoutExtension(const String& pathName) ;

/// Returns the filename without its path
JAM_API String				getBasename(const String& pathName) ;

/// Returns the path of the given argument (without filename)
JAM_API String				getDirname(const String& pathName) ;

/// Returns the passed pathName in a system depended form.
JAM_API String				normalizePathName(const String& pathName) ;

/// concatenates the two given paths
JAM_API String				appendPath(const String& path, const String& more) ;

JAM_API int64_t				getFileSize(const char* filename ) ;

JAM_API String				getCurrentDirectory() ;

JAM_API bool				exists( const String& filename ) ;

JAM_API bool				isDirectory( const String& filename ) ;

JAM_API bool				isRegularFile( const String& filename ) ;

}

#endif	// __JAM_FILESYSTEM_H__
