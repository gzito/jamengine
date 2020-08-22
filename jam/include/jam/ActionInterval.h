/**********************************************************************************
* 
* ActionInterval.h
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

#ifndef __JAM_ACTIONINTERVAL_H__
#define __JAM_ACTIONINTERVAL_H__

#include <jam/jam.h>
#include <jam/Action.h>
#include <jam/Color.h>
#include <jam/core/geom.h>

namespace jam
{

/** 
@brief An interval action is an action that takes place within a certain period of time.
It has an start time, and a finish time. The finish time is the parameter
duration plus the start time.

These IntervalAction actions have some interesting properties, like:
- They can run normally (default)
- They can run reversed with the reverse method
- They can run with the time altered with the Accelerate, AccelDeccel and Speed actions.

For example, you can simulate a Ping Pong effect running the action normally and
then running it again in Reverse mode.

Example:

Action *pingPongAction = Sequence::actions(action, action->reverse(), NULL);
*/
class JAM_API ActionInterval : public FiniteTimeAction
{
public:
	/** how many seconds had elapsed since the actions started to run. */
	jam::time getElapsed(void) { return m_elapsed; }

	/** initializes the action */
	bool initWithDuration(jam::time d);

	/** returns true if the action has finished */
	virtual bool isDone(void);

	virtual void step(jam::time dt);
	virtual void startWithTarget(Node* pTarget);
	/** returns a reversed action */
	virtual ActionInterval* reverse(void);

public:
	/** creates the action */
	static ActionInterval* actionWithDuration(jam::time d);

public:
	//extension in CCGridAction 
	void setAmplitudeRate(float amp);
	float getAmplitudeRate(void);

protected:
	ActionInterval() {}

	jam::time m_elapsed;
	bool   m_bFirstTick;
};

/** @brief Runs actions sequentially, one after another
 */
class JAM_API Sequence : public ActionInterval
{
public:
	virtual ~Sequence();

	/** initializes the action */
    bool initOneTwo(FiniteTimeAction *pActionOne, FiniteTimeAction *pActionTwo);

	virtual void startWithTarget(Node *pTarget);
	virtual void stop();
	virtual void update(jam::time time);
	virtual ActionInterval* reverse();

public:
	/** helper constructor to create an array of sequenceable actions */
	static FiniteTimeAction* actions( FiniteTimeAction *pAction1, ...) ;

	/** creates the action */
	static Sequence* actionOneTwo(FiniteTimeAction *pActionOne, FiniteTimeAction *pActionTwo);
protected:
	Sequence() ;

	FiniteTimeAction *m_pActions[2];
	jam::time m_split;
	int m_last;
};


/** @brief Repeats an action a number of times.
 * To repeat an action forever use the RepeatForever action.
 */
class JAM_API Repeat : public ActionInterval
{
public:
	~Repeat(void);

	/** initializes a Repeat action. Times is an unsigned integer between 1 and 2^30 */
	bool initWithAction(FiniteTimeAction *pAction, unsigned int times);

	virtual void startWithTarget(Node *pTarget);
	virtual void stop(void);
	virtual void update(jam::time time);
	virtual bool isDone(void);
	virtual ActionInterval* reverse(void);

public:
	/** creates a Repeat action. Times is an unsigned integer between 1 and pow(2,30) */
	static Repeat* actionWithAction(FiniteTimeAction *pAction, unsigned int times);

protected:
	Repeat();

	unsigned int m_uTimes;
	unsigned int m_uTotal;
	FiniteTimeAction *m_pOther;
};


/** @brief Repeats an action for ever.
To repeat the an action for a limited number of times use the Repeat action.
@warning This action can't be Sequenceable because it is not an FiniteTimeAction
*/
class JAM_API RepeatForever : public ActionInterval
{
public:
	virtual ~RepeatForever();

	/** initializes the action */
	bool initWithAction(ActionInterval *pAction);
	virtual void startWithTarget(Node* pTarget);
	virtual void step(jam::time dt);
	virtual bool isDone(void);
	virtual ActionInterval* reverse(void);

public:
	/** creates the action */
	static RepeatForever* actionWithAction(ActionInterval *pAction);

protected:
	RepeatForever();

	ActionInterval *m_pOther;
};


/** @brief Spawn a new action immediately
 */
class JAM_API Spawn : public ActionInterval
{
public:
	~Spawn(void);

	/** initializes the Spawn action with the 2 actions to spawn */
	bool initOneTwo(FiniteTimeAction *pAction1, FiniteTimeAction *pAction2);

	virtual void startWithTarget(Node *pTarget);
	virtual void stop(void);
	virtual void update(jam::time time);
	virtual ActionInterval* reverse(void);

public:
	/** helper constructor to create an array of spawned actions */
	static FiniteTimeAction* actions(FiniteTimeAction *pAction1, ...);

	/** creates the Spawn action */
	static Spawn* actionOneTwo(FiniteTimeAction *pAction1, FiniteTimeAction *pAction2);

protected:
	Spawn();
	FiniteTimeAction *m_pOne;
	FiniteTimeAction *m_pTwo;
};

/** @brief Rotates a Node object to a certain angle by modifying it's
 rotation attribute.
 The direction will be decided by the shortest angle.
*/ 
class JAM_API RotateTo : public ActionInterval
{
public:
	~RotateTo() ;

	/** initializes the action */
	bool initWithDuration(jam::time duration, float fDeltaAngle);

	virtual void startWithTarget(Node *pTarget);
	virtual void stop() override;
	virtual void update(jam::time time);

public:
	/** creates the action */
	static RotateTo* actionWithDuration(jam::time duration, float fDeltaAngle);

protected:
	RotateTo();

	float m_fDstAngle;
	float m_fStartAngle;
	float m_fDiffAngle;
};

/** @brief Rotates a Node object clockwise a number of degrees by modifying it's rotation attribute.
*/
class JAM_API RotateBy : public ActionInterval
{
public:
	~RotateBy() ;

	/** initializes the action */
    bool initWithDuration(jam::time duration, float fDeltaAngle);

	virtual void startWithTarget(Node *pTarget);
	virtual void stop() override;
	virtual void update(jam::time time);
	virtual ActionInterval* reverse();

public:
	/** creates the action */
	static RotateBy* actionWithDuration(jam::time duration, float fDeltaAngle);

protected:
	RotateBy();

	float m_fAngle;
	float m_fStartAngle;
};

/** @brief Moves a Node object to the position x,y. x and y are absolute coordinates by modifying it's position attribute.
*/
class JAM_API MoveTo : public ActionInterval
{
public:
	~MoveTo() ;

	/** initializes the action */
	bool initWithDuration(jam::time duration, const Vector2& position);

	virtual void startWithTarget(Node *pTarget);
	virtual void stop() override;
	virtual void update(jam::time time);

public:
	/** creates the action */
	static MoveTo* actionWithDuration(jam::time duration, const Vector2& position);

protected:
	MoveTo();

	Vector2 m_endPosition;
	Vector2 m_startPosition;
	Vector2 m_delta;
};

/** @brief Moves a Node object x,y pixels by modifying it's position attribute.
 x and y are relative to the position of the object.
 Duration is is seconds.
*/ 
class JAM_API MoveBy : public MoveTo
{
public:
	~MoveBy() ;

	/** initializes the action */
	bool initWithDuration(jam::time duration, const Vector2& position);

	virtual void startWithTarget(Node *pTarget);
	virtual ActionInterval* reverse();

public:
	/** creates the action */
	static MoveBy* actionWithDuration(jam::time duration, const Vector2& position);

protected:
	MoveBy();
};

/** @brief Moves a Node object simulating a parabolic jump movement by modifying it's position attribute.
*/
class JAM_API JumpBy : public ActionInterval
{
public:
	~JumpBy() ;

	/** initializes the action */
	bool initWithDuration(jam::time duration, const Vector2& position, jam::time height, int jumps);

	virtual void startWithTarget(Node *pTarget);
	virtual void update(jam::time time);
	virtual ActionInterval* reverse(void);

public:
	/** creates the action */
	static JumpBy* actionWithDuration(jam::time duration, const Vector2& position, jam::time height, int jumps);

protected:
	JumpBy();

	Vector2	m_startPosition;
	Vector2	m_delta;
	jam::time  m_height;
	int			m_nJumps;
};

/** @brief Moves a Node object to a parabolic position simulating a jump movement by modifying it's position attribute.
*/ 
class JAM_API JumpTo : public JumpBy
{
public:
	~JumpTo() ;
	virtual void startWithTarget(Node *pTarget);

public:
	/** creates the action */
	static JumpTo* actionWithDuration(jam::time duration, const Vector2& position, jam::time height, int jumps);

protected:
	JumpTo();
};

/** BezierConfig configuration structure */
typedef struct _BezierConfig {
	//! end position of the bezier
	Vector2 endPosition;
	//! Bezier control point 1
	Vector2 controlPoint_1;
	//! Bezier control point 2
	Vector2 controlPoint_2;
} BezierConfig;

/** @brief An action that moves the target with a cubic Bezier curve by a certain distance.
 */
class JAM_API BezierBy : public ActionInterval
{
public:
	~BezierBy() ;

	/** initializes the action with a duration and a bezier configuration */
	bool initWithDuration(jam::time t, BezierConfig c);

	virtual void startWithTarget(Node *pTarget);
	virtual void stop() override ;
	virtual void update(jam::time time);
	virtual ActionInterval* reverse(void);

public:
	/** creates the action with a duration and a bezier configuration */
	static BezierBy* actionWithDuration(jam::time t, BezierConfig c);

protected:
	BezierBy();

	BezierConfig m_sConfig;
	Vector2 m_startPosition;
};

/** @brief An action that moves the target with a cubic Bezier curve to a destination point.
 @since v0.8.2
 */
class JAM_API BezierTo : public BezierBy
{
public:
	~BezierTo() ;

    virtual void startWithTarget(Node *pTarget);

public:
	/** creates the action with a duration and a bezier configuration */
    static BezierTo* actionWithDuration(jam::time t, BezierConfig c);

protected:
	BezierTo();
};

/** @brief Scales a Node object to a zoom factor by modifying it's scale attribute.
 @warning This action doesn't support "reverse"
 */
class JAM_API ScaleTo : public ActionInterval
{
public:
	~ScaleTo() ;

	/** initializes the action with the same scale factor for X and Y */
	bool initWithDuration(jam::time duration, float s);

	/** initializes the action with and X factor and a Y factor */
	bool initWithDuration(jam::time duration, float sx, float sy);

	virtual void startWithTarget(Node *pTarget);
	virtual void update(jam::time time);

public:
	/** creates the action with the same scale factor for X and Y */
	static ScaleTo* actionWithDuration(jam::time duration, float s);

	/** creates the action with and X factor and a Y factor */
	static ScaleTo* actionWithDuration(jam::time duration, float sx, float sy);
protected:
	ScaleTo();

	float m_fScaleX;
	float m_fScaleY;
	float m_fStartScaleX;
  	float m_fStartScaleY;
    float m_fEndScaleX;
	float m_fEndScaleY;
	float m_fDeltaX;
	float m_fDeltaY;
};

/** @brief Scales a Node object a zoom factor by modifying it's scale attribute.
*/
class JAM_API ScaleBy : public ScaleTo
{
public:
	~ScaleBy() ;

    virtual void startWithTarget(Node *pTarget);
	virtual ActionInterval* reverse(void);

public:
	/** creates the action with the same scale factor for X and Y */
	static ScaleBy* actionWithDuration(jam::time duration, float s);

	/** creates the action with and X factor and a Y factor */
	static ScaleBy* actionWithDuration(jam::time duration, float sx, float sy);

protected:
	ScaleBy();
};

/** @brief Blinks a Node object by modifying it's visible attribute
*/
class JAM_API Blink : public ActionInterval
{
public:
	~Blink() ;

	/** initializes the action */
	bool initWithDuration(jam::time duration, unsigned int uBlinks);

	virtual void update(jam::time time);
	virtual ActionInterval* reverse(void);

public:
	/** creates the action */
	static Blink* actionWithDuration(jam::time duration, unsigned int uBlinks);

protected:
	Blink();

	int m_nTimes;
};

/** @brief Fades In an object. It modifies the opacity from 0 to 255.
 The "reverse" of this action is FadeOut
 */
class JAM_API FadeIn : public ActionInterval
{
public:
	~FadeIn() ;

    virtual void update(jam::time time);
	virtual ActionInterval* reverse(void);

public:
	/** creates the action */
	static FadeIn* actionWithDuration(jam::time d);

protected:
	FadeIn();
};

/** @brief Fades Out an object. It modifies the opacity from 255 to 0.
 The "reverse" of this action is FadeIn
*/
class JAM_API FadeOut : public ActionInterval
{
public:
	~FadeOut() ;

	virtual void update(jam::time time);
	virtual ActionInterval* reverse(void);

public:
	/** creates the action */
	static FadeOut* actionWithDuration(jam::time d);

protected:
	FadeOut();
};

/** @brief Fades an object. It modifies the opacity from the current value to a custom one.
 @warning This action doesn't support "reverse"
 */
class JAM_API FadeTo : public ActionInterval
{
public:
	~FadeTo() ;

	/** initializes the action with duration and opacity */
	bool initWithDuration(jam::time duration, uint8_t opacity);

	virtual void startWithTarget(Node *pTarget);
	virtual void update(jam::time time);

public:
	/** creates an action with duration and opacity */
	static FadeTo* actionWithDuration(jam::time duration, uint8_t opacity);

protected:
	FadeTo();
	uint8_t m_toOpacity;
	uint8_t m_fromOpacity;
};

/** @brief Tints a Node from current tint to a custom one.
 @warning This action doesn't support "reverse"
 @since v0.7.2
*/
class JAM_API TintTo : public ActionInterval
{
public:
	~TintTo() ;

	/** initializes the action with duration and color */
	bool initWithDuration(jam::time duration, uint8_t red, uint8_t green, uint8_t blue);

	virtual void startWithTarget(Node *pTarget);
	virtual void update(jam::time time);

public:
	/** creates an action with duration and color */
	static TintTo* actionWithDuration(jam::time duration, uint8_t red, uint8_t green, uint8_t blue);

protected:
	TintTo();
	Color m_to;
	Color m_from;
};

/** @brief Tints a Node from current tint to a custom one.
 @since v0.7.2
 */
class JAM_API TintBy : public ActionInterval
{
public:
	~TintBy();

	/** initializes the action with duration and color */
	bool initWithDuration(jam::time duration, uint16_t deltaRed, uint16_t deltaGreen, uint16_t deltaBlue);

	virtual void startWithTarget(Node *pTarget);
	virtual void update(jam::time time);
	virtual ActionInterval* reverse(void);

public:
	/** creates an action with duration and color */
	static TintBy* actionWithDuration(jam::time duration, uint16_t deltaRed, uint16_t deltaGreen, uint16_t deltaBlue);

protected:
	TintBy();
	uint16_t m_deltaR;
	uint16_t m_deltaG;
	uint16_t m_deltaB;

	uint16_t m_fromR;
	uint16_t m_fromG;
	uint16_t m_fromB;
};

/** @brief Delays the action a certain amount of seconds
*/
class JAM_API DelayTime : public ActionInterval
{
public:
	~DelayTime();

	virtual void update(jam::time time);
	virtual ActionInterval* reverse();

public:
	/** creates the action */
	static DelayTime* actionWithDuration(jam::time d);

protected:
	DelayTime();
};

/** @brief Executes an action in reverse order, from time=duration to time=0
 
 @warning Use this action carefully. This action is not
 sequenceable. Use it as the default "reversed" method
 of your own actions, but using it outside the "reversed"
 scope is not recommended.
*/

class JAM_API ReverseTime : public ActionInterval
{
public:
	~ReverseTime(void);

	/// initializes the action
    bool initWithAction(FiniteTimeAction *pAction);

	virtual void startWithTarget(Node *pTarget);
	virtual void stop(void);
	virtual void update(jam::time time);
//	virtual IntervalAction* reverse(void);

public:
	/// creates the action
	static ReverseTime* actionWithAction(FiniteTimeAction *pAction);

protected:
	ReverseTime();
	FiniteTimeAction *m_pOther;
};

class Sprite ;
class DrawItem ;
class Animation2D ;

/** @brief Animates a sprite given the name of an Animation */
class JAM_API Animate : public ActionInterval
{
public:
	virtual ~Animate();

	/** Get animation used for the animate */
	Animation2D* getAnimation() { return m_pAnimation; }
	/** Set animation used for the animate, the object is retained */
	void setAnimation(Animation2D* pAnimation) { m_pAnimation = pAnimation; }

	/** initializes the action with an Animation and will restore the original frame when the animation is over */
    bool initWithAnimation(Animation2D* pAnimation);

	/** initializes the action with an Animation */
	bool initWithAnimation(Animation2D* pAnimation, bool bRestoreOriginalFrame);

	/** initializes an action with a duration, animation and depending of the restoreOriginalFrame, it will restore the original frame or not.
	 The 'delay' parameter of the animation will be overridden by the duration parameter.
	 @since v0.99.0
	 */
	bool initWithDuration(jam::time duration, Animation2D* pAnimation, bool bRestoreOriginalFrame);

	virtual void startWithTarget(Node *pTarget);
	virtual void stop();
	virtual void update(jam::time time);
	/*virtual IntervalAction* reverse();*/

public:
	/** creates the action with an Animation and will restore the original frame when the animation is over */
	static Animate* actionWithAnimation(Animation2D* pAnimation);

	/** creates the action with an Animation */
	static Animate* actionWithAnimation(Animation2D* pAnimation, bool bRestoreOriginalFrame);

	/** creates an action with a duration, animation and depending of the restoreOriginalFrame, it will restore the original frame or not.
	 The 'delay' parameter of the animation will be overridden by the duration parameter.
	 @since v0.99.0
	 */	
     static Animate* actionWithDuration(jam::time duration, Animation2D* pAnimation, bool bRestoreOriginalFrame);
protected:
	Animate();

	Animation2D*	m_pAnimation;
	DrawItem*		m_pOrigFrame;
	Sprite*			m_pSprite ;
    bool			m_bRestoreOriginalFrame;
};

} // namespace jam

#endif // __JAM_ACTIONINTERVAL_H__
