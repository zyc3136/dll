#include "stdafx.h"



std::string _inline WcharToChar(const wchar_t* wp)
{
	std::string str;
	int len = (int)WideCharToMultiByte(CP_ACP, 0, wp, (int)wcslen(wp), NULL, 0, NULL, NULL);
	char	*m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wp, (int)wcslen(wp), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	str = m_char;
	delete m_char;
	return str;
}

Mem::Mem(wchar_t* dirname)
{
	VMProtectBeginMutation("Mem");
	std::string linkname = "\\\\.\\";
	linkname += WcharToChar(dirname);
	m_hDriver = INVALID_HANDLE_VALUE;
	m_hDriver = CreateFileA(linkname.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	for (int i = 0; i < 5; i++)
	{
		encodekey[i] = (char)(dirname[i]);
	}
	VMProtectEnd();
}


Mem::~Mem()
{
	if (m_hDriver || m_hDriver != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hDriver);
	}
}


DWORD_PTR Mem::GetBase()
{
	VMProtectBeginMutation("GetBase");
	return (DWORD_PTR)lpBase;
	VMProtectEnd();
}


BOOL Mem::RPMWSTR(SIZE_T address, PVOID buff, DWORD bufSize)
{
	VMProtectBeginMutation("RPMWSTR");
	readStruct rStruct{ (uint64_t)buff, (uint64_t)address, bufSize, (uint32_t)_getpid(), PROTO_NORMAL_READ };
	// send the struct to IOCTL
	DWORD dwWrited;
	return Send(m_hDriver, (LPCVOID)&rStruct, sizeof(ReadStruct), &dwWrited, NULL);
	VMProtectEnd();
}


BOOL Mem::InitBase()
{
	VMProtectBeginMutation("InitBase");
	int64_t base = 0;
	void* read = nullptr;
    readStruct rStruct{ (uint64_t)&base, (uint64_t)read, sizeof(int64_t), (uint32_t)_getpid(), PROTO_GET_BASEADDR };
	DWORD dwWrited;
	Send(m_hDriver, (LPCVOID)&rStruct, sizeof(ReadStruct), &dwWrited, NULL);
	if (base)
	{
		lpBase = (LPVOID)base;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	VMProtectEnd();
}

BOOL Mem::Send(HANDLE hFile,LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite,LPDWORD lpNumberOfBytesWritten,LPOVERLAPPED lpOverlapped)
{
	VMProtectBeginMutation("Send");
	char* buffer = (char*)lpBuffer;
	for (DWORD i = 0; i < nNumberOfBytesToWrite; i++)
	{
		buffer[i] = buffer[i] ^ (encodekey[i % 5] + encodekey2[i % 20]);
	}
	return WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
	VMProtectEnd();
}