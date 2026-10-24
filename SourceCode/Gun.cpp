#include "Gun.h"
#include"../Engine/Model.h"
#include"Ball.h"

Gun::Gun(GameObject* parent)
	:GameObject(parent,"Gun"),
	hModel_(-1)
{
}

Gun::~Gun()
{
}

void Gun::Initialize()
{
	hModel_ = Model::Load("GunDemo.fbx");
	assert(hModel_ >= 0);

	transform_.position_.x -= 0.1f;
	transform_.position_.z += 0.2f;

	transform_.rotate_.x -= 30;
}

void Gun::Update()
{
}

void Gun::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Gun::Release()
{
}

void Gun::ShootBullet()
{
	//ボーンを取得
	XMFLOAT3 gunMuzzle = Model::GetBonePosition(hModel_, "muzzle");
	XMFLOAT3 gunBarrel = Model::GetBonePosition(hModel_, "barrel");
	XMVECTOR muzzle = XMLoadFloat3(&gunMuzzle);
	XMVECTOR barrel = XMLoadFloat3(&gunBarrel);
	XMVECTOR vMove = muzzle - barrel;
	
	//弾生成
	Ball* ball = Instantiate<Ball>(this->GetParent()->GetParent());
	ball->SetPosition(gunMuzzle);
	ball->SetMoveDirection(vMove);
}
