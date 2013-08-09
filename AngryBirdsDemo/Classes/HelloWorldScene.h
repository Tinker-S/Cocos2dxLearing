#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

using namespace cocos2d;

class HelloWorld : public CCLayer
{
public:
    virtual bool init();
    static CCScene* scene();
    void menuCloseCallback(CCObject* pSender);
    CREATE_FUNC(HelloWorld);

	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	void update(float dt);

	b2Body *groundBody;
	b2World *world;
	CCSpriteBatchNode *batch;
	CCSprite *bird;
	CCPoint center;
	CCPoint currentPoint;
	bool isDraging;
	float ratio;
	int r;
	bool canFly;
};

#endif  // __HELLOWORLD_SCENE_H__