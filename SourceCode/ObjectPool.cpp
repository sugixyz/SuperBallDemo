#include "ObjectPool.h"

ObjectPool::ObjectPool()
	:owner_(nullptr),
	lastAcquiredIndex_(-1)
{
	pool_.clear();
}

ObjectPool::~ObjectPool()
{
	owner_ = nullptr;
	pool_.clear();
}

GameObject* ObjectPool::Acquired()
{
	int index = 0;
	for (auto& obj : pool_)
	{
		//無効なオブジェクトを検索
		if (obj->IsDisable())
		{
			//オブジェクトの有効化
			obj->Enable();
			return obj;
			lastAcquiredIndex_ = index;
		}

		index++;
	}

	//見つからなければnullptrを返す
	return nullptr;
}

GameObject* ObjectPool::AcquiredNext()
{
	int index = lastAcquiredIndex_ + 1;
	if (index >= pool_.size())index = 0;

	GameObject* obj = pool_[index];
	//もし有効化されているなら一度無効化
	if (obj->IsEnable())obj->Disable();
	obj->Enable();
	lastAcquiredIndex_++;
	if (lastAcquiredIndex_ >= pool_.size())lastAcquiredIndex_ = 0;

	return obj;
}

void ObjectPool::Register(GameObject* obj)
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
