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
	/// ファイター用のコントローラーを設定する
	/// </summary>
	/// <param name="controller"></param>
	void SetController(Controller* controller) { controller_ = controller; }
private:
	void Move(const FighterInput& input);
	void Shoot(const FighterInput& input);
	void UpdateCamera(const FighterInput& input);
private:
	//モデルハンドル
	int hModel_;
	//コントローラーのポインタ
	Controller* controller_;
	HealthComponent* health_;
	MoveComponent* move_;
private:
	//コライダーの中心位置
	static constexpr XMFLOAT3 COL_POS = { 0.0f,0.5f,0.0f };
	//コライダーのサイズ
	static constexpr XMFLOAT3 COL_SIZE = { 0.5f,1.0f,0.5f };
	//移動速度
	static constexpr float MOVE_SPEED = 1.5f;
	//最大HP
	static constexpr float MAX_HP = 100.0f;
};