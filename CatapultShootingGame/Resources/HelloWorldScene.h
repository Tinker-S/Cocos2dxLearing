#ifndef __HELLO_m_worldH__
#define __HELLO_m_worldH__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "GLES-Render.h"

class HelloWorld : public cocos2d::CCLayer {
public:
    ~HelloWorld();
//    HelloWorld();
	bool init();
	LAYER_NODE_FUNC(HelloWorld);

    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();
    
    // adds a new sprite at a given coordinate
    virtual void draw();
    void tick(cocos2d::ccTime dt);
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    void createBullets(int count);
    bool attachBullet();
    void resetGame();
private:
    b2World* m_world;
    b2Body* m_groundBody;
    b2Fixture *m_armFixture;
    b2Body *m_armBody;
    b2RevoluteJoint *m_armJoint;
    b2MouseJoint *m_mouseJoint;
	std::vector<b2Body *> m_bullets;
	int m_currentBullet;
    
    b2Body *m_bulletBody;
    b2WeldJoint *m_bulletJoint;
    
    bool m_releasingArm;
};

class Catapult : public cocos2d::CCLayer
{
public:
	static cocos2d::CCScene *scene();
	LAYER_NODE_FUNC(Catapult);
	bool init();

	virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	virtual void ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);

	virtual void draw();
	void tick(cocos2d::ccTime dt);
private:
	b2World* m_world;
	b2Body* m_groundBody;
	b2Fixture *m_armFixture;
	b2Body *m_armBody;
	b2RevoluteJoint *m_armJoint;
	b2MouseJoint *m_mouseJoint;

	b2Draw *m_debugDraw;

};

#endif // __HELLO_m_worldH__
