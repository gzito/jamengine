/**********************************************************************************
* 
* StringTokenizer.h
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

#ifndef __JAM_STRINGTOKENIZER_H__
#define __JAM_STRINGTOKENIZER_H__

#include <jam/String.h>

namespace jam
{
/*!
	* \class StringTokenizer
	* \brief Tokenize a string
	*
	* The StringTokenizer class allows an application to break a string into tokens.<BR>
	* The set of delimiters (the characters that separate tokens) may be specified either at creation
	* time or on a per-token basis.<BR>
	* An instance of StringTokenizer behaves in one of two ways, depending on whether it was created
	* with the returnDelims flag having the value true or false:<BR><BR>
	* <DL>If the flag is false, delimiter characters serve to separate tokens. A token is a maximal
	* sequence of consecutive characters that are not delimiters.<BR>
	* If the flag is true, delimiter characters are themselves considered to be tokens.</DL><BR>
	* A token is thus either one delimiter character, or a maximal sequence of consecutive
	* characters that are not delimiters.</H2><BR>
	* A StringTokenizer object internally maintains a current position within the string to be
	* tokenized. Some operations advance this current position past the characters processed.<BR>
	* A token is returned by taking a substring of the string that was used to create
	* the StringTokenizer object.
	*/
class JAM_API StringTokenizer
{
public:
	/// Constructs a string tokenizer for the specified string. 
							StringTokenizer(const String& str, bool returnDelims=false, const String& delim = " \t\n\r\f", bool trimTokens = false) ;

	/// Calculates the number of times that this tokenizer's nextToken method can be called before it generates an exception. 
	int						countTokens() ;

	/// Tests if there are more tokens available from this tokenizer's string. 
	bool					hasMoreTokens() ;

	/// Returns the next token from this string tokenizer. 
	String					nextToken(const String& delim="") ;

private:
	void					skipDelimiters() ;

private:
	int						currentPosition_ ;
	int						maxPosition_ ;
	String					str_ ;
	String					delimiters_ ;
	bool					retTokens_ ;
	bool					trimTokens_ ;
};

}

#endif

