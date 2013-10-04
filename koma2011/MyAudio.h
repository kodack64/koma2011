
#pragma once

#include "Common.h"
#include "WaveShell.h"

typedef int AudioId;

typedef enum{
	Audio_Play,
	Audio_Stop,
	Audio_FadeOut,
	Audio_FadeIn,
	Audio_LoopPlay,
	Audio_FadeInAndLoop,
	Audio_FadeOutAndLoop,
	Audio_State_Error
}AudioState;

typedef enum{
	Audio_None,
	Audio_Loaded,
	Audio_Loading,
	Audio_Load_Error
}AudioLoadState;

class MyAudio{
private:
	bool stereo;
	WaveShell* wsl;
	WaveShell* wsr;
	AudioState state;
	AudioLoadState loadState;
	AudioId id;
	string mypath;
	float vol;
	float fade;
	float speed;
	virtual unsigned char GetNextData(FILE*);
public:
	virtual void Init(AudioId id);
	virtual void Clear();
	virtual void Close();
	virtual AudioId LoadFile(string);
	virtual bool WriteFile(string);
	virtual AudioState GetState();
	virtual AudioLoadState GetLoadState();
	virtual void SetState(AudioState state);
	virtual void Rewind();
	virtual void SetFadeSpeed(float sp);

	virtual bool IsPlay();
	virtual void GetWaveData(float* left,float* right);
	virtual WaveShell* GetWaveShellLeft();
	virtual WaveShell* GetWaveShellRight();
};