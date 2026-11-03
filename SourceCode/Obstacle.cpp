#include "Obstacle.h"
#include"../Engine/BoxCollider.h"
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

void Obstacle::OnCollision(GameObject* other,Collider* col)
{
	BoxCollider* bCol = dynamic_cast<BoxCollider*>(col);

	std::string name = other->GetObjectName();
	if (name == "Ball")
	{
		CollisionBall(other, col);
	}
	else if (name == "Fighter")
	{
		CollisionPlayer(other, col);
	}
}
void Obstacle::SetObject(XMFLOAT3 position, XMFLOAT3 scale)
{
	transform_.position_ = position;
	transform_.scale_ = scale;

	BoxCollider* col = new BoxCollider(XMFLOAT3(0.0f,0.0f,0.0f), transform_.scale_);
	AddCollider(col);
}

void Obstacle::CollisionBall(GameObject* ball, Collider* col)
{
	//ボックスの座標、サイズを取得
	XMFLOAT3 obsPos = transform_.position_;
	XMFLOAT3 obsSize = transform_.scale_;
	XMVECTOR boxPos = XMLoadFloat3(&obsPos);
	XMVECTOR boxSize = XMLoadFloat3(&obsSize);
	XMVECTOR boxMin = boxPos - (boxSize / 2.0f);
	XMVECTOR boxMax = boxPos + (boxSize / 2.0f);

	XMFLOAT3 sPos = ball->GetPosition();
	float radius = col->GetSize().x;
	XMVECTOR spherePos = XMLoadFloat3(&sPos);

	XMVECTOR closestPoint = XMVectorClamp(spherePos, boxMin, boxMax);
	XMVECTOR diff = spherePos - closestPoint;
	float distance = 0.0f;

	XMStoreFloat(&distance, XMVector3Length(diff));

	if (distance < radius)
	{
		XMVECTOR normal = XMVector3Normalize(diff);
		float penertrationDepth = 0.0f;
		
		if (distance > 0.0001f)
		{
			normal = XMVector3Normalize(diff);
			penertrationDepth = radius - distance;
		}
		else
		{
			XMVECTOR boxCenterToSpehre = spherePos - boxPos;

			XMVECTOR distToMin = spherePos - boxMin;
			XMVECTOR distToMax = spherePos - boxMax;

			normal = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			penertrationDepth = radius + 0.1f;
		}

		spherePos += normal * penertrationDepth;

		XMFLOAT3 newPos;
		XMStoreFloat3(&newPos, spherePos);
		ball->SetPosition(newPos);
	}
}

void Obstacle::CollisionPlayer(GameObject* player, Collider* col)
{
	//障害物側の座標、サイズ
	XMVECTOR obsPos = XMLoadFloat3(&transform_.position_);
	XMVECTOR obsSize = XMLoadFloat3(&transform_.scale_);
	
	//プレイヤーの座標、サイズ
	XMFLOAT3 pos = player->GetPosition();
	XMFLOAT3 colCenter = col->GetCenter();
	XMFLOAT3 colSize = col->GetSize();
	XMVECTOR otherCenter = XMLoadFloat3(&colCenter);
	XMVECTOR playerPos = XMLoadFloat3(&pos);
	XMVECTOR otherPos = playerPos + otherCenter;
	XMVECTOR otherSize = XMLoadFloat3(&colSize);
	
	XMVECTOR diff = otherPos - obsPos;
	XMVECTOR overlap = (obsSize / 2.0f + otherSize / 2.0f) - XMVectorAbs(diff);

	float overlapX = XMVectorGetX(overlap);
	float overlapY = XMVectorGetY(overlap);
	float overlapZ = XMVectorGetZ(overlap);

	if (overlapX <= 0.0f)return;
	if (overlapY <= 0.0f)return;
	if (overlapZ <= 0.0f)return;

	XMVECTOR pushVector = XMVectorZero();
	float dir = 0.0f;

	if (overlapX < overlapY && overlapX < overlapZ)
	{
		dir = (XMVectorGetX(diff) < 0.0f) ? -1.0f : 1.0f;
		pushVector = XMVectorSet(overlapX * dir, 0.0f, 0.0f, 0.0f);
	}
	else if (overlapY < overlapZ)
	{
		dir = (XMVectorGetY(diff) < 0.0f) ? -1.0f : 1.0f;
		pushVector = XMVectorSet(0.0f, overlapY * dir, 0.0f, 0.0f);
	}
	else
	{
		dir = (XMVectorGetZ(diff) < 0.0f) ? -1.0f : 1.0f;
		pushVector = XMVectorSet(0.0f, 0.0f, overlapZ * dir, 0.0f);
	}

	playerPos += pushVector;
	XMFLOAT3 newPos;
	XMStoreFloat3(&newPos, playerPos);
	player->SetPosition(newPos);
}
