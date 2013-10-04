
#pragma once

#include "package.h"
#include "b2Factory.h"

class Game:public Package{
private:

	ImageId img_back;
	ImageId img_backr;
	ImageId img_title;
	ImageId img_white;
	ImageId img_num[10];

	bool isTitle;
	/*Field*/
	int frameCount;

	int highscore;
	int lastscore;

	/*Box2D*/
	b2Factory* b2f;


private:
	static Game* myInstance;
public:
	virtual int Init(GraphicApi* _gapi,AudioApi* _aapi,InputApi* _iapi);
	virtual int Reset();
	virtual int Close();
	virtual int End();
	virtual int Run();
	virtual int AudioCallback(float* in,float* out,int frames);
	virtual int GraphicCallback();

	static Package* CALLBACK CreateInstance();
	static void CALLBACK DestroyInstance();
};