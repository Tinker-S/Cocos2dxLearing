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

		CCSprite *sprite = CCSprite::spriteWithFile("bg.png");		//����ͼ
		sprite->setAnchorPoint(CCPointZero);
		this->addChild(sprite,-1);

		sprite = CCSprite::spriteWithFile("catapult_base_2.png");	//Ͷ�����ײ������ǿ�
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(ccp(181.0,FLOOR_HEIGHT));
		this->addChild(sprite,0);

		sprite = CCSprite::spriteWithFile("squirrel_1.png");	//�������
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(ccp(11.0,FLOOR_HEIGHT));
		this->addChild(sprite,0);

		sprite = CCSprite::spriteWithFile("catapult_base_1.png");	//Ͷ�����ײ�ǰ���ǿ�
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(ccp(181.0,FLOOR_HEIGHT));
		this->addChild(sprite,9);

		sprite = CCSprite::spriteWithFile("squirrel_2.png");	//�ұ�����
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(ccp(242.0,FLOOR_HEIGHT));
		this->addChild(sprite,9);

		sprite = CCSprite::spriteWithFile("fg.png");	//�����ĵ���
		sprite->setAnchorPoint(CCPointZero);
		this->addChild(sprite,10);

		b2Vec2 gravity;
		gravity.Set(0.0f,-10.0f);		//�����������������
		m_world = new b2World(gravity);
		m_world->SetAllowSleeping(true);
		m_world->SetContinuousPhysics(true);

		// Define the ground body. 
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0,0);	//���½�

		// Call the body factory which allocates memory for the ground body 
		// from a pool and creates the ground box shape (also from a pool). 
		// The body is also added to the world.
		m_groundBody = m_world->CreateBody(&groundBodyDef);

		//��������ı߽�
		b2EdgeShape groundBox;
		//�ײ�
		groundBox.Set(b2Vec2(0,FLOOR_HEIGHT/PTM_RATIO),b2Vec2(screenSize.width*2.0f/PTM_RATIO,FLOOR_HEIGHT/PTM_RATIO));
		m_groundBody->CreateFixture(&groundBox,0);
		//����
		groundBox.Set(b2Vec2(0,screenSize.height/PTM_RATIO),b2Vec2(screenSize.width*2.0f/PTM_RATIO,screenSize.height/PTM_RATIO));
		m_groundBody->CreateFixture(&groundBox,0);
		//���
		groundBox.Set(b2Vec2(0,screenSize.height/PTM_RATIO),b2Vec2(0,0));
		m_groundBody->CreateFixture(&groundBox,0);
		//�Ҳ�
		groundBox.Set(b2Vec2(screenSize.width*2.0f/PTM_RATIO,screenSize.height/PTM_RATIO),b2Vec2(screenSize.width*2.0f/PTM_RATIO,0));
		m_groundBody->CreateFixture(&groundBox,0);

		//���ɷ�������
		CCSprite *arm = CCSprite::spriteWithFile("catapult_arm.png");
		this->addChild(arm,1);

		b2BodyDef armBodyDef;
		armBodyDef.type = b2_dynamicBody;
		armBodyDef.linearDamping = 1;		//�������
		armBodyDef.angularDamping = 1;		//�������
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
		armJointDef.enableLimit = true;		//�Ƿ����ؽڵ�����
		armJointDef.motorSpeed = -10;		//�ؽ�����Ŀ���ٶ�(�����ٶ�)
		armJointDef.lowerAngle = CC_DEGREES_TO_RADIANS(9);		//����ܵ���ĽǶ�
		armJointDef.upperAngle = CC_DEGREES_TO_RADIANS(75);		//����ܵ���ĽǶ�
		armJointDef.maxMotorTorque = 700;	//�������ʹ�õ����Ť��ֵ��Ť�ؾ������ء����� = ��*���ۡ�
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
	//���͵ĵ���ֵ������һЩ׼ȷ�ԣ��෴��Ϊ��������һ��������
	int velocithIterations = 8;		//�ٶȵ���
	int positionIterations = 1;		//λ�õ���

	m_world->Step(dt,velocithIterations,positionIterations);

	for(b2Body *b = m_world->GetBodyList();b;b=b->GetNext())
	{
		if(b->GetUserData()!=NULL)
		{
			//ʵʱ����world��������λ�ú�ת���Ƕ�
			CCSprite *myActor = (CCSprite*)b->GetUserData();
			//CCLog("myActor:%f %f %f",myActor->getPosition().x,myActor->getPosition().y,myActor->getRotation());
			//CCLog("b:%f %f %f",b->GetPosition().x*PTM_RATIO,b->GetPosition().y*PTM_RATIO,b->GetAngle());
			myActor->setPosition(CCPointMake(b->GetPosition().x*PTM_RATIO,b->GetPosition().y*PTM_RATIO));
			myActor->setRotation(-1*CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}
	if (m_releasingArm && m_bulletJoint!=NULL)
	{
		//ת���Ƕ�С��10��ʱ���ͷ�����
		if (m_armJoint->GetJointAngle()<=CC_DEGREES_TO_RADIANS(10))
		{
			m_releasingArm = false;

			m_world->DestroyJoint(m_bulletJoint);		//�����Ӻ�Ͷ�����۽������Ͷ����Ҫ�ظ���ԭλ����˴��������ƶ����γɷ������ӵ�Ч����
			m_bulletJoint = NULL;
		}
	}
	if (m_bulletBody && m_bulletJoint == NULL)
	{
		b2Vec2 position = m_bulletBody->GetPosition();
		CCPoint myPosition = this->getPosition();
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

		//�ƶ����λ��
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
		//�������ؽ�
		b2MouseJointDef md;
		md.bodyA = m_groundBody;		//һ��Ϊ����߽�
		md.bodyB = m_armBody;			//��Ҫ�϶������� 
		md.target = locationWorld;		//ָ���϶�������
		md.maxForce = 2000;				//��һ���϶�����

		m_mouseJoint = (b2MouseJoint *)m_world->CreateJoint(&md);	//���ؽڣ������϶�Ͷ������
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
	//�ı�ؽ�λ��
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
		// delta������֮��ľ��� 
		// 62�ǵ�һ�����ӳ��ֵ�λ�� 
		// 165���������һ�����ӳ��ֵ�λ��
		// 30�����ӵĴ�С

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
			ballShapeDef.restitution = 0.2f;	//0��ʾ��������1��ʾ��ȫ����
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

		m_bulletJoint = (b2WeldJoint*)m_world->CreateJoint(&weldJointDef);		//�����Ӻ�Ͷ�����۰�
		return true;
	}
	return false;
}

void HelloWorld::resetGame()
{
	this->createBullets(4);
	this->attachBullet();
}