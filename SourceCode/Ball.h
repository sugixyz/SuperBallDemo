#pragma once
#include"../Engine/GameObject.h"

class Ball : public GameObject
{
public:
	Ball(GameObject* parent);
	~Ball();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void OnCollision(GameObject* other, Collider* col)override;

	/// <summary>
	/// 移動方向を設定
	/// </summary>
	/// <param name="direction">指定する移動方向</param>
	void SetMoveDirection(XMVECTOR direction);

	MoveComponent* GetMoveComponent() override { return move_; }
private:
	//障害物に接触したときの処理
	void CollisionObstacle(GameObject* obstacle, Collider* col);
	//プレイヤーに接触したときの処理
	void CollisionPlayer(GameObject* player, Collider* col);
	//ボックスの法線を計算する関数
	XMVECTOR CalculateBoxCollsionNormal(XMVECTOR spherePos, XMVECTOR boxCenter, XMVECTOR boxMin, XMVECTOR boxMax);
	//球の法線を計算する関数
	XMVECTOR CalculateSphereCollisionNormal(XMVECTOR sphereAPos, XMVECTOR sphereBPos);
private:
	//モデルハンドル
	int hModel_;
	//移動のコンポーネントポインタ
	MoveComponent* move_;
private:
	//コライダーの中心位置
	static constexpr XMFLOAT3 COL_POS = { 0.0f,0.0f,0.0f };
	//コライダーのサイズ
	static constexpr float COL_RADIUS = 0.04f;
	//移動速度
	static constexpr float MOVE_SPEED = 8.0f;
	//反発係数
	static constexpr float RESTITUTION = 0.9f;
};