/**********************************************************************************
* 
* GameManager.h
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

#ifndef __JAM_GAME_H__
#define __JAM_GAME_H__

#include <jam/jam.h>
#include <jam/Singleton.h>
#include <jam/String.h>
#include <jam/Object.h>
#include <jam/BaseManager.hpp>

namespace jam
{
namespace game
{

class GameGlobals
{
public:
	// null implemented, to be overridden
	void					newPlayer(uint32_t slot) ;

private:
	uint32_t				m_slot ;
	uint32_t				m_level ;
	uint32_t				m_round ;
};


class PlayerStats
{
public:
	// null implemented, to be overridden
	virtual void			load(uint32_t slot) ;
	// null implemented, to be overridden
	virtual void			save(uint32_t slot) ;

	uint32_t				m_bestScore ;
	float					m_timePlayed ;		// in seconds
};


// save slot
/*typedef struct PLAYERSAVESLOT
{
	uint32_t			_score ;
	uint32_t			_level	;
	uint32_t			_round ;
	String		_name;

} PlayerSaveSlot;
*/


class Player
{
public:
							Player();
	virtual					~Player();

	virtual void			init();
	virtual void			reset();

	// null implemented, to be overridden
	virtual void			load(uint32_t slot) ;
	// null implemented, to be overridden
	virtual void			save(uint32_t slot) ;
	
	// default implementation calls reset()
	virtual void			New( String _name, int _slot );

	void					setScore(uint32_t val) { m_score = val; }
	virtual void			addScore(uint32_t value) { m_score+=value; }
	virtual void			resetScore(uint32_t value) { m_score=0; }

	String					m_name ;
	String					m_filename ;
	uint32_t				m_score ;
	
	uint32_t				m_level	;
	uint32_t				m_round ;
	uint32_t				m_slot ;

	PlayerStats				m_stats ;

};

/*
class ILevelLoader
{
public:
};
*/

/** Abstract */
class Objective : public NamedObject
{
public:
	virtual bool			isEarned() const { return false; }
	virtual void			show() {}

	bool					m_earned ;
	int32_t					m_status ;
};


class JAM_API ObjectiveManager : public NamedObjectManager<Objective>, public jam::Singleton<ObjectiveManager>
{
	friend class jam::Singleton<ObjectiveManager> ;

public:
	void					updateCheck() {}
	void					load(uint32_t slot) {}
	void					save(uint32_t slot) {}


protected:
							ObjectiveManager() = default ;		
private:
};

JAM_INLINE  jam::game::ObjectiveManager& GetObjectiveMgr() { return (ObjectiveManager&) ObjectiveManager::getSingleton(); }


class GameManager : public jam::Singleton<GameManager>
{
	friend class jam::Singleton<GameManager> ;

public:
	void					setGlobals( jam::game::GameGlobals* val) ;
	jam::game::GameGlobals&	getGlobals() { return *m_globals; }

	void					setPlayer( jam::game::Player* val) ;
	jam::game::Player&		getPlayer() { return *m_player; }
	

private:
	GameManager() ;
	virtual ~GameManager() ;

	jam::game::GameGlobals*	m_globals ;
	jam::game::Player*		m_player ;
	
	
	//	jam::game::ILevelLoader*	m_levelLoader ;
};

/** Returns the singleton instance */
JAM_INLINE  jam::game::GameManager& GetGameMgr() { return (GameManager&) GameManager::getSingleton(); }

}
}

#endif // __JAM_GAME_H__
