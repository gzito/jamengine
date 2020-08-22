/**********************************************************************************
* 
* ActionManager.h
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

#ifndef __JAM_ACTIONMANAGER_H__
#define __JAM_ACTIONMANAGER_H__


#include <jam/jam.h>
#include <jam/Bank.h>
#include <jam/Action.h>
#include <jam/Node.h>
#include <jam/Singleton.h>

#include <vector>
#include <list>
#include <map>

namespace jam
{

// forward reference, struct declared in implementation file
struct ActionMgrMapElement ;

typedef std::vector<Action*>					ActionsList ;
typedef std::map<Node*,ActionMgrMapElement*>	TargetsMap ;

/**
	Internal class used to manage Action instances
*/
class JAM_API ActionManager : public Singleton<ActionManager>, public Bank<Action>
{
	friend class Singleton<ActionManager> ;

public:
	/** Adds an action with a target. 
		If the target is already present, then the action will be added to the existing target.
		If the target is not present, a new instance of this target will be created either paused or not, and the action will be added to the newly created target.
		When the target is paused, the queued actions won't be 'ticked'.
		*/
	void addAction(Action* pAction, Node* pTarget, bool paused);

	/** Removes all actions from all the targets.
	*/
	void removeAllActions();

	/** Removes all actions from a certain target.
		All the actions that belongs to the target will be removed.
		*/
	void removeAllActionsFromTarget(Node* pTarget, bool eraseTargetElement = true);

	/** Removes an action given an action reference.
	*/
	void removeAction(Action* pAction);

	/** Removes an action given its name and the target */
	void removeActionByName(const String& name, Node* pTarget);

	/** Gets an action given its name an a target
		@return the Action the with the given name
		*/
	Action* getActionByName(const String& name, Node* pTarget);

	/** Returns the numbers of actions that are running in a certain target. 
		* Composable actions are counted as 1 action. Example:
		* - If you are running 1 Sequence of 7 actions, it will return 1.
		* - If you are running 7 Sequences of 2 actions, it will return 7.
		*/
	int getNumberOfRunningActionsInTarget(Node* pTarget);

	/** Pauses the target: all running actions and newly added actions will be paused.
	*/
	void pauseTarget(Node* pTarget);

	/** Resumes the target. All queued actions will be resumed.
	*/
	void resumeTarget(Node* pTarget);

	void update(jam::time dt);
	
protected:

private:
	ActionManager() ;
	virtual ~ActionManager() ;
	void removeActionAt(ActionsList::iterator it, ActionMgrMapElement *pElement) ;

	TargetsMap				m_targets ;
	ActionMgrMapElement*	m_pCurrentTarget ;
	bool					m_bCurrentTargetSalvaged ;
};

/** Returns the singleton instance */
JAM_INLINE ActionManager& GetActionMgr() { return ActionManager::getSingleton(); }

}

#endif // __JAM_ACTIONMANAGER_H__
