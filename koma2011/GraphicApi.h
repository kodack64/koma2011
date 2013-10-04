#pragma once

#include "MyImage.h"
#include "Common.h"

class GraphicApi{
private:
public:
	virtual ImageId CreateImageFromFile(const char*)=0;
	virtual ImageId CreateImageFromFile(const char*,Color4f colorKey)=0;
	virtual void DrawImage(int x,int y,ImageId id)=0;
	virtual void DrawImage(int x,int y,int gx,int gy,int gwid,int ghei,ImageId id)=0;
	virtual void DrawImage(int x,int y,int gx,int gy,int gwid,int ghei,float zoom,float rot,ImageId id)=0;
	virtual void DrawImage(int x,int y,int gx,int gy,int gwid,int ghei,float zoom,float rot,Color4f color,ImageId id)=0;
	virtual void DrawImageLU(int x,int y,ImageId id)=0;
	virtual void DrawImageLU(int x,int y,int gx,int gy,int gwid,int ghei,ImageId id)=0;
	virtual void DrawImageLU(int x,int y,int gx,int gy,int gwid,int ghei,float zoom,float rot,ImageId id)=0;
	virtual void DrawImageLU(int x,int y,int gx,int gy,int gwid,int ghei,float zoom,float rot,Color4f color,ImageId id)=0;
	virtual void ReleaseImage(ImageId id)=0;
	virtual int GetImageHeight(ImageId)=0;
	virtual int GetImageWidth(ImageId)=0;
	virtual int GetScreenWidth()=0;
	virtual int GetScreenHeight()=0;
	virtual LPDIRECT3D9 GetDirect3D()=0;
	virtual LPDIRECT3DDEVICE9 GetDirect3DDevice()=0;
};