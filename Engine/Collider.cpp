#include "GameObject.h"
#include "Model.h"
#include "Transform.h"

//コンストラクタ
Collider::Collider():
	pGameObject_(nullptr)
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

void Collider::UpdateWorldCenter()
{
	XMMATRIX mWorld = pGameObject_->GetWorldMatrix();
	XMFLOAT3 objPos = pGameObject_->GetWorldPosition();
	XMVECTOR pos = XMLoadFloat3(&objPos);
	XMVECTOR localCenter = XMLoadFloat3(&center_);

	XMVECTOR worldCenter = pos + XMVector3TransformNormal(localCenter, mWorld);
	XMStoreFloat3(&worldCenter_, worldCenter);
}
