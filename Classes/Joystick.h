#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "cocos2d.h"
USING_NS_CC;

class JoystickCustomEvent: public Ref {
private:
	JoystickCustomEvent();
public:
	virtual ~JoystickCustomEvent();
	virtual bool init();
	CREATE_FUNC(JoystickCustomEvent);

public:
	static const std::string EVENT_JOYSTICK;
	double mAnagle;
	double mDistance;
};

class Joystick: public Layer {
private:
	Joystick();

protected:
	bool onTouchBegan(Touch *touch, Event *event);
	void onTouchMoved(Touch *touch, Event *event);
	void onTouchEnded(Touch *touch, Event *event);

public:
	virtual ~Joystick();
	virtual bool init();
	virtual void setJoystickPos(Vec2 pos);
	CREATE_FUNC(Joystick);

private:
	Sprite* mJoystickBg;
	Sprite* mJoystickCenter;
	Vec2 mJoystickPos = Vec2(100, 100);
};

#endif
