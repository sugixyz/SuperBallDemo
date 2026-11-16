#pragma once
#include"../Engine/GameObject.h"
#include"ObjectData.h"

class Balloon : public GameObject
{
public:
	Balloon(GameObject* parent);
	~Balloon() override;

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void OnCollision(const CollisionData& data);

	void SetObject(const ObjectData& data);
private:
	//ボールに接触したときの処理
	void OnCollisionBall(const CollisionData& data);
private:
	int hModel_;
private:
	static constexpr XMFLOAT3 COL_OFFSET = { 0.0f,0.0f,0.0f };
	static constexpr float COL_RADIUS = 0.2f;
};