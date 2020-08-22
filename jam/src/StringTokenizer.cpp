/**********************************************************************************
* 
* StringTokenizer.cpp
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

#include "jam/jam.h"
#include "jam/StringTokenizer.h"

namespace jam
{
	/*!
	 * Constructs a string tokenizer for the specified string. If <I>delim</I> is <I>""</I> (the default)
	 * the tokenizer uses the default
	 * delimiter set, which is " \\t\\n\\r\\f": the space character, the tab character, the newline
	 * character, the carriage-return character, and the form-feed character. If <I>returnDelims</I>
	 * flags is <B>false</B> (the default) delimiter characters themselves will not be treated as tokens.
	 *
	 * \param str a string to be parsed.
	 * \param returnDelims flag indicating whether to return the delimiters as tokens.
	 * \param delim the delimiters.
	 *
	 * \note If the returnDelims flag is true, then the delimiter characters are also returned
	 * as tokens. Each delimiter is returned as a string of length one. If the flag is false (and this is the default),
	 * the delimiter characters are skipped and only serve as separators between tokens.
	 */
	StringTokenizer::StringTokenizer(const String& str, bool returnDelims /*= false*/, const String& delim /*= " \t\n\r\f"*/, bool trimTokens /*= false*/)
		:	currentPosition_(0),
			str_(str),
			maxPosition_((int)str.size()),
			delimiters_(delim),
			retTokens_(returnDelims),
			trimTokens_(trimTokens)
	{
	}


	/*!
	 * \note The current position is not advanced.
	 * \return the number of tokens remaining in the string using the current delimiter set.
	 * \sa nextToken()
	 */
	int StringTokenizer::countTokens()
	{
		int count = 0;
		int currpos = currentPosition_ ;

		while (currpos < maxPosition_ ) {
			// This is just skipDelimiters(); but it does not affect currentPosition.
			while (!retTokens_ &&
				(currpos < maxPosition_) &&
				(delimiters_.find(str_[currpos]) != String::npos) )
			{
				currpos++;
			}

			if (currpos >= maxPosition_) {
				break;
			}

			int start = currpos;
			while ( (currpos < maxPosition_) && 
				(delimiters_.find(str_[currpos]) == String::npos) )
			{
				currpos++;
			}
			if (retTokens_ && (start == currpos) &&
				(delimiters_.find(str_[currpos]) != String::npos) )
			{
				currpos++;
			}
			count++;

		}
		return count;
	}

	/*!
	 * If this method returns true, then a subsequent call to nextToken with no argument will
	 * successfully return a token.
	 * \return true if and only if there is at least one token in the string after the current position; false otherwise.
	 */
	bool StringTokenizer::hasMoreTokens()
	{
		skipDelimiters();
		return (currentPosition_ < maxPosition_);
	}


	/*!
	 * Normally this method returns the next token in this string tokenizer's string.<BR>
	 * Also if <I>delim</I> is not <I>""</I>, first, the set of characters considered to be delimiters
	 * by this StringTokenizer object is changed to be the characters in the string <I>delim</I>.<BR>
	 * Then the next token in the string after the current position is returned. The current position
	 * is advanced beyond the recognized token. The new delimiter set remains the default after this
	 * call. 
	 * \param delim the new delimiters.
	 * \return the next token from this string tokenizer.
	 * \note Throws Base::NoSuchElementException if there are no more tokens in this tokenizer's string.
 	 */
	String StringTokenizer::nextToken(const String& delim)
	{
		String token ;
		
		if( delim != "" ) {
			delimiters_ = delim ;
		}

		skipDelimiters();

		JAM_ASSERT(currentPosition_ < maxPosition_) ;

		int start = currentPosition_ ;
		while( (currentPosition_ < maxPosition_ ) && 
			(delimiters_.find(str_[currentPosition_]) == String::npos ) )
		{
			currentPosition_++;
		}
		if( retTokens_ && (start == currentPosition_) &&
			(delimiters_.find(str_[currentPosition_]) != String::npos) )
		{
			currentPosition_++;
		}

		token = str_.substr(start, currentPosition_-start);
		if( trimTokens_ ) { 
			jam::trimInplace(token);
		}
		
		return token ;
	}

	// a private function
	void StringTokenizer::skipDelimiters()
	{
		while( !retTokens_ &&
			(currentPosition_ < maxPosition_) &&
			(delimiters_.find(str_[currentPosition_]) != String::npos) )
		{
			currentPosition_++;
		}
    }

}
