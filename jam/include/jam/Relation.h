/**********************************************************************************
* 
* Relation.h
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

#ifndef __JAM_RELATIONMANAGER_H__
#define __JAM_RELATIONMANAGER_H__

#include <jam/jam.h>
#include <utility>
#include <list>

#include <jam/Singleton.h>
#include <jam/Bank.h>

namespace jam
{
class Node ;

typedef std::pair<int,Bank<Node>*>		RelationMember ;

/**
*
*
*/
class JAM_API Relation : public BankItem
{
public:
	Relation( Node* n1, Node* n2 ) ;
	virtual ~Relation() ;

	const RelationMember&	 getSource() const { return m_source; }
	const RelationMember&	 getTarget() const { return m_target; }

	void removePeer() ;
	Node* getNodeTarget();


private:
	RelationMember	m_source ;
	RelationMember	m_target ;
};

typedef std::list<Relation*>			RelationsList ;

class RelationManager : public Singleton<RelationManager>, public Bank<Relation>
{
	friend class Singleton<RelationManager> ;

private:
	RelationManager() {}
	virtual ~RelationManager() {}
};

JAM_INLINE RelationManager& GetRelationMgr() { return RelationManager::getSingleton(); }
}

#endif // __JAM_RELATIONMANAGER_H__