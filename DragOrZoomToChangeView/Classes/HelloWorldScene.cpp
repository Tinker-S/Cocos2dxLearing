#include "HelloWorldScene.h"

using namespace cocos2d;

const double eps=1e-7;

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

		this->setTouchEnabled(true);

		sprite = CCSprite::spriteWithFile("bg.jpg");
		sprite->setAnchorPoint(CCPointZero);
		this->addChild(sprite,-1);
		dis_now=0.0f;
		dis_pre=0.0f;
		drag=false;

		bRet = true;
	} while (0);

	return bRet;
}

void HelloWorld::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch*)(*pTouches->begin());
	startPoint= (*touch).getLocationInView();
}

void HelloWorld::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
	if (pTouches->count()==1)
	{
		drag=true;
		CCTouch *touch = (CCTouch*)(*pTouches->begin());
		endPoint=touch->locationInView();
		CCPoint scenePoint = this->getPosition();		//获取场景的位置

		float moveposition = scenePoint.x+(endPoint.x-startPoint.x)/3.0f;
		if (moveposition<-sprite->getContentSize().width/2)		//场景移动距离不能超过背景图片的尺寸
		{
			moveposition=-sprite->getContentSize().width/2;
		}
		if (moveposition>0)		//场景不能再向左边移动
		{
			moveposition=0;
		}
		this->setPosition(ccp(moveposition,scenePoint.y));
	}
	else if (pTouches->count()>=2)
	{
		drag=false;
		CCTouch *touch1;
		CCTouch *touch2;

		int i=0;
		for( CCSetIterator iterTouch = pTouches->begin(); iterTouch != pTouches->end(); iterTouch++ )
		{
			if( i == 0 )
			{
				touch1 = (CCTouch*)*iterTouch;
			}else if( i == 1 )
			{
				touch2 = (CCTouch*)*iterTouch;
				break;
			}
			i++;
		}

		CCPoint position1 = touch1->getLocation();
		CCPoint position2 = touch2->getLocation();

		dis_now=fabs(position1.x-position2.x);

		if (dis_pre==0.0f)
		{
			dis_pre=dis_now;
			return;
		}
		float dis = dis_now-dis_pre;
		if (dis<0.0f)
		{
			this->setScale(this->getScale()-0.05f);
		}
		else
		{
			this->setScale(this->getScale()+0.05f);
		}

		dis_pre=dis_now;
	}
}

void HelloWorld::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	if (drag)
	{
		endPoint= (*(CCTouch*)(*pTouches->begin())).getLocationInView();

		CCPoint scenePoint = this->getPosition();

		float moveposition = scenePoint.x+(endPoint.x-startPoint.x)*5.0f;
		if (moveposition<-sprite->getContentSize().width/2)
		{
			moveposition=-sprite->getContentSize().width/2;
		}
		if (moveposition>0)
		{
			moveposition=0;
		}

		CCFiniteTimeAction *action = CCMoveTo::actionWithDuration(0.2f,ccp(moveposition,scenePoint.y));
		this->runAction(action);
	}
}