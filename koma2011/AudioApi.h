
#pragma once

#include "Common.h"
#include "MyAudio.h"



class AudioApi{
private:
public:
	virtual AudioId LoadWaveFile(string)=0;
	virtual bool WriteWaveFile(string,AudioId)=0;
	virtual void ReleaseWave(AudioId id)=0;
	virtual void SetState(AudioId,AudioState)=0;
	virtual AudioState GetState(AudioId)=0;
	virtual AudioLoadState GetLoadState(AudioId)=0;
	virtual void SetFadeSpeed(AudioId,float speed)=0;
	virtual void Rewind(AudioId)=0;

	virtual void GetWaveDataAll(float* left,float* right)=0;
	virtual void GetWaveData(AudioId,float* left,float*right)=0;
};