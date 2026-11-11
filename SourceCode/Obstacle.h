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

	void OnCollision(const CollisionData& data);

	/// <summary>
	/// 障害物の初期化
	/// </summary>
	/// <param name="transform">設定するトランスフォーム</param>
	void SetObject(Transform transform);
private:
	int hModel_;
private:
	static constexpr XMFLOAT3 COL_OFFSET = { 0.0f,0.0f,0.0f };
};
