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
        //HelloWorld *layer = HelloWorld::create();
		Box2DTest *layer = new Box2DTest();
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
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(HelloWorld::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

Box2DTest::Box2DTest()
{
	setTouchEnabled(true);
	setAccelerometerEnabled(true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,INT_MIN+1,true);

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	//������������
	b2Vec2 gravity(0.0f,-10.0f);

	m_world = new b2World(gravity);
	m_world->SetAllowSleeping(true);
	m_world->SetContinuousPhysics(true);

	//����ر�����
	b2BodyDef groundBodyDef;

	b2Body *groundBody = m_world->CreateBody(&groundBodyDef);
	b2PolygonShape groundBox;

	//ǽ��
	groundBox.SetAsBox(size.width/32,0,b2Vec2(0,0),0);
	groundBody->CreateFixture(&groundBox,0);
	
	//ǽ��
	groundBox.SetAsBox(size.width/32,0,b2Vec2(0,size.height/32),0);
	groundBody->CreateFixture(&groundBox,0);
	
	//��ǽ
	groundBox.SetAsBox(0,size.height/32,b2Vec2(0,0),0);
	groundBody->CreateFixture(&groundBox,0);
	
	//��ǽ
	groundBox.SetAsBox(0,size.height/32,b2Vec2(size.width/32,0),0);  
	groundBody->CreateFixture(&groundBox,0);

	CCSpriteBatchNode *brick = CCSpriteBatchNode::batchNodeWithFile("brick.png");
	addChild(brick,2,1);

	addSprite(ccp(size.width/2,size.height/2));

	scheduleUpdate();
}
Box2DTest::~Box2DTest()
{
	delete m_world;
	m_world = NULL;
}

void Box2DTest::addSprite(CCPoint point)
{
	CCSpriteBatchNode *batch = (CCSpriteBatchNode*)getChildByTag(1);

	sprite = CCSprite::spriteWithTexture(batch->getTexture(),CCRectMake(0,0,32,32));
	batch->addChild(sprite);

	sprite->setPosition(ccp(point.x,point.y));

	//������̬����
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(point.x/32,point.y/32);	//��Ϸ��ÿ32������Ϊbox2d��һ��
	bodyDef.userData = sprite;
	b2Body *body = m_world->CreateBody(&bodyDef);	//��sprite����������

	b2PolygonShape dynamiBox;
	dynamiBox.SetAsBox(0.5f,0.5f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamiBox;
	fixtureDef.friction = 0.3f;
	fixtureDef.density = 1.0;
	body->CreateFixture(&fixtureDef);
}

void Box2DTest::update(float dt)
{
	int velocityIterations = 8;
	int positionIterations = 1;

	//ÿ����Ϸѭ���㶼Ӧ�õ���b2World::Step
	m_world->Step(dt,velocityIterations,positionIterations);

	for(b2Body *b = m_world->GetBodyList();b;b=b->GetNext())
	{
		if(b->GetUserData()!=NULL)
		{
			CCSprite *sprite = (CCSprite*)b->GetUserData();
			sprite->setPosition(ccp(b->GetPosition().x*32,b->GetPosition().y*32));
			sprite->setRotation(-1*CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}
}

bool Box2DTest::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint location = pTouch->locationInView();
	location = CCDirector::sharedDirector()->convertToGL(location);
	addSprite(location);
	return true;
}