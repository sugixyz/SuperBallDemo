#include "BoxCollider.h"
#include "Model.h"


//コンストラクタ（当たり判定の作成）
//引数：basePos	当たり判定の中心位置（ゲームオブジェクトの原点から見た位置）
//引数：size	当たり判定のサイズ
//引数：filter	当たり判定用のフィルタ
BoxCollider::BoxCollider(XMFLOAT3 basePos, XMFLOAT3 size, CollisionFilter filter)
{
	center_ = basePos;
	SetSize(size);
	type_ = COLLIDER_BOX;
	filter_ = filter;

	//リリース時は判定枠は表示しない
#ifdef _DEBUG
	//テスト表示用判定枠
	hDebugModel_ = Model::Load("DebugCollision/boxCollider.fbx");
#endif
}

float BoxCollider::CalculateBoundingRadius(const XMFLOAT3& size)
{
	//各軸のサイズから対角線の半分の長さを計算
	return sqrtf((size.x * size.x + size.y * size.y + size.z * size.z) / 4.0f);
}
