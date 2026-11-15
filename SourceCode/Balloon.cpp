#include "Balloon.h"
#include"../Engine/Model.h"
#include"../Engine/SphereCollider.h"

Balloon::Balloon(GameObject* parent)
	:GameObject(parent,"Balloon")
	,hModel_(-1)
{}

Balloon::~Balloon()
{}

void Balloon::Initialize()
{
	CollisionFilter filter = CollisionFilter(CollisionLayer::BALLOON);
	SphereCollider* col = new SphereCollider(COL_OFFSET, COL_RADIUS, filter);
	AddCollider(col);

	hModel_ = Model::Load("Balloon.fbx");
	assert(hModel_ >= 0);

	transform_.position_.x = -2.0f;
	transform_.position_.y = 2.0f;
	transform_.position_.z = 2.0f;
}

void Balloon::Update()
{}

void Balloon::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Balloon::Release()
{}

void Balloon::OnCollision(const CollisionData & data)
{
	GameObject* other = data.other;
	Collider* col = data.otherCollider;

	std::string name = other->GetObjectName();

	if (name == "Ball")
	{
		OnCollisionBall(data);
	}
}

void Balloon::OnCollisionBall(const CollisionData& data)
{
	KillMe();
}
