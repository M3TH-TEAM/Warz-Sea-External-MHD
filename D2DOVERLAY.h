#pragma once
#include<windows.h>
#include<d2d1.h>
#include<dwrite.h>
#include<dwmapi.h>
#include<string>
#include<fstream>
#include<d3dx9math.h>
#include<d3dx9.h>
#include<d3d9.h>
#include<DxErr.h> //get error from error code
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "dwrite.lib")

class Color
{
public:

	float White[4] = { 1.f, 1.f, 1.f, 1.f };
	float sky[4] = { 0.f, 1.f, 1.f, 1.f };
	float color[4] = { 0.9f, 1.f, 0.f, 1.f };
	float red[4] = { 1, 0, 0, 1.f };
	float pink[4] = { 1.0f, 0.30f, 0.45f, 1.f };
	float green[4] = { 0, 1, 0, 1.f };
	float bule[4] = { 0, 0, 1, 1.f };
	float cyan[4] = { 0, 1, 1, 1.f };
	float bulr[4] = { 0, 0, 1, 1.f };
	float black[4] = { 0, 0, 0 , 1.f };
	float black_trans[4] = { 0.1f, 0.1f, 0.1f, 0.98f };
	float Yellow[4] = { 1.f, 1.f, 0.f, 1.f };
	float purple[4] = { 0.6f, 0, 0.8f, 1.f }; //   160 / 255 = 0.6f

};
extern Color Col;

#define M_PI       3.14159265358979323846   // pi

class Direct2DOverlay;

typedef void(*DrawCallback)(Direct2DOverlay* dis);

class Direct2DOverlay
{
public:
	Direct2DOverlay(HINSTANCE hInstance, DrawCallback callbacker);
	~Direct2DOverlay();
	bool Initialize(std::string gameWindowName, std::string overlayWindowName);
	int d3D9Init(HWND hWnd, INT width, INT height);
	bool IsGood();
	void StartLoop();
	void BeginDraw();
	void ClearScreen();
	void ClearScreen(float r, float g, float b, float a);
	void DrawBox(float x, float y, float width, float height, float thicc, float* rgb, bool filled);
	void DrawFilledRect(float x, float y, int w, int h, float* rgb, bool fill, float size);
	void DrawCircle(float x, float y, float radius, float thicc, float r, float g, float b, float a, bool filled);
	void DrawLine(float x1, float y1, float x2, float y2, float thicc, float r, float g, float b);
	void DrawString(std::wstring str, float fontSize, float x, float y, bool center, float r, float g, float b, float a);
	void hack_DrawCrosshair1(float r, float g, float b, float thicc, float len, float xoffset, float yoffset);
	void EndDraw();
	RECT GetOverlayRect();
	D2D1_SIZE_U size;
	DWORD procID;
	IDirect3D9Ex* d3dObject = NULL; //used to create device
	IDirect3DDevice9Ex* d3dDevice = NULL; //contains functions like begin and end scene 
	D3DPRESENT_PARAMETERS d3dparams; //parameters for creating device
	ID3DXFont* d3dFont = 0; // font used when displaying text

public:
	ID2D1Factory* factory;
	ID2D1HwndRenderTarget* target;
	ID2D1SolidColorBrush* solid_brush;
	IDWriteFactory* w_factory;
	IDWriteTextFormat* w_format;
	IDWriteTextLayout* w_layout;
	bool good;
	bool loopRunning;
	bool drawing;
	std::string gameWindowName;
	HWND overlayWindow;
	HINSTANCE appInstance;
	DrawCallback callback;
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam);
};

