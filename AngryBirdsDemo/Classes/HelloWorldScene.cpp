#include "HelloWorldScene.h"

using namespace cocos2d;

//PTM_RATIO��box2d�е�λ�׺�����ת������
#define PTM_RATIO 32.0

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        scene->addChild(layer);
    } while (0);

    return scene;
}

bool HelloWorld::init()
{
    bool bRet = false;
    do 
    {
        CC_BREAK_IF(! CCLayer::init());

		CCSize size = CCDirector::sharedDirector()->getWinSize();
		setTouchEnabled(true);
		r=60;			//����������������ĳ���
		canFly=false;
		isDraging=false;

		//��������
		CCSprite *bg = CCSprite::create("background.png");
		ratio=size.height/bg->getContentSize().height;		//��ȡ����ͼ�߶Ⱥ���Ļ�߶ȵı���
		bg->setAnchorPoint(CCPointZero);
		bg->setPosition(CCPointZero);
		bg->setScaleY(ratio);			//������ͼ���ŵ�������Ļ
		this->addChild(bg,-1);

		//����ͼƬ��
		batch=CCSpriteBatchNode::create("item.png");
		this->addChild(batch);
		CCTexture2D *batchTexture = batch->getTexture();

		//��ʼ������
		CCSprite *slingshot1 = CCSprite::createWithTexture(batchTexture,CCRectMake(1,1,38,199));
		slingshot1->setScale(ratio*0.7);
		slingshot1->setPosition(ccp(90,ratio*130));
		batch->addChild(slingshot1);

		CCSprite *slingshot2 = CCSprite::createWithTexture(batchTexture,CCRectMake(814,1,43,124));
		slingshot2->setScale(ratio*0.7);
		slingshot2->setPosition(ccp(78,ratio*155));
		batch->addChild(slingshot2);

		//��ʼ��С��
		bird=CCSprite::createWithTexture(batchTexture,CCRectMake(1,373,31,30));
		bird->setScale(ratio);
		bird->setPosition(ccp((slingshot1->getPositionX()+slingshot2->getPositionX())/2,slingshot1->getPositionY()*1.4));
		center=bird->getPosition();
		this->addChild(bird);

		//������������
		b2Vec2 gravity;
		gravity.Set(0.0f,-10.0f);
		world = new b2World(gravity);
		world->SetAllowSleeping(true);
		world->SetContinuousPhysics(true);

		//��������ı߽� 
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0,0);

		b2Body *groundBody = world->CreateBody(&groundBodyDef);

		b2EdgeShape groundBox;
		CCSize s = CCSize(bg->getContentSize().width,bg->getContentSize().height*ratio);
		//�ײ��߽�
		groundBox.Set(b2Vec2(0,slingshot2->getPositionY()/PTM_RATIO/2.1),b2Vec2(s.width/PTM_RATIO,slingshot2->getPositionY()/PTM_RATIO/2.1));
		groundBody->CreateFixture(&groundBox,0);
		//�����߽�
		groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO));  
		groundBody->CreateFixture(&groundBox,0); 
		//��ߵı߽�
		groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(0,0));  
		groundBody->CreateFixture(&groundBox,0);  
		//�ұߵı߽�
		groundBox.Set(b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,0));  
		groundBody->CreateFixture(&groundBox,0);

		scheduleUpdate();
		//CCFollow:��һ�����������һ�������˶�
		//��һ���ǽ�layer��bird�󶨣��Ӷ�ʵ��������������ĵ�Ч��
		this->runAction(CCFollow::create(bird,CCRectMake(0,0,bg->getContentSize().width,bg->getContentSize().height*ratio)));

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCPoint pt1 = touch->locationInView();
	pt1 = CCDirector::sharedDirector()->convertToGL(pt1);
	if (CCRect::CCRectContainsPoint(bird->boundingBox(),pt1)==false)
	{
		isDraging=false;
	}
	else
	{
		isDraging=true;
	}
}

void HelloWorld::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	if (!isDraging)
	{
		return;
	}
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCPoint pt1 = touch->locationInView();
	pt1 = CCDirector::sharedDirector()->convertToGL(pt1);

	canFly=true;
	float distance = ccpDistance(pt1,center);
	if (distance>r)
	{
		//���紥�����λ�þ��뵯��̫Զ����С��̶��ں͵�������Ϊr��λ��
		CCPoint pos;
		float cos = (pt1.x-center.x)/distance;
		float sin = (pt1.y-center.y)/distance;
		pos.x=center.x+r*cos;
		pos.y=center.y+r*sin;
		bird->setPosition(pos);
	}else
	{
		bird->setPosition(pt1);
	}
}

void HelloWorld::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	isDraging=false;
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCPoint pt1 = touch->locationInView();
	pt1 = CCDirector::sharedDirector()->convertToGL(pt1);
	if (canFly)
	{
		canFly=false;
	}
	else
	{
		return;
	}
	float distance = ccpDistance(pt1,center);
	float cos = (pt1.x-center.x)/distance;
	float sin = (pt1.y-center.y)/distance;

	//����С�������
	b2CircleShape birdShape;
	birdShape.m_radius = ratio*bird->getContentSize().width/PTM_RATIO/2;

	b2FixtureDef birdFixtureDef;
	birdFixtureDef.shape=&birdShape;
	birdFixtureDef.density=1;
	birdFixtureDef.friction=0.3;
	birdFixtureDef.restitution=0.1;

	b2BodyDef birdBodyDef;
	birdBodyDef.type=b2_dynamicBody;
	birdBodyDef.position.Set(bird->getPositionX()/PTM_RATIO,bird->getPositionY()/PTM_RATIO);

	b2Body *birdBody = world->CreateBody(&birdBodyDef);
	birdBody->SetUserData(bird);
	birdBody->CreateFixture(&birdFixtureDef);
	//�ͷ�ʱ��С��һ�����ŵ������ĵ���
	birdBody->ApplyForce(b2Vec2(-5.5f*distance*cos,-5.5f*distance*sin),birdBody->GetWorldCenter());
	birdBody->SetLinearDamping(0.2f);
}

void HelloWorld::update(float dt)
{
	int velocityIterations = 8;
	int positionIterations = 8;
	world->Step(dt,velocityIterations,positionIterations);
	for (b2Body *b = world->GetBodyList();b;b=b->GetNext())
	{
		CCSprite *myActor = (CCSprite*)b->GetUserData();
		if (myActor)
		{
			myActor->setPosition(CCPointMake(b->GetPosition().x*PTM_RATIO,b->GetPosition().y*PTM_RATIO));
			myActor->setRotation(-1*CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}
	static float countTime = 0.0f;
	countTime+=dt;
	//ÿ��һ��ʱ����С��ĵط���һ���ۼ�
	if (countTime<0.08f)
	{
		return;
	}
	//ֻ��С�������˵�����Χ���ܿ�ʼ���ۼ�
	if ((bird->getPositionX()<center.x+r)&&(bird->getPositionY()<center.y+r))
	{
		return;
	}
	
	//С����غ�Ͳ�����Ҫ���ۼ�
	if(bird->getPositionY()-60.0f<5.0f)
	{
		return;
	}

	countTime=0;
	CCSprite *route = CCSprite::spriteWithTexture(batch->getTexture(),CCRectMake(0,345,35,33));
	route->setScale(0.4*ratio);
	batch->addChild(route);
	route->setPosition(bird->getPosition());

	//�ۼ����ֺ󽥽���ʧ
	CCActionInterval* action = CCFadeOut::create(3.0f);
	route->runAction(action);
}