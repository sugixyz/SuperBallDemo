#pragma once
#include "Collider.h"


//-----------------------------------------------------------
//球体の当たり判定
//-----------------------------------------------------------
class SphereCollider : public Collider
{
public:
	//コンストラクタ（当たり判定の作成）
	//引数：basePos	当たり判定の中心位置（ゲームオブジェクトの原点から見た位置）
	//引数：radius	当たり判定のサイズ（半径）
	//引数：filter	当たり判定用のフィルタ
	SphereCollider(XMFLOAT3 center, float radius, CollisionFilter filter = CollisionFilter());
};

