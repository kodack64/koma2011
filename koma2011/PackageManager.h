#pragma once

#include "Common.h"
#include "Package.h"

#include "GraphicApi.h"
#include "AudioApi.h"
#include "InputApi.h"

class PackageManager{
private:
	static PackageManager* myInstance;
	Package* package;

	GraphicApi* gapi;
	InputApi* iapi;
	AudioApi* aapi;

	bool endFlag;

public:
	PackageManager();
	virtual ~PackageManager();

	virtual bool Init(GraphicApi* _gapi,AudioApi* _aapi,InputApi* _iapi);
	virtual void Close();

	static PackageManager* CreateInstance();
	static void DestroyInstance();

	static int TimerCallbackProc(void);
	static void GraphicCallbackProc();
	static void AudioCallbackProc(float*,float*,int);

	static void BackToStart();
	virtual void BackToStartProc();

	virtual int TimerMain();
	virtual void AudioMain(float*,float*,int);
	virtual void GraphicMain();

	virtual bool IsActive();
	virtual bool IsEnd();
};