#pragma once
#include"../Engine/GameObject.h"
#include"ObjectPool.h"

class Gun : public GameObject
{
public:
	Gun(GameObject* parent);
	~Gun() override;
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	/// <summary>
	/// 弾を発射する関数
	/// </summary>
	void ShootBullet();
	/// <summary>
	/// 弾を全て補充する関数
	/// </summary>
	void Reload();
	/// <summary>
	/// 特定の弾を補充する関数
	/// </summary>
	/// <param name="obj">補充する弾</param>
	void RefillBall(GameObject* obj);
private:
	//モデルハンドル
	int hModel_;
	//ボールのオブジェクトプール
	ObjectPool ballPool_;
private:
	static constexpr int BALL_COUNT = 20;
};