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
	/// 2次元方向を設定する関数
	/// </summary>
	/// <param name="direction">移動方向（2次元）</param>
	void SetDirection2D(XMFLOAT2 direction) { direction2D_ = direction; }
	/// <summary>
	/// 移動方向を設定する関数
	/// </summary>
	/// <param name="direction">移動方向（3次元）</param>
	void SetDirection3D(XMVECTOR direction) { direction3D_ = direction; isSetDirection3D_ = true; }
	/// <summary>
	/// 現在の移動方向を取得する関数
	/// </summary>
	/// <returns>現在の移動方向</returns>
	XMFLOAT2 GetDirection2D() { return direction2D_; }
private:
	//速度を計算する関数
	XMVECTOR CalculateVelocity(XMVECTOR forward,XMVECTOR right);
	//正面ベクトルを計算する関数
	XMVECTOR CalculateForwardVector(float rotationY);
	//右ベクトルを計算する関数
	XMVECTOR CalculateRightVector(XMVECTOR forward);
private:
	float speed_;
	XMFLOAT2 direction2D_;
	XMVECTOR direction3D_;
	bool isSetDirection3D_;
	XMVECTOR velocity_;
};