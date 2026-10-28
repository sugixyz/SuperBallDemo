#include "AutoController.h"
#include"../Engine/Input.h"
#include"../Engine/Timer.h"

AutoController::AutoController()
{
	fighterInput_ = FighterInput();
}

AutoController::~AutoController()
{
}

void AutoController::Update()
{
	fighterInput_.Initialize();

	fighterInput_.rotateInput.x = 1.0f;

	static float timer = 0.0f;
	timer += Timer::GetDeltaTime();
	if (timer >= 1.0f)
	{
		fighterInput_.isShootPressed = true;
		timer = 0.0f;
	}
}
