// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"


HMODULE hhModule = 0;
extern "C"  __declspec(dllexport) void startfunc(wchar_t* dirname)
{
	VMProtectBegin("startfunc");
	SetAllOffset();
	mem = new Mem(dirname);
	mem->InitBase();
	CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThreadWork2, hhModule, 0, 0));
	VMProtectEnd();
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	VMProtectBegin("DllMain");
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		hhModule = hModule;
		Initstring();
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
	VMProtectEnd();
}

