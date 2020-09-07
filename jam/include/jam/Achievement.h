/**********************************************************************************
* 
* Achievements.h
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

#ifndef __JAM_ACHIEVEMENT_H__
#define __JAM_ACHIEVEMENT_H__

#include <jam/jam.h>
#include <jam/Object.h>
#include <jam/BaseManager.hpp>
#include <jam/Event.h>

//#ifdef JAM_EXCLUDED_BLOCK

namespace jam
{

// fw references
class Timer ;

// queued
class JAM_API AchievementCompletedEventArgs : public EventArgs
{
public:
	/** Creates a new AchievementCompletedEventArgs */
	static AchievementCompletedEventArgs* create() ;

	String					getAchievementName() const { return m_achievementName; }
	void					setAchievementName(const String& val) { m_achievementName = val; }

private:
							AchievementCompletedEventArgs() = default ;
		
	String					m_achievementName ;
};


// ***
class JAM_API Achievement : public NamedTaggedObject
{
public:
	typedef Event<AchievementCompletedEventArgs>	AchievementCompletedEvent ;

							Achievement() ;

	bool					isCompleted() const ;
	void					setComplete( bool completeFlag );
	void					complete() ;
	void					reset() ;

	bool					isActive() const { return m_isActive; }
	void					setActive(bool val) { m_isActive = val; }
		
	String					getDescription() const { return m_description; }
	void					setDescription(const String& val) { m_description = val; }
	String					getTitle() const { return m_title; }
	void					setTitle(const String& val) { m_title = val; }
	int						getOrder() const { return m_order; }
	void					setOrder(int val) { m_order = val; }
	int						getReward() const { return m_reward; }
	void					setReward(int val) { m_reward = val; }
		
	virtual bool			check() ;
	virtual void			sidefx() {}

	/** Returns the event object associated to this Achievement */
	AchievementCompletedEvent&	getEvent() { return m_achievementEvent; }

private:
	bool					m_isActive ;
	bool					m_isCompleted ;
	String					m_description;
	String					m_title;
	int						m_order;
	int						m_reward ;

	AchievementCompletedEvent	m_achievementEvent ;
};


// ***
class JAM_API AchievementManager : public NamedTaggedObjectManager<Achievement>, public jam::Singleton<AchievementManager>
{
	friend class jam::Singleton<AchievementManager> ;

public:
	// implementare timer per scheduling e metodi di start/stop

	void					check( bool force = false ) ;

	void					setActiveByTag( bool activeStatus, const TagType& tag ) ;
	bool					setActiveByName( bool activeStatus, const String& name ) ;

	void					load( std::ifstream& ofs) ;
	void					save( std::ofstream& ofs ) ;
		
	virtual void			reset();

	/**
		Starts the timer

		\remark by default the timer is started when the manager is created
	*/
	void					pause() ;

	/**
		Stops the timer
	*/
	void					resume() ;

protected:
							AchievementManager() ;
	virtual					~AchievementManager() ;
	static void				setInstance( AchievementManager* pInstance ) ;

	Timer*					m_checkTimer ;
	jam::time				m_checkInterval ;
	static const jam::time	DefaultCheckinterval ;

private:
};

JAM_INLINE  AchievementManager& GetAchievementMgr() { return (AchievementManager&) AchievementManager::getSingleton(); }

}

//#endif // JAM_EXCLUDED_BLOCK

#endif // __JAM_ACHIEVEMENT_H__
