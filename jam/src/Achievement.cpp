/**********************************************************************************
* 
* Achievement.cpp
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

#include "jam/Achievement.h"
#include "jam/Object.h"
#include "jam/Timer.h"

#include <fstream>

#ifdef JAM_EXCLUDED_BLOCK

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

	AchievementManager::AchievementManager() : BaseManager<Achievement>(), m_checkTimer(0), m_checkInterval(DefaultCheckinterval)
	{
		m_checkTimer.reset( Timer::create() ) ;
//		m_checkTimer ->setReserved() ;
		m_checkTimer ->start();
	}


	AchievementManager::~AchievementManager()
	{
	}

	void AchievementManager::check( bool force /*= false */ )
	{
		Achievement* pAch = 0 ;

		m_checkTimer->update() ;
		if( !force && m_checkTimer->getTotalElapsed() < m_checkInterval ) {
			return ;
		}

		// itera sugli achievements
		for( auto& pAchPair : getManagerMap() )
			Ref<Achievement> pAch = pAchPair.second ;
			if( pAch->isActive() ) {
				if( !pAch->isCompleted() && pAch->check() ) {
					pAch->complete();
					pAch->sidefx();
					Ref<AchievementCompletedEventArgs> evtArgs( AchievementCompletedEventArgs::create() ) ;
					evtArgs->setAchievementId( pAch->getId() ) ;
					pAch->getEvent().enqueue(
						dynamic_ref_cast<EventArgs,AchievementCompletedEventArgs>(evtArgs),
						dynamic_ref_cast<IEventSource,AchievementManager>(Ref<AchievementManager>(this))
					) ;

				}
			}
		}

		m_checkTimer.reset() ;
	}

	void AchievementManager::setActiveByTag( bool activeStatus, const TagType& tag )
	{
		BankItemList& list = getBankItemsByTag(tag) ;
		for( size_t i = 0; i<list.size(); i++ ) {
			((Achievement*)list[i])->setActive(activeStatus) ;
		}
	}

	bool AchievementManager::setActiveByName( bool activeStatus, const String& name )
	{
		Achievement* ach = getPtrByName(name) ;
		if( ach ) {
			ach->setActive(activeStatus) ;
		}

		return ach != 0 ;
	}

	void AchievementManager::load( std::ifstream& ifs )
	{
		Achievement* pAch = 0 ;

		int n = 0, id = 0 ;
		bool completeFlag = false ;

		// load data from stream
		ifs.read( (char*)&n, sizeof(n) ); ;

		// iterate on achievements
		for( size_t i=0; i<(size_t)n; i++ ) {
			ifs.read( (char*)&id, sizeof(id) ) ;
			ifs.read( (char*)&completeFlag, sizeof(completeFlag)) ;
			pAch = (Achievement*)getPtrById(id) ;
			pAch->setComplete( completeFlag ) ;
		}
	}

	void AchievementManager::save( std::ofstream& ofs )
	{
		Achievement* pAch = 0 ;

		std::map<int,bool> tempList ;

		// itera sugli achievements
		for( size_t i=0; i<m_objectsArray.size(); i++ ) {
			if( m_objectsArray[i] != 0 ) {
				pAch = (Achievement*)m_objectsArray[i] ;
				bool b = pAch->isCompleted() ;
				tempList[pAch->getId()] = b ;
			}
		}

		// write achievements count
		int n = (int)tempList.size() ;
		ofs.write( (char*)&n, sizeof(n) ) ;

		// write achievements complete-flag into the stream
		for( std::map<int,bool>::iterator it = tempList.begin(); it!=tempList.end(); it++ ) {
			int achId = it->first ;
			bool completeFlag = it->second ;
			ofs.write((char*)&achId, sizeof(achId)) ;
			ofs.write((char*)&completeFlag, sizeof(completeFlag)) ;
		}

		ofs.flush() ;
	}

	void AchievementManager::setInstance( AchievementManager* pInstance )
	{
		destroySingleton() ;
		m_singleton = pInstance ;
	}

	void AchievementManager::reset()
	{
		Achievement* pAch = 0 ;		
		// iterate on achievements
		for( size_t i=0; i<m_objectsArray.size(); i++ ) 
		{
			if( m_objectsArray[i] != 0 ) {
				//pAch = (Achievement*)getPtrById(i) ;
				pAch = (Achievement*)m_objectsArray[i] ;
				pAch->setComplete( false) ;
				pAch->setActive( false) ;
			}
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
		AchievementCompletedEventArgs* archEvtArgs = JAM_ALLOC(AchievementCompletedEventArgs) ;
		archEvtArgs->autorelease() ;
		return archEvtArgs ;
	}

}

#endif // JAM_EXCLUDED_BLOCK 
