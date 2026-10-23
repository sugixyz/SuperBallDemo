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
	//Model::Draw(hModel_);
}

void Obstacle::Release()
{
}

void Obstacle::OnCollision(GameObject* other,Collider* col)
{
	//BoxCollider* bCol = dynamic_cast<BoxCollider*>(col);
	//if (bCol != nullptr)
	//{
	//	CollisionBox(other, bCol);
	//	return;
	//}

	//SphereCollider* sCol = dynamic_cast<SphereCollider*>(col);
	//if (sCol != nullptr)
	//{
	//	CollisionSphere(other, sCol);
	//	return;
	//}

	return;

}
void Obstacle::SetObject(XMFLOAT3 position, XMFLOAT3 scale)
{
	transform_.position_ = position;
	transform_.scale_ = scale;

	BoxCollider* col = new BoxCollider(XMFLOAT3(0.0f,0.0f,0.0f), transform_.scale_);
	AddCollider(col);
}

void Obstacle::CollisionSphere(GameObject* other, SphereCollider* col)
{
	//ボックスの座標、サイズを取得
	XMFLOAT3 obsPos = transform_.position_;
	XMFLOAT3 obsSize = transform_.scale_;
	XMVECTOR boxPos = XMLoadFloat3(&obsPos);
	XMVECTOR boxSize = XMLoadFloat3(&obsSize);
	XMVECTOR boxMin = boxPos - (boxSize / 2.0f);
	XMVECTOR boxMax = boxPos + (boxSize / 2.0f);

	XMFLOAT3 sPos = other->GetPosition();
	float radius = col->GetSize().x;
	XMVECTOR spherePos = XMLoadFloat3(&sPos);

	XMVECTOR closestPoint = XMVectorClamp(spherePos, boxMin, boxMax);
	XMVECTOR diff = spherePos - closestPoint;
	float distance = 0.0f;

	XMStoreFloat(&distance, XMVector3Length(diff));

	if (distance < radius)
	{
		XMVECTOR normal = XMVector3Normalize(diff);
		float penertrationDepth = radius - distance;

		spherePos += normal * penertrationDepth;

		XMFLOAT3 newPos;
		XMStoreFloat3(&newPos, spherePos);
		other->SetPosition(newPos);
	}
}

void Obstacle::CollisionBox(GameObject * other, BoxCollider * col)
{}
