
#pragma once

#include "GameCommon.h"

using namespace std;

enum b2ObjectKind{
	Circle,
	Square
};

class b2Object{
public:
	int id;
	int kind;
	float rot;
	vector <b2Vec2> trace;
	b2FixtureDef fixtureDef;
	b2BodyDef bodyDef;
	b2CircleShape circleShape;
	b2PolygonShape polygonShape;
	ImageId myImage;
	Color4f myColor;
	Color4f toColor;
	bool light;
	int lightCount;
	int darkCount;
	float zoom;
	int match;
	GraphicApi *gapi;
	double size;

	b2Object(GraphicApi* _gapi){
		gapi=_gapi;
	}

	virtual void InitAsCursor(ImageId img){
		id=0;
		lightCount=0;
		light=false;
		trace.clear();
		kind=0;
		rot=0.0;
		zoom=0.75f;
		myImage=img;
		toColor=myColor=Color4f(0,0,0,255);
		match=-1;

		bodyDef.type=b2_kinematicBody;
		bodyDef.position=b2Vec2(P2M(300),P2M(300));
		size=gapi->GetImageWidth(img)*zoom/2;
		circleShape.m_radius=P2M(size);
		fixtureDef.shape=&circleShape;
		fixtureDef.density=10.0;
		fixtureDef.isSensor=true;
	};
	virtual void InitAsMagnet(ImageId img){
		id=0;
		darkCount=MAXDARK;
		lightCount=0;
		light=false;
		trace.clear();
		kind=1;
		rot=0.0;
		zoom=0.125f;
		myImage=img;
		match=-2;
		toColor=myColor=Color4f(20,20,20,255);

		bodyDef.type=b2_dynamicBody;

//		int rr=rand()%3;
//		double theta = 2*PI*(rand()%360)/360.0;
//		bodyDef.position=b2Vec2(P2M(300+(rand()%300)*cos(theta)),P2M(300+(rand()%300)*sin(theta)));

		bodyDef.linearDamping=0.5;
		bodyDef.angularDamping=0.5;
		size=gapi->GetImageWidth(img)*zoom/2;
		circleShape.m_radius=P2M(size);
		fixtureDef.shape=&circleShape;
		fixtureDef.density=0.2f;
		fixtureDef.friction=0.8f;
		fixtureDef.restitution=1.0f;

	}
	virtual void InitAsSquare(ImageId img){
		id=0;
		lightCount=0;
		darkCount=MAXDARK;
		light=false;
		trace.clear();
		kind=2;
		rot=2*PI*(rand()%360)/180;
		myImage=img;
		zoom=0.125f;
		int rr=rand()%3;
		match=-2;
		toColor=myColor=Color4f(20,20,20,255);

		size=gapi->GetImageWidth(img)*zoom/2;
		polygonShape.SetAsBox(P2M(gapi->GetImageWidth(img)*zoom/2),P2M(gapi->GetImageHeight(img)*zoom/2),b2Vec2(0,0),rot);
		bodyDef.type=b2_dynamicBody;

//		double theta = 2*PI*(rand()%360)/360.0;
//		bodyDef.position=b2Vec2(P2M(300+(rand()%300)*cos(theta)),P2M(300+(rand()%300)*sin(theta)));

		bodyDef.linearDamping=1.0f;
		bodyDef.angularDamping=1.0f;
		fixtureDef.shape=&polygonShape;
		fixtureDef.density=0.2f;
		fixtureDef.friction=0.8f;
		fixtureDef.restitution=1.0f;
	};
	virtual void InitAsParticle(ImageId img,Color4f col){
		id=0;
		lightCount=0;
		light=false;
		trace.clear();
		kind=0;
		rot=0.0;
		zoom=0.25f;
		myImage=img;
		myColor=col;
		myColor.alpha/=2;
		toColor=myColor;
		myColor.alpha=255;
		toColor.alpha=255;
		match=0;

		bodyDef.type=b2_dynamicBody;
		bodyDef.position=b2Vec2(P2M(300),P2M(300));
		size=gapi->GetImageWidth(img)*zoom/2;
		circleShape.m_radius=P2M(size);
		fixtureDef.shape=&circleShape;
		fixtureDef.density=1.0;
		fixtureDef.isSensor=true;
	}
	virtual void InitAsScoreParticle(ImageId img,Color4f col){
		id=0;
		lightCount=0;
		light=false;
		trace.clear();
		kind=0;
		rot=0.0;
		zoom=0.20f;
		myImage=img;
		myColor=col;
		myColor.alpha=255;
		toColor=myColor;
		toColor.alpha=255;
		match=0;

		bodyDef.type=b2_dynamicBody;
		bodyDef.position=b2Vec2(P2M(300),P2M(300));
		size=gapi->GetImageWidth(img)*zoom/2;
		circleShape.m_radius=P2M(size);
		fixtureDef.shape=&circleShape;
		fixtureDef.density=1.0;
		fixtureDef.isSensor=false;
	}
};