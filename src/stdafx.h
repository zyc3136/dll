#pragma once

#include "targetver.h"
#include <windows.h>
#include <dwmapi.h>
#include <string>
#include <d2d1.h>
#include <Dwrite.h>
#include <time.h>
#include <vector>
#include <d3d9.h>
#include "d3d.h"
#include "VMProtectSDK.h"
#include "ctextrender.h"
#include "mem.h"
#pragma comment(lib, "VMProtectSDK64.lib")
#pragma comment(lib, "Dwrite.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Dwmapi.lib")

extern std::string windowsbasename;
extern int s_width;
extern int s_height;

extern ID2D1HwndRenderTarget* pRender;



namespace Brush
{
	extern ID2D1SolidColorBrush*  pBrush_white;
	extern ID2D1SolidColorBrush*  pBrush_Yellow;
	extern ID2D1SolidColorBrush*  pBrush_red;
	extern ID2D1SolidColorBrush*  pBrush_blue;
	extern ID2D1SolidColorBrush*  pBrush_heavy_ammo;
	extern ID2D1SolidColorBrush*  pBrush_energy_ammo;
	extern ID2D1SolidColorBrush*  pBrush_Light_ammo;
	extern ID2D1SolidColorBrush*  pBrush_weapon;
	extern ID2D1SolidColorBrush*  pBrush_gold_weapon;
	extern ID2D1SolidColorBrush*  pBrush_purple;
	extern ID2D1SolidColorBrush*  pBrush_black;
}




namespace globol
{
	extern bool gamebegaining;
	extern bool aimbot;
	extern bool player;
	extern bool goods;
}



typedef struct _PlAYER
{
	DWORD64 Pointer;
	int Team;
	wchar_t name[30];
	int m_iMaxHealth;
	int m_shieldHealthMax;
}PlAYER,*PPlAYER;



//forwindow
namespace Threadhandle
{
	extern bool bGetGamedata;
	extern HANDLE GetGamedata;
}
void Closeself();
DWORD WINAPI ThreadWork2(LPVOID pParam);
//tools
void randstr(std::string& str);
void Encodestring(char* a, char* b);
wchar_t * char2wchar(const char* cchar);
//d2d
void DirectXInit(HWND hWnd);
void DrawRenderText(int X, int Y, const wchar_t* text, ID2D1SolidColorBrush* DrawBrush);
void DrawLine(int X, int Y, int XX, int YY, ID2D1SolidColorBrush* DrawBrush);
void DrawBox(int x, int y, int width, int height, ID2D1SolidColorBrush* DrawBrush1, ID2D1SolidColorBrush* DrawBrush2);
//render
void Render();
//DLL
void SetAllOffset();
//ESP
void GetGameData();
void ESP();
void Aimbot();
void Initstring();
extern Mem* mem;






#pragma pack(push,1)
typedef struct _GAME_OFFSET
{
	DWORD64 CLIENT_PLAYER_LIST_OFFSET;
	DWORD64 CLIENT_LOCAL_VIEW_PLAYER_INDEX_OFFSET;
	DWORD64 CLIENT_LOCAL_CLIENT_PLAYER_INDEX_OFFSET;
	DWORD64 CLIENT_VIEW_RENDER_OFFSET;
	DWORD64 VIEW_RENDER_MATRIX_PTR_INDEX;
	DWORD64 ENTITY_HANDLE_OFFSET;
	DWORD64 PLAYER_TEAM_OFFSET;
	DWORD64 PLAYER_HEALTH_MAX_OFFSET;
	DWORD64 PLAYER_HEALTH_OFFSET;
	DWORD64 PLAYER_SHIELD_MAX_OFFSET;
	DWORD64 PLAYER_SHIELD_OFFSET;
	DWORD64 PLAYER_EYE_POSITION;
	DWORD64 PLAYER_FOOT_POS_OFFSET;
	DWORD64 CLIENT_PLAYER_NAME_POINTER;
	DWORD64 PLAYER_NAME_ID;
	DWORD64 PLAYER_NAME_PAD;
	DWORD64 PLAYER_LAST_WEAPON_INDEX;
	DWORD64 PLAYER_VELOCITY_OFFSET;
	DWORD64 WEAPON_BULLET_VEL;
	DWORD64 LOOT_MODEL_INDEX;
	DWORD64 LOOT_SCRI_INT;
	DWORD64 SCRIPT_NAME;
	DWORD64 PLAYER_BONE_OFFSET;
}GAME_OFFSET,*PGAME_OFFSET;
#pragma pack(pop)

extern PGAME_OFFSET poffset;
extern char key[18];