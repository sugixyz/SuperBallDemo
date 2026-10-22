#include "Ball.h"
#include"../Engine/Model.h"
#include"MoveComponent.h"
#include"../Engine/SphereCollider.h"

Ball::Ball(GameObject* parent)
	:GameObject(parent,"Ball"),
	hModel_(-1),
	move_(nullptr)
{
}

Ball::~Ball()
{
}

void Ball::Initialize()
{
	hModel_ = Model::Load("BallDemo.fbx");
	assert(hModel_ >= 0);

	SphereCollider* col = new SphereCollider(COL_POS, COL_RADIUS);
	AddCollider(col);

	move_ = new MoveComponent();
	assert(move_ != nullptr);

	move_->SetSpeed(MOVE_SPEED);
	move_->EnableGravity();
}	

void Ball::Update()
{
	transform_.position_ = move_->Move(transform_);
}

void Ball::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Ball::Release()
{
}

void Ball::OnCollision(GameObject* other)
{

}

void Ball::SetMoveDirection(XMVECTOR direction)
{
	move_->SetDirection(direction);
}
