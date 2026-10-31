#pragma once
#include "Collider.h"


//-----------------------------------------------------------
//OBBの当たり判定
//-----------------------------------------------------------
class OBBCollider : public Collider
{
public:
	//コンストラクタ（当たり判定の作成）
	//引数：basePos	当たり判定の中心位置（ゲームオブジェクトの原点から見た位置）
	//引数：size	当たり判定のサイズ（幅、高さ、奥行き）
	//引数：filter	当たり判定用のフィルタ
	OBBCollider(XMFLOAT3 basePos, XMFLOAT3 size,CollisionFilter filter = CollisionFilter());
private:
	float CalculateBoundingRadius(const XMFLOAT3& size) override;
};

