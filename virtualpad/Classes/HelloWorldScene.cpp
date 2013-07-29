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

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

		//ȡ����Ļ��С
		CCSize size = CCDirector::sharedDirector()->getWinSize();

		//�����ɻ�
		plane = CCSprite::create("plane.png");
		CC_BREAK_IF(!plane);
		//���ɻ���ӽ�����������λ��
		plane->setPosition(ccp(size.width/2, size.height/2));
		this->addChild(plane, 0);

		m_texture_normal = CCTextureCache::sharedTextureCache()->addImage("btn_normal.png");
		m_texture_normal->retain();
		m_texture_down = CCTextureCache::sharedTextureCache()->addImage("btn_down.png");
		m_texture_down->retain();
		m_texture_up = CCTextureCache::sharedTextureCache()->addImage("btn_up.png");
		m_texture_up->retain();
		m_texture_left = CCTextureCache::sharedTextureCache()->addImage("btn_left.png");
		m_texture_left->retain();
		m_texture_right = CCTextureCache::sharedTextureCache()->addImage("btn_right.png");
		m_texture_right->retain();

		virtualpad = CCSprite::create("btn_normal.png");
		virtualpad->setOpacity(191);
		virtualpad->setAnchorPoint(ccp(0,0));
		virtualpad->setPosition(ccp(0,0));
		//��Բ�뾶
		R=virtualpad->getContentSize().width/2;
		//���ĵ�
		O=ccp(R,R);
		//��ӽ�����
		this->addChild(virtualpad,1);

		this->setTouchEnabled(true);

		this->schedule(schedule_selector(HelloWorld::flying));
		isFlying=false;
		speedX=speedY=0;

        bRet = true;
    } while (0);

    return bRet;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}

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

	CCRect rect=virtualpad->boundingBox();
	if (rect.containsPoint(location))
	{
		isFlying=true;
	}

	//�жϴ����������Ƿ���ҡ�˷�Χ��
	bool inRange=pow(O.x-location.x,2)+pow(O.y-location.y,2)<pow(R,2);

	if(isFlying&&inRange){
		CCPoint position=plane->getPosition();
		
		speedX=location.x-O.x;
		speedY=location.y-O.y;

		if(speedX>speedY&&speedX+speedY>0)			//����
		{
			virtualpad->setTexture(m_texture_right);
			speedX=5;
			speedY=0;
		}
		else if(speedX<speedY&&speedX+speedY>0)		//����
		{
			virtualpad->setTexture(m_texture_up);
			speedX=0;
			speedY=5;
		}
		else if(speedX>speedY&&speedX+speedY<0)		//����
		{
			virtualpad->setTexture(m_texture_down);
			speedX=0;
			speedY=-5;
		}
		else if(speedX<speedY&&speedX+speedY<0)		//����
		{
			virtualpad->setTexture(m_texture_left);
			speedX=-5;
			speedY=0;
		}
	}
}

//���������Ļص�����
void HelloWorld::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
	virtualpad->setTexture(m_texture_normal);
	isFlying=false;
	speedX=speedY=0;
}
