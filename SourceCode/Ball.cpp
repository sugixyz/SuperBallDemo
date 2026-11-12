#include "Ball.h"
#include"../Engine/Model.h"
#include"MoveComponent.h"
#include"HealthComponent.h"
#include"../Engine/SphereCollider.h"

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

	CollisionFilter filter = CollisionFilter(CollisionLayer::BALL);
	SphereCollider* col = new SphereCollider(COL_POS, COL_RADIUS,filter);
	AddCollider(col);

	move_ = new MoveComponent();
	assert(move_ != nullptr);

	SetDynamic();

	move_->SetSpeed(MOVE_SPEED);
	move_->EnableGravity();
}	

void Ball::Update()
{
	transform_.position_ = move_->Move(transform_);

	//何にも当たっていなかければ自動敵に重力を有効化
	if (!isColliding_)move_->EnableGravity();
	else move_->DisableGravity();

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

void Ball::OnCollision(const CollisionData& data)
{
	GameObject* other = data.other;
	Collider* col = data.otherCollider;

	std::string name = other->GetObjectName();
	isColliding_ = true;

	if (name == "Obstacle")
	{
		CollisionObstacle(data);
		move_->ResetGravity();
		return;
	}
	else if (name == "Fighter")
	{
		//CollisionPlayer(other,col);
		isColliding_ = false;
		return;
	}
	else if (name == "Ball")
	{
		CollisionBall(data);
		return;
	}

}

void Ball::SetMoveDirection(XMVECTOR direction)
{
	move_->SetDirection(direction);
	move_->ResetVelocity();
}

void Ball::CollisionObstacle(CollisionData data)
{

	//現在の速度
	float speed = move_->GetSpeed();
	//もし速度が0以下だったら処理をスルー
	if (speed <= 0.0f)return;
 
	//法線を取得
	XMVECTOR normal = XMLoadFloat3(&data.normal);
	XMVECTOR pushVec = normal * data.penetrationDepth;
	XMVECTOR newPos = XMLoadFloat3(&transform_.position_) + pushVec;
	XMStoreFloat3(&transform_.position_, newPos);

	XMVECTOR velocity = move_->GetVelocity();

	//反射速度の計算
	XMVECTOR dotVal = XMVector3Dot(velocity, normal);
	XMVECTOR reflectionVelocity = velocity - 2.0f * dotVal * normal;
	move_->SetDirection(reflectionVelocity);

	if (speed < 0.05f)
	{
		move_->SetSpeed(0.0f);
		move_->DisableGravity();
	}
	else
	{
		move_->SetSpeed(speed * RESTITUTION);
	}
}

void Ball::CollisionPlayer(GameObject * player, Collider* col)
{
	HealthComponent* health = player->GetHealthComponent();
	if (health != nullptr)
	{
		health->TakeDamage(DAMAGE);
	}

	KillMe();
}

void Ball::CollisionBall(CollisionData data)
{
	GameObject* other = data.other;

	XMVECTOR normal = XMLoadFloat3(&data.normal);
	XMVECTOR tangentLine = CalculateSphereCollisionTangentLine(normal);

	XMVECTOR directionA = XMVector3Normalize(move_->GetVelocity());
	MoveComponent* moveB = other->GetMoveComponent();
	if (moveB == nullptr)return;
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
