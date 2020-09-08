/**********************************************************************************
* 
* Achievement.cpp
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

#include "jam/Achievement.h"
#include "jam/Object.h"
#include "jam/Timer.h"

#include <fstream>
#include <sstream>

namespace jam
{

	Achievement::Achievement()
		: m_isCompleted(false), m_isActive(false), m_title(), m_description(), m_order(0), m_reward(0)
	{

	}

	bool Achievement::isCompleted() const
	{
		return m_isCompleted ;
	}


	void Achievement::setComplete( bool completeFlag )
	{
		m_isCompleted=completeFlag;
	}

	void Achievement::complete()
	{
		m_isCompleted = true ;
	}

	void Achievement::reset()
	{
		m_isCompleted = false ;
	}

	bool Achievement::check()
	{
		return m_isCompleted ;
	}


	// ***
	const jam::time AchievementManager::DefaultCheckinterval = 2.0f ;

	AchievementManager::AchievementManager() : NamedTaggedObjectManager<Achievement>(), m_checkTimer(0), m_checkInterval(DefaultCheckinterval)
	{
		m_checkTimer = Timer::create() ;
		m_checkTimer ->start();
	}

	void AchievementManager::check( bool force /*= false */ )
	{
		m_checkTimer->update() ;
		if( !force && m_checkTimer->getTotalElapsed() < m_checkInterval ) {
			return ;
		}

		// itera sugli achievements
		Achievement* pAch = nullptr ;
		for( auto& pAchPair : getManagerMap() ) {
			pAch = pAchPair.second ;
			if( pAch->isActive() ) {
				if( !pAch->isCompleted() && pAch->check() ) {
					pAch->complete();
					pAch->sidefx();
					AchievementCompletedEventArgs* evtArgs = AchievementCompletedEventArgs::create() ;
					evtArgs->setAchievementName( pAch->getName() ) ;
					pAch->getEvent().enqueue( evtArgs, this ) ;
				}
			}
		}

		m_checkTimer->reset() ;
	}

	void AchievementManager::setActiveByTag( bool activeStatus, const TagType& tag )
	{
		AchievementManager::RangeTags range = findObjectsByTag(tag) ;
		for( auto it = range.first; it!=range.second; it++ ) {
			Achievement* pAch = (*it).second ;
			pAch->setActive(activeStatus) ;
		}
	}

	bool AchievementManager::setActiveByName( bool activeStatus, const String& name )
	{
		Achievement* ach = getObject(name) ;
		if( ach ) {
			ach->setActive(activeStatus) ;
		}

		return ach != nullptr ;
	}

	void AchievementManager::load( std::ifstream& ifs )
	{
		Achievement* pAch = nullptr ;

		int n = 0 ;
		String name = 0 ;
		bool completeFlag = false ;

		// load data from stream
		ifs >> n ;

		// iterate on achievements
		for( size_t i=0; i<(size_t)n; i++ ) {
			ifs >> name ;
			ifs >> completeFlag ;
			pAch = (Achievement*)getObject(name) ;
			pAch->setComplete( completeFlag ) ;
		}
	}

	void AchievementManager::save( std::ofstream& ofs )
	{
		std::map<String,bool> tempList ;

		// itera sugli achievements
		Achievement* pAch = nullptr ;
		for( auto& pAchPair : getManagerMap() ) {
			pAch = pAchPair.second ;
			bool b = pAch->isCompleted() ;
			tempList[pAch->getName()] = b ;
		}

		// write achievements count
		int n = (int)tempList.size() ;
		ofs << n << std::endl ;

		// write achievements complete-flag into the stream
		for( std::map<String,bool>::iterator it = tempList.begin(); it!=tempList.end(); it++ ) {
			String achName = it->first ;
			bool completeFlag = it->second ;
			ofs << achName << std::endl ;
			ofs << completeFlag << std::endl ;
		}

		ofs.flush() ;
	}

	void AchievementManager::reset()
	{
		Achievement* pAch = nullptr ;		
		// iterate on achievements
		for( auto& pAchPair : getManagerMap() ) {
			pAch = pAchPair.second ;
			pAch->setComplete( false) ;
			pAch->setActive( false) ;
		}
	}

	void AchievementManager::pause()
	{
		m_checkTimer->pause() ;
	}

	void AchievementManager::resume()
	{
		m_checkTimer->resume() ;
	}

	AchievementCompletedEventArgs* AchievementCompletedEventArgs::create()
	{
		return new (GC) AchievementCompletedEventArgs() ;
	}

}
