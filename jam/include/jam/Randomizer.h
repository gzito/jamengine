/**********************************************************************************
* 
* Randomizer.h
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

#ifndef __JAM_RANDOMIZER__
#define __JAM_RANDOMIZER__

#include <jam/jam.h>

#include <vector>
#include <algorithm>
#include "jam/core/bmkextras.hpp"

namespace jam
{
class Randomizer : public std::vector<int>
{
public:
	Randomizer(void);

	void					addMore(int val, int weight=1);
	void					shuffle();
	int						getRandom();
	void					restore();
	void					backup();

	bool					flagDone;
	std::vector<int>		original;
};

//----------------------------------------------------------------------------
template <class T>
class RandomizerTemplate : public std::vector<T>
{
public:
	RandomizerTemplate(void);

	void					addMore(T val, int weight=1);
	void					shuffle();
	T						getRandom();
	void					restore();
	void					backup();

	bool					flagDone;
	std::vector<T>			original;
};
//----------------------------------------------------------------------------
template <class T>
RandomizerTemplate<T>::RandomizerTemplate(void): flagDone(0)
{
	// 	this->reserve(100);
	// 	original.reserve(100);
	original.clear();
}
//----------------------------------------------------------------------------
template <class T>
void RandomizerTemplate<T>::addMore( T val, int _weight )
{
	for (int t=0;t<_weight; t++)
		this->push_back(val);
}
//----------------------------------------------------------------------------

template <class T>
void RandomizerTemplate<T>::shuffle()
{
	jam::Random::shuffle(this->begin(), this->end()); // * shuffle elements *
	flagDone=true;
}
//----------------------------------------------------------------------------

template <class T>
T RandomizerTemplate<T>::getRandom()
{
	return this->at( RND(this->size() ));
}
//----------------------------------------------------------------------------
template <class T>
void RandomizerTemplate<T>::restore()
{
	this->clear();
	for (size_t i=0; i<original.size(); i++)
	{
		this->push_back(original.at(i));
	}
}
//----------------------------------------------------------------------------
template <class T>
void RandomizerTemplate<T>::backup()
{
	original.clear();
	for (size_t i=0; i<this->size(); i++)
	{
		original.push_back(this->at(i));
	}
}
//----------------------------------------------------------------------------

/*
class RandomizerStr : public std::vector<std::string>
{
public:
Randomizer(void);

void addMore(int std::string&, int weight=1);
void shuffle();
int getRandom();
void restore();
void backup();
bool flagDone;
std::vector<int> original;
};
*/
static float const			Int16_MAX100F = ( INT16_MAX ) / 100.0f;
static int const			Int16_MAX100I = ( INT16_MAX  ) / 100;
//----------------------------------------------------------------------------
JAM_INLINE bool				Chance_Luck( const int percentage ) 		{
	return ( percentage > 99 ) || ( percentage > 0 && ( RND(INT16_MAX) <= percentage * Int16_MAX100I ) );
}
//----------------------------------------------------------------------------
JAM_INLINE bool				Chance_Luck( const float percentage )		{
	return ( percentage > 99.0f ) || ( percentage > 0.0f && ( RND(INT16_MAX) <= percentage * Int16_MAX100F ) );
}
//----------------------------------------------------------------------------
// JAM_INLINE bool	Chance_Luck(float percentage) {return (percentage>99.0f) || (percentage>0.0f && 100.0f*((IwRand() / (float)INT16_MAX)) < percentage);}
// JAM_INLINE bool	Chance_Luck(int percentage) {return (percentage>99) || (percentage>0 && (RAND(100)) < percentage);}

//{return (percentage>99.0f) ? true : (percentage>0.0f) ? (100.0f*Rndf()<percentage) : false;}
int							Chance_Amount(int amount, int percentage);
//----------------------------------------------------------------------------
#ifdef JAM_DEBUG
void						_testChance();
#endif
//----------------------------------------------------------------------------
int							Chance_TrueFalse();
//----------------------------------------------------------------------------
float						Chance_Two(float val1, float val2);
//----------------------------------------------------------------------------
float						moreOrLess(float val, float perc =5.0f);
//----------------------------------------------------------------------------
} // namespace jam

#endif // __JAM_RANDOMIZER__
