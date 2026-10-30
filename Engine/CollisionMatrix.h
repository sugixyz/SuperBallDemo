#pragma once
#include"CollisionLayer.h"

class CollisionMatrix
{
public:
	CollisionMatrix();

	/// <summary>
	/// レイヤー同士の接触関係を設定する
	/// </summary>
	/// <param name="layerA">レイヤー１</param>
	/// <param name="layerB">レイヤー２</param>
	/// <param name="enable">接触可能かどうか</param>
	void SetCollision(CollisionLayer layerA, CollisionLayer layerB, bool enable);

	/// <summary>
	/// マトリックスの初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 接触可能かどうかを判定する
	/// </summary>
	/// <param name="layerA">レイヤー１</param>
	/// <param name="layerB">レイヤー２</param>
	/// <returns>接触可能だったらtrueを返す</returns>
	bool ShouldCollide(CollisionLayer layerA, CollisionLayer layerB);
private:
	//レイヤー毎のマスクを持つ表
	uint32_t layerMasks_[static_cast<int>(CollisionLayer::SIZE)] = {};
};