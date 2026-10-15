#include "Timer.h"

namespace Timer
{
	DWORD prevTime;
	DWORD currentTime;

	void Initialize()
	{
		prevTime = timeGetTime();
		currentTime = timeGetTime();
	}

	void Update()
	{
		prevTime = currentTime;
		currentTime = timeGetTime();
	}

	void Release()
	{

	}

	float GetDeltaTime()
	{
		DWORD dt = currentTime - prevTime;
		return dt / 1000.0f;
	}
}
