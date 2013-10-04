

#include "MyAudioApi.h"
#include "Logger.h"

void MyAudioApi::Init(){
	Logger::Println("[AudioApi]	Initialize");
	audioList.clear();
};
void MyAudioApi::Close(){
	Logger::Println("[AudioApi] Closed");
	for(UINT i=0;i<audioList.size();i++){
		if(audioList[i]!=NULL){
			audioList[i]->Close();
			SAFE_DELETE(audioList[i]);
		}
	}
};
void MyAudioApi::ReleaseWave(AudioId id){
	if(IsActive(id)){
		audioList[id]->Clear();
	}
}

AudioId MyAudioApi::LoadWaveFile(string filepath){
	Logger::Println("[AudioApi] Load Wave File %s",filepath.c_str());
	AudioId id=-1;
	for(UINT i=0;i<audioList.size();i++){
		if(audioList[i]==NULL){
			id=i;
			break;
		}
	}
	if(id==-1){
		id=audioList.size();
		audioList.push_back(NULL);
	}

	audioList[id]=new MyAudio;
	audioList[id]->Init(id);
	AudioId rtn=audioList[id]->LoadFile(filepath);
	if(rtn==-1){
		audioList[id]->Close();
		SAFE_DELETE(audioList[id]);
		return -1;
	}else{
		return id;
	}
}
bool MyAudioApi::WriteWaveFile(string filepath,AudioId id){
	if(IsActive(id)){
		return audioList[id]->WriteFile(filepath);
	}
	return false;
}
void MyAudioApi::Rewind(AudioId id){
	if(IsActive(id)){
		audioList[id]->Rewind();
	}
}

void MyAudioApi::SetState(AudioId id,AudioState state){
	if(IsActive(id)){
		audioList[id]->SetState(state);
	}
}

AudioState MyAudioApi::GetState(AudioId id){
	if(IsActive(id)){
		return audioList[id]->GetState();
	}else{
		return Audio_State_Error;
	}
}
AudioLoadState MyAudioApi::GetLoadState(AudioId id){
	if(IsActive(id)){
		return audioList[id]->GetLoadState();
	}else{
		return Audio_Load_Error;
	}
}

bool MyAudioApi::IsActive(AudioId id){
	if(0<=id && id<(signed)audioList.size()){
		if(audioList[id]!=NULL){
			return true;
		}
	}
	return false;
}

void MyAudioApi::GetWaveDataAll(float *left,float* right){
	bool rel=false;
	float tl,tr;
	(*left)=(*right)=0.0f;
	for(UINT id=0;id<audioList.size();id++){
		if(IsActive(id)){
			if(audioList[id]->IsPlay()){
				rel=true;
				audioList[id]->GetWaveData(&tl,&tr);
				(*left)+=tl;
				(*right)+=tr;
			}
		}
	}
	if(!rel){
		(*left)=0.0f;
		(*right)=0.0f;
	}
}
void MyAudioApi::GetWaveData(AudioId id,float* left,float* right){
	bool rel=false;
	(*left)=(*right)=0.0f;
	if(IsActive(id)){
		if(audioList[id]->IsPlay()){
			rel=true;
			audioList[id]->GetWaveData(left,right);
		}
	}
	if(!rel){
		(*left)=0.0f;
		(*right)=0.0f;
	}
}

MyAudio* MyAudioApi::GetAudioInstance(AudioId id){
	if(IsActive(id)){
		return audioList[id];
	}else{
		return NULL;
	}
}

void MyAudioApi::SetFadeSpeed(AudioId id,float speed){
	if(IsActive(id)){
		audioList[id]->SetFadeSpeed(speed);
	}
}
