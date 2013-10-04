#pragma once

class GraphicGenerator{
public:
	virtual void Init()=0;
	virtual void Close()=0;
	virtual void Run()=0;
	virtual void Generate()=0;
};