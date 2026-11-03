#include "Fighter.h"
#include"../Engine/Model.h"
#include"Controller.h"
#include"../Engine/BoxCollider.h"
#include"../Engine/OBBCollider.h"
#include"HealthComponent.h"
#include"MoveComponent.h"
#include"../Engine/Camera.h"
#include"Gun.h"

Fighter::Fighter(GameObject* parent)
	:GameObject(parent, "Fighter"),
	hModel_(-1),
	controller_(nullptr),
	health_(nullptr),
	move_(nullptr),
	gun_(nullptr),
	camType_(CamType::TPS),
	camPitch_(0.0f)
{}

Fighter::~Fighter()
{
}

void Fighter::Initialize()
{
	hModel_ = Model::Load("PlayerDemo.fbx");
	assert(hModel_ >= 0);

	uint32_t mask = LayerBit(CollisionLayer::DEFAULT);
	CollisionFilter filter = CollisionFilter(CollisionLayer::FIGHTER,mask);
	OBBCollider* col = new OBBCollider(COL_POS, COL_SIZE,filter);
	AddCollider(col);

	health_ = new HealthComponent();
	assert(health_ != nullptr);
	move_ = new MoveComponent();
	assert(move_ != nullptr);

	health_->Initialize(MAX_HP);

	move_->SetSpeed(MOVE_SPEED);
	move_->ResetVelocity();

	gun_ = Instantiate<Gun>(this);

	camPitch_ = TPS_CAM_PITCH;
}

void Fighter::Update()
{
	//コントローラーのポインタがなければスルー
	if (controller_ == nullptr)return;

	//コントローラーのアップデート
	controller_->Update();
	//入力情報を取得
	FighterInput input = controller_->GetInput();

	//入力情報をもとに処理を実行
	UpdateCamera(input);
	Move(input);
	Rotate(input);
	Shoot(input);

	if (health_->isDead())
	{
		KillMe();
	}
}

void Fighter::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Fighter::Release()
{
	delete controller_;
	delete health_;
	delete move_;
}

void Fighter::OnCollision(GameObject* other, Collider* col)
{
}

void Fighter::Move(const FighterInput& input)
{
	//前方ベクトル、右ベクトルから移動方向を計算
	XMVECTOR forward = CalculateForwardVector(transform_.rotate_.y);
	XMVECTOR right = CalculateRightVector(forward);
	XMVECTOR moveDirection = CalculateMoveDirection(input.moveInput, forward, right);

	move_->SetDirection(moveDirection);
	transform_.position_ = move_->Move(transform_);
}

void Fighter::Rotate(const FighterInput& input)
{
	XMFLOAT2 rotateInput = input.rotateInput;
	if (rotateInput.x < 0.0f)transform_.rotate_.y--;
	else if (rotateInput.x > 0.0f)transform_.rotate_.y++;
}

void Fighter::Shoot(const FighterInput& input)
{
	if (gun_ == nullptr)return;

	bool isShoot = input.isShootPressed;

	if (isShoot)gun_->ShootBullet();
}

void Fighter::UpdateCamera(const FighterInput& input)
{
	if (!input.isCameraOwner)return;
	//カメラのピッチ変更
	XMFLOAT2 cameraInput = input.cameraInput;
	if (cameraInput.y < 0.0f)camPitch_--;
	else if (cameraInput.y > 0.0f)camPitch_++;
	Clamp(camPitch_, TPS_CAM_PITCH_MIN, TPS_CAM_PITCH_MAX);

	XMVECTOR pos = XMLoadFloat3(&transform_.position_); // プレイヤーの位置
	XMFLOAT3 camRot = XMFLOAT3(camPitch_, transform_.rotate_.y, transform_.rotate_.z);
	
	switch (camType_)
	{
	case CamType::FPS:
		UpdateFpsCam();
		break;
	case CamType::TPS:
		UpdateTpsCam(camRot, TPS_CAM_DISTANCE, pos, TPS_CAM_POS_OFFSET);
		break;
	}
}

void Fighter::UpdateTpsCam(XMFLOAT3 camRot, float dist, XMVECTOR pos, XMVECTOR offset)
{
	XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(camRot.x));
	XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(camRot.y));
	XMMATRIX mRotZ = XMMatrixRotationZ(XMConvertToRadians(camRot.z));

	XMVECTOR vMove = XMVector3TransformCoord(FRONT, mRotZ * mRotX * mRotY);

	XMFLOAT3 camTarget;
	XMStoreFloat3(&camTarget, pos + offset + vMove);
	Camera::SetTarget(camTarget);

	XMVECTOR vCam = XMVectorSet(0.0f, 0.0f, -dist, 0.0f);
	vCam = XMVector3TransformCoord(vCam, mRotX * mRotY);

	XMFLOAT3 camPos;
	XMStoreFloat3(&camPos, pos + offset + vCam);
	Camera::SetPosition(camPos);
}

void Fighter::UpdateFpsCam()
{
}

XMVECTOR Fighter::CalculateMoveDirection(XMFLOAT2 moveInput,XMVECTOR forward, XMVECTOR right)
{
	//移動ベクトルを計算 
	XMVECTOR moveDirection = (forward * moveInput.y) + (right * moveInput.x);
	moveDirection = XMVector3Normalize(moveDirection);

	return moveDirection;
}

XMVECTOR Fighter::CalculateForwardVector(float rotationY)
{
	float rad = XMConvertToRadians(rotationY);
	float x = sinf(rad);
	float y = 0.0f;
	float z = cosf(rad);
	float w = 0.0f;
	XMVECTOR forward = XMVectorSet(x, y, z, w);
	forward = XMVector3Normalize(forward);

	return forward;
}

XMVECTOR Fighter::CalculateRightVector(XMVECTOR forward)
{
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR right = XMVector3Cross(up, forward);
	right = XMVector3Normalize(right);

	return right;
}

void Fighter::Clamp(float& current, float min, float max)
{
	if (current < min)current = min;
	if (current > max)current = max;
}
