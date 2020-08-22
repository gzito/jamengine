/**********************************************************************************
* 
* String.cpp
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

#include "stdafx.h"

#include "jam/String.h"
#include "jam/StringTokenizer.h"

#include <cctype>
#ifdef _MSC_VER 
#include <stringapiset.h>
#endif

#include <algorithm>

namespace jam
{

// Maps a UTF-16 (wide character) string to UTF-8 (multibyte) string.
String ws2s(const WString& wstr)
{
#if defined(_MSC_VER)
    String convertedString;
    int requiredSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, 0, 0, NULL, NULL);
    if( requiredSize > 0 ) {
        std::vector<char> buffer(requiredSize);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &buffer[0], requiredSize, NULL, NULL);
        convertedString.assign(buffer.begin(), buffer.end() - 1);
    }
    return convertedString;
#else
#error "Feature currently implemented only for Visual C compiler"
#endif
}
 
// Maps a (multibyte) UTF-8 string to a UTF-16 (wide character) string. 
WString s2ws(const String& str)
{
#if defined(_MSC_VER)
    WString convertedString;
    int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
    if( requiredSize > 0 ) {
        std::vector<wchar_t> buffer(requiredSize);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], requiredSize);
        convertedString.assign(buffer.begin(), buffer.end() - 1);
    }
 
    return convertedString;
#else
#error "Feature currently implemented only for Visual C compiler"
#endif
}

String btos(bool v)
{
	char buf[256] = {0};
	sprintf(buf,"%s", (v ? "true" : "false") ) ;
	return buf ;
}

bool stob(const String& s)
{
	String locase = makeLower(s) ;
	return (locase == "true") ;
}

/**
	Simple hashcode for a string
	\return 0 if string is empty
*/
U32 hashcode(const jam::String& s)
{
	U32 h = 0;
	if( !s.empty() ) 
	{
		for (size_t i = 0; i < s.size(); i++) 
		{
			h = 31*h + s[i];
		}
	}
	return h;
}

jam::String toRoman(unsigned int value)
{
	struct romandata_t { unsigned int value; char const* numeral; };
	const struct romandata_t romandata[] =
	{
		{1000, "M"}, {900, "CM"},
		{500, "D"}, {400, "CD"},
		{100, "C"}, { 90, "XC"},
		{ 50, "L"}, { 40, "XL"},
		{ 10, "X"}, { 9, "IX"},
		{ 5, "V"}, { 4, "IV"},
		{ 1, "I"},
		{ 0, NULL} // end marker
	};

	char dest[20];
	strcpy(dest,"");

	for (const romandata_t* current = romandata; current->value > 0; ++current)
	{
		while (value >= current->value)
		{
			//result += current->numeral;
			strcat(dest,current->numeral);
			value -= current->value;
		}
	}
	jam::String res(dest);
	return res;
}

int convertFromHex(const String& hex)
{    
	int value = 0;        
	int a = 0;    
	int b = (int)hex.length() - 1;    
	for (; b >= 0; a++, b--)    
	{        
		if (hex[b] >= '0' && hex[b] <= '9')       
		{            
			value += (hex[b] - '0') * (1 << (a * 4));        
		}       
		else       
		{            
			switch (hex[b])           
			{                
			case 'A':                
			case 'a':                    
				value += 10 * (1 << (a * 4));                    
				break;                                    
			case 'B':                
			case 'b':                   
				value += 11 * (1 << (a * 4));                    
				break;                                    
			case 'C':               
			case 'c':                    
				value += 12 * (1 << (a * 4));                    
				break;                                    
			case 'D':                
			case 'd':                    
				value += 13 * (1 << (a * 4));                    
				break;                                    
			case 'E':                
			case 'e':                    
				value += 14 * (1 << (a * 4));                    
				break;                                    
			case 'F':                
			case 'f':                    
				value += 15 * (1 << (a * 4));                    
				break;                                    
			default:                    
				//std::cout << "Error, invalid char '" << hex[a] << "' in hex number" << endl;                    
				break;            
			}        
		}    
	}        
	return value;
} 

	
bool startsWith(const String& str,const String& what)
{
	bool result = false ;
	if(what.size() <= str.size()) {
		result = (str.substr(0,what.size()) == what) ;
	}
	return result ;
}


bool endsWith(const String& str,const String& what)
{
	bool result = false ;
	if(what.size() <= str.size()) {
		result = (str.substr(str.size() - what.size()) == what) ;
	}
	return result ;
}

int replace(String& str,const String& strOld,const String& strNew)
{
	int c = 0 ;
	String::size_type pos = 0,startingPos = 0 ;
	while((pos = str.find(strOld.c_str(),startingPos)) != String::npos) {
		str.erase(pos,strOld.size()) ;
		str.insert(pos,strNew) ;
		startingPos = pos + strNew.size() ;
		c++ ;
	}

	return c ;
}

int remove(String& str,const String& what)
{
	int c = 0 ;
	String::size_type pos = 0 ;
	while((pos = str.find(what.c_str())) != String::npos) {
		str.erase(pos,what.size()) ;
		c++ ;
	}

	return c ;
}

String trimLeft( const String& str,const String& what /*= " \t\n"*/ )
{
	String retVal ;

	// explanation of std::string::find_first_not_of() :
	//   Searches through a string for the first character that is not an element of a specified string.
	//   Returns the index of the first character of the substring searched for when successful; otherwise npos.
	String::size_type first_pos = str.find_first_not_of(what.c_str()) ;
	if(first_pos != String::npos) {
		retVal = str.substr(first_pos) ;
	}

	return retVal ;
}

String trimRight( const  String& str,const String& what /*= " \t\n"*/ )
{
	String retVal ;

	String::size_type first_pos= str.find_last_not_of(what.c_str()) ;
	if(first_pos != String::npos) {
		retVal = str.substr(0,first_pos+1) ;
	}

	return retVal ;
}


String trim( const String& str,const String& what /*= " \t\n"*/ )
{
	return trimLeft( trimRight(str,what), what ) ;
}


String makeUpper( const String& str )
{
	String retVal = str ;
	std::transform( str.begin(), str.end(), retVal.begin(), 
				   [](String::value_type c) -> String::value_type { return std::toupper(c); } ) ;

	return retVal ;
}

String makeLower( const String& str )
{
	String retVal = str ;
	std::transform( str.begin(), str.end(), retVal.begin(), 
				   [](String::value_type c) -> String::value_type { return std::tolower(c); } ) ;

	return retVal ;
}

JAM_API void trimLeftInplace(String& str,const String& what)
{
	String::size_type first_pos = str.find_first_not_of(what.c_str()) ;
	if(first_pos != String::npos) {
		str = str.substr(first_pos) ;
	} else {
		str = "" ;
	}
}

JAM_API void trimRightInplace(String& str,const String& what)
{
	String::size_type first_pos= str.find_last_not_of(what.c_str()) ;
	if(first_pos != String::npos) {
		str = str.substr(0,first_pos+1) ;
	} else {
		str = "" ;
	}
}

JAM_API void trimInplace(String& str,const String& what)
{
	trimLeftInplace (str, what) ;
	trimRightInplace(str, what) ;
}

JAM_API void makeUpperInplace(String& str)
{
	for( String::value_type& c : str ) {
		c = toupper(c) ;
	}
}

JAM_API void makeLowerInplace(String& str)
{
	for( String::value_type& c : str ) {
		c = tolower(c) ;
	}
}

}

