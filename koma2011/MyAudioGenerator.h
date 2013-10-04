#pragma once

#include "AudioGenerator.h"

class MyAudioGenerator:public AudioGenerator{
private:
	float phase;
	float df;
public:
	MyAudioGenerator();
	virtual ~MyAudioGenerator();
	virtual void Init();
	virtual void Close();
	virtual void Run();
	virtual void Generate(float*,float*,int);
};