#pragma once
#include"CollisionManager.h"

namespace Collision
{
	inline void Initialize() { CollisionManager::GetInstance().Initialize(); }
	inline void UpdateCollisions() { CollisionManager::GetInstance().UpdateCollisions(); }
	inline void DrawColliders() { CollisionManager::GetInstance().DrawColliders(); }
	inline void ReleaseColliiders() { CollisionManager::GetInstance().ReleaseColliders(); }
	inline void AddCollider(Collider* col) { CollisionManager::GetInstance().AddCollider(col); }
	inline void ClearCollider(Collider* col) { CollisionManager::GetInstance().ClearCollider(col); }
}