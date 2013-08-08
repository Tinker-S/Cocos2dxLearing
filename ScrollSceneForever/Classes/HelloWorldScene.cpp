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

		//创建两个背景
		CCSprite* bg_1 = CCSprite::create("11.png");
		CCSprite* bg_2 = CCSprite::create("22.png");

		//设置锚点为左下角
		bg_1->setAnchorPoint(CCPointZero);
		bg_2->setAnchorPoint(CCPointZero);

		//设置位置
		bg_1->setPosition(CCPointZero);
		bg_2->setPosition(ccp(0,480));

		//添加进布景，并分别设置tag为11和22
		this->addChild(bg_1,1,11);
		this->addChild(bg_2,1,22);

		flag = true;

		//开始每帧的回调方法，用于背景的滚动
		scheduleUpdate();

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::update(float dt)
{
	//取得两个背景精灵
	CCSprite* bg_1 = (CCSprite*)getChildByTag(11);
	CCSprite* bg_2 = (CCSprite*)getChildByTag(22);

	//两张图片逐渐向下移动
	bg_1->setPositionY(bg_1->getPositionY() - 2);
	bg_2->setPositionY(bg_2->getPositionY() - 2);

	if(flag&&bg_2->getPositionY()<=0)			//如果bg_2到达底部，则将bg_1放置到顶部
	{
		flag=false;
		bg_1->setPositionY(480);
	}
	else if (!flag&&bg_1->getPositionY()<=0)		//如果bg_1到达底部，则将bg_2放置到顶部
	{
		flag=true;
		bg_2->setPositionY(480);
	}
	
}