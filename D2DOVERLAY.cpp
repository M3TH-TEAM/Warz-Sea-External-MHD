#include"D2DOverlay.h"
#include <iostream>

Direct2DOverlay::Direct2DOverlay(HINSTANCE hInstance, DrawCallback callbacker)
{
	this->factory = NULL;
	this->solid_brush = NULL;
	this->target = NULL;
	this->w_factory = NULL;
	this->w_format = NULL;
	this->w_layout = NULL;
	this->good = false;
	this->loopRunning = false;
	this->drawing = false;
	this->overlayWindow = NULL;
	this->appInstance = hInstance;
	this->callback = callbacker;
}

Direct2DOverlay::~Direct2DOverlay()
{
	this->good = false;
	this->loopRunning = false;
	if (this->factory != NULL)
	{
		this->factory->Release();
		this->factory = NULL;
	}
	if (this->target != NULL)
	{
		this->target->Release();
		this->target = NULL;
	}
	if (this->solid_brush != NULL)
	{
		this->solid_brush->Release();
		this->solid_brush = NULL;
	}
	if (this->w_factory != NULL)
	{
		this->w_factory->Release();
		this->w_factory = NULL;
	}
	if (this->w_format != NULL)
	{
		this->w_format->Release();
		this->w_format = NULL;
	}
	if (this->w_layout != NULL)
	{
		this->w_layout->Release();
		this->w_layout = NULL;
	}
}

RECT rc;
bool Direct2DOverlay::Initialize(std::string gameWindowName, std::string overlayWindowName)
{
	if (!this->good && !this->loopRunning)
	{
		if (!overlayWindowName.empty() && !gameWindowName.empty())
		{
			this->gameWindowName = gameWindowName;

			WNDCLASSEX wClass;
			ZeroMemory(&wClass, sizeof(wClass));
			wClass.cbSize = sizeof(wClass);
			wClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
			wClass.hCursor = LoadCursor(NULL, IDC_CROSS);
			wClass.lpszClassName = "Y.U.R";
			wClass.style = 0;
			wClass.lpfnWndProc = WindowProc;
			wClass.hInstance = this->appInstance;

			if (RegisterClassEx(&wClass))
			{
				RECT rekt = { 0,0,200,200 };
				AdjustWindowRectEx(&rekt, WS_POPUP, FALSE, WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_COMPOSITED);

				this->overlayWindow = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST, "Y.U.R", overlayWindowName.c_str(), WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, rekt.right - rekt.left, rekt.bottom - rekt.top, NULL, NULL, this->appInstance, NULL);


				//this->overlayWindow = CreateWindowEx(WS_EX_COMPOSITED | WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_NOACTIVATE, CLASS_NAME, overlayWindowName.c_str(), WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, rekt.right - rekt.left, rekt.bottom - rekt.top, NULL, NULL, this->appInstance, NULL);
				if (this->overlayWindow != NULL)
				{
					MARGINS mar = { -1 };
					DwmExtendFrameIntoClientArea(this->overlayWindow, &mar);





					HRESULT res = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &this->factory);
					if (SUCCEEDED(res))
					{
						res = this->factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)), D2D1::HwndRenderTargetProperties(this->overlayWindow, D2D1::SizeU(200, 200), D2D1_PRESENT_OPTIONS_IMMEDIATELY), &this->target);
						if (SUCCEEDED(res))
						{
							res = this->target->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.0f, 0.0f), &this->solid_brush);
							if (SUCCEEDED(res))
							{
								res = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&this->w_factory));
								if (SUCCEEDED(res))
								{
									res = this->w_factory->CreateTextFormat(L"arial", NULL, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 10.0f, L"en-us", &this->w_format);
									if (SUCCEEDED(res))
									{
										this->good = true;
									}
								}
							}
						}
					}







				}
			}
			HWND gamewindow = FindWindow(NULL, this->gameWindowName.c_str());

			if (gamewindow != NULL)
			{
				WINDOWINFO info;
				ZeroMemory(&info, sizeof(info));
				info.cbSize = sizeof(info);
				GetWindowInfo(gamewindow, &info);

				D2D1_SIZE_U siz;
				siz.height = ((info.rcClient.bottom) - (info.rcClient.top));
				siz.width = ((info.rcClient.right) - (info.rcClient.left));
				this->d3D9Init(this->overlayWindow, siz.width, siz.height);
			}


		}

	}
	return this->good;
}

int Direct2DOverlay::d3D9Init(HWND hWnd, INT width, INT height)
{

	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &this->d3dObject)))
	{
		exit(1);
	}

	ZeroMemory(&this->d3dparams, sizeof(this->d3dparams));

	d3dparams.BackBufferWidth = width;
	d3dparams.BackBufferHeight = height;
	d3dparams.Windowed = TRUE;
	d3dparams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dparams.hDeviceWindow = hWnd;
	d3dparams.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	d3dparams.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dparams.EnableAutoDepthStencil = TRUE;
	d3dparams.AutoDepthStencilFormat = D3DFMT_D16;

	HRESULT res = this->d3dObject->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &this->d3dparams, 0, &this->d3dDevice);

	if (FAILED(res))
	{
		/*	std::wstring ws(DXGetErrorString(res));
		std::string str(ws.begin(), ws.end());
		std::wstring ws2(DXGetErrorDescription(res));
		std::string str2(ws2.begin(), ws2.end());
		std::string error = "Error: " + str + " error description: " + str2;*/
		exit(1);
	}

	D3DXCreateFont(this->d3dDevice, 50, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEVICE_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH, "Comic Sans", &this->d3dFont);

	return 0;

}

bool Direct2DOverlay::IsGood()
{
	return this->good;
}

#include <TlHelp32.h>
std::uint32_t GetProcIDByName(const char* proc)
{
	auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	auto pe = PROCESSENTRY32{ sizeof(PROCESSENTRY32) };
	uint32_t id = 0;

	if (Process32First(snapshot, &pe)) {
		do {
			if (!strcmp(proc, pe.szExeFile)) {
				id = pe.th32ProcessID;
			}
		} while (Process32Next(snapshot, &pe));
	}
	CloseHandle(snapshot);
	return id;
}
extern HWND TargetHWND;
float TimeReSet = 0;
bool FuckLoop = true;
static int         _TimerInited;
static double         _fTicksPerSec;
static LARGE_INTEGER _llTimerStart;
float r3dGetTime()
{
	if (!_TimerInited)
	{
		_TimerInited = 1;

		// Use QueryPerformanceFrequency() to get frequency of timer.
		LARGE_INTEGER qwTicksPerSec;
		QueryPerformanceFrequency(&qwTicksPerSec);
		_fTicksPerSec = (double)qwTicksPerSec.QuadPart;
		QueryPerformanceCounter(&_llTimerStart);
	}

	LARGE_INTEGER qwTime;
	QueryPerformanceCounter(&qwTime);

	return float((double)(qwTime.QuadPart - _llTimerStart.QuadPart) / _fTicksPerSec);
}
void Direct2DOverlay::StartLoop()
{
	if (this->good && !this->loopRunning && this->callback)
	{
		this->loopRunning = true;

		MSG message;
		message.message = WM_NULL;

		ShowWindow(this->overlayWindow, 1);
		UpdateWindow(this->overlayWindow);
		//	SetLayeredWindowAttributes(this->overlayWindow, RGB(0, 0, 0), 255, LWA_COLORKEY);
		SetLayeredWindowAttributes(this->overlayWindow, RGB(0, 0, 0), 255, /*ULW_COLORKEY | */LWA_ALPHA);


		/*	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &this->dx_Object)))
				exit(1);

			this->dx_Param.BackBufferFormat = D3DFMT_A8R8G8B8;
			this->dx_Param.hDeviceWindow = this->overlayWindow;
			this->dx_Param.MultiSampleQuality = DEFAULT_QUALITY;
			this->dx_Param.SwapEffect = D3DSWAPEFFECT_DISCARD;
			this->dx_Param.Windowed = true;

			if (FAILED(this->dx_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, this->overlayWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &this->dx_Param, 0, &this->dx_Device)))
				exit(1);*/


		while (message.message != WM_QUIT)
		{
			if (PeekMessage(&message, this->overlayWindow, NULL, NULL, PM_REMOVE))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
			else
			{
				HWND gamewindow = FindWindow(NULL, this->gameWindowName.c_str());

				if (gamewindow != NULL)
				{
					WINDOWINFO info;
					ZeroMemory(&info, sizeof(info));
					info.cbSize = sizeof(info);
					GetWindowInfo(gamewindow, &info);

					D2D1_SIZE_U siz;
					siz.height = ((info.rcClient.bottom) - (info.rcClient.top));
					siz.width = ((info.rcClient.right) - (info.rcClient.left));




					/*WINDOWINFO info2;
					ZeroMemory(&info2, sizeof(info2));
					info2.cbSize = sizeof(info2);
					GetWindowInfo(this->overlayWindow, &info2);

					std::cout << info2.dwWindowStatus << std::endl;
					*/
					if (!IsIconic(this->overlayWindow))
					{
						SetWindowPos(this->overlayWindow, NULL, info.rcClient.left, info.rcClient.top, siz.width, siz.height, SWP_SHOWWINDOW);
						this->target->Resize(&siz);
					}
					HWND foreground = GetForegroundWindow();
					if (foreground == gamewindow)
					{
						this->callback(this);
					}
					else
					{
						this->BeginDraw();
						this->ClearScreen();
						this->EndDraw();
					}
				}
				else
				{
					this->BeginDraw();
					this->ClearScreen();
					this->EndDraw();
				}
			}
			Sleep(1);
		}
	}
}

void Direct2DOverlay::BeginDraw()
{
	if (this->good && !this->drawing)
	{
		this->drawing = true;
		this->target->BeginDraw();
	}
}

void Direct2DOverlay::ClearScreen()
{
	this->ClearScreen(0.0f, 0.0f, 0.0f, 0.0f);
}

void Direct2DOverlay::ClearScreen(float r, float g, float b, float a)
{
	if (this->good && this->drawing)
	{
		this->target->Clear(D2D1::ColorF(r, g, b, a));
	}
}



void Direct2DOverlay::DrawBox(float x, float y, float width, float height, float thicc, float* rgb, bool filled)
{
	if (this->good && this->drawing)
	{
		this->solid_brush->SetColor(D2D1::ColorF(rgb[0], rgb[1], rgb[2], rgb[3]));
		if (filled)
		{
			this->target->FillRectangle(D2D1::RectF(x, y, width, height), this->solid_brush);
		}
		else
		{
			this->target->DrawRectangle(D2D1::RectF(x, y, width, height), this->solid_brush, thicc);
		}
	}
}

void Direct2DOverlay::DrawFilledRect(float x, float y, int w, int h, float* rgb, bool fill, float size)
{
	DrawBox(x, y, x + w, y + h, size, rgb, fill);
}


void Direct2DOverlay::DrawCircle(float x, float y, float radius, float thicc, float r, float g, float b, float a, bool filled)
{
	if (this->good && this->drawing)
	{
		this->solid_brush->SetColor(D2D1::ColorF(r, g, b, a));
		if (filled)
		{
			this->target->FillEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), this->solid_brush);
		}
		else
		{
			this->target->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius), this->solid_brush, thicc);
		}
	}
}

void Direct2DOverlay::DrawLine(float x1, float y1, float x2, float y2, float thicc, float r, float g, float b)
{
	if (this->good && this->drawing)
	{
		this->solid_brush->SetColor(D2D1::ColorF(r, g, b, 1.0f));
		this->target->DrawLine(D2D1::Point2F(x1, y1), D2D1::Point2F(x2, y2), this->solid_brush, thicc);
	}
}

void Direct2DOverlay::DrawString(std::wstring str, float fontSize, float x, float y, bool center, float r, float g, float b, float aa = 1.f)
{
	if (this->good && this->drawing)
	{
		RECT a = this->GetOverlayRect();
		FLOAT dpix, dpiy;
		dpix = static_cast<float>(a.right - a.left);
		dpiy = static_cast<float>(a.bottom - a.top);

		HRESULT res = this->w_factory->CreateTextLayout(str.c_str(), str.length() + 1, this->w_format, dpix, dpiy, &this->w_layout);
		if (SUCCEEDED(res))
		{
			DWRITE_TEXT_RANGE range = { 0, str.length() };
			this->w_layout->SetFontSize(fontSize, range);
			this->solid_brush->SetColor(D2D1::ColorF(r, g, b));

			this->target->DrawTextLayout(D2D1::Point2F(x, y), this->w_layout, this->solid_brush);
			
			this->w_layout->Release();
			this->w_layout = NULL;
		}
	}
}

void Direct2DOverlay::hack_DrawCrosshair1(float r, float g, float b, float thicc, float len, float xoffset, float yoffset)
{
	if (this->good && this->drawing)
	{
		RECT a = this->GetOverlayRect();
		FLOAT width = static_cast<float>(a.right - a.left);
		FLOAT height = static_cast<float>(a.bottom - a.top);

		this->DrawLine((width / 2.0f) - (len / 2.0f) + xoffset, (height / 2.0f) + yoffset, (width / 2.0f) + (len / 2.0f) + xoffset, (height / 2.0f) + yoffset, thicc, r, g, b);
		this->DrawLine((width / 2.0f) + xoffset, (height / 2.0f) - (len / 2.0f) + yoffset, (width / 2.0f) + xoffset, (height / 2.0f) + (len / 2.0f) + yoffset, thicc, r, g, b);
	}
}

void Direct2DOverlay::EndDraw()
{
	if (this->good && this->drawing)
	{
		this->drawing = false;
		this->target->EndDraw();
	}
}

RECT Direct2DOverlay::GetOverlayRect()
{
	RECT rekt;
	GetClientRect(this->overlayWindow, &rekt);
	return rekt;
}

LRESULT CALLBACK Direct2DOverlay::WindowProc(HWND hWnd, UINT uiMessage, WPARAM wParam, LPARAM lParam)
{
	switch (uiMessage)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, uiMessage, wParam, lParam);
	}
	return 0;
}