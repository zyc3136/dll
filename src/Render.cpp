#include "stdafx.h"




void Render()
{
	static DWORD timeBegin = 0;
	static DWORD timePhase = 0;
	static float constFps = 60.0f;                 //定义帧数
	static DWORD timeInPerFrame = (DWORD)(1000.0f / constFps);    //计算每帧的频率
	timeBegin = GetTickCount();             //循环开始的时间  
	pRender->BeginDraw();
	pRender->Clear(D2D1_COLOR_F{ 1, 0, 0, 0 });

	ESP();

	pRender->EndDraw();
	timePhase = GetTickCount() - timeBegin; //循环耗费的时间
	if (timePhase < timeInPerFrame)                //循环耗费的时间<每帧的时间
	{
		WaitForSingleObject((HANDLE)-1, DWORD(timeInPerFrame - timePhase)); //将剩余的时间等待
	}
}