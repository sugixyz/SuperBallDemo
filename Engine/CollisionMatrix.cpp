#include "CollisionMatrix.h"

CollisionMatrix::CollisionMatrix()
{
	for (int i = 0; i < static_cast<int>(CollisionLayer::SIZE); i++)
	{
		//デフォルトでは全レイヤー衝突不可に設定
		layerMasks_[i] = 0x00000000;
	}
}

void CollisionMatrix::SetCollision(CollisionLayer layerA, CollisionLayer layerB, bool enable)
{
	if (enable)
	{
		//接触可能にする
		layerMasks_[static_cast<int>(layerA)] |= LayerBit(layerB);
		layerMasks_[static_cast<int>(layerB)] |= LayerBit(layerA);
	}
	else
	{
		//接触不可にする
		layerMasks_[static_cast<int>(layerA)] &= ~LayerBit(layerB);
		layerMasks_[static_cast<int>(layerB)] &= ~LayerBit(layerA);
	}
}

void CollisionMatrix::Initialize()
{
	SetCollision(CollisionLayer::DEFAULT, CollisionLayer::DEFAULT, true);
	SetCollision(CollisionLayer::FIGHTER, CollisionLayer::DEFAULT, true);
}

bool CollisionMatrix::ShouldCollide(CollisionLayer layerA, CollisionLayer layerB)
{
	return (layerMasks_[static_cast<int>(layerA)] & LayerBit(layerB)) != 0;
}