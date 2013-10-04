#pragma once

#include "DXAPI.h"
#include "Common.h"

class GraphicAdapter{
private:
	static GraphicAdapter* myInstance;
	DXAPI* graphicApi;
	int type;
public:
	GraphicAdapter();
	virtual ~GraphicAdapter();
	static GraphicAdapter* CreateInstance();
	static void DestroyInstance();

	virtual bool Init();
	virtual void Close();
	virtual void Run();

	virtual GraphicApi* GetAPI();
};