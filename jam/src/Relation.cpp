/**********************************************************************************
* 
* Relation.cpp
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
#include "jam/Relation.h"

#include "jam/Node.h"


namespace jam
{

	Relation::Relation( Node* n1, Node* n2 )
	{
		JAM_ASSERT_MSG( n1->getBank() != 0, "Node hasn't bank" ) ;
		JAM_ASSERT_MSG( n2->getBank() != 0, "Node hasn't bank" ) ;

		m_source = std::make_pair(n1->getId(),(Bank<Node>*)n1->getBank()) ;
		m_target = std::make_pair(n2->getId(),(Bank<Node>*)n2->getBank()) ;
	}

	Relation::~Relation()
	{
	}

	void Relation::removePeer()
	{
		int targetId = m_target.first ;
		Bank<Node>* b = m_target.second ;

		Node* target = b->findPtrById(targetId) ;
		if( target ) {
			RelationsList& relations = target->getRelations() ;
			for( RelationsList::iterator it = relations.begin(); it!=relations.end(); it++) {
				Relation* r = *it ;
				if( r->getTarget().first == m_source.first && r->getTarget().second == m_source.second ) {
					relations.erase(it);
					r->removeFromBank() ;
					break ;
				}
			}
		}
	}

	Node* Relation::getNodeTarget()
	{					
			std::vector<Node*> res;

			RelationMember m=getTarget();
			Bank<Node>* b=m.second;
			int id=m.first;
			
			Node* n=&(b->getById(id));
			return n;
	}	
				
}
