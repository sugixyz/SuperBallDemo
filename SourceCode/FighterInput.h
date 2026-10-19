#pragma once
#include<DirectXMath.h>

using namespace DirectX;

struct FighterInput
{
	/// <summary>
	/// Inputの初期化
	/// </summary>
	FighterInput()
		:moveInput(0.0f, 0.0f),
		isShootPressed(false),
		cameraInput(0.0f, 0.0f)
	{}

	void Initialize()
	{
		moveInput = XMFLOAT2(0.0f, 0.0f);
		isShootPressed = false;
		cameraInput = XMFLOAT2(0.0f, 0.0f);
	}

	//移動用の2次元ベクトル
	XMFLOAT2 moveInput;
	//発射ボタンが押されたか
	bool isShootPressed;
	//カメラを操作する2次元ベクトル
	XMFLOAT2 cameraInput;
};