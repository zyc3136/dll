#include "stdafx.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
std::string windowsbasename = "custom_";
int s_width = 1920;
int s_height = 1080;

namespace Threadhandle
{
	bool bGetGamedata = true;
	HANDLE GetGamedata;
	bool bAimbot = true;
	HANDLE Aimbot;
}
HWND hWnd = 0;


ATOM MyRegisterClass(HINSTANCE hInstance)
{
	VMProtectBegin("MyRegisterClass");
	std::string windowsname;
	windowsname.resize(5);
	randstr(windowsname);
	windowsbasename += windowsname;
	WNDCLASSEXA wndClass;//EX´°¿ÚÀà
	wndClass.cbClsExtra = NULL;
	wndClass.cbSize = sizeof(WNDCLASSEXA);
	wndClass.cbWndExtra = NULL;
	wndClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	wndClass.hCursor = ::LoadCursor(0, IDC_ARROW);
	wndClass.hIcon = ::LoadIcon(0, IDI_APPLICATION);
	wndClass.hIconSm = ::LoadIcon(0, IDI_APPLICATION);
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.lpszClassName = windowsbasename.c_str();
	wndClass.lpszMenuName = windowsbasename.c_str();
	wndClass.style = CS_VREDRAW | CS_HREDRAW;

	return RegisterClassExA(&wndClass);
	VMProtectEnd();
}

BOOL inline InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	VMProtectBegin("InitInstance");
	MARGINS Margin = { -1, -1, -1, -1 };

	s_width = GetSystemMetrics(SM_CXSCREEN);
	s_height = GetSystemMetrics(SM_CYSCREEN);
	 hWnd = CreateWindowExA(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW, windowsbasename.c_str(), windowsbasename.c_str(), WS_POPUP, 1, 1, s_width, s_height, 0, 0, 0, 0);
	if (!hWnd)
	{
		return FALSE;
	}
	SetLayeredWindowAttributes(hWnd, 0, RGB(0, 0, 0), LWA_COLORKEY);
	ShowWindow(hWnd, SW_SHOW);
	DwmExtendFrameIntoClientArea(hWnd, &Margin);
	DirectXInit(hWnd);
	return TRUE;
	VMProtectEnd();
}


void GetGameDataTheard()
{
	VMProtectBegin("GetGameDataTheard");
	while (Threadhandle::bGetGamedata)
	{
		GetGameData();
		Sleep(2000);
	}
	VMProtectEnd();
}

void AmbotTheard()
{
	VMProtectBegin("AmbotTheard");
	while (Threadhandle::bAimbot)
	{
		if (globol::aimbot)
			Aimbot();
		else
			Sleep(100);
	}
	VMProtectEnd();
}


DWORD WINAPI ThreadWork2(LPVOID pParam)
{
	VMProtectBeginMutation("ThreadWork2");
	HINSTANCE hInstance = (HINSTANCE)pParam;
	MyRegisterClass(hInstance);
	if (!InitInstance(hInstance, 0))
	{
		return FALSE;
	}
	MSG Message;
	ZeroMemory(&Message, sizeof(Message));
	while (GetMessage(&Message, NULL, 0, 0))
	{
		if (!globol::gamebegaining && GetAsyncKeyState(VK_F3) & 1)
		{
			mem->InitBase();
			Threadhandle::GetGamedata = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)GetGameDataTheard, 0, 0, 0);
			Threadhandle::Aimbot  = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)AmbotTheard, 0, 0, 0);
			globol::gamebegaining = true;

		}		
		if (GetAsyncKeyState(VK_F4) & 1)
			globol::aimbot = globol::aimbot ? false : true;
		if (GetAsyncKeyState(VK_F5) & 1)
			globol::goods = globol::goods ? false : true;
		if (GetAsyncKeyState(VK_F6) & 1)
			globol::player = globol::player ? false : true;
		TranslateMessage(&Message);
		DispatchMessage(&Message);	
	}
	Threadhandle::bGetGamedata = false;
	Threadhandle::bAimbot = false;
	WaitForSingleObject(Threadhandle::GetGamedata, INFINITE);
	WaitForSingleObject(Threadhandle::Aimbot, INFINITE);
	free(poffset);
	return 0;
	VMProtectEnd();
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	VMProtectBeginMutation("ThreadWork2");
	switch (Message)
	{
	case WM_PAINT:
		if (globol::gamebegaining)
			Render();
		break;
	case WM_DESTROY:
		PostQuitMessage(1);
		return 0;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
	VMProtectEnd();
}


void Closeself()
{
	VMProtectBegin("Closeself");
	CloseWindow(hWnd);
	WaitForSingleObject(Threadhandle::GetGamedata, INFINITE);
	VMProtectEnd();
}