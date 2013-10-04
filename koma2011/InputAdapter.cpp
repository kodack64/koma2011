
#include "InputAdapter.h"
#include "ApplicationProperty.h"
#include "Logger.h"
#include "InputApi.h"


InputAdapter* InputAdapter::myInstance=NULL;

InputAdapter::InputAdapter(){
	inputApi=NULL;
}

InputAdapter::~InputAdapter(){
}

InputAdapter* InputAdapter::CreateInstance(){
	if(myInstance==NULL){
		myInstance=new InputAdapter;
	}
	return myInstance;
}
void InputAdapter::DestroyInstance(){
	SAFE_DELETE(myInstance);
}


bool InputAdapter::Init(){
	Logger::Println("[InputAdapter]	Initialize");

	inputApi = InputApi::CreateInstance();
	if(!inputApi->Init()){
		return false;
	}
	return true;
}

void InputAdapter::Close(){
	Logger::Println("[InputAdapter]	Close");
	if(inputApi!=NULL)inputApi->Close();
	InputApi::DestroyInstance();
}

InputApi* InputAdapter::GetAPI(){
	return inputApi;
}