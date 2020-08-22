#include <jam/Application.h>
#include <jam/Draw3dManager.h>
#include <jam/Event.h>
#include <jam/Sprite.h>
#include <jam/Gfx.h>
#include <jam/core/bmkextras.hpp>


using namespace jam;


class GameOverEventArgs : public EventArgs
{
public:
	GameOverEventArgs() = default ;
};


class ShotEventArgs : public EventArgs
{
public:
	ShotEventArgs(Sprite* hs=0) : EventArgs(), m_hitSprite(hs) {}

public:
	// e.g. the enemy ship which was hit by a bullet
	Sprite*		m_hitSprite ;
};


class TestEvents : public Application, public RefCountedObject
{
public:
	TestEvents() = default ;

	static void onGameOver(GameOverEventArgs& e, IEventSource& s) ;
	void onCollide(ShotEventArgs& e, IEventSource& s) ;

protected:
	virtual bool init() ;
	virtual void beforeSceneUpdate() ;

private:
	Event<GameOverEventArgs> gameOverEvent ;
	Event<ShotEventArgs> shotEvent ;
};


void TestEvents::onGameOver(GameOverEventArgs& e, IEventSource& s) 
{
	JAM_TRACE("*** Raised GameOver event\n") ;
}


void TestEvents::onCollide(ShotEventArgs& e, IEventSource& s)
{
	ShotEventArgs& evt = dynamic_cast<ShotEventArgs&>(e) ;
	JAM_TRACE("*** Raised ShotEvent, id of hit enemy ship: %d\n", evt.m_hitSprite->getId()) ;
}


bool TestEvents::init()
{
	Draw3DManager::Origin3D(320,480) ;
	GetGfx().setClearColor( Color(64,64,64,255) ) ;

	gameOverEvent += EventHandler<GameOverEventArgs>::create<&TestEvents::onGameOver>() ;
	shotEvent += EventHandler<ShotEventArgs>::create<TestEvents, &TestEvents::onCollide>(this) ;

	return true ;
}


void TestEvents::beforeSceneUpdate()
{
	JAM_TRACE("Entering beforeSceneUpdate...\n") ;

	EventArgs* evtArgs = 0 ;

	int k = RAND(15) ;
	if(k>0 && k<5) {
		evtArgs = new GameOverEventArgs() ;
		evtArgs->autorelease() ;
		gameOverEvent.enqueue(evtArgs,this) ;
	}
	else if(k>=5 && k<10) {
		Sprite s ;
		evtArgs = new ShotEventArgs(&s) ;
		shotEvent.fire(evtArgs,this) ;
		evtArgs->release() ;
	}

	JAM_TRACE("Exited beforeSceneUpdate.\n") ;
}


//*************************************************************************

int main() 
{
	jam::runEngine<TestEvents>() ;
	return 0 ;
}
