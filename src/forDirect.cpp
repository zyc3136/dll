#include "stdafx.h"

ID2D1HwndRenderTarget* pRender = NULL;
ID2D1Factory*		   pFactory = NULL;
IDWriteFactory*		   pWriteFactory = NULL;
IDWriteTextFormat*	   pTextFormat = NULL;

namespace Brush
{

	ID2D1SolidColorBrush*  pBrush_white = NULL;
	ID2D1SolidColorBrush*  pBrush_Yellow = NULL;
	ID2D1SolidColorBrush*  pBrush_red = NULL;
	ID2D1SolidColorBrush*  pBrush_blue = NULL;
	ID2D1SolidColorBrush*  pBrush_heavy_ammo = NULL;
	ID2D1SolidColorBrush*  pBrush_energy_ammo = NULL;
	ID2D1SolidColorBrush*  pBrush_Light_ammo = NULL;
	ID2D1SolidColorBrush*  pBrush_weapon = NULL;
	ID2D1SolidColorBrush*  pBrush_gold_weapon = NULL;
	ID2D1SolidColorBrush*  pBrush_purple = NULL;
	ID2D1SolidColorBrush*  pBrush_black = NULL;
}


void DirectXInit(HWND hWnd)
{
	VMProtectBegin("DirectXInit");
	if (!pRender)
	{
		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory);
		pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(/*D2D1_RENDER_TARGET_TYPE_DEFAULT*/D2D1_RENDER_TARGET_TYPE_HARDWARE, D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)), D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(s_width, s_height), D2D1_PRESENT_OPTIONS_IMMEDIATELY), &pRender);
		pRender->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_FORCE_DWORD);
		pRender->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DeepSkyBlue), &Brush::pBrush_blue);
		pRender->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::SkyBlue), &Brush::pBrush_heavy_ammo);
		pRender->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Orange), &Brush::pBrush_Light_ammo);
		pRender->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::GreenYellow), &Brush::pBrush_energy_ammo);
		pRender->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Cyan), &Brush::pBrush_weapon);
		pRender->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gold), &Brush::pBrush_gold_weapon);
		pRender->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::DarkViolet), &Brush::pBrush_purple);
		pRender->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &Brush::pBrush_black);

		pRender->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &Brush::pBrush_red);
		pRender->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &Brush::pBrush_white);
		pRender->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &Brush::pBrush_Yellow);
		DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pWriteFactory));
	}
	pWriteFactory->CreateTextFormat(L"Î¢ÈíÑÅºÚ", NULL, DWRITE_FONT_WEIGHT_SEMI_BOLD, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 12.0f, L"MyFont", &pTextFormat);
	VMProtectEnd();
}


void DrawRenderText(int X, int Y, const wchar_t* text, ID2D1SolidColorBrush* DrawBrush)
{
	pRender->DrawTextW(text, (UINT32)wcslen(text), pTextFormat, D2D1::RectF((float)X, (float)Y, (float)X + 100, (float)Y + 12), DrawBrush);
}
void DrawLine(int X, int Y, int XX, int YY, ID2D1SolidColorBrush* DrawBrush)
{
	pRender->DrawLine(D2D1::Point2F((FLOAT)X, (FLOAT)Y), D2D1::Point2F((FLOAT)XX, (FLOAT)YY), DrawBrush, 1);
}


void DrawBox(int x, int y, int width, int height, ID2D1SolidColorBrush* DrawBrush1, ID2D1SolidColorBrush* DrawBrush2)
{
	DrawLine(x, y, x + width, y, DrawBrush2);
	DrawLine(x, y, x, y - height, DrawBrush1);
	DrawLine(x, y - height, x + width, y - height, DrawBrush1);
	DrawLine(x + width, y, x + width, y - height, DrawBrush1);
}