#pragma once
#include"../Engine/GameObject.h"
#include"FighterInput.h"

class Controller;
class Gun;

enum class CamType
{
	TPS,	//三人称視点
	FPS		//一人称視点
};

class Fighter : public GameObject
{
public:
	Fighter(GameObject* parent);
	~Fighter() override;
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void OnCollision(GameObject* other,Collider* col) override;

	/// <summary>
	/// ファイター用のコントローラーを設定する
	/// </summary>
	/// <param name="controller"></param>
	void SetController(Controller* controller) { controller_ = controller; }

	MoveComponent* GetMoveComponent() override { return move_; }
	HealthComponent* GetHealthComponent() override { return health_; }
private:
	//移動
	void Move(const FighterInput& input);
	//回転
	void Rotate(const FighterInput& input);
	//攻撃
	void Shoot(const FighterInput& input);
	//カメラの移動
	void UpdateCamera(const FighterInput& input);
	//TPSカメラの更新
	void UpdateTpsCam(XMFLOAT3 camRot, float dist,XMVECTOR pos, XMVECTOR offset);
	//FPSカメラの更新
	void UpdateFpsCam();
	//速度を計算する関数
	XMVECTOR CalculateMoveDirection(XMFLOAT2 moveInput,XMVECTOR forward, XMVECTOR right);
	//正面ベクトルを計算する関数
	XMVECTOR CalculateForwardVector(float rotationY);
	//右ベクトルを計算する関数
	XMVECTOR CalculateRightVector(XMVECTOR forward);
	//クランプ関数
	void Clamp(float& current, float min, float max);
private:
	//モデルハンドル
	int hModel_;
	//コントローラーのポインタ
	Controller* controller_;
	//HPのコンポーネントポインタ
	HealthComponent* health_;
	//移動のコンポーネントポインタ
	MoveComponent* move_;
	//銃のポインタ
	Gun* gun_;
	//カメラの種類
	CamType camType_;
	//カメラのピッチ
	float camPitch_;
private:
	//コライダーの中心位置
	static constexpr XMFLOAT3 COL_POS = { 0.0f,0.5f,0.0f };
	//コライダーのサイズ
	static constexpr XMFLOAT3 COL_SIZE = { 0.4f,1.0f,0.4f };

	//移動速度
	static constexpr float MOVE_SPEED = 4.5f;
	//最大HP
	static constexpr float MAX_HP = 100.0f;
	//前方ベクトル
	static constexpr XMVECTOR FRONT = { 0.0f,0.0f,1.0f,0.0f };

	//カメラのズレ（距離）
	static constexpr float TPS_CAM_DISTANCE = 4.0f;
	static constexpr XMVECTOR TPS_CAM_POS_OFFSET = { 0.0f,0.5f,0.0f,0.0f };
	//カメラのX軸角度（ピッチ）
	static constexpr float TPS_CAM_PITCH = 30.0f;
	//カメラのX軸角度の制限
	static constexpr float TPS_CAM_PITCH_MIN = 0.0f;
	static constexpr float TPS_CAM_PITCH_MAX = 60.0f;
};