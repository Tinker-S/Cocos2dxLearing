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
		for (int i=0;i<count;i++)
		{
			maxImpulse = b2Max(maxImpulse,impulse->normalImpulses[i]);
		}
		if(maxImpulse>1.0f)
		{
			printf("in maxImpulse\n");
			if (isAEnemy)
			{
				contacts.insert(contact->GetFixtureA()->GetBody());
				printf("contacts insert EnemyA\n");
			}
			if (isBEnemy)
			{
				contacts.insert(contact->GetFixtureB()->GetBody());
				printf("contacts insert EnemyB\n");
			}
		}
	}
}