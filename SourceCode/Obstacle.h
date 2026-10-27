#pragma once
#include"../Engine/GameObject.h"

class BoxCollider;
class SphereCollider;

class Obstacle : public GameObject
{
public:
	Obstacle(GameObject* parent);
	~Obstacle() override;
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void OnCollision(GameObject* other,Collider* col);

	/// <summary>
	/// オブジェクトの初期設定
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="scale">スケール</param>
	void SetObject(XMFLOAT3 position, XMFLOAT3 scale);
private:
	//障害物に接触したときの処理
	void CollisionBall(GameObject* ball, Collider* col);
	//プレイヤーに接触したときの処理
	void CollisionPlayer(GameObject* player, Collider* col);
private:
	int hModel_;
};
