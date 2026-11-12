#include "Obstacle.h"
#include"../Engine/BoxCollider.h"
#include"../Engine/OBBCollider.h"
#include"../Engine/Model.h"

Obstacle::Obstacle(GameObject* parent)
	:GameObject(parent,"Obstacle"),
	hModel_(-1)
{
}

Obstacle::~Obstacle()
{
}

void Obstacle::Initialize()
{
	hModel_ = Model::Load("WallPrefabDemo.fbx");
	assert(hModel_ >= 0);
}

void Obstacle::Update()
{
}

void Obstacle::Draw()
{
	Model::SetTransform(hModel_,transform_);
	Model::Draw(hModel_);
}

void Obstacle::Release()
{
}

void Obstacle::OnCollision(const CollisionData& data)
{
}

void Obstacle::SetObject(Transform transform, bool isStatic)
{
	transform_ = transform;
	if (isStatic)SetStatic();
	else SetDynamic();

	CollisionFilter filter = CollisionFilter(CollisionLayer::OBSTACLE);
	if (transform.rotate_.x == 0.0f && transform.rotate_.y == 0.0f && transform.rotate_.z == 0.0f)
	{
		//OBBCollider* col = new OBBCollider(COL_OFFSET, transform_.scale_, filter);
		BoxCollider* col = new BoxCollider(COL_OFFSET, transform_.scale_, filter);
		AddCollider(col);
	}
	else
	{
		OBBCollider* col = new OBBCollider(COL_OFFSET, transform_.scale_, filter);
		AddCollider(col);
	}
}