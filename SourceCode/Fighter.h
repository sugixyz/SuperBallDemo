#pragma once
#include"../Engine/GameObject.h"

class Fighter : public GameObject
{
public:
	Fighter(GameObject* parent);
	~Fighter() override;
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

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

	//コントローラーからのリクエスト関数
	/// <summary>
	/// 弾の発射をリクエスト
	/// </summary>
	void OnShoot();
	/// <summary>
	/// 走りをリクエスト
	/// </summary>
	void OnRun();
	/// <summary>
	/// しゃがみをリクエスト
	/// </summary>
	void OnCrouch();
	/// <summary>
	/// 構えをリクエスト
	/// </summary>
	void OnReadyWeapon();
private:
	//速度
	XMFLOAT3 velocity_;
	//モデルハンドル
	int hModel_;
private:
	void Move();
	void Shoot();
	void UpdateCamera();
};