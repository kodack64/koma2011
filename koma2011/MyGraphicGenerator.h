#pragma once

#include "GraphicGenerator.h"

class MyGraphicGenerator:public GraphicGenerator{
private:
public:
	MyGraphicGenerator();
	virtual ~MyGraphicGenerator();
	virtual void Init();
	virtual void Close();
	virtual void Run();
	virtual void Generate();
};