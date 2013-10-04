#pragma once

#include "Common.h"

class MouseAdapter{
private:
	static MouseAdapter* myInstance;

	POINT cursorPos;
	POINT cursorPosPrev;
	bool buttonState[3];
	bool buttonStatePrev[3];
	bool f_init;
	bool f_loop;
	bool f_start;

public:
	static MouseAdapter* CreateInstance();
	static void DestroyInstance();
	MouseAdapter(){};
	~MouseAdapter(){};

	virtual bool Init();
	virtual void Update();
	virtual void Close();

	virtual POINT GetPoint(){
		return cursorPos;
	}
	virtual POINT GetMove(){
		POINT p;
		p.x=cursorPos.x-cursorPosPrev.x;
		p.y=cursorPos.y-cursorPosPrev.y;
		return p;
	}
	virtual bool GetPush(int num){
		return buttonState[num];
	}
	virtual bool GetPushNow(int num){
		return buttonState[num]&&(!buttonStatePrev[num]);
	}
	virtual bool GetReleaseNow(int num){
		return (!buttonState[num])&&buttonStatePrev[num];
	}
};
