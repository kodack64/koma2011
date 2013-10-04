
#pragma once 

#include "Common.h"
#include "KeyboardAdapter.h"
#include "MouseAdapter.h"

class InputApi{
private:
	static InputApi* myInstance;
public:
	KeyboardAdapter* key;
	MouseAdapter* mouse;
	InputApi();
	virtual ~InputApi();
	static InputApi* CreateInstance();
	static void DestroyInstance();
	virtual bool Init();
	virtual void Update();
	virtual void Close();
};