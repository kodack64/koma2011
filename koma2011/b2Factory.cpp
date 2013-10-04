
#include "b2Factory.h"
#include "Logger.h"
#include "b2Object.h"
#include "b2MyContact.h"

b2Factory::b2Factory()
:gapi(NULL)
,world(NULL)
,fxworld(NULL)
{
}

b2Factory::~b2Factory(){
}

void ennear(int* src,int to,int spd){
	int from=(*src);
	if(from+spd<to){
		(*src)+=spd;
	}else if(from<to){
		(*src)+=to-from;
	}if(from-spd>to){
		(*src)-=spd;
	}else{
		(*src)-=from-to;
	}
}

int b2Factory::Init(GraphicApi* _gapi,InputApi* _iapi){
	gapi=_gapi;
	iapi=_iapi;
	magnetNum=0;

	/*Load Graphic*/
	img_ball=gapi->CreateImageFromFile("Image\\lightball2.png");
	img_cursor=gapi->CreateImageFromFile("Image\\cursor4.png");
	img_square=gapi->CreateImageFromFile("Image\\lightsquare.png");
	img_trace=gapi->CreateImageFromFile("Image\\cursor4.png");
	img_backstr=gapi->CreateImageFromFile("image\\backstr.png",Color4f(0,0,0,0));

	for(int i=0;i<10;i++){
		char buf[1024];
		sprintf_s(buf,"image\\%d.png",i);
		img_num[i]=gapi->CreateImageFromFile(buf);
	}

	world=new b2World(b2Vec2(0.0f,0.0f));
	lis =new MyContactListener();
	world->SetContactListener(lis);
	world->SetAllowSleeping(false);

	fxworld=new b2World(b2Vec2(0.0f,-10.0f));

	Reset();

	return 0;
}

int b2Factory::Reset(){
	DestroyAll();
	deadBodyList.clear();
	deadParticleList.clear();
	deadScoreList.clear();
	bodyStock.clear();
	score=0;
	showscore=0;
	count=FIRSTCOUNT;
	framecount=0;

	br=bg=bb=false;
	magnetNum=0;
	maxMagnetNum=FIRSTBALL;

	balltimediv=10;
	prevballtime=0;

	this->CreateField();
	this->CreateFxField();
	this->CreateCursor();

	return 0;
}

int b2Factory::Close(){
	this->DestroyAll();
	gapi->ReleaseImage(img_ball);
	gapi->ReleaseImage(img_cursor);
	gapi->ReleaseImage(img_trace);
	gapi->ReleaseImage(img_backstr);
	for(int i=0;i<10;i++){
		gapi->ReleaseImage(img_num[i]);
	}
	SAFE_DELETE(lis);
	SAFE_DELETE(world);
	SAFE_DELETE(fxworld);
	return 0;
}

int b2Factory::Run(){
	framecount++;
	
	
	if(framecount%1==0){
		if(score>showscore){
			showscore++;
		}
	}

	//generate lack of ball
	if(framecount-prevballtime>balltimediv && magnetNum<maxMagnetNum){
		prevballtime=framecount;
		b2Vec2 p;
		b2Vec2 fv(0,0);
		double theta=2*PI*(rand()%120-60)/360.0;
		switch(rand()%4){
		case 0:
			theta+=PI;
			p=b2Vec2(P2M(300+270),P2M(300));
			fv=b2Vec2(cos(theta),sin(theta));
			break;
		case 1:
			p=b2Vec2(P2M(300-270),P2M(300));
			fv=b2Vec2(cos(theta),sin(theta));
			break;
		case 2:
			theta-=PI/2;
			p=b2Vec2(P2M(300),P2M(300+270));
			fv=b2Vec2(cos(theta),sin(theta));
			break;
		case 3:
			theta+=PI/2;
			p=b2Vec2(P2M(300),P2M(300-270));
			fv=b2Vec2(cos(theta),sin(theta));
			break;
		}
		fv.x*=rand()%5+1;
		fv.y*=rand()%5+1;
		if(rand()%2==0){
			this->CreateBall(p,fv);
		}else{
			this->CreateSquare(p,fv);
		}
	}


	//proc dead body
	magnetNum-=deadBodyList.size();
	for(unsigned int i=0;i<deadBodyList.size();i++){
		if(deadBodyList[i]->GetUserData()!=NULL){
			b2Object* obj = (b2Object*)deadBodyList[i]->GetUserData();

			Color4f col = obj->myColor;
			b2Vec2 deadPoint=deadBodyList[i]->GetPosition();
			if(obj->match!=-1){
				this->CreateDeadParticle(2*mc[obj->match],deadPoint,col);
			}

			SAFE_DELETE(obj);
			deadBodyList[i]->SetUserData(NULL);
		}
		world->DestroyBody(deadBodyList[i]);
	}
	deadBodyList.clear();

	//proc dead particle
	for(unsigned int i=0;i<deadParticleList.size();i++){
		if(deadParticleList[i]->GetUserData()!=NULL){
			b2Object* obj = (b2Object*)deadParticleList[i]->GetUserData();

			Color4f col = obj->myColor;
			b2Vec2 deadPoint=deadParticleList[i]->GetPosition();
			if(obj->match!=-1){
				this->CreateScoreParticle(deadPoint,col);
			}

			SAFE_DELETE(obj);
			deadParticleList[i]->SetUserData(NULL);
		}
		fxworld->DestroyBody(deadParticleList[i]);
	}
	deadParticleList.clear();

	//proc extends
	unsigned int ext=EXTENDS;
	if(bodyStock.size()>ext){
		for(unsigned int i=0;i<ext;i++){
			b2Body* body = bodyStock[i];
			if(body->GetUserData()!=NULL){
				b2Object* obj=(b2Object*)(body->GetUserData());
				SAFE_DELETE(obj);
				body->SetUserData(NULL);
			}
			fxworld->DestroyBody(body);
		}
		bodyStock.erase(bodyStock.begin(),bodyStock.begin()+ext);
		maxMagnetNum++;
	}

	//steps
	world->Step(1.0f/60.0f,6,2);
	fxworld->Step(1.0f/60.0f,6,2);
	//proc cursor
	POINT p=iapi->mouse->GetPoint();
	b2Vec2 pv;
	pv=b2Vec2(P2M(p.x),P2M(p.y));
	pv=pv-cursor->GetPosition();
	cursor->SetLinearVelocity(b2Vec2(pv.x*10,pv.y*10));

	//check its sleep
	bool isAllSleep=true;
	b2Body* body=world->GetBodyList();
	b2Vec2 v;
	Color4f c;
	memset(mc,0,sizeof(mc));
	while(body!=NULL){
		if(body->GetUserData()!=NULL){
			b2Object* obj=(b2Object*)(body->GetUserData());
			if(obj!=NULL){
				if(obj->kind!=0 && body->GetLinearVelocity().Length()>P2M(10)){
					isAllSleep=false;
				}
				if(obj->match>=0 && obj->light){
					mc[obj->match]++;
				}
				if(obj->kind!=0){
					ennear(&(obj->myColor.red),obj->toColor.red,1);
					ennear(&(obj->myColor.green),obj->toColor.green,1);
					ennear(&(obj->myColor.blue),obj->toColor.blue,1);
					if(obj->light)obj->lightCount++;
					if(obj->lightCount>MAX_LIGHT){
						deadBodyList.push_back(body);
					}
				}
			}
		}
		body=body->GetNext();
	}

	//if all sleep
	// remove lighting ball
	// if clicked
	//  proc apply force
//	if(isAllSleep){
		body=world->GetBodyList();
		while(body!=NULL){
			if(body->GetUserData()!=NULL){
				b2Object* obj=(b2Object*)(body->GetUserData());
				if(obj->match==-2){
					if(obj->darkCount<=0){
						obj->match=rand()%3;
						switch(obj->match){
						case 0:
							obj->toColor=Color4f(255,0,0,200);
							break;
						case 1:
							obj->toColor=Color4f(0,255,0,200);
							break;
						case 2:
							obj->toColor=Color4f(0,0,255,200);
							break;
						}
					}else{
						obj->darkCount--;
					}
				}
/*				if(obj!=NULL){
					if(obj->light){
						deadBodyList.push_back(body);
					}
				}
*/			}
			body=body->GetNext();
		}
//	}

	if(br){
		((b2Object*)(cursor->GetUserData()))->myColor.red+=2;
	}else{
		((b2Object*)(cursor->GetUserData()))->myColor.red-=2;
	}
	if(((b2Object*)(cursor->GetUserData()))->myColor.red>255){
		br=!br;
		((b2Object*)(cursor->GetUserData()))->myColor.red=255;
	}
	if(((b2Object*)(cursor->GetUserData()))->myColor.red<0){
		br=!br;
		((b2Object*)(cursor->GetUserData()))->myColor.red=0;
	}

	if(bg){
		((b2Object*)(cursor->GetUserData()))->myColor.green+=3;
	}else{
		((b2Object*)(cursor->GetUserData()))->myColor.green-=3;
	}
	if(((b2Object*)(cursor->GetUserData()))->myColor.green>255){
		bg=!bg;
		((b2Object*)(cursor->GetUserData()))->myColor.green=255;
	}
	if(((b2Object*)(cursor->GetUserData()))->myColor.green<0){
		bg=!bg;
		((b2Object*)(cursor->GetUserData()))->myColor.green=0;
	}

	if(bb){
		((b2Object*)(cursor->GetUserData()))->myColor.blue+=5;
	}else{
		((b2Object*)(cursor->GetUserData()))->myColor.blue-=5;
	}
	if(((b2Object*)(cursor->GetUserData()))->myColor.blue>255){
		bb=!bb;
		((b2Object*)(cursor->GetUserData()))->myColor.blue=255;
	}
	if(((b2Object*)(cursor->GetUserData()))->myColor.blue<0){
		bb=!bb;
		((b2Object*)(cursor->GetUserData()))->myColor.blue=0;
	}
//	else{
//		((b2Object*)(cursor->GetUserData()))->myColor=Color4f(0,0,0,255);
//	}
	// mouse click

	if(iapi->mouse->GetPushNow(0) && count>0){
		count--;
		body=world->GetBodyList();
		while(body!=NULL){
			if(body->GetUserData()!=NULL){
				b2Object* obj=(b2Object*)(body->GetUserData());
				if(obj->kind!=0){
					pv=body->GetPosition()-cursor->GetPosition();
					if(pv.Length()<P2M(100)){
						float len=pv.Length()/P2M(100);
						if(len<P2M(32))len=P2M(32);
						pv.Normalize();
						body->ApplyForce(b2Vec2(pv.x/pow(len,2),pv.y/pow(len,2)),body->GetPosition());
					}
				}
			}
			body=body->GetNext();
		}
	}

	// fx dead body
	body=fxworld->GetBodyList();
	while(body!=NULL){
		if(body->GetUserData()!=NULL){
			b2Object* obj=(b2Object*)(body->GetUserData());
			if(obj!=NULL){
				if(body->GetPosition().y<P2M(0)){
					deadParticleList.push_back(body);
				}
			}
		}
		body=body->GetNext();
	}


	if(isAllSleep && count==0){
		return 1;
	}
	return 0;
}

int b2Factory::Draw(){

	gapi->DrawImageLU(600,0,img_backstr);

	int fs;
	fs=showscore;
	if(fs==0){
		gapi->DrawImageLU(750,50,img_num[0]);
	}else{
		for(int i=0;fs>0;i++){
			gapi->DrawImageLU(750-i*32,50,img_num[fs%10]);
			fs/=10;
		}
	}

	fs=count;
	if(fs==0){
		gapi->DrawImageLU(750,130,img_num[0]);
	}else{
		for(int i=0;fs>0;i++){
			gapi->DrawImageLU(750-i*32,130,img_num[fs%10]);
			fs/=10;
		}
	}

	fs=maxMagnetNum;
	if(fs==0){
		gapi->DrawImageLU(750,200,img_num[0]);
	}else{
		for(int i=0;fs>0;i++){
			gapi->DrawImageLU(750-i*32,200,img_num[fs%10]);
			fs/=10;
		}
	}

	//add trace
	//draw all game worlds body
	b2Vec2 v;
	Color4f c;
	int kind;
	b2Body* body;
	body=world->GetBodyList();
	while(body!=NULL){
		if(body->GetUserData()!=NULL){
			b2Object* obj=(b2Object*)(body->GetUserData());
			kind=obj->kind;
			c = obj->myColor;

			obj->trace.push_back(body->GetPosition());
			if(obj->trace.size()>=MAX_TRACE){
				obj->trace.erase(obj->trace.begin());
			}
			obj->rot=body->GetAngle();

			double zc=obj->zoom;
			Color4f cc=obj->myColor;
			for(int i=obj->trace.size()-1;i>=0;i--){
				if(i==obj->trace.size()-1){
					if(obj->light){
					}else{
						cc.alpha=(int)(cc.alpha*0.5);
					}
					gapi->DrawImage(M2P(obj->trace[i].x),M2P(obj->trace[i].y),0,0,0,0,zc,obj->rot,cc,obj->myImage);
				}else{
					gapi->DrawImage(M2P(obj->trace[i].x),M2P(obj->trace[i].y),0,0,0,0,zc*2,obj->rot,cc,img_trace);
				}
				zc=(int)(zc*0.8);
				cc.alpha=(int)(cc.alpha*0.5);
			}
		}
		body=body->GetNext();
	}

	//draw fx world all body
	body=fxworld->GetBodyList();
	while(body!=NULL){
		if(body->GetUserData()!=NULL){
			b2Object* obj=(b2Object*)(body->GetUserData());
			kind=obj->kind;
			c = obj->myColor;

			obj->trace.push_back(body->GetPosition());
			if(obj->trace.size()>=MAX_TRACE){
				obj->trace.erase(obj->trace.begin());
			}
			obj->rot=body->GetAngle();

			double zc=obj->zoom;
			Color4f cc=obj->myColor;
			for(int i=obj->trace.size()-1;i>=0;i--){
				gapi->DrawImage(M2P(obj->trace[i].x),M2P(obj->trace[i].y),0,0,0,0,zc,obj->rot,cc,obj->myImage);
				zc=(int)(zc*0.8);
				cc.alpha=(int)(cc.alpha*0.5);
			}
		}
		body=body->GetNext();
	}

	return 0;
}

//destroy all
int b2Factory::DestroyAll(){
	b2Body* body=world->GetBodyList();
	for(;body!=NULL;){
		b2Body* sub=body->GetNext();
		if(body->GetUserData()!=NULL){
			b2Object* obj=(b2Object*)(body->GetUserData());
			SAFE_DELETE(obj);
			body->SetUserData(NULL);
		}
		world->DestroyBody(body);
		body=sub;
	}
	body=fxworld->GetBodyList();
	for(;body!=NULL;){
		b2Body* sub=body->GetNext();
		if(body->GetUserData()!=NULL){
			b2Object* obj=(b2Object*)(body->GetUserData());
			SAFE_DELETE(obj);
			body->SetUserData(NULL);
		}
		fxworld->DestroyBody(body);
		body=sub;
	}
	return 0;
}


int b2Factory::CreateField(){
	b2BodyDef bodyDef;
	b2FixtureDef fixtureDef;
	b2ChainShape chain;

	int cent= min(gapi->GetScreenHeight() , gapi->GetScreenWidth())/2;
	bodyDef.type=b2_staticBody;
	bodyDef.userData=NULL;
	bodyDef.position=b2Vec2(P2M(cent),P2M(cent));
	b2Vec2 v[256];
	for(int i=0;i<256;i++){
		v[i].Set(P2M(cent*cos(2*PI/256*i)),P2M(cent*sin(2*PI/256*i)));
	}
	chain.CreateLoop(v,256);

	fixtureDef.shape=&chain;

	b2Body* body=world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

	return 0;
}

int b2Factory::CreateFxField(){
	b2Body* body;
	b2BodyDef bodyDef;
	b2FixtureDef fixtureDef;
	b2EdgeShape edge;

	bodyDef.type=b2_staticBody;
	bodyDef.userData=NULL;
	bodyDef.position=b2Vec2(P2M(gapi->GetScreenHeight()),P2M(gapi->GetScreenHeight()/2));
	edge.Set(b2Vec2(0,P2M(-gapi->GetScreenHeight())),b2Vec2(0,P2M(gapi->GetScreenHeight())));
	fixtureDef.shape=&edge;
	body=fxworld->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

	bodyDef.position=b2Vec2(P2M(gapi->GetScreenWidth()),P2M(gapi->GetScreenHeight()/2));
	body=fxworld->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);


	bodyDef.type=b2_staticBody;
	bodyDef.userData=NULL;
	bodyDef.position=b2Vec2(P2M(700),P2M(260));
	edge.Set(
		b2Vec2(P2M(-200),0)
		,b2Vec2(P2M(200),0)
		);
	fixtureDef.shape=&edge;
	body=fxworld->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);

	return 0;
}

int b2Factory::CreateCursor(){
	b2Body* body;
	b2Fixture* fixture;
	b2Object* obj=new b2Object(gapi);
	obj->InitAsCursor(img_cursor);
	body=world->CreateBody(&(obj->bodyDef));
	fixture=body->CreateFixture(&(obj->fixtureDef));
	body->ResetMassData();
	body->SetUserData(obj);
	cursor=body;
	return 0;
}

int b2Factory::CreateBall(b2Vec2 p,b2Vec2 v){
	b2Body* body;
	b2Fixture* fixture;
	b2Object* obj=new b2Object(gapi);
	obj->InitAsMagnet(img_ball);
	obj->bodyDef.position=p;
	obj->bodyDef.linearVelocity=v;
	body=world->CreateBody(&(obj->bodyDef));
	fixture=body->CreateFixture(&(obj->fixtureDef));
	body->ResetMassData();
	body->SetUserData(obj);
	magnet[magnetNum]=body;
	magnetNum++;
	return 0;
}

int b2Factory::CreateSquare(b2Vec2 p,b2Vec2 v){
	b2Body* body;
	b2Fixture* fixture;
	b2Object* obj=new b2Object(gapi);
	obj->InitAsSquare(img_square);
	obj->bodyDef.position=p;
	obj->bodyDef.linearVelocity=v;
	body=world->CreateBody(&(obj->bodyDef));
	fixture=body->CreateFixture(&(obj->fixtureDef));
	body->ResetMassData();
	body->SetUserData(obj);
	magnet[magnetNum]=body;
	magnetNum++;
	return 0;
}

int b2Factory::CreateDeadParticle(int num,b2Vec2 dp,Color4f col){
	score+=num;
	b2Body* body;
	b2Fixture* fixture;
	for(int i=0;i<num;i++){
		b2Object* obj=new b2Object(gapi);
		obj->InitAsParticle(img_cursor,col);
		b2BodyDef bodyDef = obj->bodyDef;
		bodyDef.position=dp;
		body=fxworld->CreateBody(&(bodyDef));
		body->ApplyForceToCenter(b2Vec2(P2M(rand()%1000-500),P2M(rand()%400-200)));
		fixture=body->CreateFixture(&(obj->fixtureDef));
		body->ResetMassData();
		body->SetUserData(obj);
	}
	return 0;
}

int b2Factory::CreateScoreParticle(b2Vec2 dp,Color4f col){
	b2Body* body;
	b2Fixture* fixture;

	b2Object* obj=new b2Object(gapi);
	obj->InitAsScoreParticle(img_cursor,col);
	b2BodyDef bodyDef = obj->bodyDef;
//	bodyDef.position=dp;
	b2Vec2 fv(700,600);
	fv.x+=(dp.x/600.0f)*100.0f;
	fv.x=P2M(fv.x);
	fv.y=P2M(fv.y);
	bodyDef.position=fv;
	body=fxworld->CreateBody(&(bodyDef));
	body->ApplyForceToCenter(b2Vec2(P2M(rand()%400-200),0));
	fixture=body->CreateFixture(&(obj->fixtureDef));
	body->ResetMassData();
	body->SetUserData(obj);
	bodyStock.push_back(body);
	return 0;
}

