
#pragma once

#include "Common.h"
#include "InputApi.h"

class InputAdapter{
private:
	static InputAdapter* myInstance;
	InputApi* inputApi;
public:
	InputAdapter();
	virtual ~InputAdapter();
	static InputAdapter* CreateInstance();
	static void DestroyInstance();

	virtual bool Init();
	virtual void Close();

	virtual InputApi* GetAPI();
};