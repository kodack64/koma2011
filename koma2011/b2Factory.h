
#pragma once

#include "GameCommon.h"
#include "b2MyContact.h"
class b2Factory{
private:
	GraphicApi* gapi;
	InputApi* iapi;
	vector <b2Body*> deadBodyList;
	vector <b2Body*> deadParticleList;
	vector <b2Body*> deadScoreList;

	vector <b2Body*> bodyStock;

	int maxMagnetNum;
	int balltimediv;
	int prevballtime;

	long framecount;
	int score;
	int count;
	int showscore;
	b2World* world;
	b2World* fxworld;
	int magnetNum;
	b2Body* magnet[MAX_MAGNET];
	b2Body* cursor;
	b2ContactListener * lis;
	ImageId img_ball;
	ImageId img_cursor;
	ImageId img_square;
	ImageId img_trace;
	ImageId img_backstr;
	ImageId img_num[10];
	int mc[1024];

	bool br;
	bool bg;
	bool bb;

public:
	b2Factory();
	virtual ~b2Factory();
	virtual int Init(GraphicApi*,InputApi*);
	virtual int Run();
	virtual int Close();
	virtual int Draw();
	virtual int Reset();
	virtual int DestroyAll();

	virtual int CreateField();
	virtual int CreateCursor();
	virtual int CreateBall(b2Vec2 p,b2Vec2 v);
	virtual int CreateSquare(b2Vec2 p,b2Vec2 v);
	virtual int CreateFxField();
	virtual int CreateDeadParticle(int,b2Vec2,Color4f);
	virtual int CreateScoreParticle(b2Vec2,Color4f);
	virtual int GetScore(){return score;};
};