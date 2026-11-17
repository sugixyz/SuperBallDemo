#include "ObjectPool.h"

ObjectPool::ObjectPool()
	:owner_(nullptr)
{
	pool_.clear();
}

ObjectPool::~ObjectPool()
{
	owner_ = nullptr;
	pool_.clear();
}

GameObject* ObjectPool::GetPoolObject()
{
	for (auto& obj : pool_)
	{
		//無効なオブジェクトを検索
		if (obj->IsDisable())
		{
			//オブジェクトの有効化
			obj->Enable();
			return obj;
		}
	}

	//見つからなければnullptrを返す
	return nullptr;
}

void ObjectPool::SetObject(GameObject* obj)
{
	//オブジェクトを無効化してプールに登録
	obj->Disable();
	pool_.push_back(obj);
}

void ObjectPool::Reset()
{
	for (auto& obj : pool_)
	{
		obj->Disable();
	}
}
