#include "Fighter.h"
#include"../Engine/Model.h"
#include"Controller.h"

Fighter::Fighter(GameObject* parent)
	:GameObject(parent, "Fighter"),
	hModel_(-1),
	velocity_(0.0f, 0.0f, 0.0f),
	controller_(nullptr)
{}

Fighter::~Fighter()
{

}

void Fighter::Initialize()
{
	hModel_ = Model::Load("PlayerDemo.fbx");
	assert(hModel_ >= 0);
}

void Fighter::Update()
{
	Move();
}

void Fighter::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Fighter::Release()
{}

void Fighter::OnMove(XMFLOAT2 move)
{
}

void Fighter::OnShoot()
{
	//ボールを発射
	Shoot();
}

void Fighter::OnRun()
{}

void Fighter::OnCrouch()
{}

void Fighter::OnReadyWeapon()
{}

void Fighter::Move()
{

}

void Fighter::Shoot()
{}

void Fighter::UpdateCamera()
{}
