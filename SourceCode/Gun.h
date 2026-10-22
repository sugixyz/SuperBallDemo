#pragma once
#include"../Engine/GameObject.h"

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
private:
	//モデルハンドル
	int hModel_;
};