#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static CCScene* scene();

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

	~HelloWorld();

	CCSprite *virtualpad;
	CCPoint O;
	float R;
	CCSprite *plane;
	CCTexture2D *m_texture_normal;
	CCTexture2D *m_texture_down;
	CCTexture2D *m_texture_up;
	CCTexture2D *m_texture_left;
	CCTexture2D *m_texture_right;

	bool isFlying;

	void flying(float dt);
	float speedX;
	float speedY;

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif  // __HELLOWORLD_SCENE_H__