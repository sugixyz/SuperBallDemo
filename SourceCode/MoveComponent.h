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
	void SetDirection(XMVECTOR direction) { direction_ = XMVector3Normalize(direction); }
	/// <summary>
	/// 現在の移動方向を取得する関数
	/// </summary>
	/// <returns>現在の移動方向</returns>
	XMVECTOR GetDirection() { return direction_; }
	/// <summary>
	/// 現在の速度を取得する関数
	/// </summary>
	/// <returns>現在の移動速度</returns>
	XMVECTOR GetVelocity() { return velocity_; }
	/// <summary>
	/// 重力を使用するかどうかを設定
	/// </summary>
	/// <param name="useGravity">重力を使用するかどうか</param>
	void SetUseGravity(bool useGravity) { useGravity_ = useGravity; }
	/// <summary>
	/// 現在重力を使用しているかどうか
	/// </summary>
	/// <returns>使用していればtrueを返す</returns>
	bool GetUseGravity() { return useGravity_; }
	/// <summary>
	/// 重力を有効化する関数
	/// </summary>
	void EnableGravity() { SetUseGravity(true); }
	/// <summary>
	/// 重力を無効化する関数
	/// </summary>
	void DisableGravity() { SetUseGravity(false); }
	/// <summary>
	/// 重力による影響をリセットする
	/// </summary>
	void ResetGravity();
	/// <summary>
	/// 速度をリセットする
	/// </summary>
	void ResetVelocity();
private:
	float speed_;
	XMVECTOR direction_;
	XMVECTOR velocity_;
	bool useGravity_;
private:
	static constexpr float GRAVITY = -9.8f;
};