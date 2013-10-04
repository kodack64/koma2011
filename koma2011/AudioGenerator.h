#pragma once

class AudioGenerator{
public:
	virtual void Init()=0;
	virtual void Close()=0;
	virtual void Run()=0;
	virtual void Generate(float*,float*,int)=0;
};