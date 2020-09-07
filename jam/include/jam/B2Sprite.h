/**********************************************************************************
* 
* B2Sprite.h
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

#ifndef __JAM_B2Sprite_H__
#define __JAM_B2Sprite_H__

#include <jam/jam.h>
#include <jam/Sprite.h>
#include <jam/core/geom.h>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>


namespace jam
{
	
/**
	A sprite with physics support
*/
class JAM_API B2Sprite : public Sprite
{
public:
							B2Sprite();
							B2Sprite( DrawItem* frame ) ;
	virtual					~B2Sprite() ;


	// Body definition
	//
	// Body types:
	//	b2_staticBody - A static body has does not move under simulation and behaves as if it has infinite mass.
	//	Internally, Box2D stores zero for the mass and the inverse mass. Static bodies can be moved manually by the user.
	//	A static body has zero velocity. Static bodies do not collide with other static or kinematic bodies.
	//
	//	b2_kinematicBody- A kinematic body moves under simulation according to its velocity. Kinematic bodies do not respond to forces.
	//	They can be moved manually by the user, but normally a kinematic body is moved by setting its velocity.
	//	A kinematic body behaves as if it has infinite mass, however, Box2D stores zero for the mass and the inverse mass.
	//	Kinematic bodies do not collide with other static or kinematic bodies.
	//
	//	b2_dynamicBody - A dynamic body is fully simulated. They can be moved manually by the user, but normally they move according to forces.
	//	A dynamic body can collide with all body types. A dynamic body always has finite, non-zero mass.
	//	If you try to set the mass of a dynamic body to zero, it will automatically acquire a mass of one kilogram.
	// 		bodyDef.type
	//
	// The body definition gives you the chance to initialize the position of the body on creation.
	// This has far better performance than creating the body at the world origin and then moving the body.
	// 		bodyDef.position				// the position of the body's origin (this is different from center of mass)
	// 		bodyDef.angle					// world angle in radians.
	// 		bodyDef.linearVelocity			// The linear velocity of the body's origin in world co-ordinates. 
	// 		bodyDef.angularVelocity			// The angular velocity of the body. 
	//
	// Damping is used to reduce the world velocity of bodies. Damping is different than friction because friction only occurs with contact.
	// Damping is not a replacement for friction and the two effects should be used together.
	// Damping parameters should be between 0 and infinity, with 0 meaning no damping, and infinity meaning full damping.
	// Normally you will use a damping value between 0 and 0.1. I generally do not use linear damping because it makes bodies look floaty.
	// 		bodyDef.linearDamping
	// 		bodyDef.angularDamping
	//
	// Set this flag to false if this body should never fall asleep. Note that this increases CPU usage.
	// 		bodyDef.allowSleep				
	//
	// Is this body initially awake or sleeping? 
	// 		bodyDef.awake
	//
	//You may want a rigid body, such as a character, to have a fixed rotation. Such a body should not rotate, even under load.
	// 		bodyDef.fixedRotation
	//
	// You may wish a body to be created but not participate in collision or dynamics.
	// This state is similar to sleeping except the body will not be woken by other bodies
	// and the body's fixtures will not be placed in the broad-phase.
	// This means the body will not participate in collisions, ray casts, etc.
	// You can create a body in an inactive state and later re-activate it.
	// 		bodyDef.active
	void					createBody(b2BodyDef& bodyDef) ;



	// Fixture Definition
	//
	// The shape
	//		fixtureDef.shape
	//
	// The fixture density is used to compute the mass properties of the parent body. The density can be zero or positive.
	// You should generally use similar densities for all your fixtures. This will improve stacking stability. Usually expressed in kg/m^2
	//		fixtureDef.density
	//
	// Friction is used to make objects slide along each other realistically. Box2D supports static and dynamic friction,
	// but uses the same parameter for both. Friction is simulated accurately in Box2D and the friction strength is proportional
	// to the normal force (this is called Coulomb friction). The friction parameter is usually set between 0 and 1,
	// but can be any non-negative value. A friction value of 0 turns off friction and a value of 1 makes the friction strong.
	// Usually in the range [0,1]
	//		fixtureDef.friction = 0.3f;
	//
	// Restitution is used to make objects bounce. The restitution value is usually set to be between 0 and 1.
	// Consider dropping a ball on a table. A value of zero means the ball won't bounce. This is called an inelastic collision.
	// A value of one means the ball's velocity will be exactly reflected. This is called a perfectly elastic collision.
	// Usually in the range [0,1].
	//		fixtureDef.restitution ;
	//
	// Collision filtering is a system for preventing collision between fixtures. For example, say you make a character that rides a bicycle.
	// You want the bicycle to collide with the terrain and the character to collide with the terrain, but you don't want the character to collide
	// with the bicycle (because they must overlap). Box2D supports such collision filtering using categories and groups.
	//		fixtureDef.filter
	//
	// Sometimes game logic needs to know when two fixtures overlap yet there should be no collision response.
	// This is done by using sensors. A sensor is a fixture that detects collision but does not produce a response.
	//		fixtureDef.isSensor
	b2Fixture*				createFixture(b2FixtureDef& fixtureDef) ;

	// overrides
	virtual void			setFrame(DrawItem* frame ) override ;

	// overrides
	virtual void			update() ;

	b2Body*					getBody() const;

	void					setActive(bool activeFlag) ;
	bool					isActive() const ;

	void					setAwake(bool awakeFlag) ;
	bool					isAwake() const ;

	void					setTransform(const Vector2& pos, float angle) ;

	void					applyForce( const Vector2& force, const Vector2& point ) ;
	void					applyLinearImpulse( const Vector2& impulse, const Vector2& point ) ;
	void					applyTorque( float torque ) ;
	void					applyAngularImpulse( float impulse ) ;

	Vector2					getWorldCenter() const ;

protected:
	b2Body*					m_pBody ;
};

}	

#endif // __JAM_B2Sprite_H__
