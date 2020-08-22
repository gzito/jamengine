/**********************************************************************************
* 
* ActionEase.h
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

#ifndef __JAM_ACTIONEASE_H__
#define __JAM_ACTIONEASE_H__

#include <jam/jam.h>
#include <jam/ActionInterval.h>

namespace jam
{

/** 
 @brief Base class for Easing actions

 Ease actions are special composition actions that alter the time of the inner action. In the Flash world they are often called Tweening or Easing actions. 
 These actions modify the speed of the inner action, but they don't modify the total running time. If the inner action lasts 5 seconds, then the total will continue to be 5 seconds. 
 The Ease actions alter the linearity of the time. 
*/
class JAM_API ActionEase : public ActionInterval
{
public:
	virtual ~ActionEase();

	/** initializes the action */
    bool initWithAction(ActionInterval *pAction);

	virtual void startWithTarget(Node *pTarget);
	virtual void stop(void);
	virtual void update(jam::time time);
	virtual ActionInterval* reverse(void);

public:
	/** creates the action */
	static ActionEase* actionWithAction(ActionInterval *pAction);

protected:
	ActionEase();

	ActionInterval *m_pOther;
};

/** 
 @brief Base class for Easing actions with rate parameters
 */
class JAM_API EaseRateAction : public ActionEase
{
public:
	/** set rate value for the actions */
	JAM_INLINE void setRate(float rate) { m_fRate = rate; }
	/** get rate value for the actions */
	JAM_INLINE float getRate(void) { return m_fRate; }

	/** Initializes the action with the inner action and the rate parameter */
	bool initWithAction(ActionInterval *pAction, float fRate);

	virtual ActionInterval* reverse(void);

public:
	/** Creates the action with the inner action and the rate parameter */
	static EaseRateAction* actionWithAction(ActionInterval* pAction, float fRate);

protected:
	EaseRateAction();

	float m_fRate;
};

/** 
 @brief EaseIn action with a rate
 */
class JAM_API EaseIn : public EaseRateAction
{
public:
	virtual void update(jam::time time);

public:
	/** Creates the action with the inner action and the rate parameter */
	static EaseIn* actionWithAction(ActionInterval* pAction, float fRate);

protected:
	EaseIn();
};

/** 
 @brief EaseOut action with a rate
 */
class JAM_API EaseOut : public EaseRateAction
{
public:
	virtual void update(jam::time time);

public:
	/** Creates the action with the inner action and the rate parameter */
    static EaseOut* actionWithAction(ActionInterval* pAction, float fRate);

protected:
	EaseOut();
};

/** 
 @brief EaseInOut action with a rate
 */
class JAM_API EaseInOut : public EaseRateAction
{
public:
	virtual void update(jam::time time);
	virtual ActionInterval* reverse(void);

public:
	/** Creates the action with the inner action and the rate parameter */
	static EaseInOut* actionWithAction(ActionInterval* pAction, float fRate);

protected:
	EaseInOut();

};

/** 
 @brief Ease Exponential In
 */
class JAM_API EaseExponentialIn : public ActionEase
{
public:
	virtual void update(jam::time time);
	virtual ActionInterval* reverse(void);

public:
	/** creates the action */
	static EaseExponentialIn* actionWithAction(ActionInterval* pAction);

protected:
	EaseExponentialIn();
};

/** 
 @brief Ease Exponential Out
 */
class JAM_API EaseExponentialOut : public ActionEase
{
public:
	virtual void update(jam::time time);
	virtual ActionInterval* reverse(void);

public:
	/** creates the action */
	static EaseExponentialOut* actionWithAction(ActionInterval* pAction);


protected:
	EaseExponentialOut();
};

/** 
 @brief Ease Exponential InOut
 */
class JAM_API EaseExponentialInOut : public ActionEase
{
public:
	virtual void update(jam::time time);

public:
	/** creates the action */
	static EaseExponentialInOut* actionWithAction(ActionInterval* pAction);

protected:
	EaseExponentialInOut();
};

/** 
 @brief Ease Elastic abstract class
 */
class JAM_API EaseElastic : public ActionEase
{
public:
	/** get period of the wave in radians. default is 0.3 */
	JAM_INLINE float getPeriod(void) { return m_fPeriod; }
	/** set period of the wave in radians. */
	JAM_INLINE void setPeriod(float fPeriod) { m_fPeriod = fPeriod; }

	/** Initializes the action with the inner action and the period in radians (default is 0.3) */
	bool initWithAction(ActionInterval *pAction, float fPeriod);
	/** initializes the action */
	bool initWithAction(ActionInterval *pAction);

	virtual ActionInterval* reverse(void);

public:
	/** creates the action */
	static EaseElastic* actionWithAction(ActionInterval *pAction);
	/** Creates the action with the inner action and the period in radians (default is 0.3) */
	static EaseElastic* actionWithAction(ActionInterval *pAction, float fPeriod);


protected:
	EaseElastic();

protected:
	float m_fPeriod;
};

/** 
 @brief Ease Elastic In action.
 @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
*/
class JAM_API EaseElasticIn : public EaseElastic
{
public:
	virtual void update(jam::time time);
	virtual ActionInterval* reverse(void);

public:
	/** creates the action */
	static EaseElasticIn* actionWithAction(ActionInterval *pAction);
	/** Creates the action with the inner action and the period in radians (default is 0.3) */
	static EaseElasticIn* actionWithAction(ActionInterval *pAction, float fPeriod);

protected:
	EaseElasticIn();
};

/** 
 @brief Ease Elastic Out action.
 @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 */
class JAM_API EaseElasticOut : public EaseElastic
{
public:
	virtual void update(jam::time time);
	virtual ActionInterval* reverse(void);

public:
	/** creates the action */
	static EaseElasticOut* actionWithAction(ActionInterval *pAction);
	/** Creates the action with the inner action and the period in radians (default is 0.3) */
	static EaseElasticOut* actionWithAction(ActionInterval *pAction, float fPeriod);

protected:
	EaseElasticOut();
};

/** 
 @brief Ease Elastic InOut action.
 @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 */
class JAM_API EaseElasticInOut : public EaseElastic
{
public:
	virtual void update(jam::time time);
	virtual ActionInterval* reverse(void);

public:
	/** creates the action */
	static EaseElasticInOut* actionWithAction(ActionInterval *pAction);
	/** Creates the action with the inner action and the period in radians (default is 0.3) */
	static EaseElasticInOut* actionWithAction(ActionInterval *pAction, float fPeriod);

protected:
	EaseElasticInOut();
};

/** 
 @brief EaseBounce abstract class.
*/
class JAM_API EaseBounce : public ActionEase
{
public:
	jam::time bounceTime(jam::time time);

public:
	/** creates the action */
	static EaseBounce* actionWithAction(ActionInterval* pAction);

protected:
	EaseBounce();
};

/** 
 @brief EaseBounceIn action.
 @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
*/
class JAM_API EaseBounceIn : public EaseBounce
{
public:
	virtual void update(jam::time time);
	virtual ActionInterval* reverse(void);

public:
	/** creates the action */
	static EaseBounceIn* actionWithAction(ActionInterval* pAction);

protected:
	EaseBounceIn();
};

/** 
 @brief EaseBounceOut action.
 @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 @since v0.8.2
 */
class JAM_API EaseBounceOut : public EaseBounce
{
public:
	virtual void update(jam::time time);
	virtual ActionInterval* reverse(void);

public:
	/** creates the action */
	static EaseBounceOut* actionWithAction(ActionInterval* pAction);

protected:
	EaseBounceOut();
};

/** 
 @brief EaseBounceInOut action.
 @warning This action doesn't use a bijective fucntion. Actions like Sequence might have an unexpected result when used with this action.
 */
class JAM_API EaseBounceInOut : public EaseBounce
{
public:
	virtual void update(jam::time time);

public:
	/** creates the action */
	static EaseBounceInOut* actionWithAction(ActionInterval* pAction);

protected:
	EaseBounceInOut();
};


/** 
 @brief EaseBackIn action.
 @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 */
class JAM_API EaseBackIn : public ActionEase
{
public:
	virtual void update(jam::time time);
	virtual ActionInterval* reverse(void);

public:
	/** creates the action */
	static EaseBackIn* actionWithAction(ActionInterval* pAction);

protected:
	EaseBackIn();
};

/** 
 @brief EaseBackOut action.
 @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
*/
class JAM_API EaseBackOut : public ActionEase
{
public:
	virtual void update(jam::time time);
	virtual ActionInterval* reverse(void);

public:
	/** creates the action */
	static EaseBackOut* actionWithAction(ActionInterval* pAction);

protected:
	EaseBackOut();
};

/** 
 @brief EaseBackInOut action.
 @warning This action doesn't use a bijective function. Actions like Sequence might have an unexpected result when used with this action.
 */
class JAM_API EaseBackInOut : public ActionEase
{
public:
	virtual void update(jam::time time);

public:
	/** creates the action */
	static EaseBackInOut* actionWithAction(ActionInterval* pAction);

protected:
	EaseBackInOut();
};

}

#endif // __JAM_ACTIONEASE_H__
