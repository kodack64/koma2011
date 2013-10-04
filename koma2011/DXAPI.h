#pragma once

#include "GraphicApi.h"
#include "DXImage.h"

class DXAPI:public GraphicApi{
private:
	static DXAPI* myInstance;
	bool isEndFlag;

	LPDIRECT3D9 lpd3d;
	LPDIRECT3DDEVICE9 lpd3ddev;
	D3DPRESENT_PARAMETERS d3dpw;
	D3DPRESENT_PARAMETERS d3dpf;
	IDirect3DVertexBuffer9* pVertex;
	TEXTMETRIC TM;
	GLYPHMETRICS GM;
	HWND hWnd;
	bool gamemode;
	int width;
	int height;
	int winx;
	int winy;
	int graphicFps;
	int timerFps;
	string name;
	vector <DXImage*> myImage;
	virtual void ChangeWindowed();
	virtual void ResetState();
	virtual bool InitFont();
public:
	DXAPI();
	virtual ~DXAPI();
	static DXAPI* CreateInstance();
	static void DestroyInstance();
	virtual bool Init();
	virtual bool Close();
	virtual bool Run();

	static LRESULT	CALLBACK WndProc(HWND hWnd,UINT uiMessage,WPARAM wparam,LPARAM lparam);

	virtual ImageId CreateImageFromFile(const char* filename);
	virtual ImageId CreateImageFromFile(const char* filename,Color4f colorKey);
	virtual void DrawImageLU(int x,int y,ImageId id);
	virtual void DrawImageLU(int x,int y,int gx,int gy,int gwid,int ghei,ImageId id);
	virtual void DrawImageLU(int x,int y,int gx,int gy,int gwid,int ghei,float zoom,float rot,ImageId id);
	virtual void DrawImageLU(int x,int y,int gx,int gy,int gwid,int ghei,float zoom,float rot,Color4f color,ImageId id);
	virtual void DrawImage(int x,int y,ImageId id);
	virtual void DrawImage(int x,int y,int gx,int gy,int gwid,int ghei,ImageId id);
	virtual void DrawImage(int x,int y,int gx,int gy,int gwid,int ghei,float zoom,float rot,ImageId id);
	virtual void DrawImage(int x,int y,int gx,int gy,int gwid,int ghei,float zoom,float rot,Color4f color,ImageId id);
	virtual void ReleaseImage(ImageId id);
	virtual int GetImageHeight(ImageId);
	virtual int GetImageWidth(ImageId);
	virtual int GetScreenWidth();
	virtual int GetScreenHeight();
	virtual LPDIRECT3D9 GetDirect3D();
	virtual LPDIRECT3DDEVICE9 GetDirect3DDevice();
	virtual bool IsActiveImage(ImageId);
};