#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"
#include "MyContackListener.h"

#define FLOOR_HEIGHT 62.0f
#define PTM_RATIO 32

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);

	cocos2d::CCSize screenSize;
	void tick(float dt);

	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);


private:
	b2World *m_world;
	b2Body *m_groundBody;
	b2Fixture *m_armFixture;
	b2Body *m_armBody;
	b2RevoluteJoint *m_armJoint;
	b2MouseJoint *m_mouseJoint;
	b2Body *m_bulletBody;
	b2WeldJoint *m_bulletJoint;

	MyContactListener *contactListener;

	std::vector<b2Body *>m_bullets;
	std::vector<b2Body *>targets;
	std::vector<b2Body *>enemies;
	int m_currentBullet;
	void createBullets(int count);
	bool attachBullet();
	void resetGame();
	bool m_releasingArm;
	void resetBullet();
	void createTarget();
	void createTarget(char *imageName,cocos2d::CCPoint position,float rotation,bool isCircle,bool isStatic,bool isEnemy);
};

#endif  // __HELLOWORLD_SCENE_H__