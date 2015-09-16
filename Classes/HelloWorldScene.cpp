#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Joystick.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

	auto joystick = Joystick::create();
	scene->addChild(joystick, 100);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("MainScene.csb");

    addChild(rootNode);

	auto _listener = EventListenerCustom::create(JoystickCustomEvent::EVENT_JOYSTICK, [=](EventCustom* event) {
		JoystickCustomEvent* jsevent = static_cast<JoystickCustomEvent*>(event->getUserData());
		log("++++++++++++++++++++++++++++++++++++angle=%f, distance = %f", jsevent->mAnagle, jsevent->mDistance);
	});

	_eventDispatcher->addEventListenerWithFixedPriority(_listener, 1);

    return true;
}
