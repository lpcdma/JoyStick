var Joystick = cc.Layer.extend({
    sprite: null,
    mJoystickBg: null,
    mJoystickCenter: null,
	mJoystickPos: cc.p(100, 100),
	_listener: null,
    ctor:function () {
        this._super();
        var size = cc.winSize;
        mJoystickPos = cc.p(100, 100);
        mJoystickBg = new cc.Sprite("joystick_bg.png");
        mJoystickBg.setPosition(mJoystickPos);
        this.addChild(mJoystickBg);

        mJoystickCenter = new cc.Sprite("joystick_center.png");
        mJoystickCenter.setPosition(mJoystickPos);
        this.addChild(mJoystickCenter);

        // Make sprite1 touchable
        var listener1 = cc.EventListener.create({
            event: cc.EventListener.TOUCH_ONE_BY_ONE,
            swallowTouches: true,
            onTouchBegan: function (touch, event) {
                var target = event.getCurrentTarget();

                var locationInNode = target.convertToNodeSpace(touch.getLocation());
                var s = target.getContentSize();
                var rect = cc.rect(0, 0, s.width, s.height);

                if (cc.rectContainsPoint(rect, locationInNode)) {
                    cc.log("sprite began... x = " + locationInNode.x + ", y = " + locationInNode.y);
                    target.opacity = 180;
                    return true;
                }
                return false;
            },
            onTouchMoved: function (touch, event) {
            	var touchPos = touch.getLocation();
				var y = touchPos.y - mJoystickPos.y;
				var x = touchPos.x - mJoystickPos.x;
				var angle = Math.atan2(y, x) * 180 / 3.1415926;

				var joystickBgRadis = mJoystickBg.getContentSize().width / 2;

				var distanceOfTouchPointToCenter = Math.sqrt(Math.pow(mJoystickPos.x - touchPos.x, 2) + Math.pow(mJoystickPos.y - touchPos.y, 2));
				if (distanceOfTouchPointToCenter >= joystickBgRadis) {
					var X = x * (joystickBgRadis / distanceOfTouchPointToCenter);
					var Y = y * (joystickBgRadis / distanceOfTouchPointToCenter);
					mJoystickCenter.setPosition(cc.p(mJoystickPos.x + X, mJoystickPos.y + Y));
					distanceOfTouchPointToCenter = joystickBgRadis;
				}
				else {
					mJoystickCenter.setPosition(touchPos);
				}
				
				var eventCustom = new cc.EventCustom("event_of_joystick");
				eventCustom.mAngle = angle;
				eventCustom.mDistance = distanceOfTouchPointToCenter;
		   		cc.eventManager.dispatchEvent(eventCustom);
            },
            onTouchEnded: function (touch, event) {
                mJoystickCenter.setPosition(mJoystickPos);
            }
        });

        cc.eventManager.addListener(listener1, mJoystickCenter);
        //event test
        this._listener = cc.EventListener.create({
            event: cc.EventListener.CUSTOM,
            eventName: "event_of_joystick",
            callback: function(event){
                cc.log("++++++++++++++++++++++++angle = " + event.mAngle + "+++++++distance = " + event.mDistance);
            }
        });

        cc.eventManager.addListener(this._listener, 1);
        return true;
    }
});