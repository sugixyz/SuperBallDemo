#pragma once
#include"../Engine/GameObject.h"

class Obstacle : public GameObject
{
public:
	Obstacle(GameObject* parent);
	~Obstacle() override;
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	/// <summary>
	/// オブジェクトの初期設定
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="scale">スケール</param>
	void SetObject(XMFLOAT3 position, XMFLOAT3 scale);
private:
	int hModel_;
};
