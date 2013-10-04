
#include "InputApi.h"

InputApi::InputApi()
:key(NULL)
,mouse(NULL){
}

InputApi::~InputApi(){
}


InputApi* InputApi::myInstance=NULL;

InputApi* InputApi::CreateInstance(){
	if(myInstance==NULL){
		myInstance=new InputApi;
	}
	return myInstance;
}
void InputApi::DestroyInstance(){
	SAFE_DELETE(myInstance);
}


bool InputApi::Init(){
	key=KeyboardAdapter::CreateInstance();
	key->Init();
	mouse=MouseAdapter::CreateInstance();
	mouse->Init();
	return true;
}
void InputApi::Update(){
	key->Update();
	mouse->Update();
}
void InputApi::Close(){
	key->Close();
	KeyboardAdapter::DestroyInstance();
	mouse->Close();
	MouseAdapter::DestroyInstance();
}