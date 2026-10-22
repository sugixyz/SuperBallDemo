#pragma once
#include"../Engine/GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet(GameObject* parent);
	~Bullet();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	/// <summary>
	/// 移動方向を設定
	/// </summary>
	/// <param name="direction">指定する移動方向</param>
	void SetMoveDirection(XMVECTOR direction);

	MoveComponent* GetMoveComponent() override { return move_; }
private:
	//モデルハンドル
	int hModel_;
	//移動のコンポーネントポインタ
	MoveComponent* move_;
private:
	//コライダーの中心位置
	static constexpr XMFLOAT3 COL_POS = { 0.0f,0.0f,0.0f };
	//コライダーのサイズ
	static constexpr float COL_RADIUS = 0.1f;
	//移動速度
	static constexpr float MOVE_SPEED = 1.5f;
};