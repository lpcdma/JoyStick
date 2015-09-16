#include "Joystick.h"

#define PI 3.1415926

const std::string JoystickCustomEvent::EVENT_JOYSTICK = "event_of_joystick";

JoystickCustomEvent::JoystickCustomEvent() {
}

JoystickCustomEvent::~JoystickCustomEvent() {
}

bool JoystickCustomEvent::init() {
	return true;
}

Joystick::Joystick() {
}

Joystick::~Joystick() {
	_eventDispatcher->removeEventListenersForTarget(this);
}

bool Joystick::init() {
	bool result = false;
	do {
		if (!Layer::init()) {
			break;
		}

		mJoystickBg = Sprite::create("joystick_bg.png");
		if (nullptr == mJoystickBg) {
			break;
		}
		mJoystickBg->setPosition(mJoystickPos);
		addChild(mJoystickBg);

		mJoystickCenter = Sprite::create("joystick_center.png");
		if (nullptr == mJoystickCenter) {
			break;
		}
		mJoystickCenter->setPosition(mJoystickPos);
		addChild(mJoystickCenter);

		auto touchListener = EventListenerTouchOneByOne::create();
		if (nullptr == touchListener) {
			break;
		}
		touchListener->setSwallowTouches(true);
		touchListener->onTouchBegan =
				CC_CALLBACK_2(Joystick::onTouchBegan, this);
		touchListener->onTouchMoved =
				CC_CALLBACK_2(Joystick::onTouchMoved, this);
		touchListener->onTouchEnded =
				CC_CALLBACK_2(Joystick::onTouchEnded, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,
				this);

		result = true;
	} while (0);

	return result;
}

void Joystick::setJoystickPos(Vec2 pos) {
	mJoystickPos = pos;
}

bool Joystick::onTouchBegan(Touch *touch, Event *event) {
	auto touchPos = touch->getLocation();
	if (mJoystickCenter->getBoundingBox().containsPoint(touchPos)) {
		return true;
	}
	return false;
}

void Joystick::onTouchMoved(Touch *touch, Event *event) {
	Vec2 touchPos = touch->getLocation();
	double y = touchPos.y - mJoystickPos.y;
	double x = touchPos.x - mJoystickPos.x;
	double angle = atan2(y, x) * 180 / PI;

	double joystickBgRadis = mJoystickBg->getContentSize().width / 2;

	double distanceOfTouchPointToCenter = sqrt(
			pow(mJoystickPos.x - touchPos.x, 2) + pow(mJoystickPos.y - touchPos.y, 2));
	if (distanceOfTouchPointToCenter >= joystickBgRadis) {
		/*
		      /|
	     R  /  | 
		  / |  |y
		/  Y|  |
		--------
		  X | x
		*/
		double X = x * (joystickBgRadis / distanceOfTouchPointToCenter);
		double Y = y * (joystickBgRadis / distanceOfTouchPointToCenter);
		mJoystickCenter->setPosition(Vec2(mJoystickPos.x + X, mJoystickPos.y + Y));
		distanceOfTouchPointToCenter = joystickBgRadis;
	} else {
		mJoystickCenter->setPosition(touchPos);
	}

	JoystickCustomEvent* joystickEvent = JoystickCustomEvent::create();
	joystickEvent->mAnagle = angle;
	joystickEvent->mDistance = distanceOfTouchPointToCenter;
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(
		JoystickCustomEvent::EVENT_JOYSTICK, joystickEvent);
}

void Joystick::onTouchEnded(Touch *touch, Event *event) {
	mJoystickCenter->setPosition(mJoystickPos);
}
