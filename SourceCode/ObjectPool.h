#pragma once
#include"../Engine/GameObject.h"
#include<vector>

class ObjectPool
{
public:
	ObjectPool();
	~ObjectPool();

	/// <summary>
	/// プールの初期化
	/// </summary>
	/// <typeparam name="T">作成したいオブジェクトの型</typeparam>
	/// <param name="owner">プールの持ち主</param>
	/// <param name="objCount">事前に作成するオブジェクトの数</param>
	template <class T>
	void InitializePool(GameObject* owner, int objCount);

	/// <summary>
	/// オブジェクトをプールから取得
	/// </summary>
	/// <returns>プール内の使用されていないオブジェクト</returns>
	GameObject* Acquired();

	/// <summary>
	/// 次のインデックスのオブジェクトをプールから取得
	/// </summary>
	/// <returns>前回取り出したものの次のオブジェクト</returns>
	GameObject* AcquiredNext();

	/// <summary>
	/// オブジェクトをプールに登録
	/// </summary>
	/// <param name="obj">登録するオブジェクト</param>
	void Register(GameObject* obj);

	/// <summary>
	/// プールの中身をリセットする
	/// </summary>
	void Reset();

private:
	GameObject* owner_;
	std::vector<GameObject*> pool_;
	//最後に取り出したオブジェクトのインデックス
	int lastAcquiredIndex_;
};

template<class T>
inline void ObjectPool::InitializePool(GameObject* owner, int objCount)
{
	GameObject* testScene = owner->FindObject("TestScene");
	for (int i = 0;i < objCount;i++)
	{
		GameObject* obj = Instantiate<T>(testScene);
		Register(obj);
	}
	owner_ = owner;
}