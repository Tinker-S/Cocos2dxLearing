#include "HelloWorldScene.h"

using namespace cocos2d;

//PTM_RATIO是box2d中单位米和像素转换比率
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
		r=60;			//弹弓的最大可以拉伸的长度
		canFly=false;
		isDraging=false;

		//创建背景
		CCSprite *bg = CCSprite::create("background.png");
		ratio=size.height/bg->getContentSize().height;		//获取背景图高度和屏幕高度的比例
		bg->setAnchorPoint(CCPointZero);
		bg->setPosition(CCPointZero);
		bg->setScaleY(ratio);			//将背景图缩放到充满屏幕
		this->addChild(bg,-1);

		//加入图片集
		batch=CCSpriteBatchNode::create("item.png");
		this->addChild(batch);
		CCTexture2D *batchTexture = batch->getTexture();

		//初始化弹弓
		CCSprite *slingshot1 = CCSprite::createWithTexture(batchTexture,CCRectMake(1,1,38,199));
		slingshot1->setScale(ratio*0.7);
		slingshot1->setPosition(ccp(90,ratio*130));
		batch->addChild(slingshot1);

		CCSprite *slingshot2 = CCSprite::createWithTexture(batchTexture,CCRectMake(814,1,43,124));
		slingshot2->setScale(ratio*0.7);
		slingshot2->setPosition(ccp(78,ratio*155));
		batch->addChild(slingshot2);

		//初始化小鸟
		bird=CCSprite::createWithTexture(batchTexture,CCRectMake(1,373,31,30));
		bird->setScale(ratio);
		bird->setPosition(ccp((slingshot1->getPositionX()+slingshot2->getPositionX())/2,slingshot1->getPositionY()*1.4));
		center=bird->getPosition();
		this->addChild(bird);

		//定义物理世界
		b2Vec2 gravity;
		gravity.Set(0.0f,-10.0f);
		world = new b2World(gravity);
		world->SetAllowSleeping(true);
		world->SetContinuousPhysics(true);

		//设置世界的边界 
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(0,0);

		b2Body *groundBody = world->CreateBody(&groundBodyDef);

		b2EdgeShape groundBox;
		CCSize s = CCSize(bg->getContentSize().width,bg->getContentSize().height*ratio);
		//底部边界
		groundBox.Set(b2Vec2(0,slingshot2->getPositionY()/PTM_RATIO/2.1),b2Vec2(s.width/PTM_RATIO,slingshot2->getPositionY()/PTM_RATIO/2.1));
		groundBody->CreateFixture(&groundBox,0);
		//顶部边界
		groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO));  
		groundBody->CreateFixture(&groundBox,0); 
		//左边的边界
		groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(0,0));  
		groundBody->CreateFixture(&groundBox,0);  
		//右边的边界
		groundBox.Set(b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,0));  
		groundBody->CreateFixture(&groundBox,0);

		scheduleUpdate();
		//CCFollow:让一个物体跟随另一个物体运动
		//这一步是将layer与bird绑定，从而实现类似摄像机跟拍的效果
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
		//假如触摸点的位置距离弹弓太远，则将小鸟固定在和弹弓距离为r的位置
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

	//设置小鸟的属性
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
	//释放时给小鸟一个向着弹弓中心的力
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
	//每隔一段时间在小鸟的地方画一个痕迹
	if (countTime<0.08f)
	{
		return;
	}
	//只有小鸟脱离了弹弓周围才能开始画痕迹
	if ((bird->getPositionX()<center.x+r)&&(bird->getPositionY()<center.y+r))
	{
		return;
	}
	
	//小鸟落地后就不再需要画痕迹
	if(bird->getPositionY()-60.0f<5.0f)
	{
		return;
	}

	countTime=0;
	CCSprite *route = CCSprite::spriteWithTexture(batch->getTexture(),CCRectMake(0,345,35,33));
	route->setScale(0.4*ratio);
	batch->addChild(route);
	route->setPosition(bird->getPosition());

	//痕迹出现后渐渐消失
	CCActionInterval* action = CCFadeOut::create(3.0f);
	route->runAction(action);
}