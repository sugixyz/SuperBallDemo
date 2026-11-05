#include "TargetObject.h"
#include"../Engine/Model.h";
#include"../Engine/BoxCollider.h"
#include"../Engine/OBBCollider.h"
#include"../Engine/Text.h"

TargetObject::TargetObject(GameObject* parent)
	:GameObject(parent,"TargetObject"),
	hModel_(-1),
	text_(nullptr),
	score_(0)
{
}

TargetObject::~TargetObject()
{
}

void TargetObject::Initialize()
{
	OBBCollider* col = new OBBCollider(XMFLOAT3(0.0f, 0.0f, 0.0f), COL_SIZE);
	AddCollider(col);

	hModel_ = Model::Load("Target.fbx");
	assert(hModel_ >= 0);

	text_ = new Text();
	text_->Initialize();
}

void TargetObject::Update()
{
}

void TargetObject::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	char score[20];
	sprintf_s(score, sizeof(score), "SCORE : %4d", score_);
	text_->Draw(50, 50, score);
}

void TargetObject::Release()
{
}

void TargetObject::OnCollision(const CollisionData& data)
{
	GameObject* other = data.other;
	Collider* col = data.otherCollider;

	std::string name = other->GetObjectName();

	if (name == "Ball")
	{
		OnCollisionBall(data);
	}
}

void TargetObject::OnCollisionBall(CollisionData data)
{
	XMFLOAT3 worldPos = this->GetWorldPosition();
	XMVECTOR diff = XMLoadFloat3(&worldPos) - XMLoadFloat3(&data.contactPoint);
	float distance = 0.0f;

	XMStoreFloat(&distance, XMVector3Length(diff));

	score_ = 100 - static_cast<int>(distance * 100);
}
