#include "Timer.h"

namespace Timer
{
	DWORD prevTime_;
	DWORD currentTime_;

	void Initialize()
	{
		prevTime_ = timeGetTime();
		currentTime_ = timeGetTime();
	}

	void Update()
	{
		prevTime_ = currentTime_;
		currentTime_ = timeGetTime();
	}

	void Release()
	{

	}

	float GetDeltaTime()
	{
		DWORD dt = currentTime_ - prevTime_;
		return dt / 1000.0f;
	}
}
