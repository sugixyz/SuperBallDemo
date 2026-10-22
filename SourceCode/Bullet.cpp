#include "Bullet.h"
#include"../Engine/Model.h"
#include"MoveComponent.h"
#include"../Engine/SphereCollider.h"

Bullet::Bullet(GameObject* parent)
	:GameObject(parent,"Bullet"),
	hModel_(-1),
	move_(nullptr)
{
}

Bullet::~Bullet()
{
}

void Bullet::Initialize()
{
	hModel_ = Model::Load("BulletDemo.fbx");
	assert(hModel_ >= 0);

	SphereCollider* col = new SphereCollider(COL_POS, COL_RADIUS);
	AddCollider(col);

	move_ = new MoveComponent();
	assert(move_ != nullptr);

	move_->SetSpeed(MOVE_SPEED);
}

void Bullet::Update()
{
	transform_.position_ = move_->Move(transform_);
}

void Bullet::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Bullet::Release()
{
}

void Bullet::SetMoveDirection(XMVECTOR direction)
{
	move_->SetDirection(direction);
}
