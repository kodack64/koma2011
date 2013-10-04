
#include "MouseAdapter.h"
#include "Logger.h"
#include "ApplicationProperty.h"

MouseAdapter* MouseAdapter::myInstance=NULL;

MouseAdapter* MouseAdapter::CreateInstance(){
	if(myInstance!=NULL)return myInstance;
	else{
		myInstance=new MouseAdapter;
		return myInstance;
	}
}
void MouseAdapter::DestroyInstance(){
	SAFE_DELETE(myInstance);
}

bool MouseAdapter::Init(){
	Logger::Println("[MouseAdapter]	Initialize");
	cursorPos.x=0;
	cursorPos.y=0;
	cursorPosPrev.x=0;
	cursorPosPrev.y=0;
	return true;
}

void MouseAdapter::Close(){
	Logger::Println("[MouseAdapter]	Close");
	return;
}

void MouseAdapter::Update(){
	cursorPosPrev.x=cursorPos.x;
	cursorPosPrev.y=cursorPos.y;
	GetCursorPos(&cursorPos);
	ScreenToClient(ApplicationProperty::hWnd,&cursorPos);
	memcpy(buttonStatePrev,buttonState,sizeof(buttonState));
	if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		buttonState[0]=true;
	else buttonState[0]=false;
	if(GetAsyncKeyState(VK_MBUTTON) & 0x8000)buttonState[1]=true;
	else buttonState[1]=false;
	if(GetAsyncKeyState(VK_RBUTTON) & 0x8000)buttonState[2]=true;
	else buttonState[2]=false;
}
