#include "Ball.h"
#include"../Engine/Model.h"
#include"MoveComponent.h"
#include"../Engine/SphereCollider.h"
#include"../Engine/BoxCollider.h"

Ball::Ball(GameObject* parent)
	:GameObject(parent,"Ball"),
	hModel_(-1),
	move_(nullptr),
	isColliding_(false)
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

	if (!isColliding_)move_->EnableGravity();
	isColliding_ = false;
}

void Ball::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Ball::Release()
{
}

void Ball::OnCollision(GameObject* other, Collider* col)
{
	std::string name = other->GetObjectName();
	isColliding_ = true;

	if (name == "Obstacle")
	{
		CollisionObstacle(other,col);
		move_->ResetGravity();
		return;
	}
	else if (name == "Fighter")
	{
		CollisionPlayer(other,col);
		return;
	}
	else if (name == "Ball")
	{
		CollisionBall(other, col);
		return;
	}

}

void Ball::SetMoveDirection(XMVECTOR direction)
{
	move_->SetDirection(direction);
	move_->ResetVelocity();
}

void Ball::CollisionObstacle(GameObject* obstacle, Collider* col)
{
	//現在の速度
	float speed = move_->GetSpeed();
	//もし速度が0以下だったら処理をスルー
	if (speed <= 0.0f)return;

	//ボックスの座標、サイズを取得
	XMFLOAT3 obsPos = obstacle->GetPosition();
 	XMFLOAT3 obsSize = col->GetSize();
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
	XMVECTOR reflectionVelocity = velocity - 2.0f * dotVal * normal;
	move_->SetDirection(reflectionVelocity);

	if (speed < 0.1f)
	{
		move_->SetSpeed(0.0f);
		move_->DisableGravity();
	}
	else
	{
		move_->SetSpeed(speed * RESTITUTION);

		//埋まりこみ防止
		XMVECTOR pushOutPos = XMLoadFloat3(&transform_.position_) + normal * 0.02f;
		XMStoreFloat3(&transform_.position_, pushOutPos);
	}
}

void Ball::CollisionPlayer(GameObject * player, Collider* col)
{

}

void Ball::CollisionBall(GameObject* ball, Collider* col)
{
	XMFLOAT3 ballAPos = transform_.position_;
	XMVECTOR sphereAPos = XMLoadFloat3(&ballAPos);

	XMFLOAT3 ballBPos = ball->GetPosition();
	XMVECTOR sphereBPos = XMLoadFloat3(&ballBPos);

	XMVECTOR normal = CalculateSphereCollisionNormal(sphereAPos, sphereBPos);
	XMVECTOR tangentLine = CalculateSphereCollisionTangentLine(normal);

	XMVECTOR directionA = XMVector3Normalize(move_->GetVelocity());
	MoveComponent* moveB = ball->GetMoveComponent();
	XMVECTOR directionB = XMVector3Normalize(moveB->GetVelocity());

	XMVECTOR nA = XMVector3Dot(directionA, normal);
	XMVECTOR nB = XMVector3Dot(directionB, normal);

	XMVECTOR tA = directionA - nA;
	XMVECTOR tB = directionB - nB;

	XMVECTOR direction = nB * normal + tA;
	move_->SetDirection(direction);

	float speed = moveB->GetSpeed();
	move_->SetSpeed(speed);
}

XMVECTOR Ball::CalculateBoxCollsionNormal(XMVECTOR spherePos, XMVECTOR boxCenter, XMVECTOR boxMin, XMVECTOR boxMax)
{
	//ボックス表面上の、球の中心の最近点を計算
	XMVECTOR closestPoint = XMVectorClamp(spherePos, boxMin, boxMax);

	//法線
	XMVECTOR normal = spherePos - closestPoint;

	return XMVector3Normalize(normal);
}

XMVECTOR Ball::CalculateSphereCollisionNormal(XMVECTOR sphereAPos, XMVECTOR sphereBPos)
{
	XMVECTOR direction = sphereAPos - sphereBPos;

	return XMVector3Normalize(direction);
}

XMVECTOR Ball::CalculateSphereCollisionTangentLine(XMVECTOR normal)
{
	XMVECTOR tangentLine = XMVector3Cross(normal, UP);

	return XMVector3Normalize(tangentLine);
}
