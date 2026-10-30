#include "OBBCollider.h"
#include "Model.h"


//コンストラクタ（当たり判定の作成）
//引数：basePos	当たり判定の中心位置（ゲームオブジェクトの原点から見た位置）
//引数：size	当たり判定のサイズ
//引数：filter	当たり判定用のフィルタ
OBBCollider::OBBCollider(XMFLOAT3 basePos, XMFLOAT3 size, CollisionFilter filter)
{
	center_ = basePos;
	size_ = size;
	type_ = COLLIDER_OBB;
	filter_ = filter;

	//リリース時は判定枠は表示しない
#ifdef _DEBUG
	//テスト表示用判定枠
	hDebugModel_ = Model::Load("DebugCollision/boxCollider.fbx");
#endif
}