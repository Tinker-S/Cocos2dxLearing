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
	//���ĵ�O
	cocos2d::CCPoint O;
	//��Բ�뾶
	float R;

	cocos2d::CCSprite *plane;
	//�ɻ����е��ٶȷ���ֵ
	float speedX;
	float speedY;
	//�Ƿ���еı�־
	bool isFlying;
};

#endif  // __HELLOWORLD_SCENE_H__