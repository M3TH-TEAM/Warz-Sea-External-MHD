#pragma once

#define Red		D3DCOLOR_ARGB(255, 255, 000, 000)
#define Green	D3DCOLOR_ARGB(255, 0, 255, 34)
#define Blue	D3DCOLOR_ARGB(255, 63, 50, 226)
#define White	D3DCOLOR_ARGB(255, 255, 255, 255)
#define Black	D3DCOLOR_ARGB(255, 000, 000, 000)
#define Yellow	D3DCOLOR_ARGB(255, 255, 255, 000)
#define Purple	D3DCOLOR_ARGB(255, 100, 000, 150)
#define Pink	D3DCOLOR_ARGB(255, 255, 020, 145)
#define ORANGE            D3DCOLOR_ARGB(255, 255, 125, 000)

D3DCOLOR redF = D3DCOLOR_XRGB(255, 0, 0);

LPD3DXFONT tFont;
void PrintText(int x, int y, DWORD color, const char* fmt, ...)
{
	if (!tFont)
	{
		D3DXCreateFontA(Overlay->d3dDevice, 14, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &tFont);
		//	D3DXCreateFont(DirectX->Direct3DDevice, 16, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, FIXED_PITCH, "Arial", &tFont);
	}

	char draw[99] = "";
	va_list va_alist;
	va_start(va_alist, fmt);
	RECT Rectangle = { x, y, 1000 + x, 1000 + y };
	_vsnprintf_s(draw + strlen(draw), 512, 511, fmt, va_alist);
	va_end(va_alist);
	// menu->drawText(x,y, r,g,b,    draw);
	tFont->DrawText(NULL, draw, -1, &Rectangle, 0, color);
}

IDirect3DPixelShader9* pShader;

struct D3DTLVERTEXX
{
	float x, y, z, rhw;
	DWORD color;
};

D3DTLVERTEXX CreateD3DTLVERTEXX(float X, float Y, float Z, float RHW, D3DCOLOR color)
{
	D3DTLVERTEXX v = { X, Y, Z, RHW, color };
	return v;
}

void DrawCircle2(IDirect3DDevice9* pDevice, int x, int y, float radius, int Sides, D3DCOLOR dwColor)
{
	const int NUMPOINTS = 24;
	D3DTLVERTEXX Circle[NUMPOINTS + 1];
	int i;
	float X;
	float Y;
	float Theta;
	float WedgeAngle = (float)((2 * D3DX_PI) / NUMPOINTS);

	for (i = 0; i <= NUMPOINTS; i++)
	{
		Theta = i * WedgeAngle;
		X = (float)(x + radius * cos(Theta));
		Y = (float)(y - radius * sin(Theta));
		Circle[i] = CreateD3DTLVERTEXX((float)X, (float)Y, 0.0f, 1.0f, dwColor);
	}

	pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	pDevice->SetTexture(0, NULL);
	pDevice->SetPixelShader(pShader);

	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, NUMPOINTS, &Circle[0], sizeof(Circle[0]));
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	pDevice->SetTexture(0, NULL);
	pDevice->SetPixelShader(pShader);
}