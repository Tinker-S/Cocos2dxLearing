#include "HelloWorldScene.h"

using namespace cocos2d;

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! CCLayer::init());

		this->setTouchEnabled(true);

		screenSize = CCDirector::sharedDirector()->getWinSize();

		CCSprite *sprite = CCSprite::spriteWithFile("bg.png");		//背景图
		sprite->setAnchorPoint(CCPointZero);
		this->addChild(sprite,-1);

		sprite = CCSprite::spriteWithFile("catapult_base_2.png");	//投射器底部后面那块
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(CCPointMake(181.0,FLOOR_HEIGHT));
		this->addChild(sprite,0);

		sprite = CCSprite::spriteWithFile("squirrel_1.png");	//左边松鼠
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(CCPointMake(11.0,FLOOR_HEIGHT));
		this->addChild(sprite,0);

		sprite = CCSprite::spriteWithFile("catapult_base_1.png");	//投射器底部前面那块
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(CCPointMake(181.0,FLOOR_HEIGHT));
		this->addChild(sprite,9);

		sprite = CCSprite::spriteWithFile("squirrel_2.png");	//右边松鼠
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(CCPointMake(242.0,FLOOR_HEIGHT));
		this->addChild(sprite,9);

		sprite = CCSprite::spriteWithFile("fg.png");	//带冰的地面
		sprite->setAnchorPoint(CCPointZero);
		this->addChild(sprite,10);

		b2Vec2 gravity;
		gravity.Set(0.0f,-10.0f);
		m_world = new b2World(gravity);
		m_world->SetAllowSleeping(true);
		m_world->SetContinuousPhysics(true);

		// Define the ground body. 
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0,0);	// bottom-left corner 

		// Call the body factory which allocates memory for the ground body 
		// from a pool and creates the ground box shape (also from a pool). 
		// The body is also added to the world.
		m_groundBody = m_world->CreateBody(&groundBodyDef);

		//设置世界的边界
		b2EdgeShape groundBox;
		//bottom
		groundBox.Set(b2Vec2(0,FLOOR_HEIGHT/PTM_RATIO),b2Vec2(screenSize.width*2.0f/PTM_RATIO,FLOOR_HEIGHT/PTM_RATIO));
		m_groundBody->CreateFixture(&groundBox,0);
		//top
		groundBox.Set(b2Vec2(0,screenSize.height/PTM_RATIO),b2Vec2(screenSize.width*2.0f/PTM_RATIO,screenSize.height/PTM_RATIO));
		m_groundBody->CreateFixture(&groundBox,0);
		//left
		groundBox.Set(b2Vec2(0,screenSize.height/PTM_RATIO),b2Vec2(0,0));
		m_groundBody->CreateFixture(&groundBox,0);
		//right
		groundBox.Set(b2Vec2(screenSize.width*1.5f/PTM_RATIO,screenSize.height/PTM_RATIO),b2Vec2(screenSize.width*1.5f/PTM_RATIO,0));
		m_groundBody->CreateFixture(&groundBox,0);

		//生成发射器臂
		CCSprite *arm = CCSprite::spriteWithFile("catapult_arm.png");
		this->addChild(arm,1);

		b2BodyDef armBodyDef;
		armBodyDef.type = b2_dynamicBody;
		armBodyDef.linearDamping = 1;
		armBodyDef.angularDamping = 1;
		armBodyDef.position.Set(230.0f/PTM_RATIO,(FLOOR_HEIGHT+91.0f)/PTM_RATIO);
		armBodyDef.userData = arm;
		m_armBody = m_world->CreateBody(&armBodyDef);

		b2PolygonShape armBox;
		b2FixtureDef armBoxDef;
		armBoxDef.shape = &armBox;
		armBoxDef.density = 0.3f;
		armBox.SetAsBox(11.0f/PTM_RATIO,91.0f/PTM_RATIO);
		m_armFixture = m_armBody->CreateFixture(&armBoxDef);

		b2RevoluteJointDef armJointDef;
		armJointDef.Initialize(m_groundBody,m_armBody,b2Vec2(233.0f/PTM_RATIO,FLOOR_HEIGHT/PTM_RATIO));
		armJointDef.enableMotor = true;
		armJointDef.enableLimit = true;
		armJointDef.motorSpeed = -10;
		armJointDef.lowerAngle = CC_DEGREES_TO_RADIANS(9);
		armJointDef.upperAngle = CC_DEGREES_TO_RADIANS(75);
		armJointDef.maxMotorTorque = 700;
		m_armJoint = (b2RevoluteJoint *)m_world->CreateJoint(&armJointDef);

		m_mouseJoint=NULL;

		schedule(schedule_selector(HelloWorld::tick));

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

void HelloWorld::tick(float dt)
{
	int velocithIterations = 8;
	int positionIterations = 1;

	m_world->Step(dt,velocithIterations,positionIterations);

	for(b2Body *b = m_world->GetBodyList();b;b=b->GetNext())
	{
		if(b->GetUserData()!=NULL)
		{
			CCSprite *myActor = (CCSprite*)b->GetUserData();
			myActor->setPosition(CCPointMake(b->GetPosition().x*PTM_RATIO,b->GetPosition().y*PTM_RATIO));
			myActor->setRotation(-1*CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}
}

void HelloWorld::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	if (m_mouseJoint!=NULL)
	{
		return;
	}
	CCTouch *touch = (CCTouch *)pTouches->anyObject();
	CCPoint location = touch->locationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);
	b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO,location.y/PTM_RATIO);

	if (locationWorld.x < m_armBody->GetWorldCenter().x+150.0f/PTM_RATIO)
	{
		b2MouseJointDef md;
		md.bodyA = m_groundBody;
		md.bodyB = m_armBody;
		md.target = locationWorld;
		md.maxForce = 2000;

		m_mouseJoint = (b2MouseJoint *)m_world->CreateJoint(&md);
	}
}

void HelloWorld::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	if (m_mouseJoint==NULL)
	{
		return;
	}
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCPoint location = touch->locationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);
	b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO,location.y/PTM_RATIO);

	m_mouseJoint->SetTarget(locationWorld);
}

void HelloWorld::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	if (m_mouseJoint!=NULL)
	{
		m_world->DestroyJoint(m_mouseJoint);
		m_mouseJoint=NULL;
	}
}