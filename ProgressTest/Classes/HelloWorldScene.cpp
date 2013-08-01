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
		
		CCSize size=CCDirector::sharedDirector()->getWinSize();;

		//创建一个精灵
		CCSprite *psSprite = CCSprite::create("green.png");

		//利用精灵创建进度条，并设置一些属性
		progresstime = CCProgressTimer::create(psSprite);		//初始化CCProgressTimer
		progresstime->setPercentage(0.0f);						//设置初始百分比的值
		progresstime->setScale(0.5f);							//设置进度条大小为原始的一半
		progresstime->setBarChangeRate(ccp(1,0));				//设置进度条的长度和高度开始变化的大小
		progresstime->setType(kCCProgressTimerTypeBar);			//设置进度条为水平
		progresstime->setPosition(ccp(size.width/2,size.height/2));		//放置进度条位置
		progresstime->setMidpoint(ccp(0,0));			//向左，向右，向上，向下或中间像两头移动的，如ccp（0,0）这表示从左到右的增加，ccp（1,0）表示从右到左的增加  ccp（0,1）表示从下到上的增加 ccp（0.5f，0）表示从中间向左向右两头增加，还有很多，看你怎么设置了

		this->addChild(progresstime,100);
		this->scheduleUpdate();

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

void HelloWorld::update(float dt)
{
	float ct = progresstime->getPercentage();

	ct+=0.5f;

	if(ct<=100)
	{
		progresstime->setPercentage(ct);
	}
	else
	{
		CCLog("done");
		unscheduleAllSelectors();
	}
}

