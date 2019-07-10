#include"stdafx.h"


namespace local
{
	DWORD64 Local_entity = 0;
	int local_client_player_index = 0;
	int loacal_team = -1;
	DWORD64 CameraMatrix = 0;
	D3DXMATRIX Matrix;
	Vector3 Localpos;
}


namespace Aimplayer
{
	DWORD64 pointer = 0;
	int health = 0;
	Vector3 Headpos;
	Vector3 footpos;
}

namespace entity_handle
{
	char player[40];
	char prop_survival[40];
}
std::vector<PlAYER> playerlist;

bool WorldToScreen(const Vector3& vIn, Vector3& vOut)
{
	float w = local::Matrix.m[3][0] * vIn.x + local::Matrix.m[3][1] * vIn.y + local::Matrix.m[3][2] * vIn.z + local::Matrix.m[3][3];
	if (w < 0.01)
		return false;
	vOut.x = local::Matrix.m[0][0] * vIn.x + local::Matrix.m[0][1] * vIn.y + local::Matrix.m[0][2] * vIn.z + local::Matrix.m[0][3];
	vOut.y = local::Matrix.m[1][0] * vIn.x + local::Matrix.m[1][1] * vIn.y + local::Matrix.m[1][2] * vIn.z + local::Matrix.m[1][3];
	float invw = 1.0f / w;
	vOut.x *= invw;
	vOut.y *= invw;
	float x = (float)s_width / 2;
	float y = (float)s_height / 2;
	x += 0.5f * vOut.x * (float)s_width + 0.5f;
	y -= 0.5f * vOut.y * (float)s_height + 0.5f;
	vOut.x = x;
	vOut.y = y;
	return true;
}

void Initstring()
{
	VMProtectBegin("Initstring");
	std::string rand;
	rand.resize(18);
	randstr(rand);
	memcpy(key, rand.c_str(), 18);
	char* player = (char*)VMProtectDecryptStringA("player");
	char* prop_survival = (char*)VMProtectDecryptStringA("prop_survival");
	Encodestring(entity_handle::player, player);
	Encodestring(entity_handle::prop_survival, prop_survival);
	VMProtectFreeString(player);
	VMProtectFreeString(prop_survival);
	VMProtectEnd();
}


void GetGameData()
{
	VMProtectBeginMutation("GetGameData");

	std::vector<PlAYER> tmp_playerlist;


	local::local_client_player_index = mem->RPM<INT16>(mem->GetBase() + poffset->CLIENT_LOCAL_CLIENT_PLAYER_INDEX_OFFSET);
	DWORD64 view_Render = mem->RPM<DWORD64>(mem->GetBase() + poffset->CLIENT_VIEW_RENDER_OFFSET);
	InterlockedExchange64((volatile LONG64 *)&local::CameraMatrix, (LONG64)mem->RPM<DWORD64>(view_Render + poffset->VIEW_RENDER_MATRIX_PTR_INDEX));
	if (!mem->RPM<DWORD64>(mem->GetBase() + poffset->CLIENT_PLAYER_LIST_OFFSET))
		return;
	DWORD64 name_list = mem->RPM<DWORD64>(mem->GetBase() + poffset->CLIENT_PLAYER_NAME_POINTER);
	for (int i = 0; i < 0x10000; i++)
	{
		DWORD64 Entity= mem->RPM<DWORD64>(mem->GetBase() + poffset->CLIENT_PLAYER_LIST_OFFSET + 32 * i);
		if (!Entity)
			continue;
		if (local::local_client_player_index == i)
		{
			InterlockedExchange64((volatile LONG64 *)&local::Local_entity, Entity);
			local::loacal_team = mem->RPM<int>(local::Local_entity + poffset->PLAYER_TEAM_OFFSET);
			continue;
		}
		DWORD64 entity_handle = mem->RPM<DWORD64>(Entity + poffset->ENTITY_HANDLE_OFFSET);
		if (!entity_handle)
			continue;
		char ghandle[20];
		mem->RPM(entity_handle, ghandle, 20);
		Encodestring(ghandle, ghandle);


		if (strcmp(ghandle, entity_handle::player) == 0)
		{
			PlAYER player;
			memset(&player,0,sizeof(PlAYER));
			player.Pointer = Entity;

			player.Team = mem->RPM<int>(Entity + poffset->PLAYER_TEAM_OFFSET);
			player.m_iMaxHealth = mem->RPM<int>(Entity + poffset->PLAYER_HEALTH_MAX_OFFSET);
			player.m_shieldHealthMax = mem->RPM<int>(Entity + poffset->PLAYER_SHIELD_MAX_OFFSET);

			int name_index = mem->RPM<int>(Entity + poffset->PLAYER_NAME_ID);
			DWORD64 name_ptr = mem->RPM<DWORD64>(name_list + name_index * 8 + poffset->PLAYER_NAME_PAD);
			char playername[21] = { NULL };
			mem->RPM(name_ptr, playername, 20);
			wchar_t* outstring = char2wchar(playername);
			memcpy(player.name, playername,wcslen(outstring) * 2);
			delete outstring;

			tmp_playerlist.push_back(player);
		}

	}

	tmp_playerlist.swap(playerlist);
	tmp_playerlist.clear();
	VMProtectEnd();
}


void ESP()
{
	wchar_t drawstr[30] = { NULL };
	int playernum = 0;
	int closeplayer = 2000;
	if (!Aimplayer::pointer)
		mem->RPM(local::CameraMatrix, &local::Matrix, sizeof(local::Matrix));
	mem->RPM(local::Local_entity + poffset->PLAYER_FOOT_POS_OFFSET, &local::Localpos, sizeof(Vector3));
	for (int i = 0; i < playerlist.size(); i++)
	{
		PlAYER Entity = playerlist[i];
		if (local::loacal_team == Entity.Team)
			continue;

		Vector3 footpos;
		Vector3 Headpos;
		Vector3 screen_footpos;
		Vector3 screen_Headpos;
		int health = 0;
		if (Entity.Pointer = Aimplayer::pointer)
		{
			health = Aimplayer::health;
			if (health <= 0)
				continue;
			playernum++;
			memcpy(&footpos, &Aimplayer::footpos, sizeof(Vector3));
			if (!WorldToScreen(footpos, screen_footpos))
				continue;
			if (screen_footpos.x <= 0 || screen_footpos.y <= 0 || screen_footpos.x >= s_width || screen_footpos.y >= s_height)
				continue;

			memcpy(&Headpos, &Aimplayer::Headpos, sizeof(Vector3));
			if (!WorldToScreen(Headpos, screen_Headpos))
				continue;
			if (screen_Headpos.x <= 0 || screen_Headpos.y <= 0 || screen_Headpos.x >= s_width || screen_Headpos.y >= s_height)
				continue;
		}
		else
		{
			health = mem->RPM<int>(Entity.Pointer + poffset->PLAYER_HEALTH_OFFSET);
			if (health <= 0)
				continue;
			playernum++;
			mem->RPM(Entity.Pointer + poffset->PLAYER_EYE_POSITION, &Headpos, sizeof(Vector3));
			if (!WorldToScreen(Headpos, screen_Headpos))
				continue;
			if (screen_Headpos.x <= 0 || screen_Headpos.y <= 0 || screen_Headpos.x >= s_width || screen_Headpos.y >= s_height)
				continue;
			mem->RPM(Entity.Pointer + poffset->PLAYER_FOOT_POS_OFFSET, &footpos, sizeof(Vector3));
			if (!WorldToScreen(footpos, screen_footpos))
				continue;
			if (screen_footpos.x <= 0 || screen_footpos.y <= 0 || screen_footpos.x >= s_width || screen_footpos.y >= s_height)
				continue;

		}
		if (abs(Headpos.y - footpos.y) > 200)
			continue;
		int distance = (int)(local::Localpos.Distance(footpos) * 0.01905);
		if (distance < closeplayer)
			closeplayer = distance;
		float Box_Height = abs(screen_Headpos.y - screen_footpos.y);
		//显示方框
		if (distance < 250 || Box_Height > 10)
		{
			if (Box_Height < 16)
				Box_Height = 16;
			int shield = mem->RPM<int>(Entity.Pointer + poffset->PLAYER_SHIELD_OFFSET);
			int heal_YY = (int)(health / Entity.m_iMaxHealth * Box_Height);
			int shield_YY;
			if (shield <= 0)
				shield_YY = 0;
			else
				shield_YY = (int)(shield / Entity.m_shieldHealthMax* Box_Height);
			DrawBox((int)screen_footpos.x - (int)Box_Height / 4, (int)screen_footpos.y, (int)Box_Height / 2, (int)Box_Height, Brush::pBrush_Yellow, Brush::pBrush_Yellow);
			if (heal_YY >= 0)
				pRender->DrawLine(D2D1::Point2F(screen_footpos.x - Box_Height / 4 - 1, screen_footpos.y), D2D1::Point2F(screen_footpos.x - Box_Height / 4 - 1, screen_footpos.y - heal_YY), Brush::pBrush_red, 2);
			if (shield_YY >= 0)
			{
				//白甲
				if (Entity.m_shieldHealthMax == 50)
				{
					pRender->DrawLine(D2D1::Point2F(screen_footpos.x - Box_Height / 4 - 3, screen_footpos.y), D2D1::Point2F(screen_footpos.x - Box_Height / 4 - 3, screen_footpos.y - shield_YY), Brush::pBrush_weapon, 2);
				}
				//蓝甲
				else if (Entity.m_shieldHealthMax == 75)
				{
					pRender->DrawLine(D2D1::Point2F(screen_footpos.x - Box_Height / 4 - 3, screen_footpos.y), D2D1::Point2F(screen_footpos.x - Box_Height / 4 - 3, screen_footpos.y - shield_YY), Brush::pBrush_blue, 2);
				}
				//紫甲
				else if (Entity.m_shieldHealthMax == 100)
				{
					pRender->DrawLine(D2D1::Point2F(screen_footpos.x - Box_Height / 4 - 3, screen_footpos.y), D2D1::Point2F(screen_footpos.x - Box_Height / 4 - 3, screen_footpos.y - shield_YY), Brush::pBrush_purple, 2);
				}

			}
	
			//显示名字
			DrawRenderText(screen_footpos.x, screen_footpos.y, Entity.name, Brush::pBrush_Yellow);
			wsprintfW(drawstr, L"[%d]", distance);
			DrawRenderText(screen_footpos.x, screen_footpos.y + 12, drawstr, Brush::pBrush_white);
			continue;
		}
		if (distance < 100)
		{
			wsprintfW(drawstr, L"[%d]", distance);
			DrawRenderText((int)screen_footpos.x, (int)screen_footpos.y, drawstr, Brush::pBrush_white);
			continue;

		}
		wsprintfW(drawstr, L"[%d]", distance);
		DrawRenderText((int)screen_footpos.x, (int)screen_footpos.y, drawstr, Brush::pBrush_Yellow);
		continue;
	}

	wsprintfW(drawstr, L"%d [%d]", playernum, closeplayer);
	DrawRenderText(10, 10, drawstr, Brush::pBrush_Yellow);
}


void Aimbot()
{
	float bulletspead = 0;
	static DWORD timeBegin = 0;
	static DWORD timePhase = 0;
	static float constFps = 60.0f;                 //定义帧数
	static DWORD timeInPerFrame = (DWORD)(1000.0f / constFps);    //计算每帧的频率


	INT16 weap_index = mem->RPM<INT16>(local::Local_entity + poffset->PLAYER_LAST_WEAPON_INDEX);
	if (weap_index != -1 && weap_index < 0x10000)
	{
		DWORD64 weapon = mem->RPM<DWORD64>(mem->GetBase() + poffset->CLIENT_PLAYER_LIST_OFFSET + 32 * weap_index);
		bulletspead = mem->RPM<float>(weapon + poffset->WEAPON_BULLET_VEL);
		if (bulletspead < 1000)
			bulletspead = 0;
	}
	

	for (int i = 0; i < playerlist.size(); i++)
	{
		DWORD64 pointer = playerlist[i].Pointer;
		if (local::loacal_team == playerlist[i].Team)
			continue;
		Aimplayer::health = mem->RPM<int>(pointer + poffset->PLAYER_HEALTH_OFFSET);
		if (Aimplayer::health <= 0)
			continue;
		Vector3 screen_footpos;
		Vector3 screen_Headpos;
		mem->RPM(pointer + poffset->PLAYER_EYE_POSITION, &Aimplayer::Headpos, sizeof(Vector3));
		if (!WorldToScreen(Aimplayer::Headpos, screen_Headpos))
			continue;
		if (screen_Headpos.x <= 0 || screen_Headpos.y <= 0 || screen_Headpos.x >= s_width || screen_Headpos.y >= s_height)
			continue;
		mem->RPM(pointer + poffset->PLAYER_FOOT_POS_OFFSET, &Aimplayer::footpos, sizeof(Vector3));
		if (!WorldToScreen(Aimplayer::footpos, screen_footpos))
			continue;
		if (screen_footpos.x <= 0 || screen_footpos.y <= 0 || screen_footpos.x >= s_width || screen_footpos.y >= s_height)
			continue;

		float Box_Height = abs(screen_Headpos.y - screen_footpos.y);
		if (Box_Height < 16)
			Box_Height = 16;

		if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0 && screen_footpos.x - Box_Height / 2 <= s_width / 2
			&& screen_footpos.x + Box_Height / 2 >= s_width / 2 && screen_footpos.y + 5 >= s_height / 2
			&& screen_footpos.y - Box_Height <= s_height / 2)
		{

			D3DXMATRIX lastMatrix;
			DWORD64 bone = mem->RPM<DWORD64>(pointer + poffset->PLAYER_BONE_OFFSET);
			float matrix[3][4];
			InterlockedExchange64((volatile LONG64 *)&Aimplayer::pointer, (LONG64)pointer);
			while (true)
			{
				timeBegin = GetTickCount();             //循环开始的时间  


				Vector3 pos;
				Vector3 spos;
				mem->RPM(local::CameraMatrix, &local::Matrix, sizeof(D3DXMATRIX));
				if (memcmp(&lastMatrix, &local::Matrix, sizeof(D3DXMATRIX)) == 0)
					goto mcon;
				memcpy(&lastMatrix, &local::Matrix, sizeof(D3DXMATRIX));
				mem->RPM(pointer + poffset->PLAYER_EYE_POSITION, &Aimplayer::Headpos, sizeof(Vector3));
				mem->RPM(pointer + poffset->PLAYER_FOOT_POS_OFFSET, &Aimplayer::footpos, sizeof(Vector3));



				if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0)
				{
				
					mem->RPM(bone + 10 * 0x30, &matrix, sizeof(matrix));
					if (matrix[2][3] > 5 && matrix[2][3] < 200)
					{
						pos.x = matrix[0][3] + Aimplayer::footpos.x;
						pos.y = matrix[1][3] + Aimplayer::footpos.y;
						pos.z = matrix[2][3] + Aimplayer::footpos.z;
					}
				}
				else
				{

					pos = Aimplayer::Headpos;
					pos.z -= 20;
				}
				//预判
				if (bulletspead != 0)
				{
					float dis = local::Localpos.Distance(pos);
					float time = dis / bulletspead;
					Vector3 speed;
					mem->RPM(pointer + poffset->PLAYER_VELOCITY_OFFSET, &speed, sizeof(Vector3));
					pos.x += time * speed.x;
					pos.y += time * speed.y;
					pos.z += time * speed.z + 400 * time * time;
				}

				WorldToScreen(pos, spos);
				if (spos.x <= 0 || spos.y <= 0 || spos.x >= s_width || spos.y >= s_height)
					goto mcon;

				spos.x = spos.x - s_width / 2;
				spos.y = spos.y - s_height / 2;

				MY_MOUSE m_mousedata;
				m_mousedata.LastX = (LONG)spos.x;
				m_mousedata.LastY = (LONG)spos.y;
				mem->mouseevent(&m_mousedata);

				Aimplayer::health = mem->RPM<int>(pointer + poffset->PLAYER_HEALTH_OFFSET);
				if (GetAsyncKeyState(VK_RBUTTON) == 0 || Aimplayer::health <= 0)
				{
					InterlockedExchange64((volatile LONG64 *)&Aimplayer::pointer, (LONG64)0);
					break;
				}
			mcon:
				timePhase = GetTickCount() - timeBegin; //循环耗费的时间
				if (timePhase < timeInPerFrame)                //循环耗费的时间<每帧的时间
				{
					WaitForSingleObject((HANDLE)-1, DWORD(timeInPerFrame - timePhase)); //将剩余的时间等待
				}
			}
		}

	}
}