#pragma once
#include"../Engine/GameObject.h"

//キャラクター用のインターフェース
class Character : public GameObject
{
public:
	virtual ~Character() = default;
	virtual void Initialize() override = 0;
	virtual void Update() override = 0;
	virtual void Draw() override = 0;
	virtual void Release() override = 0;

	/// <summary>
	/// 速度を設定する関数
	/// </summary>
	/// <param name="velocity">設定したい速度</param>
	void SetVelocity(XMFLOAT3 velocity) { velocity_ = velocity; }
	/// <summary>
	/// 速度を取得する関数
	/// </summary>
	/// <returns>現在の速度</returns>
	XMFLOAT3 GetVelocity() { return velocity_; }
protected:
	//速度
	XMFLOAT3 velocity_;
	//モデルハンドル
	int hModel_;
};