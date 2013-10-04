
#include "KeyboardAdapter.h"
#include "Logger.h"
#include "ApplicationProperty.h"

KeyboardAdapter* KeyboardAdapter::myInstance=NULL;

KeyboardAdapter* KeyboardAdapter::CreateInstance(){
	if(myInstance!=NULL)return myInstance;
	else{
		myInstance=new KeyboardAdapter;
		return myInstance;
	}
}
void KeyboardAdapter::DestroyInstance(){
	SAFE_DELETE(myInstance);
}

bool KeyboardAdapter::Init()
{
	Logger::Println("[KeybaordAdapter]	Initialize");
	memset(key,0,sizeof(key));
	memset(oldkey,0,sizeof(oldkey));
	return true;
}

void KeyboardAdapter::Update(){
	memcpy(key,oldkey,sizeof(key));
	GetKeyboardState(key);
	return;
}

void KeyboardAdapter::Close(void){
	Logger::Println("[KeyboardAdapter]	Close");
}
