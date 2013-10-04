
#include "WaveShell.h"

WaveShell::WaveShell(){
}
WaveShell::~WaveShell(){
	shell.clear();
}

void WaveShell::Init(){
	cursor=0;
	shell.clear();
}

void WaveShell::Close(){
	Clear();
}

void WaveShell::Clear(){
	shell.clear();
	cursor=0;
}

bool WaveShell::Empty(){
	return (shell.size()==0);
}

WAVEPOS WaveShell::GetLength(){
	return (signed)shell.size();
}

void WaveShell::Extend(WAVEPOS ext){
	shell.resize(GetLength()+ext,0.0f);
}

void WaveShell::Resize(WAVEPOS size){
	shell.resize(size,0.0f);
}

void WaveShell::Add(float wave){
	shell.push_back(wave);
}

WAVEPOS WaveShell::GetCursor(){
	return cursor;
}
void WaveShell::SetCursor(WAVEPOS cur){
	cursor=cur;
}
float WaveShell::GetCursorPos(){
	if(Empty())return 0.0f;
	return shell[cursor];
}
float WaveShell::GetPosFromCursor(WAVEPOS at){
	WAVEPOS len=GetLength();
	if(abs(at)>len)	return 0.0f;
	else return shell[(cursor+len+at)%len];
}
void WaveShell::SetCursorPos(float wave){
	if(Empty())return;
	shell[cursor]=wave;
}
void WaveShell::SetPosFromCursor(float wave,WAVEPOS at){
	WAVEPOS len=GetLength();
	if(abs(at)>len);
	else shell[(cursor+len+at)%len]=wave;
}
void WaveShell::Forward(){
	if(Empty())return;
	cursor++;
	cursor%=GetLength();
}
void WaveShell::Forward(WAVEPOS distance){
	if(Empty())return;
	cursor+=distance;
	if(cursor<0)cursor+=GetLength();
	if(cursor>GetLength())cursor%=GetLength();
}

float WaveShell::GetPos(WAVEPOS at){
	if(at<0 || at>=GetLength())
		return 0.0f;
	else return shell[at];
}
void WaveShell::SetPos(float wave,WAVEPOS at){
	if(at<0 || at>=GetLength())
		return;
	else shell[at]=wave;
}