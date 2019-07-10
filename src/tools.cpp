#include "stdafx.h"

char key[18] = { NULL };


void Encodestring(char* a, char* b)
{
	VMProtectBeginMutation("Encodestring");
	for (int i = 0; i < strlen(a); i++)
	{
		if (key[i % 18] == a[i])
			b[i] = a[i];
		else
			b[i] = a[i] ^ key[i % 18];
	}
	VMProtectEnd();
}


void randstr(std::string& str)
{
	VMProtectBeginMutation("randstr");
	srand((byte)time(NULL));
	for (int i = 0; i < str.length(); ++i)
	{
		switch ((rand() % 3))
		{
		case 1:
			str[i] = 'A' + rand() % 26;
			break;
		case 2:
			str[i] = 'a' + rand() % 26;
			break;
		default:
			str[i] = '0' + rand() % 10;
			break;
		}
	}
	VMProtectEnd();
}

wchar_t * char2wchar(const char* cchar)
{
	VMProtectBeginMutation("char2wchar");
	wchar_t *m_wchar;
	int len = MultiByteToWideChar(CP_ACP, 0, cchar, (int)strlen(cchar), NULL, 0);
	m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, cchar, (int)strlen(cchar), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
	VMProtectEnd();
}

