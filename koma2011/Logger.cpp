
#include "Logger.h"
#include "ApplicationProperty.h"

#define DEF_LOG_NAME "Log.txt"

bool Logger::isConsoleLog=false;
bool Logger::isTextLog=false;
string Logger::myTextLogPath="";

void Logger::PrintLog(const char* str,va_list arg,bool rtn){
	if(isConsoleLog){
		vprintf_s(str,arg);
		if(rtn)printf_s("\n");
	}
	if(isTextLog){
		FILE* logFp;
		fopen_s(&logFp,myTextLogPath.c_str(),"a");
		if(logFp!=NULL){
			vfprintf_s(logFp,str,arg);
			fprintf(logFp,"\n");
			fclose(logFp);
		}
	}
}

void Logger::SetTextLog(bool flag){
	FILE* logFp;
	if(flag){
		myTextLogPath=ApplicationProperty::ReadSetupString("Logger","LogPath",DEF_LOG_NAME);
		fopen_s(&logFp,myTextLogPath.c_str(),"w");
		if(logFp!=NULL){
			fclose(logFp);
		}else{
			flag=false;
		}
	}
	Logger::isTextLog=flag;
}
void Logger::SetConsoleLog(bool flag){
	Logger::isConsoleLog=flag;
}



void Logger::Print(const char* str,...){
	va_list arg;
	va_start(arg,str);
	Logger::PrintLog(str,arg,false);
	va_end(arg);
}

void Logger::Println(const char* str,...){
	va_list arg;
	va_start(arg,str);
	Logger::PrintLog(str,arg,true);
	va_end(arg);
}

void Logger::Debug(const char* str,...){
#ifdef _DEBUG
	va_list arg;
	va_start(arg,str);
	Logger::PrintLog(str,arg,false);
	va_end(arg);
#endif
}

void Logger::Debugln(const char* str,...){
#ifdef _DEBUG
	va_list arg;
	va_start(arg,str);
	Logger::PrintLog(str,arg,true);
	va_end(arg);
#endif
}