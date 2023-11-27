
#include "D2DOverlay.h"
Direct2DOverlay* Overlay;
Color Col;
static int menuIndex = 0;
#define MAXMENU	13 //จำนวนเมนู
char ColorOn[1024];
char * onoff[] = { "Off", "On" };
char * AimFov[] = { "Off","50", "60", "70", "80", "90" };
char * color[] = { "Off", "Red", "Green", "Blue", "Cyan", "White" };
char * keyaim[] = { "RMouse", "LMouse" };
char * bone[] = { "Head", "Neck" };

#define black_								D3DCOLOR_ARGB(255, 1, 1, 1)
#define black_tran							D3DCOLOR_ARGB(10, 1, 1, 1)
#define red_									D3DCOLOR_ARGB(255, 255, 000, 000)
#define green_								D3DCOLOR_ARGB(255, 000, 255, 000)
#define blue								D3DCOLOR_ARGB(255, 000, 000, 255)
#define cyan								D3DCOLOR_ARGB(255, 000, 255, 255)
#define yellow								D3DCOLOR_ARGB(255, 255, 255, 000)
#define white								D3DCOLOR_ARGB(255, 255, 255, 255)

class MyMenu_t
{
public:

	char * text;
	char ** status;
	int value;
	int maxvalue;

};

MyMenu_t MyMenu[MAXMENU] =
{
	{ "ESP 2D Box", onoff, 0, 2 },
	{ "ESP Skeleton", onoff, 0, 2 },
	{ "ESP Health", onoff, 0, 2},
	{ "ESP Distance", onoff, 0, 2},
	{ "Aimbot Enabled", onoff, 0, 2 },
	{ "Aimbot Key", keyaim, 0, 2 },
	{ "Aimbot Bone", bone, 0, 2 },
	{ "Bandages 3s", onoff, 0, 2 },
	{ "Fastup Iteam", onoff, 0, 2 },
	{ "Superjump", onoff, 0, 2 },
	{ "Stamina Hack", onoff, 0, 2 },
	{ "No Spread", onoff, 0, 2 },
	{ "Disconnect [G]", onoff, 0, 2 }

};


void KeyMenu()
{
	
	if (GetAsyncKeyState(VK_UP) & 1)
	{
		menuIndex--;
		if (menuIndex < 0)
			menuIndex = MAXMENU - 1;
	}

	if (GetAsyncKeyState(VK_DOWN) & 1)
	{
		menuIndex++;
		if (menuIndex > MAXMENU - 1)
			menuIndex = 0;
	}

	if (GetAsyncKeyState(VK_RIGHT) & 1)
	{
		MyMenu[menuIndex].value += 1;
		if (MyMenu[menuIndex].value > MyMenu[menuIndex].maxvalue - 1)
			MyMenu[menuIndex].value = MyMenu[menuIndex].maxvalue - 1;
	}
	
	if (GetAsyncKeyState(VK_LEFT) & 1)
	{
		MyMenu[menuIndex].value -= 1;
		if (MyMenu[menuIndex].value <= 0)
			MyMenu[menuIndex].value = 0;
	}
}

void RenderMenu()
{
	
	KeyMenu();
	Overlay->DrawFilledRect(20, 50, 280, 420, Col.black_trans, true, 1);
	Overlay->DrawString(L"Infestation SEA [H] On - Off / Menu", 14, 42, 71, false, Col.White[0], Col.White[1], Col.White[2], Col.White[3]);
	//Overlay->DrawString(L"> Power By M HD-Cheats.Tk <", 14, 70, 450, false, Col.White[0], Col.White[1], Col.White[2], Col.White[3]);
	
	wchar_t txtMenu[0xff], txtStaticMenu[0xff];
	for (int i = 0; i < MAXMENU; i++)
	{
		if (i == menuIndex)
		{
			
			Overlay->DrawFilledRect(20, 118 + (i * 25), 280, 19, Col.White, false, 1);
			if (MyMenu[menuIndex].value == 1)
			{
				swprintf(txtMenu, L"%S\n", MyMenu[i].status[MyMenu[i].value]);
				Overlay->DrawString(txtMenu, 14, 237, 120 + (i * 25), false, Col.red[0], Col.red[1], Col.red[2], Col.red[3]);
			}
			else if (MyMenu[menuIndex].value == 2)
			{
				//DrawStringOutline(237, 120 + (i * 18), Green, DT_NOCLIP, MyMenu[i].status[MyMenu[i].value]);
				swprintf(txtMenu, L"%S\n", MyMenu[i].status[MyMenu[i].value]);
				Overlay->DrawString(txtMenu, 14, 237, 120 + (i * 25), false, Col.green[0], Col.green[1], Col.green[2], Col.green[3]);
			}
			else if (MyMenu[menuIndex].value == 3)
			{
				//DrawStringOutline(237, 120 + (i * 18), Blue, DT_NOCLIP, MyMenu[i].status[MyMenu[i].value]);
				swprintf(txtMenu, L"%S\n", MyMenu[i].status[MyMenu[i].value]);
				Overlay->DrawString(txtMenu, 14, 237, 120 + (i * 25), false, Col.purple[0], Col.purple[1], Col.purple[2], Col.purple[3]);
			}
			else if (MyMenu[menuIndex].value == 5)
			{
				//DrawStringOutline(237, 120 + (i * 18), Green, DT_NOCLIP, MyMenu[i].status[MyMenu[i].value]);
				swprintf(txtMenu, L"%S\n", MyMenu[i].status[MyMenu[i].value]);
				Overlay->DrawString(txtMenu, 14, 237, 120 + (i * 25), false, Col.green[0], Col.green[1], Col.green[2], Col.green[3]);
			}
			else if (MyMenu[menuIndex].value == 4)
			{
				//DrawStringOutline(237, 120 + (i * 18), Cyan, DT_NOCLIP, MyMenu[i].status[MyMenu[i].value]);
				swprintf(txtMenu, L"%S\n", MyMenu[i].status[MyMenu[i].value]);
				Overlay->DrawString(txtMenu, 14, 237, 120 + (i * 25), false, Col.bulr[0], Col.bulr[1], Col.bulr[2], Col.bulr[3]);
			}
			else
			{
				
				swprintf(txtMenu, L"%S\n", MyMenu[i].status[MyMenu[i].value]);
				Overlay->DrawString(txtMenu, 14, 237, 120 + (i * 25), false, Col.White[0], Col.White[1], Col.White[2], Col.White[3]);
				//DrawStringOutline(237, 120 + (i * 18), White, DT_NOCLIP, MyMenu[i].status[MyMenu[i].value]);
			}

		}
		else
		{
			swprintf(txtMenu, L"%S\n", MyMenu[i].status[MyMenu[i].value]);
			Overlay->DrawString(txtMenu, 14, 237, 120 + (i * 25), false, Col.White[0], Col.White[1], Col.White[2], Col.White[3]);
			//DrawStringOutline(237, 120 + (i * 18), White, DT_NOCLIP, MyMenu[i].status[MyMenu[i].value]);
		}
		
		swprintf(txtStaticMenu, L"%S\n", MyMenu[i].text);
		Overlay->DrawString(txtStaticMenu, 14, 50, 120 + (i * 25), false, Col.White[0], Col.White[1], Col.White[2], Col.White[3]);
	}
	
}
