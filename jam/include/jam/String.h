/**********************************************************************************
* 
* String.h
* 
* This file is part of Jam
* 
* Copyright (c) 2014-2019 Giovanni Zito, Gianluca Sclano.
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

#ifndef __JAM_STRING_H__
#define __JAM_STRING_H__

#include <jam/jam.h>
#include <string>
#include <vector>

namespace jam 
{

using String = std::string ;
using WString = std::wstring ;

using TagType = String ;

/// convert from wide string (UTF-16) to multibyte String (UTF-8)
JAM_API String				ws2s(const WString& wstr) ;

/// convert from multibyte String (UTF-8) to wide string (UTF-16)
JAM_API WString				s2ws(const String& str) ;

/// Converts bool to string
JAM_API String				btos(bool v) ;

/// Converts string to bool
JAM_API bool				stob(const String& s) ;

/// simple 32-bit hash code function for strings
JAM_API U32					hashcode(const String& s) ;

/// convert the given number into Roman notation
JAM_API String				toRoman(unsigned int value);

/// Converts hexadecimal string to int
JAM_API int					convertFromHex(const String& hex);

/// Check if the string starts with a given substring.
JAM_API bool				startsWith(const String& str,const String& what) ;

/// Check if the string ends with a given substring
JAM_API bool				endsWith(const String& str,const String& what) ;

/// Replaces indicated characters with other characters.
JAM_API int					replace(String& str,const String& strOld,const String& strNew) ;

/// Removes indicated characters from the string.
JAM_API int					remove(String& str,const String& what) ;

/// Returns a new string with leading whitespace characters trimmed.
JAM_API String				trimLeft(const String& str,const String& what = " \t\n") ;

/// Returns a new string with trailing whitespace characters trimmed.
JAM_API String				trimRight(const String& str,const String& what = " \t\n") ;

/// Returns a new string with leading and trailing whitespace characters trimmed.
JAM_API String				trim(const String& str,const String& what = " \t\n") ;

/// Returns a new string with all character in uppercase
JAM_API String				makeUpper(const String& str) ;

/// Returns a new string with all character in lowercase
JAM_API String				makeLower(const String& str) ;

/// Trims leading whitespace characters from the string.
JAM_API void				trimLeftInplace(String& str,const String& what = " \t\n") ;

/// Trims trailing whitespace characters from the string.
JAM_API void				trimRightInplace(String& str,const String& what = " \t\n") ;

/// Trims leading and trailing whitespace characters from the string. Modifies "str"
JAM_API void				trimInplace(String& str,const String& what = " \t\n") ;

/// Converts all the characters in the string to uppercase characters.
JAM_API void				makeUpperInplace(String& str) ;

/// Converts all the characters in the string to lowercase characters.
JAM_API void				makeLowerInplace(String& str) ;

}	// namespace jam 

#endif
