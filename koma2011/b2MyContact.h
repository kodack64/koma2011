
#pragma once
#include "GameCommon.h"
#include "b2Object.h"

class MyContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* cont){
		b2Body* body1=cont->GetFixtureA()->GetBody();
		b2Body* body2=cont->GetFixtureB()->GetBody();
		b2Object* obj1=(b2Object*)(body1->GetUserData());
		b2Object* obj2=(b2Object*)(body2->GetUserData());
		if(obj1!=NULL && obj2!=NULL){
			if(obj1->match==obj2->match && obj1->match>=0 && body1!=body2){
				b2Vec2 v1=body1->GetPosition();
				b2Vec2 v2=body2->GetPosition();
//				if(M2P((v1-v2).Length())<obj1->size*2){
					if(!(obj1->light)){obj1->light=true;obj1->lightCount=0;}
					if(!(obj2->light)){obj2->light=true;obj2->lightCount=0;}
//				}
			}
		}
	}
	void EndContact(b2Contact* contact){
	}
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold){
	}
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){ 
	}
};