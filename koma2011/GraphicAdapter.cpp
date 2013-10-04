
#include "GraphicAdapter.h"
#include "ApplicationProperty.h"
#include "Logger.h"
#include "DXAPI.h"


GraphicAdapter* GraphicAdapter::myInstance=NULL;

GraphicAdapter::GraphicAdapter(){
	graphicApi=NULL;
}

GraphicAdapter::~GraphicAdapter(){
}

GraphicAdapter* GraphicAdapter::CreateInstance(){
	if(myInstance==NULL){
		myInstance=new GraphicAdapter;
	}
	return myInstance;
}
void GraphicAdapter::DestroyInstance(){
	SAFE_DELETE(myInstance);
}


bool GraphicAdapter::Init(){
	Logger::Println("[GraphicAdapter]	Initialize");

	graphicApi = DXAPI::CreateInstance();

	if(!graphicApi->Init()){
		return false;
	}
	return true;
}

void GraphicAdapter::Close(){
	Logger::Println("[GraphicAdapter]	Close");
	if(graphicApi!=NULL)graphicApi->Close();
	DXAPI::DestroyInstance();
}

void GraphicAdapter::Run(){
	Logger::Println("[GraphicAdapter]	Run");
	graphicApi->Run();
}

GraphicApi* GraphicAdapter::GetAPI(){
	return graphicApi;
}