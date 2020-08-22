/**********************************************************************************
* 
* ActionManager.cpp
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
#include "jam/ActionManager.h"

using namespace std ;

namespace jam
{

struct ActionMgrMapElement
{
	ActionsList					actions;
	Node*						target;
	unsigned int				actionIndex;
	Action*						currentAction;
	bool						currentActionSalvaged;
	bool						paused;
};


ActionManager::ActionManager() :
	Bank<Action>(), m_pCurrentTarget(0), m_bCurrentTargetSalvaged(false)
{
}

ActionManager::~ActionManager()
{
	removeAllActions() ;
}

void ActionManager::addAction( Action* pAction, Node* pTarget, bool paused )
{
	assert(pAction) ;
	assert(pTarget) ;
	
	// add the action as bankitem
	add(pAction) ;
	
	ActionMgrMapElement* pElement = 0 ;
	TargetsMap::iterator it = m_targets.find(pTarget) ;
	if( it == m_targets.end() ) {
		pElement = new ActionMgrMapElement();
		pElement->actionIndex = 0 ;
		pTarget->addRef() ;
		pElement->target = pTarget ;
		pElement->currentAction = 0 ;
		pElement->currentActionSalvaged = false ;
		pElement->paused = paused ;
		m_targets[pTarget] = pElement ;
	}
	else {
		pElement = it->second ;
	}
	pElement->actions.push_back(pAction) ;
	pAction->addRef() ;

	pAction->startWithTarget(pTarget) ;
}

void ActionManager::removeAllActions()
{
	for( TargetsMap::iterator it = m_targets.begin(); it!=m_targets.end(); it++ )
	{
		removeAllActionsFromTarget(it->first, false) ;
	}
	m_targets.clear() ;
	removeAllBankItems();	// release managed actions
}

void ActionManager::removeAllActionsFromTarget( Node* pTarget, bool eraseTargetElement /*= true*/ )
{
	if( pTarget==0 ) {
		return ;
	}

	TargetsMap::iterator it = m_targets.find(pTarget) ;
	if( it != m_targets.end() ) {
		ActionMgrMapElement* pElement = it->second ;

		ActionsList::iterator currentActionIt = std::find(pElement->actions.begin(), pElement->actions.end(), pElement->currentAction) ;
		if( currentActionIt != pElement->actions.end() && !pElement->currentActionSalvaged ) {

			pElement->currentAction->addRef();
			pElement->currentActionSalvaged = true ;
		} 

		for( ActionsList::iterator vit = pElement->actions.begin(); vit != pElement->actions.end(); vit++ ) {
//			if( vit!=currentActionIt || pElement->currentActionSalvaged==false ) {
				(*vit)->release() ;
				(*vit)->removeFromBank() ;
//			}
		}
		pElement->actions.clear() ;

		if( m_pCurrentTarget == pElement ) {
			m_bCurrentTargetSalvaged = true;
		}
		else {
			pElement->target->release() ;
			JAM_DELETE(pElement) ;
			if( eraseTargetElement ) { 
				m_targets.erase(it) ;
			}
		}
	}
}

void ActionManager::removeAction( Action* pAction )
{
	if( pAction == 0 ) {
		return ;
	}

	Node* pTarget = pAction->getOriginalTarget() ;
	TargetsMap::iterator it = m_targets.find(pTarget) ;
	if( it != m_targets.end() ) {
		ActionMgrMapElement* pElement = it->second ;
		for( ActionsList::iterator vit=pElement->actions.begin(); vit != pElement->actions.end(); vit++ ) {
			if( *vit == pAction ) {
				removeActionAt(vit,pElement) ;
				break ;
			}
		}
	}
}

void ActionManager::removeActionByName( const String& name, Node* pTarget )
{
	Action* pAction = getActionByName(name,pTarget) ;
	if( pAction ) {
		removeAction(pAction) ;
	}
}

Action* ActionManager::getActionByName( const String& name, Node* pTarget )
{
	Action* action = 0 ;
	TargetsMap::iterator it = m_targets.find(pTarget) ;
	if( it != m_targets.end() ) {
		ActionsList::iterator vit = it->second->actions.begin() ;
		while(  vit != it->second->actions.end() ) {
			if( (*vit)->getName() == name ) {
				action = *vit ;
				break ;
			}
			vit++ ;
		}
	}

	return action ;
}

int ActionManager::getNumberOfRunningActionsInTarget( Node* pTarget )
{
	assert(pTarget) ;

	int numOfActions = 0 ;

	TargetsMap::iterator it = m_targets.find(pTarget) ;
	if( it != m_targets.end() ) {
		numOfActions = (int)it->second->actions.size() ;
	}
	
	return numOfActions ;
}

void ActionManager::pauseTarget( Node* pTarget )
{
	assert(pTarget) ;

	TargetsMap::iterator it = m_targets.find(pTarget) ;
	if( it != m_targets.end() ) {
		it->second->paused = true ;
	}
}

void ActionManager::resumeTarget( Node* pTarget )
{
	assert(pTarget) ;

	TargetsMap::iterator it = m_targets.find(pTarget) ;
	if( it != m_targets.end() ) {
		it->second->paused = false ;
	}
}

void ActionManager::update(jam::time dt)
{
	for( TargetsMap::iterator it = m_targets.begin(); it!=m_targets.end(); ) {
		m_pCurrentTarget = it->second ;

		if( !m_pCurrentTarget->paused ) {

			Node* node = it->first ;
			node->clearActionFlags( Node::ActionFlags::MOVING | Node::ActionFlags::ROTATING | Node::ActionFlags::ANIMATING ) ;

			for( m_pCurrentTarget->actionIndex = 0; m_pCurrentTarget->actionIndex < m_pCurrentTarget->actions.size(); m_pCurrentTarget->actionIndex++ ) {
				m_pCurrentTarget->currentAction = m_pCurrentTarget->actions[m_pCurrentTarget->actionIndex] ;
				if( m_pCurrentTarget->currentAction == 0 ) {
					continue ;
				}
					
				m_pCurrentTarget->currentActionSalvaged = false ;
				float aspeed=node->getActionSpeed();
				m_pCurrentTarget->currentAction->step(dt*aspeed) ;

				if( m_pCurrentTarget->currentActionSalvaged )
				{
					// The currentAction told the node to remove it. To prevent the action from
					// accidentally deallocating itself before finishing its step, we retained
					// it. Now that step is done, it's safe to release it.
					m_pCurrentTarget->currentAction->release();
				}
				else if(m_pCurrentTarget->currentAction->isDone())
				{
						m_pCurrentTarget->currentAction->stop() ;
						Action* pAction = m_pCurrentTarget->currentAction ;
						// Make currentAction nil to prevent removeAction from salvaging it.
						m_pCurrentTarget->currentAction = 0 ;
						removeAction(pAction) ;
				}

				m_pCurrentTarget->currentAction = 0;
			} // for( m_pCurrentTarget->actionIndex = 0; m_pCurrentTarget->actionIndex < m_pCurrentTarget->actions.size(); m_pCurrentTarget->actionIndex++ )
		} // if( !m_pCurrentTarget->paused )

		if (m_bCurrentTargetSalvaged && m_pCurrentTarget->actions.size() == 0) {
			delete it->second ;

			it->first->release() ;
			m_targets.erase(it++);
		}
		else {
			++it ;
		}
	} // for( TargetsMap::iterator it = m_targets.begin(); it!=m_targets.end(); it++ )

	m_pCurrentTarget = 0 ;	
}

void ActionManager::removeActionAt( ActionsList::iterator it, ActionMgrMapElement *pElement )
{
	Action *pAction = *it ;

	if (pAction == pElement->currentAction && (! pElement->currentActionSalvaged))
	{
		pElement->currentAction->addRef() ;
		pElement->currentActionSalvaged = true;
	}

	pAction->release() ;
	size_t uIndex = it - pElement->actions.begin() ;
	pElement->actions.erase(it);

	// remove from bank
	pAction->removeFromBank() ;

	// update actionIndex in case we are in tick. looping over the actions
	if (pElement->actionIndex >= uIndex)
	{
		pElement->actionIndex--;
	}

	if (pElement->actions.size() == 0)
	{
		if (m_pCurrentTarget == pElement)
		{
			m_bCurrentTargetSalvaged = true;
		}
		else
		{
			TargetsMap::iterator it = m_targets.find(pElement->target) ;
			if( it!= m_targets.end() ) {
				pElement->target->release() ;
				m_targets.erase(it);
			}
		}
	}
	
}

}
