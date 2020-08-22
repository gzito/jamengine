/**********************************************************************************
* 
* GameManager.cpp
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

#include "jam/GameManager.h"

#include <iostream>
#include <fstream>

using namespace std;

namespace jam
{
	namespace game
	{

		void GameGlobals::newPlayer( uint32_t slot )
		{

		}

		Player::Player()
		{
			init();
		}

		Player::~Player()
		{

		}

		void Player::load( uint32_t slot )
		{
		
				
		}

		void Player::save( uint32_t slot )
		{

			
		}

		void Player::init()
		{
			m_name ="JAM";
			m_slot =-1;
			m_filename="";
			reset();
		}

		void Player::reset()
		{		
			m_score =0;
			m_level =0;
			m_round =0;		
			m_slot=-1;
		}

		void Player::New( String _name, int _slot )
		{
			reset();
		}

		void PlayerStats::load( uint32_t slot )
		{

		}

		void PlayerStats::save( uint32_t slot )
		{

		}

		void GameManager::setPlayer( jam::game::Player* val )
		{
			JAM_ASSERT_MSG( val!=0, ("Null Player class is not allowed") ) ;
			JAM_DELETE(m_player) ;
			m_player = val ;
		}

		void GameManager::setGlobals( jam::game::GameGlobals* val )
		{
			JAM_ASSERT_MSG( val!=0, ("Null GameGlobals class is not allowed") ) ;
			JAM_DELETE(m_globals) ;
			m_globals = val ;
		}

		GameManager::GameManager() : m_globals(0), m_player(0)/*, m_levelLoader(0)*/
		{
			setGlobals( new GameGlobals() ) ;
			setPlayer( new Player() ) ;
		}

		GameManager::~GameManager()
		{
			JAM_DELETE(m_globals) ;
			JAM_DELETE(m_player) ;
		}

	}
}
