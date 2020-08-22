/**********************************************************************************
* 
* Randomizer.cpp
* 
* This file is part of Jam
* 
* Copyright (c) 2014-2019 Giovanni Zito, Gianluca Sclano
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

#include "jam/Randomizer.h"
#include "jam/String.h"

using namespace std ;

namespace jam
{

Randomizer::Randomizer( void ): flagDone( 0 )
{
	original.clear();
}
//----------------------------------------------------------------------------
void Randomizer::addMore( int val, int _weight )
{
	for ( int t = 0; t < _weight; t++ )
		push_back( val );
}
//----------------------------------------------------------------------------
void Randomizer::shuffle()
{
	jam::Random::shuffle( begin(), end() ); // * shuffle elements *
	flagDone = true;
}
//----------------------------------------------------------------------------
int Randomizer::getRandom()
{
	return at( RND(size()) );
}
//----------------------------------------------------------------------------
void Randomizer::restore()
{
	clear();
	for ( size_t i = 0; i < original.size(); i++ )
	{
		push_back( original.at( i ) );
	}
}
//----------------------------------------------------------------------------
void Randomizer::backup()
{
	original.clear();
	for ( size_t i = 0; i < size(); i++ )
	{
		original.push_back( at( i ) );
	}
}
//----------------------------------------------------------------------------
// *********************************************************************************
//http://www.gamasutra.com/blogs/RogerHgensen/20101123/6489/Chance_Probability_Luck_Percentage_calculation_for_games.php

// *********************************************************************************
//Chance of success or failure given the percentage of probability.
//if (0% chance or lower (negative modifer overload, penality), failure always.
//if (100% chance or higher (positive modifer overload, bonus), success always.
//Otherwise 1% to 99% chance (+/- modifiers etc.).
/*
bool Chance_Luck(int percentage)
{
if (percentage>99) return true;

if (percentage>0) //1% to 99% chance
{
//int range=100-percentage;
int chance=IwRandRange(100)+1 ; //Kinda clunky but needed since the number range starts at 0 rather than 1.
//if (chance<=100-percentage)	return false; //lucky chance
if (chance>percentage)	return false; //lucky chance
}
return true;
}	// (ES: 10% means...1..10=OK..11..100=NOK)
*/

/*
bool Chance_Luck(int percentage)
{
return (percentage>99) ? true : (percentage>0) ? (IwRandRange(100)<percentage) : false;

/ *if (percentage>99) return true;
else if (percentage>0) return (IwRandRange(100)<percentage);
else return false;* /
}	// (ES: 10% means...1..10=OK..11..100=NOK)
*/

//Chance of full amount given the percentage of probability.
//if (0% chance or lower (negative modifer overload, penality), amount is 0 always.
//if (100% chance or higher (positive modifer overload, bonus), amount is full always.
//Otherwise amount is 1% to 99% (+/- modifiers etc.).
int Chance_Amount( int amount, int percentage )
{
	// Protected chance.f,result.i=#False,range.i

	if ( percentage > 99 )
		return amount;
	else if ( percentage > 0 )
	{
		int range = 100 - percentage; //Get range based on percentage
		float chance = ( float ) RANGERAND( 1,range ); //Kinda clunky but needed since the number range starts at 0 rather than 1.
		chance = ( chance + percentage ) / 100.0f; //Create the modifier based on percentage
		return int( amount * chance ); //Apply modifer
	}
	return 0;
}
//----------------------------------------------------------------------------
int Chance_TrueFalse()
{
	return Chance_Luck( 50 );
}
//----------------------------------------------------------------------------
float Chance_Two( float val1, float val2 )
{
	return ( Chance_Luck( 50 ) ) ? val1 : val2;
}
//----------------------------------------------------------------------------
//#define RANGERAND( Mi, Ma)			((Mi)+ ((Ma) - (Mi)) * (IwRand() / (float)0x7fff))		[Mi,Ma)

//#define BOUNDRAND(Mi, Ma)	RANGERAND((Mi)+1, Ma+1)+1)
float moreOrLess( float val, float perc /*=5.0f*/)
{
	float delta = ( perc * val ) / 100.0f;
	float r = RANGERANDF( 0, delta );
	return val + Chance_Two( -r, r );
}
//----------------------------------------------------------------------------
#ifdef JAM_DEBUG
void _testChance()
{
	//Lucky chance test
	int n = 100000;
	int percentage = 50;
	float s = 0;
	for ( int i = 1; i < n; i++ )
	{
		s += Chance_Luck( percentage );
	}
	String strDebug = to_string( ( ( s / n ) * 100.0f ) ) + "% lucky chance average with " + to_string( percentage ) + "% probability.";
	//Amount probability test
	n = 1000;
	percentage = 90;
	strDebug = to_string( Chance_Amount( n, percentage ) ) + " of max amount " + to_string( n ) + " with " + to_string( percentage ) + "% probability.";
}
#endif
} // namespace jam