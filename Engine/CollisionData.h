#pragma once
#include<DirectXMath.h>

class GameObject;
class Collider;

using namespace DirectX;

struct CollisionData
{
	CollisionData()
		:isHit(false),
		normal(0.0f,0.0f,0.0f),
		penetrationDepth(0.0f),
		contactPoint(0.0f,0.0f,0.0f),
		other(nullptr),
		otherCollider(nullptr)
	{}

	//接触したかどうか
	bool isHit = false;
	//相手から自分への法線
	XMFLOAT3 normal = { 0.0f,0.0f,0.0f };
	//めり込み深度
	float penetrationDepth = 0.0f;
	//接触点
	XMFLOAT3 contactPoint = { 0.0f,0.0f,0.0f };

	//相手の情報
	GameObject* other = nullptr;
	Collider* otherCollider = nullptr;
};