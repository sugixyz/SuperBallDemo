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
	/// ジャンプさせる
	/// </summary>
	/// <param name="jumpSpeed">ジャンプの初速</param>
	void Jump(float jumpSpeed);

	/// <summary>
	/// 天井に接触した時の処理
	/// </summary>
	void OnCelingHit();

	void SetGrounded(bool isGrounded);
	bool IsGrounded() { return isGrounded_; }

	void SetSpeed(float speed) { speed_ = speed; }
	float GetSpeed() { return speed_; }

	void SetDirection(XMFLOAT3 direction) { direction_ = direction; }
	XMFLOAT3 GetDirection() { return direction_; }

	XMFLOAT3 GetVelocity() { return velocity_; }

	void SetUseGravity(bool useGravity) { useGravity_ = useGravity; }
	bool IsUseGravity() { return useGravity_; }

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
	XMFLOAT3 direction_;
	XMFLOAT3 velocity_;
	bool useGravity_;
	bool isGrounded_;
private:
	static constexpr float GRAVITY = -9.8f;
};