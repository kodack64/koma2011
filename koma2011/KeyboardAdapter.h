
#pragma once

#include "Common.h"

class KeyboardAdapter{
private:
	static KeyboardAdapter* myInstance;
	BYTE key[256];
	BYTE oldkey[256];

public:
	static KeyboardAdapter* CreateInstance();
	static void DestroyInstance();

	virtual bool Init();
	virtual void Update();
	virtual void Close();
	virtual bool GetPush(int num){
		return (key[num] & 0x80)>0;
	}
	virtual bool GetPushNow(int num){
		return (key[num] & 0x80)&&(!(oldkey[num] & 0x80));
	}
	virtual bool GetReleaseNow(int num){
		return (!(key[num] & 0x80))&&(oldkey[num] & 0x80);
	}
};

