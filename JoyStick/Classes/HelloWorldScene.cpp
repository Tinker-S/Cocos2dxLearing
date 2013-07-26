#include "HelloWorldScene.h"

using namespace cocos2d;

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

		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			this,
			menu_selector(HelloWorld::menuCloseCallback));
		CC_BREAK_IF(! pCloseItem);

		pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

		CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
		pMenu->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu);

		this->addChild(pMenu, 1);


		//取得屏幕大小
		CCSize size = CCDirector::sharedDirector()->getWinSize();

		//创建飞机
		plane = CCSprite::create("plane.png");
		CC_BREAK_IF(!plane);
		//将飞机添加进布景并设置位置
		plane->setPosition(ccp(size.width/2, size.height/2));
		this->addChild(plane, 0);

		//创建摇杆下面部分
		CCSprite *joystick1=CCSprite::create("joystick1.png");
		//设置透明度，锚点，位置
		joystick1->setOpacity(191);
		joystick1->setAnchorPoint(ccp(0,0));
		joystick1->setPosition(ccp(0,0));
		//大圆半径
		R=joystick1->getContentSize().width/2;
		//中心点
		O=ccp(R,R);
		//添加进布景
		this->addChild(joystick1,1);

		//创建摇杆上面圆圈部分
		joystick=CCSprite::create("joystick2.png");
		//设置位置为摇杆中心点并添加进布景
		joystick->setPosition(ccp(O.x,O.y));
		this->addChild(joystick,2);

		//设置可触摸
		this->setTouchEnabled(true);

		//每帧要执行的函数
		this->schedule(schedule_selector(HelloWorld::flying));

		//初始化需要的变量
		isFlying=false;
		speedX=speedY=0;

		bRet = true;
	} while (0);

	return bRet;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();
}

//飞机飞行函数
void HelloWorld::flying(float dt)
{
	if (isFlying&&(speedX!=0||speedY!=0)){
		//飞机飞行
		CCPoint position=ccp(plane->getPosition().x+speedX,plane->getPosition().y+speedY);

		CCSize size=CCDirector::sharedDirector()->getWinSize();
		CCRect rect=CCRectMake(0,0,size.width,size.height);

		//判断飞机飞行的位置是否在屏幕内
		if(rect.containsPoint(position)){
			plane->setPosition(position);
		}

	}
}

//触摸开始函数，判断触摸开始点是否在圆圈内，若是，则设置isFlying标志为true
void HelloWorld::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCPoint location = touch->getLocation();

	CCRect rect=joystick->boundingBox();
	if (rect.containsPoint(location))
	{
		isFlying=true;
	}
}

//触摸滑动函数
void HelloWorld::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
{
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCPoint location = touch->getLocation();

	//判断触摸滑动点是否在摇杆范围内
	bool inRange=pow(O.x-location.x,2)+pow(O.y-location.y,2)<pow(R,2);

	if(isFlying&&inRange){
		CCPoint position=plane->getPosition();
		joystick->setPosition(location);

		float r=R*2/6;
		float d=R*2/3;

		speedX=(location.x-O.x)/15;
		speedY=(location.y-O.y)/15;
	}
}

//触摸结束函数
void HelloWorld::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
	isFlying=false;

	CCFiniteTimeAction* actionMove = CCMoveTo::create(0.2f, O);			//joystick回到初始位置
	joystick->runAction( CCSequence::actions(actionMove, NULL, NULL) );

	speedX=speedY=0;
}