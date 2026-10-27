#include "TargetObject.h"
#include"../Engine/Model.h";
#include"../Engine/BoxCollider.h"
#include"../Engine/Text.h"

TargetObject::TargetObject(GameObject* parent)
	:GameObject(parent,"TargetObject"),
	hModel_(-1),
	text_(nullptr)
{
}

TargetObject::~TargetObject()
{
}

void TargetObject::Initialize()
{
	BoxCollider* col = new BoxCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), COL_SIZE);
	AddCollider(col);

	hModel_ = Model::Load("Target.fbx");
	assert(hModel_ >= 0);
}

void TargetObject::Update()
{
}

void TargetObject::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void TargetObject::Release()
{
}

void TargetObject::OnCollision(GameObject* other, Collider* col)
{

}
