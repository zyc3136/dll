// Dll1.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

PGAME_OFFSET poffset = NULL;

namespace globol
{
	bool gamebegaining = false;
	bool aimbot = true;
	bool player = true;
	bool goods = true;
}




void SetAllOffset()
{
	VMProtectBegin("SetAllOffset");
	poffset = (PGAME_OFFSET)malloc(sizeof(GAME_OFFSET));
	if (!poffset)
		return;
	poffset->CLIENT_PLAYER_LIST_OFFSET = 0x1F97EB8;
	poffset->CLIENT_LOCAL_VIEW_PLAYER_INDEX_OFFSET = 0x17448B4;
	poffset->CLIENT_LOCAL_CLIENT_PLAYER_INDEX_OFFSET = 0x17452EC;
	poffset->CLIENT_VIEW_RENDER_OFFSET = 0xC5F0A10;
	poffset->VIEW_RENDER_MATRIX_PTR_INDEX = 0x1a93d0;
	poffset->ENTITY_HANDLE_OFFSET = 0x508;
	poffset->PLAYER_TEAM_OFFSET = 1008;
	poffset->PLAYER_HEALTH_MAX_OFFSET = 1280;
	poffset->PLAYER_HEALTH_OFFSET = 992;
	poffset->PLAYER_SHIELD_MAX_OFFSET = 340;
	poffset->PLAYER_SHIELD_OFFSET = 336;
	poffset->PLAYER_EYE_POSITION = 0x4264;
	poffset->PLAYER_FOOT_POS_OFFSET = 956;
	poffset->CLIENT_PLAYER_NAME_POINTER = 0xC5F1860;
	poffset->PLAYER_NAME_ID = 48;
	poffset->PLAYER_NAME_PAD = 2560;
	poffset->PLAYER_LAST_WEAPON_INDEX = 5764;
	poffset->PLAYER_VELOCITY_OFFSET = 288;
	poffset->WEAPON_BULLET_VEL = 0x1c0c;
	poffset->LOOT_MODEL_INDEX = 4888;
	poffset->LOOT_SCRI_INT = 4872;
	poffset->SCRIPT_NAME = 1568;
	poffset->PLAYER_BONE_OFFSET = 0xed8;
	VMProtectEnd();
}

