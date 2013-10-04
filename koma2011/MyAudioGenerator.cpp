
#include "MyAudioGenerator.h"
#include "Common.h"
#include "Logger.h"

MyAudioGenerator::MyAudioGenerator(){
}
MyAudioGenerator::~MyAudioGenerator(){
}

void MyAudioGenerator::Init(){
	Logger::Println("[MyAudioGenerator]	Initialize");
	phase=0.0f;
	df=0.0f;
}

void MyAudioGenerator::Close(){
	Logger::Println("[MyAudioGenerator]	Close");
}

void MyAudioGenerator::Run(){
}

void MyAudioGenerator::Generate(float* in,float* out,int frames){
	float speed=PI/SAMPLE_RATE*440+df;
	float pos;
	long i;
	for(i=0;i<frames;i++){
		phase+=speed;
		pos = sin(phase);
		*out++ = pos;
		*out++ = pos;
		df+=0.0f;
	}
}
