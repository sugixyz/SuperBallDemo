#include "Fighter.h"
#include"../Engine/Model.h"
#include"Controller.h"
#include"../Engine/BoxCollider.h"
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
	camType_(CamType::TPS)
{}

Fighter::~Fighter()
{
}

void Fighter::Initialize()
{
	hModel_ = Model::Load("PlayerDemo.fbx");
	assert(hModel_ >= 0);

	BoxCollider* col = new BoxCollider(COL_POS, COL_SIZE);
	AddCollider(col);

	health_ = new HealthComponent();
	assert(health_ != nullptr);
	move_ = new MoveComponent();
	assert(move_ != nullptr);

	health_->Initialize(MAX_HP);

	move_->SetSpeed(MOVE_SPEED);
	move_->ResetVelocity();

	gun_ = Instantiate<Gun>(this);
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
	if (rotateInput.x < 0)transform_.rotate_.y--;
	else if (rotateInput.x > 0)transform_.rotate_.y++;
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

	XMVECTOR vPos = XMLoadFloat3(&transform_.position_); //ロード：読み込み
	XMMATRIX mRotX = XMMatrixRotationX(XMConvertToRadians(transform_.rotate_.x));
	XMMATRIX mRotY = XMMatrixRotationY(XMConvertToRadians(transform_.rotate_.y));
	XMMATRIX mRotZ = XMMatrixRotationZ(XMConvertToRadians(transform_.rotate_.z));
	XMVECTOR vMove = XMVector3TransformCoord(FRONT, mRotZ * mRotX * mRotY);

	switch (camType_)
	{
	case CamType::FPS:
		UpdateFpsCam();
		break;
	case CamType::TPS:
		UpdateTpsCam(mRotY, vPos, vMove);
		break;
	}
}

void Fighter::UpdateTpsCam(XMMATRIX mRotY, XMVECTOR vPos, XMVECTOR vMove)
{
	//自分の位置に
	XMFLOAT3 camPos;
	XMVECTOR vCam = { 0.0f,TPS_HEIGHT_BIAS,-TPS_FRONT_BIAS,0.0f };
	vCam = XMVector3TransformCoord(vCam, mRotY);
	XMStoreFloat3(&camPos, vPos + vCam);
	Camera::SetPosition(camPos);
	XMFLOAT3 camTarget;
	XMStoreFloat3(&camTarget, vPos + vMove);
	Camera::SetTarget(camTarget);
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
