
#include "Game.h"
#include "Common.h"

#ifdef _DEBUG
#pragma comment(lib,"Box2D.lib")
#else
#pragma comment(lib,"Box2D.lib")
#endif

Game* Game::myInstance=NULL;

Package* Game::CreateInstance(){
	if(myInstance==NULL){
		myInstance=new Game;
	}
	return myInstance;
}
void Game::DestroyInstance(){
	if(myInstance!=NULL){
		SAFE_DELETE(myInstance);
	}
}

int Game::Init(GraphicApi* _gapi,AudioApi* _aapi,InputApi* _iapi){
	gapi=_gapi;
	iapi=_iapi;
	aapi=_aapi;
	srand((unsigned)time(NULL));

	img_white=gapi->CreateImageFromFile("image\\white.png",Color4f(0,0,0,0));
	img_back=gapi->CreateImageFromFile("image\\back2.png",Color4f(0,0,0,0));
	img_backr=gapi->CreateImageFromFile("image\\back5.png",Color4f(0,0,0,0));
	img_title=gapi->CreateImageFromFile("image\\title.png",Color4f(0,0,0,0));
	for(int i=0;i<10;i++){
		char buf[1024];
		sprintf_s(buf,"image\\%d.png",i);
		img_num[i]=gapi->CreateImageFromFile(buf);
	}

	b2f=new b2Factory;
	b2f->Init(gapi,iapi);

	Reset();

	return 0;
}

int Game::Reset(){
	isTitle=true;
	b2f->Reset();
	lastscore=0;
	FILE* fp;
	fopen_s(&fp,"score.txt","r");
	if(fp==NULL){
		highscore=0;
	}else{
		char buf[1024];
		fgets(buf,1024,fp);
		highscore=atoi(buf);
		fclose(fp);
	}
	return 0;
}
int Game::Close(){
	gapi->ReleaseImage(img_back);
	gapi->ReleaseImage(img_backr);
	gapi->ReleaseImage(img_white);
	gapi->ReleaseImage(img_title);
	for(int i=0;i<10;i++){
		gapi->ReleaseImage(img_num[i]);
	}
	b2f->Close();
	SAFE_DELETE(b2f);
	return 0;
}
int Game::End(){
	return 0;
}
int Game::Run(){
	//タッチによるバーの生成消滅処理
	iapi->Update();
	if(isTitle){
		if(iapi->mouse->GetPushNow(0)){
			b2f->Reset();
			isTitle=false;
		}
	}else{
		if(b2f->Run()==1){
			lastscore=b2f->GetScore();
			if(lastscore>highscore){
				FILE* fp;
				fopen_s(&fp,"score.txt","w");
				if(fp!=NULL){
					fprintf_s(fp,"%d\n",lastscore);
					highscore=lastscore;
					fclose(fp);
				}
			}
			isTitle=true;
		}
	}
	return 0;
}

int Game::GraphicCallback(){
	if(isTitle){
		gapi->DrawImageLU(0,0,0,0,0,0,1.0f,0.0f,Color4f(255,255,255,255),img_white);
		gapi->DrawImageLU(0,0,0,0,0,0,1.0f,0.0f,Color4f(255,255,255,60),img_back);
		gapi->DrawImageLU(600,0,0,0,0,0,1.0f,0.0f,Color4f(255,255,255,60),img_backr);
		gapi->DrawImageLU(100,60,img_title);
		int fs;
		fs=lastscore;
		if(fs==0){
			gapi->DrawImageLU(600,270,img_num[0]);
		}else{
			for(int i=0;fs>0;i++){
				gapi->DrawImageLU(600-i*32,270,img_num[fs%10]);
				fs/=10;
			}
		}
		fs=highscore;
		if(fs==0){
			gapi->DrawImageLU(600,335,img_num[0]);
		}else{
			for(int i=0;fs>0;i++){
				gapi->DrawImageLU(600-i*32,335,img_num[fs%10]);
				fs/=10;
			}
		}
	}else{
		gapi->DrawImageLU(0,0,0,0,0,0,1.0f,0.0f,Color4f(255,255,255,255),img_back);
		gapi->DrawImageLU(600,0,0,0,0,0,1.0f,0.0f,Color4f(255,255,255,255),img_backr);
		b2f->Draw();
	}
	return 0;
}

int Game::AudioCallback(float* in,float* out,int frames){
	int i;
	float left,right;
	for(i=0;i<frames;i++){
		left=right=0.0f;
		aapi->GetWaveDataAll(&left,&right);
		*out++ = left;
		*out++ = right;
	}
	return 0;
}
