/**********************************************************************************
* 
* filesystem.cpp
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

#include "stdafx.h"

#include "jam/core/filesystem.h"

namespace jam
{

// The following function was found on http://xoomer.virgilio.it/acantato/dev/wildcard/wildmatch.html, where it was attributed to 
// the C/C++ Users Journal, written by Mike Cornelison. It is a little ugly, but it is FAST. Use this as an excercise in not reinventing the
// wheel, even if you see gotos. 
bool wildcardMatch(const char *pat, const char *str) {
	int i,star;

new_segment:

	star = 0;
	if(*pat == '*') {
		star = 1;
		do { pat++; } while(*pat == '*'); /* enddo */
	} /* endif */

test_match:

	for(i = 0; pat[i] && (pat[i] != '*'); i++) {
		//if (mapCaseTable[str[i]] != mapCaseTable[pat[i]]) {
		if(str[i] != pat[i]) {
			if(!str[i]) return 0;
			if((pat[i] == '?') && (str[i] != '.')) continue;
			if(!star) return 0;
			str++;
			goto test_match;
		}
	}
	if(pat[i] == '*') {
		str += i;
		pat += i;
		goto new_segment;
	}
	if(!str[i]) return 1;
	if(i && pat[i - 1] == '*') return 1;
	if(!star) return 0;
	str++;
	goto test_match;
}


String getFileNameExtension(const String& pathName)
{
	String ext ;
	String::size_type pos = pathName.find_last_of(".") ;
	if(pos != String::npos && pos != pathName.size()-1) {
		ext = pathName.substr(pos+1) ;
	}

	return ext ;
}

void getFileNameExtension(const String& pathName,char* ext)
{
	String::size_type pos = pathName.find_last_of(".") ;
	if(pos != String::npos && pos != pathName.size()-1) {
		strcpy( ext, pathName.substr(pos+1).c_str() ) ;
	}
	else {
		ext[0] = '\0' ;
	}
}

String getFileNameWithoutExtension(const String& pathName)
{
	String filename ;
	String::size_type pos = pathName.find_last_of(".") ;
	if(pos != String::npos) {
		filename = pathName.substr(0,pos) ;
	}

	return filename ;
}

String getBasename(const String& pathName)
{
	String result = pathName ;

	const size_t last_slash_idx = result.find_last_of("\\/");
	if (String::npos != last_slash_idx)
	{
		result.erase(0, last_slash_idx + 1);
	}

	return result.c_str() ;
}

String getDirname(const String & pathName)
{
	return pathName.substr(0, pathName.find_last_of("\\/"));
}

/*!
* Returns the passed path in a system dependent format
* Converts all the "/" to "\" on Windows
* Converts all the "\" to "/" on Unix 
*/
String normalizePathName(const String& pathName)
{
	String out = pathName ;

	unsigned char new_ds = SYSTEM_DIRECTORY_SEPARATOR_CHAR ;
	String old_ds ;
	if( new_ds == '\\' ) {
		old_ds = "/" ;
	} else {
		old_ds = "\\" ;
	}

	jam::replace( out, old_ds,String(1,new_ds) ) ;

	return out.c_str() ;
}

String appendPath(const String & path,const String & more)
{
	String p = normalizePathName(path) ;
	String f = normalizePathName(more) ;
	String sep(1,SYSTEM_DIRECTORY_SEPARATOR_CHAR) ;

	// pathAppend("","donald") -> "donald"
	if(p.empty()) {
		return f ;
	}

	// pathAppend("/duck","") -> "/duck"
	if(f.empty()) {
		return p ;
	}

	// pathAppend("donald/","duck") -> "donald/duck"
	if(endsWith(p,sep) && !startsWith(f,sep)) {
		return p+f ;
	}

	// pathAppend("donald","/duck") -> "donald/duck"
	if(!endsWith(p,sep) && startsWith(f,sep)) {
		return p+f ;
	}

	// pathAppend("donald/","/duck") -> "donald/duck"
	if(endsWith(p,sep) && startsWith(f,sep)) {
		return p + f.substr(1) ;
	}

	// pathAppend("donald","duck") -> "donald/duck"
	return p + sep + f ;
}

int64_t getFileSize(const char* filename)
{
	struct _stat64 stat_buf;
	int rc = _stat64(filename,&stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

String getCurrentDirectory()
{
	String retValue ;

#if defined(_MSC_VER)
	DWORD requiredSize = GetCurrentDirectoryW( 0, NULL ) ;
	if( requiredSize > 0 ) {
		std::vector<wchar_t> buffer(requiredSize);
		WString wRetValue ;
		GetCurrentDirectoryW( requiredSize, buffer.data() ) ;
		wRetValue.assign(buffer.begin(), buffer.end() - 1) ;
		retValue = ws2s( wRetValue ) ;
	}
#else
	static char buffer[MAX_PATH] = { 0 } ;

	memset( buffer, 0, MAX_PATH ) ;
	char* b = getcwd( buffer, MAX_PATH ) ;
	if( b != NULL ) {
		retValue = buffer ;
	}
#endif

	return retValue ;
}

bool exists( const String& filename )
{
	struct _stat64 s ;
	int r = _wstat64( s2ws(filename).c_str(), &s ) ;
	if( r == -1 ) {
		return false ;
	}
	return S_ISREG(s.st_mode) || S_ISDIR(s.st_mode) ;
}

bool isDirectory( const String& filename )
{
	struct _stat64 s ;
	int r = _wstat64( s2ws(filename).c_str(), &s ) ;
	if( r == -1 ) {
		return false ;
	}
	return S_ISDIR(s.st_mode) ;
}

bool isRegularFile( const String& filename )
{
	struct _stat64 s ;
	int r = _wstat64( s2ws(filename).c_str(), &s ) ;
	if( r == -1 ) {
		return false ;
	}
	return S_ISREG(s.st_mode) ;
}

}

