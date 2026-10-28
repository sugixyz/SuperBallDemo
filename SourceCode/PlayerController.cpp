#include "PlayerController.h"
#include"../Engine/Input.h"

PlayerController::PlayerController()
{
	fighterInput_ = FighterInput();
}

PlayerController::~PlayerController()
{
}

void PlayerController::Update()
{
	fighterInput_.Initialize();

	fighterInput_.isCameraOwner = true;

	if (Input::IsKey(DIK_W))fighterInput_.moveInput.y = 1.0f;
	else if (Input::IsKey(DIK_S))fighterInput_.moveInput.y = -1.0f;

	if (Input::IsKey(DIK_A))fighterInput_.moveInput.x = -1.0f;
	else if (Input::IsKey(DIK_D))fighterInput_.moveInput.x = 1.0f;

	if (Input::IsKey(DIK_LEFT))fighterInput_.rotateInput.x = -1.0f;
	else if (Input::IsKey(DIK_RIGHT))fighterInput_.rotateInput.x = 1.0f;

	if (Input::IsKeyDown(DIK_SPACE))fighterInput_.isShootPressed = true;
}
