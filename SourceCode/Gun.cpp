#include "Gun.h"
#include"../Engine/Model.h"
#include"Ball.h"
#include"../Engine/Text.h"

namespace
{
	Text* text = new Text();
	int count = 0;
}

Gun::Gun(GameObject* parent)
	:GameObject(parent,"Gun"),
	hModel_(-1),
	ballPool_()
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
	transform_.position_.z += 0.4f;

	transform_.rotate_.x -= 20;
	//transform_.rotate_.x -= 0;

	SetDynamic();

	text->Initialize();

	ballPool_.InitializePool<Ball>(this, BALL_COUNT);
}

void Gun::Update()
{
}

void Gun::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	text->Draw(500, 100, count);
}

void Gun::Release()
{
}

void Gun::ShootBullet()
{
	Ball* ball = static_cast<Ball*>(ballPool_.AcquiredNext());
	if (ball == nullptr)return;

	//ボーンを取得
	XMFLOAT3 gunMuzzle = Model::GetBonePosition(hModel_, "muzzle");
	XMFLOAT3 gunBarrel = Model::GetBonePosition(hModel_, "barrel");
	XMVECTOR muzzle = XMLoadFloat3(&gunMuzzle);
	XMVECTOR barrel = XMLoadFloat3(&gunBarrel);
	XMVECTOR vMove = muzzle - barrel;
	
	//座標と移動方向を指定
	ball->SetPosition(gunMuzzle);

	XMFLOAT3 dir;
	XMStoreFloat3(&dir, XMVector3Normalize(vMove));
	ball->SetMoveDirection(dir);

	count++;
}

void Gun::Reload()
{
	ballPool_.Reset();
}

void Gun::RefillBall(GameObject* obj)
{
	//無効化すればプールから取り出せるように
	obj->Disable();
}
