#include "GameObject.h"
#include "Model.h"
#include "Transform.h"

//コンストラクタ
Collider::Collider() :
	pGameObject_(nullptr),
	type_(COLLIDER_CIRCLE),
	center_(0.0f, 0.0f, 0.0f),
	size_(0.0f, 0.0f, 0.0f),
	hDebugModel_(-1),
	filter_(),
	boundingRadius_(0.0f),
	worldCenter_(0.0f, 0.0f, 0.0f),
	isStatic(false)
{
}

//デストラクタ
Collider::~Collider()
{
}

//テスト表示用の枠を描画
//引数：position	オブジェクトの位置
//引数：rotation	オブジェクトの回転
void Collider::Draw(Transform transform)
{
	transform.position_ = XMFLOAT3(transform.position_.x + center_.x, transform.position_.y + center_.y, transform.position_.z + center_.z);
	transform.scale_ = size_;
	transform.Calclation();
	Model::SetTransform(hDebugModel_, transform);
	Model::Draw(hDebugModel_);
}

void Collider::InitializeWorldCenter()
{
	//静的かどうか関係なく計算
	worldCenter_ = CalculateWorldCenter();
}

void Collider::UpdateWorldCenter()
{
	//静的オブジェクトなら再計算しない
	if (isStatic)return;

	worldCenter_ = CalculateWorldCenter();
}

XMFLOAT3 Collider::CalculateWorldCenter()
{
	XMMATRIX mWorld = pGameObject_->GetWorldMatrix();
	XMFLOAT3 objPos = pGameObject_->GetWorldPosition();
	XMVECTOR pos = XMLoadFloat3(&objPos);
	XMVECTOR localCenter = XMLoadFloat3(&center_);

	XMVECTOR worldCenter = pos + XMVector3TransformNormal(localCenter, mWorld);
	XMFLOAT3 result;
	XMStoreFloat3(&result, worldCenter);

	return result;
}
