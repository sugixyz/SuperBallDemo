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
	hModel_ = Model::Load("BulletDemo.fbx");
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
	std::string name = other->GetObjectName();

	if (name == "Obstacle")
	{
		CollisionObstacle(other);
	}
	else if (name == "Player")
	{
		CollisionPlayer(other);
	}
}

void Ball::SetMoveDirection(XMVECTOR direction)
{
	move_->SetDirection(direction);
}

void Ball::CollisionObstacle(GameObject* obstacle)
{
	//ボックスの座標、サイズを取得
	XMFLOAT3 obsPos = obstacle->GetPosition();
	XMFLOAT3 obsSize = obstacle->GetScale();
	XMVECTOR boxPos = XMLoadFloat3(&obsPos);
	XMVECTOR boxSize = XMLoadFloat3(&obsSize);
	XMVECTOR boxMin = boxPos - (boxSize / 2.0f);
	XMVECTOR boxMax = boxPos + (boxSize / 2.0f);

	XMVECTOR pos = XMLoadFloat3(&transform_.position_);

	//法線を取得
	XMVECTOR normal = CalculateBoxCollsionNormal(pos, boxPos, boxMin, boxMax);
	XMVECTOR velocity = move_->GetVelocity();

	//反射速度の計算
	XMVECTOR dotVal = XMVector3Dot(velocity, normal);
	XMVECTOR reflectionVelocity = velocity - (1.0f + RESTITUTION) * dotVal * normal;

	XMVECTOR speedVec = XMVector3Length(reflectionVelocity);
	float speed;
	XMStoreFloat(&speed, speedVec);

	move_->SetDirection(reflectionVelocity);
	move_->SetSpeed(speed);

	//埋まりこみ防止
	XMVECTOR pushOutPos = XMLoadFloat3(&transform_.position_) + normal * 0.05f;
	XMStoreFloat3(&transform_.position_, pushOutPos);
}

void Ball::CollisionPlayer(GameObject * player)
{

}

XMVECTOR Ball::CalculateBoxCollsionNormal(XMVECTOR spherePos, XMVECTOR boxCenter, XMVECTOR boxMin, XMVECTOR boxMax)
{
	//ボックス表面上の、球の中心の最近点を計算
	XMVECTOR colosestPoint = XMVectorClamp(spherePos, boxMin, boxMax);

	//法線
	XMVECTOR normal = spherePos - colosestPoint;

	return XMVector3Normalize(normal);
}

XMVECTOR Ball::CalculateSphereCollisionNormal(XMVECTOR sphereAPos, XMVECTOR sphereBPos)
{
	XMVECTOR direction = sphereBPos - sphereAPos;

	return XMVector3Normalize(direction);
}
