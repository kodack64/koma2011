#pragma once

#include "Common.h"
#include "PackageManager.h"
#include "portaudio.h"
#include "MyAudioApi.h"

class AudioAdapter{
private:
	static AudioAdapter* myInstance;
	MyAudioApi *audioApi;

	PaStream *stream;
	PaDeviceIndex devCount;
	const PaDeviceInfo *devinfo;
	PaHostApiIndex hostApiCount;
	PaStreamParameters inputParameters;
	PaStreamParameters outputParameters;
	int inChan;
	int outChan;
	int inDevNum;
	int outDevNum;
	int hostApiNum;
	unsigned long framesPerBuffer;

	bool f_init;
	bool f_open;
	bool f_start;

	void getDeviceType(int in,int out,char* buf);
	static int AudioCallback( const void *inputBuffer, void *outputBuffer,
							   unsigned long framesPerBuffer,
							   const PaStreamCallbackTimeInfo* timeInfo,
							   PaStreamCallbackFlags statusFlags,
							   void *userData );

	virtual int SelectApi();
	virtual int SelectInput();
	virtual int SelectOutput();
public:
	/*debug*/
	float phase;

	static AudioAdapter* CreateInstance();
	static void DestroyInstance();

	AudioAdapter();
	virtual ~AudioAdapter();

	virtual bool Init();
	virtual void Close();
	virtual void Run();

	virtual AudioApi* GetAPI();
};