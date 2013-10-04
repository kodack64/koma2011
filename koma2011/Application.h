#pragma once

#include "Common.h"
#include "AudioAdapter.h"
#include "GraphicAdapter.h"
#include "InputAdapter.h"
#include "PackageManager.h"

class Application{
private:
	AudioAdapter* audioAdapter;
	GraphicAdapter* graphicAdapter;
	InputAdapter* inputAdapter;
	PackageManager* packageManager;
public:
	Application();
	virtual ~Application();

	virtual bool Init(int* argc,char** argv,HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	virtual void Close();
	virtual void Run();
};