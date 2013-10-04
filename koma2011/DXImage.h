#pragma once

#include "Common.h"
#include "MyImage.h"

typedef struct{  
   float x, y, z;
   float rhw;
   DWORD dwColor;
   float u,v;
}CUSTOMVERTEX,*LPCUSTOMVERTEX;

#define FVF_CUSTOM ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )


class DXImage:public MyImage{
private:
	ImageId myId;
	LPDIRECT3DTEXTURE9 myTexture;
	LPDIRECT3DDEVICE9 lpd3ddev;
	IDirect3DVertexBuffer9* pVertex;
	D3DSURFACE_DESC desc;
public:
	DXImage(int id);
	virtual ~DXImage();
	virtual bool Load(const char* filename,Color4f colorKey);
	virtual void SetDevice(LPDIRECT3DDEVICE9 lpdev);
	virtual void SetVertex(IDirect3DVertexBuffer9* pVertex);
	virtual void Draw(int x,int y,int gx,int gy,int gwid,int ghei,float zoom,float rot,Color4f color);
	virtual void DrawLU(int x,int y,int gx,int gy,int gwid,int ghei,float zoom,float rot,Color4f color);
	virtual void MoveMatrix(LPCUSTOMVERTEX v, float x, float y, float z);
	virtual void RotateMatrix(LPCUSTOMVERTEX v, float Rotate);
	virtual void ZoomMatrix(LPCUSTOMVERTEX v, float x, float y, float z);
	virtual void MoveMatrixLU(LPCUSTOMVERTEX v, float x, float y, float z);
	virtual void RotateMatrixLU(LPCUSTOMVERTEX v, float Rotate);
	virtual void ZoomMatrixLU(LPCUSTOMVERTEX v, float x, float y, float z);
	virtual int GetWidth();
	virtual int GetHeight();
	virtual LPDIRECT3DTEXTURE9 GetTexture(){return myTexture;};
};