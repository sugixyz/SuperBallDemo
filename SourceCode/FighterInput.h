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
		rotateInput(0.0f, 0.0f),
		isCameraOwner(false),
		cameraInput(0.0f,0.0f)
	{}

	void Initialize()
	{
		moveInput = XMFLOAT2(0.0f, 0.0f);
		isShootPressed = false;
		rotateInput = XMFLOAT2(0.0f, 0.0f);
		isCameraOwner = false;
		cameraInput = XMFLOAT2(0.0f, 0.0f);
	}

	//移動用の2次元ベクトル
	XMFLOAT2 moveInput;
	//発射ボタンが押されたか
	bool isShootPressed;
	//プレイヤーを回転させる2次元ベクトル
	XMFLOAT2 rotateInput;
	//カメラのオーナーかどうか
	bool isCameraOwner;
	//カメラを移動（回転）させる2次元ベクトル
	XMFLOAT2 cameraInput;
};