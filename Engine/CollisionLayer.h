#pragma once
#include<cstdint>

//接触管理用のレイヤー
enum class CollisionLayer : uint32_t
{
	DEFAULT = 0,
	FIGHTER,
	BALL,
	OBSTACLE,
	TARGET,
	SIZE
};

static inline uint32_t LayerBit(CollisionLayer layer)
{
	return 1u << static_cast<uint32_t>(layer);
}