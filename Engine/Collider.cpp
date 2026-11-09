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
	GameObject* pGameObject_;	//この判定をつけたゲームオブジェクト
	ColliderType	type_;			//種類
	XMFLOAT3		center_;		//中心位置（ゲームオブジェクトの原点から見た位置）
	XMFLOAT3		size_;			//判定サイズ（幅、高さ、奥行き）
	int				hDebugModel_;	//デバッグ表示用のモデルのID
	CollisionFilter filter_;		//判定用のレイヤーフィルタ
	float			boundingRadius_;//外接球の半径
	XMFLOAT3		worldCenter_;	//オフセットを加味したワールド座標
	bool			isMoving_;		//動くオブジェクトかどうか
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
	//動かないオブジェクトなら計算しない
	if (isStatic)return;

	XMMATRIX mWorld = pGameObject_->GetWorldMatrix();
	XMFLOAT3 objPos = pGameObject_->GetWorldPosition();
	XMVECTOR pos = XMLoadFloat3(&objPos);
	XMVECTOR localCenter = XMLoadFloat3(&center_);

	XMVECTOR worldCenter = pos + XMVector3TransformNormal(localCenter, mWorld);
	XMStoreFloat3(&worldCenter_, worldCenter);
}
