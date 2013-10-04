#pragma once

#include <Windows.h>
#include "GraphicApi.h"
#include "AudioApi.h"
#include "InputApi.h"

class Package{
private:
protected:
	AudioApi* aapi;
	GraphicApi* gapi;
	InputApi* iapi;
public:
	Package(){
		iapi=NULL;
		aapi=NULL;
		gapi=NULL;
	}
	virtual ~Package(){
	}
	virtual int Init(GraphicApi* _gapi,AudioApi* _aapi,InputApi* _iapi)=0;
	virtual int Reset()=0;
	virtual int End()=0;
	virtual int Close()=0;
	virtual int Run()=0;
	virtual int AudioCallback(float* in,float* out,int frames)=0;
	virtual int GraphicCallback()=0;
};