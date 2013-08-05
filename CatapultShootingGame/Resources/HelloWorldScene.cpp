#include "HelloWorldScene.h"


using namespace cocos2d;

#define PTM_RATIO       32
#define FLOOR_HEIGHT    62.0f

enum 
{
	kTagTileMap = 1,
	kTagSpriteManager = 1,
	kTagAnimation1 = 1,
}; 

//HelloWorld::HelloWorld()
bool HelloWorld::init()
{
	CCLayer::init();
	setIsTouchEnabled( true );
   	CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
	//UXLOG(L"Screen width %0.2f screen height %0.2f",screenSize.width,screenSize.height);
    
	// Define the gravity vector.
	b2Vec2 gravity;
	gravity.Set(0.0f, -10.0f);
	bool doSleep = true;
	m_world = new b2World(gravity);
	m_world->SetAllowSleeping(doSleep);
	m_world->SetContinuousPhysics(true);
    
    /*	
     m_debugDraw = new GLESDebugDraw( PTM_RATIO );
     world->SetDebugDraw(m_debugDraw);
     
     uint flags = 0;
     flags += b2DebugDraw::e_shapeBit;
     flags += b2DebugDraw::e_jointBit;
     flags += b2DebugDraw::e_aabbBit;
     flags += b2DebugDraw::e_pairBit;
     flags += b2DebugDraw::e_centerOfMassBit;
     m_debugDraw->SetFlags(flags);		
     //*/
	
    CCSprite *sprite = CCSprite::spriteWithFile("bg.png");
    sprite->setAnchorPoint(CCPointZero);
    this->addChild(sprite, -1);
    
    sprite = CCSprite::spriteWithFile("catapult_base_2.png");
    sprite->setAnchorPoint(CCPointZero);
    sprite->setPosition(CCPointMake(181.0, FLOOR_HEIGHT));
    this->addChild(sprite, 0);
    
    sprite = CCSprite::spriteWithFile("squirrel_1.png");
    sprite->setAnchorPoint(CCPointZero);
    sprite->setPosition(CCPointMake(11.0, FLOOR_HEIGHT));
    this->addChild(sprite, 0);
    
    sprite = CCSprite::spriteWithFile("catapult_base_1.png");
    sprite->setAnchorPoint(CCPointZero);
    sprite->setPosition(CCPointMake(181.0, FLOOR_HEIGHT));
    this->addChild(sprite, 9);
    
    sprite = CCSprite::spriteWithFile("squirrel_2.png");
    sprite->setAnchorPoint(CCPointZero);
    sprite->setPosition(CCPointMake(240.0, FLOOR_HEIGHT));
    this->addChild(sprite, 9);
    
    sprite = CCSprite::spriteWithFile("fg.png");
    sprite->setAnchorPoint(CCPointZero);
    this->addChild(sprite, 10);
    

	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0); // bottom-left corner
	
	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	m_groundBody = m_world->CreateBody(&groundBodyDef);
	
	// Define the ground box shape.
//	b2PolygonShape groundBox;		
	
	
	b2EdgeShape groundBox;
	// bottom
    groundBox.Set(b2Vec2(0,FLOOR_HEIGHT/PTM_RATIO), b2Vec2(screenSize.width*2.0f/PTM_RATIO,FLOOR_HEIGHT/PTM_RATIO));
	m_groundBody->CreateFixture(&groundBox, 0);
	
	// top
	groundBox.Set(b2Vec2(0,screenSize.height/PTM_RATIO), b2Vec2(screenSize.width*2.0f/PTM_RATIO,screenSize.height/PTM_RATIO));
	m_groundBody->CreateFixture(&groundBox, 0);
	
	// left
	groundBox.Set(b2Vec2(0,screenSize.height/PTM_RATIO), b2Vec2(0,0));
	m_groundBody->CreateFixture(&groundBox, 0);
	
	// right
	groundBox.Set(b2Vec2(screenSize.width*1.5f/PTM_RATIO,screenSize.height/PTM_RATIO), b2Vec2(screenSize.width*1.5f/PTM_RATIO,0));
	m_groundBody->CreateFixture(&groundBox, 0);
    
    // Create the catapult's arm
    CCSprite *arm = CCSprite::spriteWithFile("catapult_arm.png");
    this->addChild(arm, 1);
    
    b2BodyDef armBodyDef;
    armBodyDef.type = b2_dynamicBody;
    armBodyDef.linearDamping = 1;
    armBodyDef.angularDamping = 1;
    armBodyDef.position.Set(230.0f/PTM_RATIO, (FLOOR_HEIGHT+91.0f)/PTM_RATIO);
    armBodyDef.userData = arm;
    m_armBody = m_world->CreateBody(&armBodyDef);
    
    b2PolygonShape armBox;
    b2FixtureDef armBoxDef;
    armBoxDef.shape = &armBox;
    armBoxDef.density = 0.3F;
    armBox.SetAsBox(11.0f/PTM_RATIO, 91.0f/PTM_RATIO);
    m_armFixture = m_armBody->CreateFixture(&armBoxDef);


/*    
    b2RevoluteJointDef armJointDef;
    armJointDef.Initialize(m_groundBody, m_armBody, b2Vec2(233.0f/PTM_RATIO, FLOOR_HEIGHT/PTM_RATIO));
    armJointDef.enableMotor = true;
    armJointDef.enableLimit = true;
    armJointDef.motorSpeed  = -10; //-1260;
    armJointDef.lowerAngle  = CC_DEGREES_TO_RADIANS(9);
    armJointDef.upperAngle  = CC_DEGREES_TO_RADIANS(75);
    armJointDef.maxMotorTorque = 700;
    m_armJoint = (b2RevoluteJoint*)m_world->CreateJoint(&armJointDef);
    
    m_mouseJoint = NULL;
	m_releasingArm = false;

	CCDelayTime *delayAction = CCDelayTime::actionWithDuration(0.2f);
	CCCallFunc *callSelectorAction = CCCallFunc::actionWithTarget(this, callfunc_selector(HelloWorld::resetGame));
	this->runAction(CCSequence::actions(delayAction,
										callSelectorAction,
										NULL));
*/
    schedule( schedule_selector(HelloWorld::tick) );

	return true;
}

void HelloWorld::resetGame( )
{
	this->createBullets(3);
	this->attachBullet();
}

void HelloWorld::tick(ccTime dt)
{
	int velocityIterations = 8;
	int positionIterations = 1;

	m_world->Step(dt, velocityIterations, positionIterations);

	//Iterate over the bodies in the physics world
	for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL) {
			//Synchronize the AtlasSprites position and rotation with the corresponding body
			CCSprite* myActor = (CCSprite*)b->GetUserData();
			myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
			myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
		}	
	}

	/*
	// Arm is being released
	if (m_releasingArm && m_bulletJoint != NULL)
	{
		// Check if the arm reached the end so we can return the limits
		if (m_armJoint->GetJointAngle() <= CC_DEGREES_TO_RADIANS(10))
		{
			m_releasingArm = false;

			// Destroy joint so the bullet will be free
			m_world->DestroyJoint(m_bulletJoint);
			m_bulletJoint = NULL;

		}
	}

	// Bullet is moving.
	if (m_bulletBody && m_bulletJoint == NULL)
	{
		b2Vec2 position = m_bulletBody->GetPosition();
		CCPoint myPosition = this->getPosition();
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

		// Move the camera.
		if (position.x > screenSize.width / 2.0f / PTM_RATIO)
		{
			myPosition.x = -MIN(screenSize.width * 2.0f - screenSize.width, position.x * PTM_RATIO - screenSize.width / 2.0f);
			this->setPosition(myPosition);
		}
	}
	*/
}

HelloWorld::~HelloWorld()
{
	delete m_world;
	m_world = NULL;
	//delete m_debugDraw;
}

void HelloWorld::createBullets(int count)
{
    m_currentBullet = 0;
    float pos = 62.0f;
    
    if (count > 0)
    {
        // delta is the spacing between corns
        // 62 is the position o the screen where we want the corns to start appearing
        // 165 is the position on the screen where we want the corns to stop appearing
        // 30 is the size of the corn
        
        float delta = (count > 1)?((165.0f - 62.0f - 30.0f) / (count - 1)):0.0f;
                
        for (int i=0; i<count; i++, pos += delta)
        {
            // Create the bullet
            CCSprite *sprite = CCSprite::spriteWithFile("acorn.png");
            this->addChild(sprite, 1);
            
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
            ballShapeDef.restitution = 0.2f;
            ballShapeDef.friction = 0.99f;
            bullet->CreateFixture(&ballShapeDef);
            
            m_bullets.push_back(bullet);
        }
    }
}

bool HelloWorld::attachBullet()
{
    if (m_currentBullet < m_bullets.size())
    {
        m_bulletBody = (b2Body*)m_bullets.at(m_currentBullet++);
        m_bulletBody->SetTransform(b2Vec2(230.0f/PTM_RATIO, (155.0f+FLOOR_HEIGHT)/PTM_RATIO), 0.0f);
        m_bulletBody->SetActive(true);
        
        b2WeldJointDef weldJointDef;
        weldJointDef.Initialize(m_bulletBody, m_armBody, b2Vec2(230.0f/PTM_RATIO,(155.0f+FLOOR_HEIGHT)/PTM_RATIO));
        weldJointDef.collideConnected = false;
        
        m_bulletJoint = (b2WeldJoint*)m_world->CreateJoint(&weldJointDef);
        return true;
    }
    
    return false;
}

void HelloWorld::draw()
{
	// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	// Needed states:  GL_VERTEX_ARRAY, 
	// Unneeded states: GL_TEXTURE_2D, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	//world->DrawDebugData();
	
	// restore default GL states
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
}

void HelloWorld::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    if (m_mouseJoint != NULL)
    {
        return;
    }
    
    CCTouch *myTouch = (CCTouch *)touches->anyObject();
    CCPoint location = myTouch->locationInView(myTouch->view());
    location = CCDirector::sharedDirector()->convertToGL(location);
    b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
    
    if (locationWorld.x < m_armBody->GetWorldCenter().x + 150.0/PTM_RATIO)
    {
        b2MouseJointDef md;
        md.bodyA = m_groundBody;
        md.bodyB = m_armBody;
        md.target = locationWorld;
        md.maxForce = 2000;
        
        m_mouseJoint = (b2MouseJoint *)m_world->CreateJoint(&md);
    }
}

void HelloWorld::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    if (m_mouseJoint == NULL)
    {
        return;
    }
    
    CCTouch *myTouch = (CCTouch *)touches->anyObject();
    CCPoint location = myTouch->locationInView(myTouch->view());
    location = CCDirector::sharedDirector()->convertToGL(location);
    b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);
    
    m_mouseJoint->SetTarget(locationWorld);
}

void HelloWorld::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    if (m_mouseJoint != NULL)
    {
        if (m_armJoint->GetJointAngle() >= CC_DEGREES_TO_RADIANS(5))
        {
            m_releasingArm = true;
        }
        
        m_world->DestroyJoint(m_mouseJoint);
        m_mouseJoint = NULL;
        return;
    }
}

CCScene* HelloWorld::scene()
{
    CCScene *scene = CCScene::node();
    
//    CCLayer* layer = new HelloWorld();
	HelloWorld *layer = HelloWorld::node();
    scene->addChild(layer);
//    layer->release();
    
    return scene;
}

CCScene *Catapult::scene()
{
	CCScene *scene = CCScene::node();
	Catapult *layer = Catapult::node();
	scene->addChild(layer);
	return scene;
}

bool Catapult::init()
{
	bool bRet = false;
	do 
	{
		CCLayer::init();
		setIsTouchEnabled( true );
   		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    
		b2Vec2 gravity;
		gravity.Set(0.0f, -10.0f);
		bool doSleep = true;
		m_world = new b2World(gravity);
		m_world->SetAllowSleeping(doSleep);
    
		m_world->SetContinuousPhysics(true);
    
		//*	
		 m_debugDraw = new GLESDebugDraw( PTM_RATIO );
		 m_world->SetDebugDraw(m_debugDraw);
     
		 uint32 flags = 0;
		 flags += b2Draw::e_shapeBit;
		 flags += b2Draw::e_jointBit;
		 flags += b2Draw::e_aabbBit;
		 flags += b2Draw::e_pairBit;
		 flags += b2Draw::e_centerOfMassBit;
		 m_debugDraw->SetFlags(flags);		
		 //*/
	
		CCSprite *sprite = CCSprite::spriteWithFile("bg.png");	//背景图
		sprite->setAnchorPoint(CCPointZero);
		this->addChild(sprite, -1);
    
		sprite = CCSprite::spriteWithFile("catapult_base_2.png");	//投射器底部后面那块
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(CCPointMake(181.0, FLOOR_HEIGHT));
		this->addChild(sprite, 0);
    
		sprite = CCSprite::spriteWithFile("squirrel_1.png");		//左边松鼠
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(CCPointMake(11.0, FLOOR_HEIGHT));
		this->addChild(sprite, 0);
    
		sprite = CCSprite::spriteWithFile("catapult_base_1.png");	//投射器底部前面那块
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(CCPointMake(181.0, FLOOR_HEIGHT));
		this->addChild(sprite, 9);
    
		sprite = CCSprite::spriteWithFile("squirrel_2.png");	//右边松鼠
		sprite->setAnchorPoint(CCPointZero);
		sprite->setPosition(CCPointMake(240.0, FLOOR_HEIGHT));
		this->addChild(sprite, 9);
    
		sprite = CCSprite::spriteWithFile("fg.png");	//带冰的地面
		sprite->setAnchorPoint(CCPointZero);
		this->addChild(sprite, 10);
    
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0, 0); 
		m_groundBody = m_world->CreateBody(&groundBodyDef);

		// bottom
		b2EdgeShape groundBox;

		groundBox.Set(b2Vec2(0,FLOOR_HEIGHT/PTM_RATIO), b2Vec2(screenSize.width*2.0f/PTM_RATIO,FLOOR_HEIGHT/PTM_RATIO));
		m_groundBody->CreateFixture(&groundBox, 0);
		// top
		groundBox.Set(b2Vec2(0,screenSize.height/PTM_RATIO), b2Vec2(screenSize.width*2.0f/PTM_RATIO,screenSize.height/PTM_RATIO));
		m_groundBody->CreateFixture(&groundBox, 0);
		// left
		groundBox.Set(b2Vec2(0,screenSize.height/PTM_RATIO), b2Vec2(0,0));
		m_groundBody->CreateFixture(&groundBox, 0);
		// right
		groundBox.Set(b2Vec2(screenSize.width*1.5f/PTM_RATIO,screenSize.height/PTM_RATIO), b2Vec2(screenSize.width*1.5f/PTM_RATIO,0));
		m_groundBody->CreateFixture(&groundBox, 0);
    
		// Create the catapult's arm
		CCSprite *arm = CCSprite::spriteWithFile("catapult_arm.png");
//		arm->setAnchorPoint(CCPointZero);
//		arm->setPosition(ccp(230.0f, (FLOOR_HEIGHT+91.0f)));
		this->addChild(arm, 1);
    
		b2BodyDef armBodyDef;
		armBodyDef.type = b2_dynamicBody;
		armBodyDef.linearDamping = 1;
		armBodyDef.angularDamping = 1;
		armBodyDef.position.Set(230.0f/PTM_RATIO, (FLOOR_HEIGHT+91.0f)/PTM_RATIO);
		armBodyDef.userData = arm;
		m_armBody = m_world->CreateBody(&armBodyDef);
    
		b2PolygonShape armBox;
		b2FixtureDef armBoxDef;
		armBoxDef.shape = &armBox;
		armBoxDef.density = 0.3F;
		armBox.SetAsBox(11.0f/PTM_RATIO, 91.0f/PTM_RATIO);
		m_armFixture = m_armBody->CreateFixture(&armBoxDef);
    
		b2RevoluteJointDef armJointDef;
		armJointDef.Initialize(m_groundBody, m_armBody, b2Vec2(233.0f/PTM_RATIO, FLOOR_HEIGHT/PTM_RATIO));
		armJointDef.enableMotor = true;
		armJointDef.enableLimit = true;
		armJointDef.motorSpeed  = -10; //-1260;
		armJointDef.lowerAngle  = CC_DEGREES_TO_RADIANS(9);
		armJointDef.upperAngle  = CC_DEGREES_TO_RADIANS(75);
		armJointDef.maxMotorTorque = 700;
		m_armJoint = (b2RevoluteJoint*)m_world->CreateJoint(&armJointDef);
 /*   
		m_mouseJoint = NULL;

		CCDelayTime *delayAction = CCDelayTime::actionWithDuration(0.5f);
		CCCallFunc *callSelectorAction = CCCallFunc::actionWithTarget(this, callfunc_selector(HelloWorld::resetGame));
		this->runAction(CCSequence::actions(delayAction,
			callSelectorAction,
			NULL));
*/
		schedule(schedule_selector(Catapult::tick));


		bRet = true;
	} while (0);
	return bRet;	
}

void Catapult::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	if (m_mouseJoint != NULL)	{		return;		}

	CCTouch *touch = (CCTouch *)touches->anyObject();
	CCPoint location = touch->locationInView(touch->view());
	location = CCDirector::sharedDirector()->convertToGL(location);
	b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);

	if (locationWorld.x < m_armBody->GetWorldCenter().x + 150.0/PTM_RATIO)
	{
		b2MouseJointDef md;
		md.bodyA = m_groundBody;
		md.bodyB = m_armBody;
		md.target = locationWorld;
		md.maxForce = 2000;

		m_mouseJoint = (b2MouseJoint *)m_world->CreateJoint(&md);
	}
}

void Catapult::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	if (m_mouseJoint == NULL)	{	return;	}

	CCTouch *touch = (CCTouch *)touches->anyObject();
	CCPoint location = touch->locationInView(touch->view());
	location = CCDirector::sharedDirector()->convertToGL(location);
	b2Vec2 locationWorld = b2Vec2(location.x/PTM_RATIO, location.y/PTM_RATIO);

	m_mouseJoint->SetTarget(locationWorld);
}

void Catapult::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	if (m_mouseJoint != NULL)
	{
		if (m_armJoint->GetJointAngle() >= CC_DEGREES_TO_RADIANS(5))
		{
//			m_releasingArm = true;
		}

		m_world->DestroyJoint(m_mouseJoint);
		m_mouseJoint = NULL;
		return;
	}
}


void Catapult::draw()
{
	// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	// Needed states:  GL_VERTEX_ARRAY, 
	// Unneeded states: GL_TEXTURE_2D, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

//	m_world->DrawDebugData();

	// restore default GL states
	glEnable(GL_TEXTURE_2D);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
}


void Catapult::tick(ccTime dt)
{
	int velocityIterations = 8;
	int positionIterations = 1;

	m_world->Step(dt, velocityIterations, positionIterations);

	//Iterate over the bodies in the physics world
	for (b2Body* b = m_world->GetBodyList(); b; b = b->GetNext())
	{
		if (b->GetUserData() != NULL) {
			//Synchronize the AtlasSprites position and rotation with the corresponding body
			CCSprite* myActor = (CCSprite*)b->GetUserData();
			myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
			myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
		}	
	}

	/*
	// Arm is being released
	if (m_releasingArm && m_bulletJoint != NULL)
	{
		// Check if the arm reached the end so we can return the limits
		if (m_armJoint->GetJointAngle() <= CC_DEGREES_TO_RADIANS(10))
		{
			m_releasingArm = false;

			// Destroy joint so the bullet will be free
			m_world->DestroyJoint(m_bulletJoint);
			m_bulletJoint = NULL;

		}
	}

	// Bullet is moving.
	if (m_bulletBody && m_bulletJoint == NULL)
	{
		b2Vec2 position = m_bulletBody->GetPosition();
		CCPoint myPosition = this->getPosition();
		CCSize screenSize = CCDirector::sharedDirector()->getWinSize();

		// Move the camera.
		if (position.x > screenSize.width / 2.0f / PTM_RATIO)
		{
			myPosition.x = -MIN(screenSize.width * 2.0f - screenSize.width, position.x * PTM_RATIO - screenSize.width / 2.0f);
			this->setPosition(myPosition);
		}
	}
	*/
}