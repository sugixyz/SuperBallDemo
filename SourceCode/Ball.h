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

	void OnCollision(const CollisionData& data)override;

	/// <summary>
	/// 移動方向を設定
	/// </summary>
	/// <param name="direction">指定する移動方向</param>
	void SetMoveDirection(XMVECTOR direction);

	MoveComponent* GetMoveComponent() override { return move_; }
private:
	//障害物に接触したときの処理
	void CollisionObstacle(CollisionData data);
	//プレイヤーに接触したときの処理
	void CollisionPlayer(GameObject* player, Collider* col);
	//ボールに接触したときの処理
	void CollisionBall(CollisionData data);
	//ボックスとの法線を計算する関数
	XMVECTOR CalculateBoxCollsionNormal(XMVECTOR spherePos, XMVECTOR boxCenter, XMVECTOR boxMin, XMVECTOR boxMax);
	//球との法線を計算する関数
	XMVECTOR CalculateSphereCollisionNormal(XMVECTOR sphereAPos, XMVECTOR sphereBPos);
	//球との接線を計算する関数
	XMVECTOR CalculateSphereCollisionTangentLine(XMVECTOR normal);
private:
	//モデルハンドル
	int hModel_;
	//移動のコンポーネントポインタ
	MoveComponent* move_;
	//何かに接触したかどうか
	bool isColliding_;
private:
	//コライダーの中心位置
	static constexpr XMFLOAT3 COL_POS = { 0.0f,0.0f,0.0f };
	//コライダーのサイズ
	static constexpr float COL_RADIUS = 0.04f;
	//移動速度
	static constexpr float MOVE_SPEED = 8.0f;
	//反発係数
	static constexpr float RESTITUTION = 0.9f;
	//与えるダメージ
	static constexpr int DAMAGE = 10;
	//ワールド座標上の上ベクトル
	static constexpr XMVECTOR UP = { 0.0f,0.0f,1.0f,0.0f };
};