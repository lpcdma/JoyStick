Joystick = {
	mJoystickBg,
	mJoystickCenter,
	mJoystickPos = cc.p(100, 100),
	mLayer
}

function Joystick:init()
	local eventDispatcher = cc.Director:getInstance():getEventDispatcher()
	self.mLayer = cc.Layer:create()
	self.mJoystickBg = cc.Sprite:create("joystick_bg.png");
	if nil == self.mJoystickBg then
		return
	end
	self.mJoystickBg:setPosition(self.mJoystickPos);
	self.mLayer:addChild(self.mJoystickBg);
	self.mJoystickCenter = cc.Sprite:create("joystick_center.png");
	if nil == self.mJoystickCenter then
		return
	end
	self.mJoystickCenter:setPosition(self.mJoystickPos)
	self.mLayer:addChild(self.mJoystickCenter);
	local function onTouchBegan(touch, event)
		local  touchPos = touch:getLocation()
		--self.mJoystickCenter:getBoundingBox():containsPoint(touchPos)
		local target = event:getCurrentTarget()
        
        local locationInNode = target:convertToNodeSpace(touch:getLocation())
        local s = target:getContentSize()
        local rect = cc.rect(0, 0, s.width, s.height)
        
        if cc.rectContainsPoint(rect, locationInNode) then
            print(string.format("sprite began... x = %f, y = %f", locationInNode.x, locationInNode.y))
            --target:setOpacity(180)
            return true
        end
        return false
    end

    local function onTouchMoved(touch, event)
    	local touchPos = touch:getLocation()
		local y = touchPos.y - self.mJoystickPos.y
		local x = touchPos.x - self.mJoystickPos.x
		local angle = math.atan2(y, x) * 180 / 3.1415926

		local joystickBgRadis = self.mJoystickBg:getContentSize().width / 2

		local distanceOfTouchPointToCenter = math.sqrt(math.pow(self.mJoystickPos.x - touchPos.x, 2) + math.pow(self.mJoystickPos.y - touchPos.y, 2));
		if distanceOfTouchPointToCenter >= joystickBgRadis then
			local X = x * (joystickBgRadis / distanceOfTouchPointToCenter);
			local Y = y * (joystickBgRadis / distanceOfTouchPointToCenter);
			self.mJoystickCenter:setPosition(cc.p(self.mJoystickPos.x + X, self.mJoystickPos.y + Y))
			distanceOfTouchPointToCenter = joystickBgRadis
		else
			self.mJoystickCenter:setPosition(touchPos)
		end
		
		local event = cc.EventCustom:new("event_of_joystick")
		event.mAngle = angle
		event.mDistance = distanceOfTouchPointToCenter
   		eventDispatcher:dispatchEvent(event)
    end

    local function onTouchEnded(touch, event)
    	self.mJoystickCenter:setPosition(self.mJoystickPos);
    end

    local listener1 = cc.EventListenerTouchOneByOne:create()
    listener1:setSwallowTouches(true)
    listener1:registerScriptHandler(onTouchBegan,cc.Handler.EVENT_TOUCH_BEGAN )
    listener1:registerScriptHandler(onTouchMoved,cc.Handler.EVENT_TOUCH_MOVED )
    listener1:registerScriptHandler(onTouchEnded,cc.Handler.EVENT_TOUCH_ENDED )
    
    eventDispatcher:addEventListenerWithSceneGraphPriority(listener1, self.mJoystickCenter)
    local function eventUpdateSuccessListener(event)
    	print("++++++++++++++++++++++++++++" .. "angle = " .. event.mAngle .. "distance = "  .. event.mDistance)
    end

	eventDispatcher:removeCustomEventListeners("event_of_joystick")

	local updateSuccessListener = cc.EventListenerCustom:create("event_of_joystick", eventUpdateSuccessListener)
	eventDispatcher:addEventListenerWithFixedPriority(updateSuccessListener, 1)
    return self.mLayer
end