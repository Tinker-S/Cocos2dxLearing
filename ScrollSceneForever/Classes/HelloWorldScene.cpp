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

		//������������
		CCSprite* bg_1 = CCSprite::create("11.png");
		CCSprite* bg_2 = CCSprite::create("22.png");

		//����ê��Ϊ���½�
		bg_1->setAnchorPoint(CCPointZero);
		bg_2->setAnchorPoint(CCPointZero);

		//����λ��
		bg_1->setPosition(CCPointZero);
		bg_2->setPosition(ccp(0,480));

		//��ӽ����������ֱ�����tagΪ11��22
		this->addChild(bg_1,1,11);
		this->addChild(bg_2,1,22);

		flag = true;

		//��ʼÿ֡�Ļص����������ڱ����Ĺ���
		scheduleUpdate();

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::update(float dt)
{
	//ȡ��������������
	CCSprite* bg_1 = (CCSprite*)getChildByTag(11);
	CCSprite* bg_2 = (CCSprite*)getChildByTag(22);

	//����ͼƬ�������ƶ�
	bg_1->setPositionY(bg_1->getPositionY() - 2);
	bg_2->setPositionY(bg_2->getPositionY() - 2);

	if(flag&&bg_2->getPositionY()<=0)			//���bg_2����ײ�����bg_1���õ�����
	{
		flag=false;
		bg_1->setPositionY(480);
	}
	else if (!flag&&bg_1->getPositionY()<=0)		//���bg_1����ײ�����bg_2���õ�����
	{
		flag=true;
		bg_2->setPositionY(480);
	}
	
}