#pragma once
#include"../Engine/GameObject.h"
#include"FighterInput.h"

class Controller;

class Fighter : public GameObject
{
public:
	Fighter(GameObject* parent);
	~Fighter() override;
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	/// <summary>
	/// 速度を設定する関数
	/// </summary>
	/// <param name="velocity">設定したい速度</param>
	void SetVelocity(XMFLOAT3 velocity) { velocity_ = velocity; }
	/// <summary>
	/// 速度を取得する関数
	/// </summary>
	/// <returns>現在の速度</returns>
	XMFLOAT3 GetVelocity() { return velocity_; }

	/// <summary>
	/// ファイター用のコントローラーを設定する
	/// </summary>
	/// <param name="controller"></param>
	void SetController(Controller* controller) { controller_ = controller; }
private:
	void Move(const FighterInput& input);
	void Shoot(const FighterInput& input);
	void UpdateCamera(const FighterInput& input);
private:
	//速度
	XMFLOAT3 velocity_;
	//モデルハンドル
	int hModel_;
	//コントローラーのポインタ
	Controller* controller_;
private:
	//コライダーの中心位置
	static constexpr XMFLOAT3 COL_POS = { 0.25f,0.5f,0.25f };
	//コライダーのサイズ
	static constexpr XMFLOAT3 COL_SIZE = { 0.5f,1.0f,0.5f };
};