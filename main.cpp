#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <Windows.h>
#include<iostream>
#include<TlHelp32.h>
#include<time.h>
#include<vector>
#include<d3dx9math.h>
#include<algorithm>
#include<math.h>
#include<ctime>
#include<random>
#include <oleauto.h>
#pragma warning(disable : 4996)
#pragma comment(lib,"OleAut32.lib" )
#pragma comment(lib,"ws2_32.lib" )
#pragma comment(lib, "d3dx9.lib")
#define _USE_MATH_DEFINES
using namespace std;
#include"D2DOVERLAY.h"
#include"menu.h"
#include"memory.h"
#include "Xorstr.h"

void Drawer(Direct2DOverlay* dis);
HINSTANCE asdd;

int ticks = 0;
int beforeclock = 0;
int FPS = 0;
RECT Render;
wchar_t txt[999];
string ranbomoverlaywinbowname;
HMODULE hModule;
int darkgreen[3] = { 0,1,0 };
int green[3] = { 0,2,0 };
int darkyellow[3] = { 1,1,0 };
int darkred[3] = { 1,0,0 };
int red[3] = { 2,0,0 };
int magenta[3] = { 2,0,2 };

char ClassNameGame[MAX_PATH];
HWND TargetHWND;

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	
	if (IsWindowVisible(hwnd))
	{
		char wnd_titled[256];
		GetWindowTextA(hwnd, wnd_titled, sizeof(wnd_titled));
		std::string str(wnd_titled);
		std::size_t found = str.find("Infestation : Survivor Stories SEA");
		if (found != std::string::npos)
		{
			sprintf(ClassNameGame, "%s", str.c_str());
			TargetHWND = hwnd;
			GetWindowThreadProcessId(hwnd, &pid);
		}
	}
	
	return true;
}

static const char consoleNameAlphanum[] = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
int consoleNameLength = sizeof(consoleNameAlphanum) - 1;
char genRandomConsoleName()
{
	return consoleNameAlphanum[rand() % consoleNameLength];
}

void main()
{
	HWND ConsWind = GetConsoleWindow();
	srand(time(0));
	std::string Consolenamestr;

	for (unsigned int i = 0; i < 40; ++i)
	{
		Consolenamestr += genRandomConsoleName();
	}

	SetConsoleTitle(Consolenamestr.c_str());

	//printf("\n[ M3TH ] : Waiting For Start Game....");
	printf("\n[ M3TH ] : Infestation SEA Cheats By.M3TH\n");
	
	Sleep(1000);

	printf("\n[ M3TH ] : OK !");
	MessageBox(0, "@DEV : กดปุ่ม OK เมื่ออยู่หน้า Lobby !!", "Infestation SEA Cheats By.M3TH", MB_OK | MB_ICONINFORMATION);

	while (pid == NULL)
	{
		EnumWindows(EnumWindowsProc, 0);
	}

	//HWND hWnd = GetConsoleWindow();// Console
	//ShowWindow(hWnd, SW_HIDE);

	Sleep(2500);
	processhandle = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ, FALSE, pid);
    if (processhandle == NULL)
    {

    }

    while (BaseGameAddress() == NULL)
    {
		CalcBase(pid);
		Sleep(100);
    }

	//printf("-> PID %d %p\n", pid, BaseGameAddress());

	if (!Attach(ClassNameGame))
		MessageBox(0, "Error while attaching to the target window!", "Error", MB_ICONERROR | MB_OK);

	if (!OpenTarget(Target, PROCESS_ALL_ACCESS))
		MessageBox(0, "Error while open up the process!", "Error", MB_ICONERROR | MB_OK);

	Direct2DOverlay* asd = new Direct2DOverlay(asdd, Drawer);

	ranbomoverlaywinbowname = random_string(10).c_str();
	asd->Initialize(ClassNameGame, ranbomoverlaywinbowname);
	asd->StartLoop();
}

bool Menu = true;

float ScreenCenterX = NULL;
float ScreenCenterY = NULL;

#include "AllOffset.h"
#include "Class.h"
#include "Engine.h"
#include "Drawing.h"
#include "DrawESP.h"


void LoopHack()
{
	if (MyMenu[7].value)
	{
		write<float>(LocalPlayerAddress + Fastdx1_offset, 0);
		write<float>(LocalPlayerAddress + Fastdx2_offset, 0);
	}

	if (MyMenu[8].value)
	{
		write<float>(LocalPlayerAddress + PickupIteam_off, 1.1f);
	}

	if (MyMenu[9].value)
	{
		if (GetAsyncKeyState(0x20))
		{
			write<float>(LocalPlayerAddress + Superjumping, 15);
		}
	}

	if (MyMenu[10].value)
	{
		write<float>(LocalPlayerAddress + Stamina_offset, 999);
	}

	if (MyMenu[11].value)
	{
		write<float>(LocalPlayerAddress + Nospead_offset, 0);
	}

	if (MyMenu[12].value)
	{
		if (GetAsyncKeyState('G') & 1)
		{
			write<float>(GetGame() + Disconnect, 0);
		}
	}
}

IDirect3DDevice9Ex* d3dDevice2 = NULL;
void Drawer(Direct2DOverlay* D2DOverlay)
{
	Overlay = D2DOverlay;
	int vsyncA = clock();

	ticks += 1;

	if (beforeclock == 0) 
	{
		beforeclock = clock();
	}

	d3dDevice2 = Overlay->d3dDevice;
	Render = Overlay->GetOverlayRect();
	Overlay->BeginDraw();
	Overlay->ClearScreen();

	D3DDEVICE_CREATION_PARAMETERS d3dcp;
	Overlay->d3dDevice->GetCreationParameters(&d3dcp);

	SetWindowDisplayAffinity(d3dcp.hFocusWindow, 0x11);

	static bool Menu = true;

	if (Menu)
	{
		RenderMenu();
	}

	if (GetAsyncKeyState('H') & 1)
	{
		Menu = !Menu;
	}

	if (GetAsyncKeyState(VK_END) & 1)
	{
		ExitProcess(0);
	}

	LocalPlayerAddress = read<DWORD>(GetGame() + Localplayer_Adr);
	LocalPlayerAddress ^= Localplayer_Xor;

	
	if (LocalPlayerAddress)
	{
		MatrixToScreen();
		ESPPlayer();
		LoopHack();
	}
	
	if (FindWindowA(0, ClassNameGame))
	{

	}
	else
	{
		exit(1);
	}

	Overlay->EndDraw();


	int afterclock = clock();
	
	if (afterclock - beforeclock > 1000)
	{
		FPS = ticks;
		beforeclock = 0;
		ticks = 0;
	}
}

