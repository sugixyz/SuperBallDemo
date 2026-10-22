#include "Obstacle.h"
#include"../Engine/BoxCollider.h"
#include"../Engine/Model.h"

Obstacle::Obstacle(GameObject* parent)
	:GameObject(parent,"Obstacle"),
	hModel_(-1),
	col_(nullptr)
{
}

Obstacle::~Obstacle()
{
}

void Obstacle::Initialize()
{
	hModel_ = Model::Load("WallPrefabDemo.fbx");
	assert(hModel_ >= 0);

	col_ = new BoxCollider(transform_.position_, transform_.scale_);
	AddCollider(col_);
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
	this->SetPosition(position);
	this->SetScale(scale);

	BoxCollider* col = new BoxCollider(transform_.position_, XMFLOAT3(1.0f,1.0f,1.0f));
	AddCollider(col);
}
