#include "MyContackListener.h"

MyContactListener::MyContactListener():contacts()
{}
MyContactListener::~MyContactListener()
{}

void MyContactListener::BeginContact(b2Contact* contact)
{}

void MyContactListener::EndContact(b2Contact* contact)
{}

void MyContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{}

void MyContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	bool isAEnemy = (contact->GetFixtureA()->GetUserData()!=NULL);
	bool isBEnemy = (contact->GetFixtureB()->GetUserData()!=NULL);
	if(isAEnemy||isBEnemy)
	{
		int count = contact->GetManifold()->pointCount;
		float maxImpulse = 0.0f;
		//ȡ��ײ�������ֵ
		for (int i=0;i<count;i++)
		{
			maxImpulse = b2Max(maxImpulse,impulse->normalImpulses[i]);
		}
		//�����ײ������1����ݻٵ���
		if(maxImpulse>1.0f)
		{
			if (isAEnemy)
			{
				contacts.insert(contact->GetFixtureA()->GetBody());
			}
			if (isBEnemy)
			{
				contacts.insert(contact->GetFixtureB()->GetBody());
			}
		}
	}
}