#include "Obstacle.h"
#include"../Engine/BoxCollider.h"
#include"../Engine/SphereCollider.h"
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

void Obstacle::SetObject(XMFLOAT3 position, XMFLOAT3 scale)
{
	transform_.position_ = position;
	transform_.scale_ = scale;

	BoxCollider* col = new BoxCollider(XMFLOAT3(0.0f,0.0f,0.0f), transform_.scale_);
	AddCollider(col);
}
