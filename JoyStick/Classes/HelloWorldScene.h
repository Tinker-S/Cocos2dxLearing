#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::CCLayer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	static cocos2d::CCScene* scene();

	void menuCloseCallback(CCObject* pSender);

	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	
	CREATE_FUNC(HelloWorld);

private:
	void flying(float dt);

	cocos2d::CCSprite *joystick;
	//中心点O
	cocos2d::CCPoint O;
	//大圆半径
	float R;

	cocos2d::CCSprite *plane;
	//飞机飞行的速度分量值
	float speedX;
	float speedY;
	//是否飞行的标志
	bool isFlying;
};

#endif  // __HELLOWORLD_SCENE_H__