
#pragma once

#include "Common.h"

typedef long WAVEPOS;

class WaveShell{
private:
	vector <float> shell;
	long cursor;
public:
	WaveShell();
	virtual ~WaveShell();
	virtual void Init();
	virtual void Close();
	virtual void Clear();
	virtual bool Empty();

	virtual WAVEPOS GetLength();
	virtual void Extend(WAVEPOS ext);
	virtual void Resize(WAVEPOS size);
	virtual void Add(float pos);

	virtual WAVEPOS GetCursor();
	virtual float GetCursorPos();
	virtual float GetPosFromCursor(WAVEPOS distance);
	virtual float GetPos(WAVEPOS cursor);

	virtual void SetCursor(WAVEPOS cursor);
	virtual void SetCursorPos(float pos);
	virtual void SetPosFromCursor(float pos,WAVEPOS distance);
	virtual void SetPos(float pos,WAVEPOS cursor);
	virtual void Forward();
	virtual void Forward(WAVEPOS dist);
};
