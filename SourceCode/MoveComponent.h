#pragma once
#include"Component.h"
#include<DirectXMath.h>

using namespace DirectX;

class Transform;

class MoveComponent : public Component
{
public:
	MoveComponent();
	~MoveComponent() override;

	/// <summary>
	///	所有者を移動させる関数
	/// </summary>
	/// <param name="ownerTransform">所有者の座標情報</param>
	/// <returns>最新座標</returns>
	XMFLOAT3 Move(Transform ownerTransform);

	/// <summary>
	/// 移動速度を設定する関数
	/// </summary>
	/// <param name="speed">移動速度</param>
	void SetSpeed(float speed) { speed_ = speed; }
	/// <summary>
	/// 現在の移動速度を取得する関数
	/// </summary>
	/// <returns>現在の移動速度</returns>
	float GetSpeed() { return speed_; }
	/// <summary>
	/// 移動方向を設定する関数
	/// </summary>
	/// <param name="direction">移動方向（3次元）</param>
	void SetDirection(XMVECTOR direction) { direction_ = direction; }
	/// <summary>
	/// 現在の移動方向を取得する関数
	/// </summary>
	/// <returns>現在の移動方向</returns>
	XMVECTOR GetDirection() { return direction_; }
private:
	float speed_;
	XMVECTOR direction_;
	XMVECTOR velocity_;
};