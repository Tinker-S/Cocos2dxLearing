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


		//ȡ����Ļ��С
		CCSize size = CCDirector::sharedDirector()->getWinSize();

		//�����ɻ�
		plane = CCSprite::create("plane.png");
		CC_BREAK_IF(!plane);
		//���ɻ���ӽ�����������λ��
		plane->setPosition(ccp(size.width/2, size.height/2));
		this->addChild(plane, 0);

		//����ҡ�����沿��
		CCSprite *joystick1=CCSprite::create("joystick1.png");
		//����͸���ȣ�ê�㣬λ��
		joystick1->setOpacity(191);
		joystick1->setAnchorPoint(ccp(0,0));
		joystick1->setPosition(ccp(0,0));
		//��Բ�뾶
		R=joystick1->getContentSize().width/2;
		//���ĵ�
		O=ccp(R,R);
		//��ӽ�����
		this->addChild(joystick1,1);

		//����ҡ������ԲȦ����
		joystick=CCSprite::create("joystick2.png");
		//����λ��Ϊҡ�����ĵ㲢��ӽ�����
		joystick->setPosition(ccp(O.x,O.y));
		this->addChild(joystick,2);

		//���ÿɴ���
		this->setTouchEnabled(true);

		//ÿ֡Ҫִ�еĺ���
		this->schedule(schedule_selector(HelloWorld::flying));

		//��ʼ����Ҫ�ı���
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

//�ɻ����к���
void HelloWorld::flying(float dt)
{
	if (isFlying&&(speedX!=0||speedY!=0)){
		//�ɻ�����
		CCPoint position=ccp(plane->getPosition().x+speedX,plane->getPosition().y+speedY);

		CCSize size=CCDirector::sharedDirector()->getWinSize();
		CCRect rect=CCRectMake(0,0,size.width,size.height);

		//�жϷɻ����е�λ���Ƿ�����Ļ��
		if(rect.containsPoint(position)){
			plane->setPosition(position);
		}

	}
}

//������ʼ�������жϴ�����ʼ���Ƿ���ԲȦ�ڣ����ǣ�������isFlying��־Ϊtrue
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

//������������
void HelloWorld::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
{
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	CCPoint location = touch->getLocation();

	//�жϴ����������Ƿ���ҡ�˷�Χ��
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

//������������
void HelloWorld::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
	isFlying=false;

	CCFiniteTimeAction* actionMove = CCMoveTo::create(0.2f, O);			//joystick�ص���ʼλ��
	joystick->runAction( CCSequence::actions(actionMove, NULL, NULL) );

	speedX=speedY=0;
}