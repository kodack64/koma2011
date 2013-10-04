
#pragma once

#include "Common.h"
#include "AudioApi.h"
#include "MyAudio.h"

class MyAudioApi:public AudioApi{
private:
	vector <MyAudio*> audioList;
	virtual bool IsActive(AudioId id);
public:
	virtual void Init();
	virtual void Close();
	virtual void Rewind(AudioId);
	virtual void SetFadeSpeed(AudioId,float speed);

	virtual AudioId LoadWaveFile(string);
	virtual bool WriteWaveFile(string,AudioId);
	virtual void ReleaseWave(AudioId id);

	virtual void SetState(AudioId id,AudioState state);
	virtual AudioState GetState(AudioId);
	virtual AudioLoadState GetLoadState(AudioId);

	virtual void GetWaveDataAll(float* left,float* right);
	virtual void GetWaveData(AudioId,float* left,float*right);
	virtual MyAudio* GetAudioInstance(AudioId);
};