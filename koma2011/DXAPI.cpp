
#include "DXAPI.h"
#include "Logger.h"
#include "ApplicationProperty.h"
#include "PackageManager.h"

#define DEF_WINDOW_SIZE_X 640
#define DEF_WINDOW_SIZE_Y 480
#define DEF_WINDOW_POS_X 100
#define DEF_WINDOW_POS_Y 100
#define DEF_USE_GAMEMODE 0
#define DEF_PROGRAM_NAME "synth"
#define DEF_GRAPHIC_FPS 60
#define DEF_TIMER_FPS 60
#define DEF_USE_DISPLAY_SETTING 1


DXAPI* DXAPI::myInstance=NULL;

DXAPI* DXAPI::CreateInstance(){
	if(myInstance==NULL){
		myInstance=new DXAPI;
	}
	return myInstance;
}
void DXAPI::DestroyInstance(){
	SAFE_DELETE(myInstance);
}

DXAPI::DXAPI(){
	lpd3d=NULL;
	lpd3ddev=NULL;
	pVertex=NULL;
}

DXAPI::~DXAPI(){
	SAFE_RELEASE(pVertex);
	SAFE_RELEASE(lpd3ddev);
	SAFE_RELEASE(lpd3d);
}

LRESULT	CALLBACK DXAPI::WndProc(HWND hWnd,UINT uiMessage,WPARAM wparam,LPARAM lparam)
{
	switch(uiMessage)
	{
	case WM_CREATE:
		return 0;
	case WM_CLOSE:
		SendMessage(hWnd,WM_DESTROY,0,0L);
		return 0;
	case WM_KEYDOWN:
		switch(wparam){
		case VK_TAB:
			myInstance->ChangeWindowed();
			break;
		case VK_F1:
		case VK_ESCAPE:
			SendMessage(hWnd,WM_DESTROY,0,0L);
			break;
		case VK_F2:
			PackageManager::BackToStart();
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		myInstance->isEndFlag=true;
		return 0;
	default:
		break;
	}
	return DefWindowProc(hWnd,uiMessage,wparam,lparam);
}

void DXAPI::ChangeWindowed(){
	HRESULT hr;
	Logger::Debug("[GraphicAPI] Change Window Type ");
	Logger::Debugln(gamemode?"Fullsc -> Window":"Window->Fullsc");
	if (gamemode){
		hr = lpd3ddev->Reset(&d3dpw);
	}else{
		hr = lpd3ddev->Reset(&d3dpf);
	}

	if (FAILED(hr)){
		Logger::Println("[GraphicAPI]	Device Reset Fail");
		SendMessage(hWnd,WM_DESTROY,0,0L);
		return;
	}

	if (gamemode){
		//make window
		SetWindowLong(hWnd,GWL_STYLE,WS_OVERLAPPEDWINDOW&(~WS_MAXIMIZEBOX)&(~WS_MINIMIZEBOX)&(~WS_THICKFRAME));
		SetWindowPos(hWnd, NULL,winx,winy,width + GetSystemMetrics(SM_CXDLGFRAME) * 2, height + GetSystemMetrics(SM_CYDLGFRAME) * 2 +GetSystemMetrics(SM_CYCAPTION),SWP_SHOWWINDOW);
	}else{
		//make fullsc
		SetWindowLong(hWnd,GWL_STYLE,WS_POPUP|WS_VISIBLE);
		SetWindowPos(hWnd, NULL,0,0,width,height,SWP_SHOWWINDOW);
	}
	gamemode=!gamemode;

	ResetState();

	return;
}

bool DXAPI::Init(){
	int count;
	WNDCLASSEX wcex;
	D3DDISPLAYMODE displayMode;
	D3DFORMAT d3dFormat;
	vector<D3DDISPLAYMODE>    displayModeList;
	D3DCAPS9 d3dcaps;

	string mySection = "Graphic";
	width=ApplicationProperty::ReadSetupInt(mySection,"WindowSizeX",DEF_WINDOW_SIZE_X);
	height=ApplicationProperty::ReadSetupInt(mySection,"WindowSizeY",DEF_WINDOW_SIZE_Y);
	winx=ApplicationProperty::ReadSetupInt(mySection,"WindowPositionX",DEF_WINDOW_POS_X);
	winy=ApplicationProperty::ReadSetupInt(mySection,"WindowPositionY",DEF_WINDOW_POS_Y);
	gamemode=(ApplicationProperty::ReadSetupInt(mySection,"UseGameMode",DEF_USE_GAMEMODE)!=0);
	name=ApplicationProperty::ReadSetupString(mySection,"ProgramName",DEF_PROGRAM_NAME);
	graphicFps=ApplicationProperty::ReadSetupInt(mySection,"Fps",DEF_GRAPHIC_FPS);
	timerFps=ApplicationProperty::ReadSetupInt("Timer","Fps",DEF_TIMER_FPS);
	int useDisplaySetting=ApplicationProperty::ReadSetupInt(mySection.c_str(),"UseDisplaySetting",DEF_USE_DISPLAY_SETTING);

	isEndFlag=false;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = DXAPI::WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = ApplicationProperty::hInstance;
	wcex.hIcon          = NULL;
	wcex.hCursor        = NULL;
	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName   = NULL;
	wcex.lpszClassName  = name.c_str();
	wcex.hIconSm        = NULL;

	Logger::Debugln("[GraphicAPI]	Register Window Class");
	if(!RegisterClassEx(&wcex)){
		Logger::Println("[GraphicAPI]	Window Regist Fail");
		return false;
	}

	Logger::Debugln("[GraphicAPI]	Create Window");

	DWORD windowStyle;
	int cwwidth;
	int cwheight;
	if(gamemode){
		windowStyle=WS_POPUP;
		cwwidth=width;
		cwheight=height;
	}else{
		windowStyle=WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU;
		cwwidth = width+GetSystemMetrics(SM_CXDLGFRAME)*2;
		cwheight= height+GetSystemMetrics(SM_CXDLGFRAME)*2+GetSystemMetrics(SM_CYCAPTION);
	}
	hWnd = CreateWindow(
		name.c_str()
		,name.c_str()
		,windowStyle
		,winx
		,winy 
		,cwwidth
		,cwheight
		,NULL
		,NULL
		,ApplicationProperty::hInstance
		,NULL
		);
	if(hWnd==NULL){
		Logger::Println("[GraphicAPI]	Create Window Fail");
		return false;
	}
	SetCursor(NULL);
	ApplicationProperty::hWnd=hWnd;

	Logger::Debugln("[GraphicAPI]	Create Direct3D Interface");
	lpd3d=Direct3DCreate9(D3D_SDK_VERSION);
	if(lpd3d==NULL){
		Logger::Println("[GraphicAPI]	DirectX Create Fail");
	}
	ZeroMemory(&d3dpw,sizeof(D3DPRESENT_PARAMETERS));
	ZeroMemory(&d3dpf,sizeof(D3DPRESENT_PARAMETERS));

	lpd3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&displayMode);

	d3dFormat=D3DFMT_X8R8G8B8;

	d3dpw.BackBufferCount = 1;
	d3dpf.BackBufferCount = 1;

	d3dpw.SwapEffect = D3DSWAPEFFECT_FLIP;
	d3dpf.SwapEffect = D3DSWAPEFFECT_FLIP;

	d3dpw.hDeviceWindow = hWnd;
	d3dpf.hDeviceWindow = hWnd;

	d3dpw.Flags = 0;
	d3dpf.Flags = 0;

	d3dpw.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpf.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	d3dpw.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	d3dpf.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	d3dpw.Windowed = TRUE;
	d3dpf.Windowed = FALSE;

//	d3dpw.BackBufferWidth = 640;
//	d3dpw.BackBufferHeight = 480;
//	d3dpf.BackBufferWidth = 640;
//	d3dpf.BackBufferHeight = 480;
	d3dpw.BackBufferWidth = width;
	d3dpw.BackBufferHeight = height;
	d3dpf.BackBufferWidth = width;
	d3dpf.BackBufferHeight = height;

	d3dpw.BackBufferFormat=displayMode.Format;
	d3dpf.BackBufferFormat = d3dFormat;

	d3dpw.EnableAutoDepthStencil = TRUE;
	d3dpf.EnableAutoDepthStencil = TRUE;
	d3dpf.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpw.AutoDepthStencilFormat = D3DFMT_D16;


	d3dpf.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpf.MultiSampleQuality = 0;

	lpd3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps);
	count = lpd3d->GetAdapterModeCount(D3DADAPTER_DEFAULT,d3dFormat);
	if(FAILED(lpd3d->CheckDeviceFormat(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,d3dFormat,D3DUSAGE_DEPTHSTENCIL,D3DRTYPE_TEXTURE,d3dpf.AutoDepthStencilFormat))){
		Logger::Println("[GraphicAPI]	Fullscreen Device Format Check Fail");
		return false;
	}
	if(FAILED(lpd3d->CheckDeviceFormat(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,d3dFormat,D3DUSAGE_DEPTHSTENCIL,D3DRTYPE_TEXTURE,d3dpw.AutoDepthStencilFormat))){
		Logger::Println("[GraphicAPI]	Windowed Device Format Check Fail");
		return false;
	}
	if(FAILED(lpd3d->CheckDeviceType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,d3dpf.BackBufferFormat,d3dpf.BackBufferFormat,FALSE))){
		Logger::Println("[GraphicAPI]	Fullscreen DeviceType Check Fail");
		return false;
	}
	if(FAILED(lpd3d->CheckDeviceType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,d3dpw.BackBufferFormat,d3dpw.BackBufferFormat,TRUE))){
		Logger::Println("[GraphicAPI]	Windowed DeviceType Check Fail");
		return false;
	}
	Logger::Debugln("[GraphicAPI]	Device Parameter Check OK");
	D3DPRESENT_PARAMETERS* d3dpp;
	if(gamemode){
		d3dpp=&d3dpf;
	}else{
		d3dpp=&d3dpw;
	}
	if( FAILED( lpd3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, d3dpp, &lpd3ddev ) ) ){
		if( FAILED( lpd3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, d3dpp, &lpd3ddev ) ) ){
			if( FAILED( lpd3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, d3dpp, &lpd3ddev ) ) ){
				if( FAILED( lpd3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, d3dpp, &lpd3ddev ) ) ){
					Logger::Println("[GraphicAPI]	Create Device Fail");
					SAFE_RELEASE(lpd3d);
					return false;
				}
				else Logger::Println("[GraphicAPI]	Create Device REF");
			}
			else Logger::Println("[GraphicAPI]	Create Device T&L REF");
		}
		else Logger::Println("[GraphicAPI]	Create Device HAL");
	}
	else Logger::Println("[GraphicAPI]	Create Device T&L HAL");

	ResetState();

//	InitFont();

	if(FAILED(lpd3ddev->CreateVertexBuffer( sizeof(CUSTOMVERTEX)*4, D3DUSAGE_WRITEONLY, FVF_CUSTOM, D3DPOOL_MANAGED, &pVertex, NULL))){
		SAFE_RELEASE(lpd3ddev);
		SAFE_RELEASE(lpd3d);
		return false;
	}

	return true;
}
bool DXAPI::Close(){
	Logger::Println("[GraphicAPI]	Close");
	Logger::Debugln("[GraphicAPI]	Destroy Window");
	DestroyWindow(hWnd);
	for(unsigned int i=0;i<myImage.size();i++){
		SAFE_DELETE(myImage[i]);
	}
	SAFE_RELEASE(pVertex);
	SAFE_RELEASE(lpd3ddev);
	SAFE_RELEASE(lpd3d);
	return true;
}
bool DXAPI::Run(){

	ShowWindow(hWnd,SW_SHOW);
	UpdateWindow(hWnd);
	MSG msg;
	Logger::Println("[GraphicAPI]	Enter MainLoop");

	int timerFrameCount=0,graphicFrameCount=0;
	DWORD time,timeStart;
	timeBeginPeriod(1);
	timeStart=GetTickCount();

	while(!isEndFlag){
		if(PeekMessage(&msg,NULL,0,0,PM_NOREMOVE))
		{
			if(!GetMessage(&msg,NULL,0,0)) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if(msg.message==WM_QUIT){
				isEndFlag=true;
			}
		}
		else{
			time=GetTickCount()-timeStart;
			if(time>=1000){
				char buf[256];
				sprintf_s(buf,256,"%s GraphicFps = %d   TimerFps = %d",name.c_str(),graphicFrameCount,timerFrameCount);
				SetWindowText(hWnd,buf);
				timerFrameCount=0;
				graphicFrameCount=0;
				timeStart=GetTickCount();
				time=0;
			}
			if(time>(1000.0*timerFrameCount/timerFps)){
				timerFrameCount++;
				if(PackageManager::TimerCallbackProc()==-1){
					SendMessage(hWnd,WM_DESTROY,0,0L);
				}
			}
			if(time>(1000.0*graphicFrameCount/graphicFps)){
				graphicFrameCount++;
				lpd3ddev->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_XRGB(0,0,0),1.0f,0);
				lpd3ddev->BeginScene();

				PackageManager::GraphicCallbackProc();

				lpd3ddev->EndScene();
				if (FAILED(lpd3ddev->Present(NULL,NULL,NULL,NULL)))
				{
					if(gamemode){
						lpd3ddev->Reset(&d3dpf);
					}else{
						lpd3ddev->Reset(&d3dpw);
					}
				}
			}

			InvalidateRgn(hWnd,NULL,FALSE);
			UpdateWindow(hWnd);
		}
	}
	timeEndPeriod(1);
	Logger::Println("[GraphicAPI]	Exit MainLoop");
	return true;
}

void DXAPI::ResetState(){
	lpd3ddev->SetRenderState(D3DRS_ZENABLE,TRUE);
	lpd3ddev->SetRenderState(D3DRS_LIGHTING,FALSE);
	lpd3ddev->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	lpd3ddev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
//	lpd3ddev->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
	lpd3ddev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	lpd3ddev->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
	lpd3ddev->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	lpd3ddev->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
	lpd3ddev->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_MODULATE);
	lpd3ddev->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	lpd3ddev->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
}

ImageId DXAPI::CreateImageFromFile(const char* name){
	return CreateImageFromFile(name,Color4f(0,0,0,0));
}
ImageId DXAPI::CreateImageFromFile(const char* name,Color4f colorKey){
	int i;
	int id=-1;
	Logger::Println("[GraphicAPI]	Load Image : %s",name);
	for(i=0;i<(signed)myImage.size();i++){
		if(myImage[i]==NULL){
			id=i;
			myImage[i]=new DXImage(id);
			break;
		}
	}
	if(id==-1){
		id=myImage.size();
		myImage.push_back(new DXImage(id));
	}

	myImage[id]->SetDevice(lpd3ddev);
	myImage[id]->SetVertex(pVertex);
	if(myImage[id]->Load(name,colorKey)==false){
		SAFE_DELETE(myImage[id]);
		return -1;
	}
	return id;
}
void DXAPI::DrawImage(int x,int y,ImageId id){
	if(IsActiveImage(id)){
		myImage[id]->Draw(x,y,0,0,0,0,1.0f,0.0f,Color4f(255,255,255,255));
	}
	return;
}
void DXAPI::DrawImage(int x,int y,int gx,int gy,int gwid,int ghei,ImageId id){
	if(IsActiveImage(id)){
		myImage[id]->Draw(x,y,gx,gy,gwid,ghei,1.0f,0.0f,Color4f(255,255,255,255));
	}
	return;
}
void DXAPI::DrawImage(int x,int y,int gx,int gy,int gwid,int ghei,float zoom,float rot,ImageId id){
	if(IsActiveImage(id)){
		myImage[id]->Draw(x,y,gx,gy,gwid,ghei,zoom,rot,Color4f(255,255,255,255));
	}
	return;
}
void DXAPI::DrawImage(int x,int y,int gx,int gy,int gwid,int ghei,float zoom,float rot,Color4f color,ImageId id){
	if(IsActiveImage(id)){
		myImage[id]->Draw(x,y,gx,gy,gwid,ghei,zoom,rot,color);
	}
	return;
}
void DXAPI::DrawImageLU(int x,int y,ImageId id){
	if(IsActiveImage(id)){
		myImage[id]->DrawLU(x,y,0,0,0,0,1.0f,0.0f,Color4f(255,255,255,255));
	}
	return;
}
void DXAPI::DrawImageLU(int x,int y,int gx,int gy,int gwid,int ghei,ImageId id){
	if(IsActiveImage(id)){
		myImage[id]->DrawLU(x,y,gx,gy,gwid,ghei,1.0f,0.0f,Color4f(255,255,255,255));
	}
	return;
}
void DXAPI::DrawImageLU(int x,int y,int gx,int gy,int gwid,int ghei,float zoom,float rot,ImageId id){
	if(IsActiveImage(id)){
		myImage[id]->DrawLU(x,y,gx,gy,gwid,ghei,zoom,rot,Color4f(255,255,255,255));
	}
	return;
}
void DXAPI::DrawImageLU(int x,int y,int gx,int gy,int gwid,int ghei,float zoom,float rot,Color4f color,ImageId id){
	if(IsActiveImage(id)){
		myImage[id]->DrawLU(x,y,gx,gy,gwid,ghei,zoom,rot,color);
	}
	return;
}

void DXAPI::ReleaseImage(ImageId id){
	if(0<=id && id < (signed)myImage.size()){
		SAFE_DELETE(myImage[id]);
	}
}

int DXAPI::GetImageHeight(ImageId id){
	if(IsActiveImage(id)){
		return myImage[id]->GetHeight();
	}else{
		return 0;
	}
}
int DXAPI::GetImageWidth(ImageId id){
	if(IsActiveImage(id)){
		return myImage[id]->GetWidth();
	}else{
		return 0;
	}
}

bool DXAPI::InitFont(){
	int fontsize = 500;
	LOGFONT lf = {fontsize, 0, 0, 0, 0, 0, 0, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
	CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH | FF_MODERN,"ＭＳ 明朝"};
	HFONT hFont;
	if(!(hFont = CreateFontIndirect(&lf))){
		return false;
	}
	HDC hdc = GetDC(NULL);
	HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

	// 文字コード取得
	TCHAR *c = "あ";
	UINT code = 0;
	if(IsDBCSLeadByte(*c))
		code = (BYTE)c[0]<<8 | (BYTE)c[1];
	else
		code = c[0];

	/*
	// フォントビットマップ取得
	GetTextMetrics( hdc, &TM );
	CONST MAT2 Mat = {{0,1},{0,0},{0,0},{0,1}};
	DWORD size = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, 0, NULL, &Mat);
	BYTE *ptr = new BYTE[size];
	GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &GM, size, ptr, &Mat);
	*/

	// デバイスコンテキストとフォントハンドルの開放
	SelectObject(hdc, oldFont);
	DeleteObject(hFont);
	ReleaseDC(NULL, hdc);

	return true;
}

bool DXAPI::IsActiveImage(ImageId id){
	bool flag=false;
	if(0<=id && id<(signed)myImage.size()){
		if(myImage[id]!=NULL){
			flag=true;
		}
	}
	return flag;
}

LPDIRECT3D9 DXAPI::GetDirect3D(){
	return lpd3d;
}
LPDIRECT3DDEVICE9 DXAPI::GetDirect3DDevice(){
	return lpd3ddev;
}
int DXAPI::GetScreenWidth(){
	if(gamemode){
		return d3dpf.BackBufferWidth;
	}else{
		return d3dpw.BackBufferWidth;
	}
	return 0;
}
int DXAPI::GetScreenHeight(){
	if(gamemode){
		return d3dpf.BackBufferHeight;
	}else{
		return d3dpw.BackBufferHeight;
	}
	return 0;
}
