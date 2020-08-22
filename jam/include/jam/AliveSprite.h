/**********************************************************************************
* 
* AliveSprite.h
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

#ifndef __JAM_ALIVESPRITE_H__
#define __JAM_ALIVESPRITE_H__

#include <jam/jam.h>
#include <jam/Application.h>
#include <jam/Draw3dManager.h>
#include <jam/SpriteManager.h>
#include <jam/Node.h>
#include <jam/Singleton.h>

#define SCENE_PARENT		0

namespace jam
{
	// ************************************************************************************
	/**
	* Manages NodeState instances
	*/
#define CHECKLIFE

class NodeState : public String
{	
};

typedef std::map<NodeState, bool> NodeMap ;

class NodeStateManager : public jam::Singleton<NodeStateManager>
{
	friend class jam::Singleton<NodeStateManager> ;
public:		
	void attach(const String& nodeFrom, const String& nodeTo);
	void detach(const String& nodeFrom, const String& nodeTo);
	void clearAll();
	bool check(const String& nodeFrom, const String& nodeTo);
		
private:

	std::map<NodeState, NodeMap > m_nodeLinks;
	NodeStateManager() {}		
	virtual ~NodeStateManager() {} 
};
JAM_INLINE NodeStateManager& GetNodeStateMgr() { return NodeStateManager::getSingleton(); }
// ************************************************************************************

/**
* The class represent a sprite with basic lifecycle support
*/
class JAM_API AliveSprite : public jam::Sprite
{
public:
	enum LifeCycleStatus {
		INITIALIZED=-10,
		DISABLED=-8,
		CREATED=-9,
		IDLE=-1,
		WAITING=0,
		APPEARED=1,
		READY=2,
		WORKING=10,
		FALLING = 11,
		COLLIDED=18,
		HITTED=20,
		ARRIVED=30,
		THROWING=31,
		DESTROYED=99
	} ;

							AliveSprite();
							AliveSprite(DrawItem* item);
							AliveSprite(const String& frameName);

	virtual					~AliveSprite();

	bool					getAutodestroy() const { return m_autodestroy; }
	void					setAutodestroy(bool val) { m_autodestroy = val; }
	LifeCycleStatus			getLifeCycleStatus() const { return (LifeCycleStatus)getStatus(); }
	String					getLifeCycleStatusName() {	return retrieveName(getLifeCycleStatus());	}
	void					setLifeCycleStatus(LifeCycleStatus val) ;
	LifeCycleStatus			getOldStatus() const { return m_oldStatus; }
	void					setOldStatus(LifeCycleStatus val) {if (m_oldStatus!=val) m_oldStatus = val; }
	String					getOldLifeCycleStatusName() {	return retrieveName(getOldStatus());	}



	virtual void			hitted();
	virtual void			killed();
	virtual void			created();

	virtual void			show();
	virtual void			show(float x, float y, const String& frameName = "" );

	/**
	Returns the side the sprite is facing to (negative value for left or positive for right)
	*/
	int						getDirection() const { return m_direction; }

	/**
	Sets the side the sprite is facing to (left or right), this is used to flip around x-axis
	\remark Use a negative value to flip around x-axis
	*/
	void					setDirection(int val) { m_direction = val; }

	/** Controls sprite orientation */
	// GZ todo : to be moved in Node class

	float					facingToPos( const Vector2& vdest );
	float					facingToWorldPos( const Vector2& vdest );

	float					facingTo( const Node& sp2) {return facingToPos(sp2.getPos());}		
	float					facingToWorld(const Node& sp2) {return facingToWorldPos(sp2.getWorldPos());}

	/** Calls base class update, then updates the status */
	virtual void			update() ;

	Vector2				getOldPos() const { return m_oldPos; }	
	jam::Animation2D*		animate(const String& animName, int startframe=0, bool followdirection=true, bool looping=true);
	String					retrieveName( LifeCycleStatus status );
protected:
	virtual void			render();

	Vector2				m_oldPos ;
	int						m_direction;
	LifeCycleStatus			m_oldStatus;
	bool					m_autodestroy;
		
private:
	void					init() ;	



};



/**
* AliveSprite Factory interface
*/
class IAliveSpriteFactory 
{
public:
	virtual					~IAliveSpriteFactory() ;
	virtual AliveSprite*	create()=0;
};


/**
* Concrete aliveSprite factory
*/
class AliveSpriteFactory : public IAliveSpriteFactory
{
public:
	virtual AliveSprite*	create();

};


/**
* Manages AliveSprite instances
*/
class AliveSpriteFactoryManager : public jam::Singleton<AliveSpriteFactoryManager>, public jam::SpriteManager
{
	friend class jam::Singleton<AliveSpriteFactoryManager> ;
public:
	void					subscribe(const TagType& tag, IAliveSpriteFactory* factory);

	std::vector<AliveSprite*> createSome(const TagType& tag, int howmany=1, Node* parent=SCENE_PARENT);
	AliveSprite*			create(const TagType& tag, int zOrder=0,  Node* parent=SCENE_PARENT);

	virtual void			update();

	void					destroyAll();

	IAliveSpriteFactory*	getActualFactory(const TagType& tag) { 	return m_FactoryMap[tag];}

	int						garbageDestroyed(const TagType& tag);

protected:
	std::map<TagType, IAliveSpriteFactory*> m_FactoryMap;

private:
							AliveSpriteFactoryManager();		
	virtual					~AliveSpriteFactoryManager(); /* delete delle factory*/	// *** gestire errore

};

JAM_INLINE AliveSpriteFactoryManager& GetAliveSpriteFactoryMgr() { return AliveSpriteFactoryManager::getSingleton(); }

}



/*
Come lavorare con le Creature:
------------------------------

PRIMA:
1) Usare i bank per immagazzinare i frames (sia per id che per nome) nei vari drawItemManager (loadXmlSheet)
2) Creare le animazioni con gli id o i nomi emetterle in GetAnim2DMgr()
3) Creare SpriteManager per ogni tipo di creatura
4) Creare vari sprite del tipo di creatura e aggiungerli allo sprite manager


ORA:
1) Usare i bank per immagazzinare i frames (sia per id che per nome) nei vari drawItemManager (loadXmlSheet)
2) Creare le animazioni con gli id o i nomi emetterle in GetAnim2DMgr()

3) Scrivere le classi implementazione di creature e creatureFactory se necessario
4) Sottoscrivere la creatureFactory al CreatureFactoryManager che viene inserita nella mappa
5) Far instanziare creature al CreatureFactoryManager con create(tag, howmany)	(il CreatureFactoryManager è singleton che contiene, crea...e distrugge)

//++sbagliato:
//GetFactoryManager().subscribe("base_creature",creatureFactory);
//newCreature("base_creature");

GetCreatureFactoryManager().subscribe("sushi_a",sushiFactory);
GetCreatureFactoryManager().subscribe("sushi_b",sushiFactory);
GetCreatureFactoryManager().create("sushi_a",4); //che chiama newCreature("sushi_a");
GetCreatureFactoryManager().create("sushi_a",5); //che chiama newCreature("sushi_a");

// Sostanzialmente le creature sono sprite che sono riconosciute dai tag 

6) per accedere agli elementi
std::vector<jam::Creature*>	creature_a=	getCreatureManager->getByTag("sushi_a");
std::vector<jam::Creature*>	creature_a=	getCreatureManager->getByTag("sushi_b");

*/


#endif //__JAM_ALIVESPRITE_H__
