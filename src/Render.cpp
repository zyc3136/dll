#include "stdafx.h"




void Render()
{
	static DWORD timeBegin = 0;
	static DWORD timePhase = 0;
	static float constFps = 60.0f;                 //����֡��
	static DWORD timeInPerFrame = (DWORD)(1000.0f / constFps);    //����ÿ֡��Ƶ��
	timeBegin = GetTickCount();             //ѭ����ʼ��ʱ��  
	pRender->BeginDraw();
	pRender->Clear(D2D1_COLOR_F{ 1, 0, 0, 0 });

	ESP();

	pRender->EndDraw();
	timePhase = GetTickCount() - timeBegin; //ѭ���ķѵ�ʱ��
	if (timePhase < timeInPerFrame)                //ѭ���ķѵ�ʱ��<ÿ֡��ʱ��
	{
		WaitForSingleObject((HANDLE)-1, DWORD(timeInPerFrame - timePhase)); //��ʣ���ʱ��ȴ�
	}
}