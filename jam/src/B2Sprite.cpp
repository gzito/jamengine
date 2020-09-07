/**********************************************************************************
* 
* B2Sprite.cpp
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
#include "jam/B2Sprite.h"
#include "jam/Application.h"
#include "jam/core/geom.h"

#include <Box2D/Collision/Shapes/b2PolygonShape.h>
#include <Box2D/Dynamics/b2Fixture.h>

namespace jam
{

B2Sprite::B2Sprite() :
	Sprite(), m_pBody(0)
{
}

B2Sprite::B2Sprite( DrawItem* frame ) :
	Sprite(frame), m_pBody(0)
{
}

B2Sprite::~B2Sprite()
{
#ifdef JAM_PHYSIC_ENABLED
if( m_pBody ) {
		GetAppMgr().getB2World()->DestroyBody(m_pBody) ;
		m_pBody = 0 ;
	}
#endif
}

void B2Sprite::setFrame( DrawItem* frame )
{
	// GZ : should adjust shape here
	Sprite::setFrame(frame) ;	
}

void B2Sprite::update()
{
	Sprite::update() ;

#ifdef JAM_PHYSIC_ENABLED
	if( m_pBody ) {
		if( m_pBody->IsActive() && m_pBody->IsAwake() ) {
			// update sprite properties from physic attributes
			const b2Vec2 pos = m_pBody->GetPosition() ;
			float ptmRatio = GetAppMgr().getPtmRatio() ;
			setWorldPos( pos.x * ptmRatio, pos.y * ptmRatio ) ;
			float orientation = m_pBody->GetAngle() ;
			setWorldRotationAngle( -ToDegree(orientation) ) ;
		}
		else {
			// update physic properties from sprite attributes
			setTransform( getWorldPos(), getWorldRotationAngle() ) ;
		}
	}
#endif
}

void B2Sprite::createBody( b2BodyDef& bodyDef )
{
	#ifdef JAM_PHYSIC_ENABLED
bodyDef.userData = this ;
	m_pBody = GetAppMgr().getB2World()->CreateBody(&bodyDef) ;
#endif
}

b2Fixture* B2Sprite::createFixture( b2FixtureDef& fixtureDef )
{
	return m_pBody->CreateFixture(&fixtureDef) ;
}

b2Body* B2Sprite::getBody() const
{
	return m_pBody;
}

void B2Sprite::setActive(bool activeFlag)
{
	m_pBody->SetActive(activeFlag) ;
}

bool B2Sprite::isActive() const
{
	return m_pBody->IsActive() ;
}

void B2Sprite::setAwake( bool awakeFlag )
{
	m_pBody->SetAwake(awakeFlag) ;

}

bool B2Sprite::isAwake() const
{
	return m_pBody->IsAwake() ;
}

void B2Sprite::setTransform( const Vector2& pos, float angle )
{
#ifdef JAM_PHYSIC_ENABLED
	float ptmRatio = GetAppMgr().getPtmRatio() ;
	m_pBody->SetTransform( b2Vec2(pos.x/ptmRatio,pos.y/ptmRatio), -ToRadian(angle) ) ;
#endif
}

void B2Sprite::applyForce( const Vector2& force, const Vector2& point )
{
	m_pBody->ApplyForce( b2Vec2(force.x,force.y), b2Vec2(point.x,point.y), true ) ;
}

void B2Sprite::applyTorque( float torque )
{
	m_pBody->ApplyTorque( torque, true ) ;
}

void B2Sprite::applyLinearImpulse( const Vector2& impulse, const Vector2& point )
{
	m_pBody->ApplyLinearImpulse( b2Vec2(impulse.x,impulse.y), b2Vec2(point.x,point.y), true ) ;
}

void B2Sprite::applyAngularImpulse( float impulse )
{
	m_pBody->ApplyAngularImpulse( impulse, true ) ;
}

Vector2 B2Sprite::getWorldCenter() const
{
	return Vector2( m_pBody->GetWorldCenter().x, m_pBody->GetWorldCenter().y ) ;
}

}
