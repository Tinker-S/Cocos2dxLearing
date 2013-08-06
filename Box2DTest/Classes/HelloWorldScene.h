#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

using namespace cocos2d;

class HelloWorld : public CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
};

class Box2DTest:public CCLayer
{
protected:
	b2World *m_world;

public:
	Box2DTest();
	~Box2DTest();

	void addSprite(CCPoint point);
	void update(float dt);

	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
};

#endif  // __HELLOWORLD_SCENE_H__