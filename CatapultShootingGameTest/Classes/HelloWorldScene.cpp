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
		m_mouseJoint=NULL;
		m_releasingArm = false;

		screenSize = CCDirector::sharedDirector()->getWinSize();

		CCSprite *sprite = CCSprite::spriteWithFile("bg.png");		//背景图
		sprite->setAnchorPoint(CCPointZero);
		this->addChild(sprite,-1);

		sprite = CCSprite::spriteWithFile("catapult_base_2.png");	//投射器底部后面那块
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(ccp(181.0,FLOOR_HEIGHT));
		this->addChild(sprite,0);

		sprite = CCSprite::spriteWithFile("squirrel_1.png");	//左边松鼠
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(ccp(11.0,FLOOR_HEIGHT));
		this->addChild(sprite,0);

		sprite = CCSprite::spriteWithFile("catapult_base_1.png");	//投射器底部前面那块
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(ccp(181.0,FLOOR_HEIGHT));
		this->addChild(sprite,9);

		sprite = CCSprite::spriteWithFile("squirrel_2.png");	//右边松鼠
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(ccp(242.0,FLOOR_HEIGHT));
		this->addChild(sprite,9);

		sprite = CCSprite::spriteWithFile("fg.png");	//带冰的地面
		sprite->setAnchorPoint(CCPointZero);
		this->addChild(sprite,10);

		b2Vec2 gravity;
		gravity.Set(0.0f,-10.0f);		//设置世界的重力方向
		m_world = new b2World(gravity);
		m_world->SetAllowSleeping(true);
		m_world->SetContinuousPhysics(true);

		// Define the ground body. 
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0,0);	//左下角

		// Call the body factory which allocates memory for the ground body 
		// from a pool and creates the ground box shape (also from a pool). 
		// The body is also added to the world.
		m_groundBody = m_world->CreateBody(&groundBodyDef);

		//设置世界的边界
		b2EdgeShape groundBox;
		//底部
		groundBox.Set(b2Vec2(0,FLOOR_HEIGHT/PTM_RATIO),b2Vec2(screenSize.width*2.0f/PTM_RATIO,FLOOR_HEIGHT/PTM_RATIO));
		m_groundBody->CreateFixture(&groundBox,0);
		//顶部
		groundBox.Set(b2Vec2(0,screenSize.height/PTM_RATIO),b2Vec2(screenSize.width*2.0f/PTM_RATIO,screenSize.height/PTM_RATIO));
		m_groundBody->CreateFixture(&groundBox,0);
		//左侧
		groundBox.Set(b2Vec2(0,screenSize.height/PTM_RATIO),b2Vec2(0,0));
		m_groundBody->CreateFixture(&groundBox,0);
		//右侧
		groundBox.Set(b2Vec2(screenSize.width*2.0f/PTM_RATIO,screenSize.height/PTM_RATIO),b2Vec2(screenSize.width*2.0f/PTM_RATIO,0));
		m_groundBody->CreateFixture(&groundBox,0);

		//生成发射器臂
		CCSprite *arm = CCSprite::spriteWithFile("catapult_arm.png");
		this->addChild(arm,1);

		b2BodyDef armBodyDef;
		armBodyDef.type = b2_dynamicBody;
		armBodyDef.linearDamping = 1;		//阻尼参数
		armBodyDef.angularDamping = 1;		//阻尼参数
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
		armJointDef.enableLimit = true;		//是否开启关节的限制
		armJointDef.motorSpeed = -10;		//关节马达的目标速度(线性速度)
		armJointDef.lowerAngle = CC_DEGREES_TO_RADIANS(9);		//最低能到达的角度
		armJointDef.upperAngle = CC_DEGREES_TO_RADIANS(75);		//最高能到达的角度
		armJointDef.maxMotorTorque = 700;	//马达允许使用的最大扭矩值。扭矩就是力矩。力矩 = 力*力臂。
		m_armJoint = (b2RevoluteJoint *)m_world->CreateJoint(&armJointDef);


		CCDelayTime *delayAction = CCDelayTime::actionWithDuration(0.2f);
		CCCallFunc *callSelectorAction = CCCallFunc::actionWithTarget(this,callfunc_selector(HelloWorld::resetGame));
		this->runAction(CCSequence::actions(delayAction,callSelectorAction,NULL));

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
	//更低的迭代值会牺牲一些准确性，相反的为程序提升一部分性能
	int velocithIterations = 8;		//速度迭代
	int positionIterations = 1;		//位置迭代

	m_world->Step(dt,velocithIterations,positionIterations);

	for(b2Body *b = m_world->GetBodyList();b;b=b->GetNext())
	{
		if(b->GetUserData()!=NULL)
		{
			//实时更新world里的物体的位置和转动角度
			CCSprite *myActor = (CCSprite*)b->GetUserData();
			//CCLog("myActor:%f %f %f",myActor->getPosition().x,myActor->getPosition().y,myActor->getRotation());
			//CCLog("b:%f %f %f",b->GetPosition().x*PTM_RATIO,b->GetPosition().y*PTM_RATIO,b->GetAngle());
			myActor->setPosition(CCPointMake(b->GetPosition().x*PTM_RATIO,b->GetPosition().y*PTM_RATIO));
			myActor->setRotation(-1*CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}
	if (m_releasingArm && m_bulletJoint!=NULL)
	{
		//转动角度小于10度时不释放橡子
		if (m_armJoint->GetJointAngle()<=CC_DEGREES_TO_RADIANS(10))
		{
			m_releasingArm = false;

			m_world->DestroyJoint(m_bulletJoint);		//将橡子和投射器臂解绑。由于投射器要回复到原位，因此带动橡子移动，形成发射橡子的效果。
			m_bulletJoint = NULL;
		}
	}
	if (m_bulletBody && m_bulletJoint == NULL)
	{
		b2Vec2 position = m_bulletBody->GetPosition();
		CCPoint myPosition = this->getPosition();
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

		//移动相机位置
		if (position.x > screenSize.width/2.0f/PTM_RATIO)
		{
			myPosition.x = -MIN(screenSize.width * 2.0f - screenSize.width,position.x*PTM_RATIO-screenSize.width/2.0f);
			this->setPosition(myPosition);
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
		//创建鼠标关节
		b2MouseJointDef md;
		md.bodyA = m_groundBody;		//一般为世界边界
		md.bodyB = m_armBody;			//需要拖动的物体 
		md.target = locationWorld;		//指定拖动的坐标
		md.maxForce = 2000;				//给一个拖动的力

		m_mouseJoint = (b2MouseJoint *)m_world->CreateJoint(&md);	//鼠标关节，用于拖动投射器臂
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
	//改变关节位置
	m_mouseJoint->SetTarget(locationWorld);
}

void HelloWorld::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	if (m_mouseJoint!=NULL)
	{
		if (m_armJoint->GetJointAngle()>=CC_DEGREES_TO_RADIANS(10))
		{
			//CCLog("-----------------------------------------release----------------------------------------");
			m_releasingArm = true;
		}

		m_world->DestroyJoint(m_mouseJoint);
		m_mouseJoint=NULL;
	}
}

void HelloWorld::createBullets(int count)
{
	m_currentBullet = 0;
	float pos = 62.0f;

	if (count>0)
	{
		// delta是橡子之间的距离 
		// 62是第一个橡子出现的位置 
		// 165是期望最后一个橡子出现的位置
		// 30是橡子的大小

		float delta = (count>1)?((165.f-62.0f-30.0f)/(count-1)):0.0f;

		for(int i =0;i<count;i++,pos+=delta)
		{
			CCSprite *sprite = CCSprite::spriteWithFile("acorn.png");
			this->addChild(sprite,11);

			b2BodyDef bulletBodyDef;
			bulletBodyDef.type = b2_dynamicBody;
			bulletBodyDef.bullet = true;
			bulletBodyDef.position.Set(pos/PTM_RATIO,(FLOOR_HEIGHT+15.0f)/PTM_RATIO);
			bulletBodyDef.userData = sprite;
			b2Body *bullet = m_world->CreateBody(&bulletBodyDef);
			bullet->SetActive(false);

			b2CircleShape circle;
			circle.m_radius = 15.0/PTM_RATIO;

			b2FixtureDef ballShapeDef;
			ballShapeDef.shape = &circle;
			ballShapeDef.density = 0.8f;
			ballShapeDef.restitution = 0.2f;	//0表示不反弹，1表示完全反弹
			ballShapeDef.friction = 0.99f;
			bullet->CreateFixture(&ballShapeDef);

			m_bullets.push_back(bullet);
		}
	}
	
}

bool HelloWorld::attachBullet()
{
	if(m_currentBullet<m_bullets.size())
	{
		m_bulletBody = (b2Body *)m_bullets.at(m_currentBullet++);
		m_bulletBody->SetTransform(b2Vec2(230.0f/PTM_RATIO,(155.0f+FLOOR_HEIGHT)/PTM_RATIO),0.0f);
		m_bulletBody->SetActive(true);

		b2WeldJointDef weldJointDef;
		weldJointDef.Initialize(m_bulletBody,m_armBody,b2Vec2(230.0f/PTM_RATIO,(155.0f+FLOOR_HEIGHT)/PTM_RATIO));
		weldJointDef.collideConnected = false;

		m_bulletJoint = (b2WeldJoint*)m_world->CreateJoint(&weldJointDef);		//将橡子和投射器臂绑定
		return true;
	}
	return false;
}

void HelloWorld::resetGame()
{
	this->createBullets(4);
	this->attachBullet();
}