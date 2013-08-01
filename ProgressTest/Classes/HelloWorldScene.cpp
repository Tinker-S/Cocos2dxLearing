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

		//����һ������
		CCSprite *psSprite = CCSprite::create("green.png");

		//���þ��鴴����������������һЩ����
		progresstime = CCProgressTimer::create(psSprite);		//��ʼ��CCProgressTimer
		progresstime->setPercentage(0.0f);						//���ó�ʼ�ٷֱȵ�ֵ
		progresstime->setScale(0.5f);							//���ý�������СΪԭʼ��һ��
		progresstime->setBarChangeRate(ccp(1,0));				//���ý������ĳ��Ⱥ͸߶ȿ�ʼ�仯�Ĵ�С
		progresstime->setType(kCCProgressTimerTypeBar);			//���ý�����Ϊˮƽ
		progresstime->setPosition(ccp(size.width/2,size.height/2));		//���ý�����λ��
		progresstime->setMidpoint(ccp(0,0));			//�������ң����ϣ����»��м�����ͷ�ƶ��ģ���ccp��0,0�����ʾ�����ҵ����ӣ�ccp��1,0����ʾ���ҵ��������  ccp��0,1����ʾ���µ��ϵ����� ccp��0.5f��0����ʾ���м�����������ͷ���ӣ����кܶ࣬������ô������

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

