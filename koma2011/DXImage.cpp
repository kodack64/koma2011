
#include "DXImage.h"
#include "Logger.h"

DXImage::DXImage(int id){
	myId=id;
	lpd3ddev=NULL;
	myTexture=NULL;
}

DXImage::~DXImage(){
	SAFE_RELEASE(myTexture);
}

bool DXImage::Load(const char* filename,Color4f c){
	if(lpd3ddev==NULL)return false;
	HRESULT hr = D3DXCreateTextureFromFileEx(lpd3ddev,filename,D3DX_DEFAULT,D3DX_DEFAULT,D3DX_DEFAULT,0,D3DFMT_A8R8G8B8
		,D3DPOOL_MANAGED,D3DX_DEFAULT,D3DX_DEFAULT,D3DCOLOR_RGBA(c.red,c.green,c.blue,c.alpha),NULL,NULL,&myTexture);
	if(FAILED(hr)){
		Logger::Println("[GraphicAPI] Load Fail : %s",filename);
		return false;
	}
	myTexture->GetLevelDesc(0,&desc);
	return true;
}


void DXImage::MoveMatrix(LPCUSTOMVERTEX v, float x, float y, float z)
{
    D3DXMATRIX PosMatrix, MoveMatrix;
    int i;
    D3DXMatrixIdentity(&PosMatrix);
    D3DXMatrixIdentity(&MoveMatrix);
    D3DXMatrixTranslation(&MoveMatrix, x, y, z);
    for ( i=0 ; i<4 ; i++ ) {
        D3DXMatrixTranslation(&PosMatrix, v[i].x, v[i].y, v[i].z);
        PosMatrix *= MoveMatrix;
        v[i].x = PosMatrix._41;
        v[i].y = PosMatrix._42;
        v[i].z = PosMatrix._43;
    }
}

void DXImage::ZoomMatrix(LPCUSTOMVERTEX v, float x, float y, float z)
{
    D3DXMATRIX PosMatrix, ZoomMatrix;
    float x1, y1;
    int i;
    x1 = (v[2].x - v[0].x) / 2.0f + v[0].x;
    y1 = (v[3].y - v[1].y) / 2.0f + v[1].y;
    MoveMatrix(v, -x1, -y1, 0.0f);
    D3DXMatrixIdentity(&PosMatrix);
    D3DXMatrixIdentity(&ZoomMatrix);
    D3DXMatrixScaling(&ZoomMatrix, x, y, z);
    for ( i=0 ; i<4 ; i++ ) {
        D3DXMatrixTranslation(&PosMatrix, v[i].x, v[i].y, v[i].z);
        PosMatrix *= ZoomMatrix;
        v[i].x = PosMatrix._41;
        v[i].y = PosMatrix._42;
        v[i].z = PosMatrix._43;
    }
    MoveMatrix(v, x1, y1, 0.0f);
}

void DXImage::RotateMatrix(LPCUSTOMVERTEX v, float Rotate)
{
    D3DXMATRIX PosMatrix, RotateMatrix;
    float x1, y1;
    int i;
    x1 = (v[2].x - v[0].x) / 2.0f + v[0].x;
    y1 = (v[3].y - v[1].y) / 2.0f + v[1].y;
    MoveMatrix(v, -x1, -y1, 0.0f);
    D3DXMatrixIdentity(&PosMatrix);
    D3DXMatrixIdentity(&RotateMatrix);
    D3DXMatrixRotationZ(&RotateMatrix, (float)Rotate);
    for ( i=0 ; i<4 ; i++ ) {
        D3DXMatrixTranslation(&PosMatrix, v[i].x, v[i].y, v[i].z);
        PosMatrix *= RotateMatrix;
        v[i].x = PosMatrix._41;
        v[i].y = PosMatrix._42;
        v[i].z = PosMatrix._43;
    }
    MoveMatrix(v, x1, y1, 0.0f);
}

void DXImage::MoveMatrixLU(LPCUSTOMVERTEX v, float x, float y, float z)
{
    D3DXMATRIX PosMatrix, MoveMatrix;
    int i;
    D3DXMatrixIdentity(&PosMatrix);
    D3DXMatrixIdentity(&MoveMatrix);
    D3DXMatrixTranslation(&MoveMatrix, x, y, z);
    for ( i=0 ; i<4 ; i++ ) {
        D3DXMatrixTranslation(&PosMatrix, v[i].x, v[i].y, v[i].z);
        PosMatrix *= MoveMatrix;
        v[i].x = PosMatrix._41;
        v[i].y = PosMatrix._42;
        v[i].z = PosMatrix._43;
    }
}

void DXImage::ZoomMatrixLU(LPCUSTOMVERTEX v, float x, float y, float z)
{
    D3DXMATRIX PosMatrix, ZoomMatrix;
    float x1, y1;
    int i;
    x1 = (v[2].x - v[0].x) / 2.0f + v[0].x;
    y1 = (v[3].y - v[1].y) / 2.0f + v[1].y;
    MoveMatrixLU(v, -x1, -y1, 0.0f);
    D3DXMatrixIdentity(&PosMatrix);
    D3DXMatrixIdentity(&ZoomMatrix);
    D3DXMatrixScaling(&ZoomMatrix, x, y, z);
    for ( i=0 ; i<4 ; i++ ) {
        D3DXMatrixTranslation(&PosMatrix, v[i].x, v[i].y, v[i].z);
        PosMatrix *= ZoomMatrix;
        v[i].x = PosMatrix._41;
        v[i].y = PosMatrix._42;
        v[i].z = PosMatrix._43;
    }
    MoveMatrixLU(v, x1*x, y1*y, 0.0f*z);
}

void DXImage::RotateMatrixLU(LPCUSTOMVERTEX v, float Rotate)
{
    D3DXMATRIX PosMatrix, RotateMatrix;
    float x1, y1;
    int i;
    x1 = (v[2].x - v[0].x) / 2.0f + v[0].x;
    y1 = (v[3].y - v[1].y) / 2.0f + v[1].y;
    MoveMatrixLU(v, -x1, -y1, 0.0f);
    D3DXMatrixIdentity(&PosMatrix);
    D3DXMatrixIdentity(&RotateMatrix);
    D3DXMatrixRotationZ(&RotateMatrix, (float)Rotate);
    for ( i=0 ; i<4 ; i++ ) {
        D3DXMatrixTranslation(&PosMatrix, v[i].x, v[i].y, v[i].z);
        PosMatrix *= RotateMatrix;
        v[i].x = PosMatrix._41;
        v[i].y = PosMatrix._42;
        v[i].z = PosMatrix._43;
    }
    MoveMatrixLU(v, x1, y1, 0.0f);
}

void DXImage::Draw(int x,int y,int gx,int gy,int gwid,int ghei,float zoom,float rot,Color4f color){

	if(lpd3ddev==NULL)return;
	if(myTexture==NULL)return;

	float alpha=255;
	float ux,uy,uw,uh;

	int wid=desc.Width;
	int hei=desc.Height;

	if(gwid==0)gwid=wid-gx;
	if(ghei==0)ghei=hei-gy;
		
/*	ux=(float)(gx+0.5f)/wid;
	uy=(float)(gy+0.5f)/hei;
	uw=(float)(gx+gwid+0.5f)/wid;
	uh=(float)(gy+ghei+0.5f)/hei;*/

	ux=((float)gx)/wid;
	uy=((float)gy)/hei;
	uw=((float)gx+gwid)/wid;
	uh=((float)gy+ghei)/hei;

	if(alpha>255)alpha=255;
	if(alpha<0)alpha=0;
	if(ux<0.0f)ux=0.0f;
	if(uy<0.0f)uy=0.0f;
	if(ux>1.0f)ux=1.0f;
	if(uy>1.0f)uy=1.0f;

	float xf,yf,wf,hf;

	xf=(float)x-wid/2;
	yf=(float)y-hei/2;
	wf=(float)x+wid/2;
	hf=(float)y+hei/2;

	DWORD c = D3DCOLOR_RGBA(
		(int)(color.red),
		(int)(color.green),
		(int)(color.blue),
		(int)(color.alpha));
	CUSTOMVERTEX vertex[4]={
		{xf	,yf	,0.0f	,1.0f	,c	,ux	,uy	},
		{wf	,yf	,0.0f	,1.0f	,c	,uw	,uy	},
		{wf	,hf	,0.0f	,1.0f	,c	,uw	,uh	},
		{xf	,hf	,0.0f	,1.0f	,c	,ux	,uh	}
	};

	if(rot!=0)RotateMatrix(vertex,rot);
	if(zoom!=1.0f)ZoomMatrix(vertex,zoom,zoom,zoom);

//	void *pData;
//	if(FAILED(pVertex->Lock(0, sizeof(CUSTOMVERTEX)*4, (void**)&pData, 0))){
//		return;
//	}
//	memcpy(pData,vertex, sizeof(CUSTOMVERTEX)*4);
//	pVertex->Unlock();

	lpd3ddev->SetTexture(0,myTexture);
	lpd3ddev->SetFVF(FVF_CUSTOM);
//	lpd3ddev->SetStreamSource(0, pVertex, 0, sizeof(CUSTOMVERTEX));
//	lpd3ddev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
	lpd3ddev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,(void*)vertex,sizeof(CUSTOMVERTEX));
}

void DXImage::DrawLU(int x,int y,int gx,int gy,int gwid,int ghei,float zoom,float rot,Color4f color){

	if(lpd3ddev==NULL)return;
	if(myTexture==NULL)return;

	float alpha=255;
	float ux,uy,uw,uh;

	int wid=desc.Width;
	int hei=desc.Height;

	if(gwid==0)gwid=wid-gx;
	if(ghei==0)ghei=hei-gy;

	ux=((float)gx)/wid;
	uy=((float)gy)/hei;
	uw=((float)gx+gwid)/wid;
	uh=((float)gy+ghei)/hei;

	if(alpha>255)alpha=255;
	if(alpha<0)alpha=0;
	if(ux<0.0f)ux=0.0f;
	if(uy<0.0f)uy=0.0f;
	if(ux>1.0f)ux=1.0f;
	if(uy>1.0f)uy=1.0f;

	float xf,yf,wf,hf;

	xf=(float)x;
	yf=(float)y;
	wf=(float)x+wid;
	hf=(float)y+hei;

	DWORD c = D3DCOLOR_RGBA(
		(int)(color.red),
		(int)(color.green),
		(int)(color.blue),
		(int)(color.alpha));
	CUSTOMVERTEX vertex[4]={
		{xf	,yf	,0.0f	,1.0f	,c	,ux	,uy	},
		{wf	,yf	,0.0f	,1.0f	,c	,uw	,uy	},
		{wf	,hf	,0.0f	,1.0f	,c	,uw	,uh	},
		{xf	,hf	,0.0f	,1.0f	,c	,ux	,uh	}
	};

	if(rot!=0)RotateMatrixLU(vertex,rot);
	if(zoom!=1.0f)ZoomMatrixLU(vertex,zoom,zoom,zoom);

//	void *pData;
//	if(FAILED(pVertex->Lock(0, sizeof(CUSTOMVERTEX)*4, (void**)&pData, 0))){
//		return;
//	}
//	memcpy(pData,vertex, sizeof(CUSTOMVERTEX)*4);
//	pVertex->Unlock();

	lpd3ddev->SetTexture(0,myTexture);
//	lpd3ddev->SetStreamSource(0, pVertex, 0, sizeof(CUSTOMVERTEX));
	lpd3ddev->SetFVF(FVF_CUSTOM);
//	lpd3ddev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
	lpd3ddev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,(void*)vertex,sizeof(CUSTOMVERTEX));
}

void DXImage::SetDevice(LPDIRECT3DDEVICE9 dev){
	lpd3ddev=dev;
}

void DXImage::SetVertex(IDirect3DVertexBuffer9* pv){
	pVertex=pv;
}

int DXImage::GetWidth(){
	return desc.Width;
}
int DXImage::GetHeight(){
	return desc.Height;
}