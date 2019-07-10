#pragma once
#include <process.h>
#include <string>
#include <stdint.h>
#include <Windows.h>
#include <stdio.h>
enum PROTO_MESSAGE {
	PROTO_GET_BASEADDR = 12221,
	PROTO_NORMAL_WRITE = 23331,
	PROTO_NORMAL_READ = 33225,
	PROTO_MOUSE_EVENT = 41123,
	PROTO_DELETE_FILE = 51123
};

typedef struct readStruct
{
	ULONGLONG UserBufferAdress;
	ULONGLONG GameAddressOffset;
	ULONGLONG ReadSize;
	ULONG     UserPID;
	UINT32	  ProtocolMsg;
} ReadStruct, *pReadStruct;

typedef struct _MY_MOUSE
{
	LONG LastX;
	LONG LastY;
}MY_MOUSE, *PMY_MOUSE;

class Mem
{
public:
	Mem(wchar_t* dirname);
	~Mem();
	BOOL InitBase();
	DWORD_PTR GetBase();
	HANDLE m_hDriver;
	char encodekey[10] = { NULL };
	char encodekey2[20] = { 3,6,5,2,9,7,4,2,5,6,0xd,0xb,0xe,3,0xd,6,4,0xa,0xd,5 };
	BOOL RPMWSTR(SIZE_T address, PVOID buff, DWORD bufSize);
	void mouseevent(PMY_MOUSE mouse)
	{
		VMProtectBeginMutation("mouseevent");
		int writeMe;
		readStruct rStruct{ (uint64_t)&writeMe,*(uint64_t*)mouse, sizeof(MY_MOUSE), (uint32_t)_getpid(), PROTO_MOUSE_EVENT };
		DWORD dwWrited;
		Send(m_hDriver, (LPCVOID)&rStruct, sizeof(ReadStruct), &dwWrited, NULL);
		VMProtectEnd();
	}

	template<typename T> T RPM(uint64_t address)
	{
		VMProtectBeginMutation("RPM(uint64_t address)");
		T writeMe;
		readStruct rStruct{ (uint64_t)&writeMe,(uint64_t)address, sizeof(T), (uint32_t)_getpid(), PROTO_NORMAL_READ };
		// send the struct to IOCTL
		DWORD dwWrited;
		Send(m_hDriver, (LPCVOID)&rStruct, sizeof(ReadStruct), &dwWrited, NULL);
		return writeMe;
		VMProtectEnd();
	}
	VOID RPM(uint64_t address, void* writeMe, int size)
	{
		VMProtectBeginMutation("RPM(uint64_t address, void* writeMe, int size)");
		readStruct rStruct{ (uint64_t)writeMe,(uint64_t)address, (ULONGLONG)size, (uint32_t)_getpid(), PROTO_NORMAL_READ };
		DWORD dwWrited;
		Send(m_hDriver, (LPCVOID)&rStruct, sizeof(ReadStruct), &dwWrited, NULL);
		VMProtectEnd();
	}

	BOOL Send(HANDLE hFile,LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite,LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);

private:

	LPVOID lpBase;
};



