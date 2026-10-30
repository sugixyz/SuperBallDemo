#pragma once
#include"CollisionLayer.h"

//クエリ用フィルタ
struct CollisionFilter
{
	/// <summary>
	/// フィルタのコンストラクタ
	/// </summary>
	CollisionFilter()
		:layer(CollisionLayer::DEFAULT),
		mask(0xFFFFFFFF)
	{}

	/// <summary>
	/// フィルタのコンストラクタ
	/// </summary>
	/// <param name="myLayer">自身を識別するレイヤー</param>
	CollisionFilter(const CollisionLayer& myLayer)
		:layer(myLayer),
		mask(0xFFFFFFFF)	
	{}

	/// <summary>
	/// フィルタのコンストラクタ
	/// </summary>
	/// <param name="myLayer">自身を識別するレイヤー</param>
	/// <param name="collisionMask">許可するレイヤーのビットマスク</param>
	CollisionFilter(const CollisionLayer& myLayer,const uint32_t& collisionMask)
		:layer(myLayer),
		mask(collisionMask)
	{}

	//自身のレイヤー
	CollisionLayer layer;
	//レイヤーのビットマスク
	uint32_t mask;
};