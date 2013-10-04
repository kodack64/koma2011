#pragma once

#include "Common.h"

class Logger{
private:
	static bool isTextLog;
	static bool isConsoleLog;
	static string myTextLogPath;
	static void PrintLog(const char* str,va_list arg,bool rtn);
public:
	static void Print(const char*,...);
	static void Println(const char*,...);
	static void Debug(const char*,...);
	static void Debugln(const char*,...);
	static void SetTextLog(bool);
	static void SetConsoleLog(bool);
};