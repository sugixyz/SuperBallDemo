#pragma once
#include"CollisionMatrix.h"
#include"CollisionData.h"
#include<vector>

class CollisionFilter;
class Collider;

class CollisionManager
{
public:
	/// <summary>
	/// インスタンス取得用
	/// </summary>
	/// <returns>CollisionManagerのインスタンス</returns>
	static CollisionManager& GetInstance()
	{
		static CollisionManager instance;
		return instance;
	}
	//コピー、移動を禁止
	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator=(const CollisionManager&) = delete;
	CollisionManager(CollisionManager&&) = delete;
	CollisionManager& operator=(CollisionManager&&) = delete;

	/// <summary>
	/// マネージャーの初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// コライダーを追加する
	/// </summary>
	/// <param name="col">追加するコライダー</param>
	void AddCollider(Collider* col);
	/// <summary>
	/// コライダーを削除する
	/// </summary>
	/// <param name="col">削除するコライダー</param>
	void ClearCollider(Collider* col);

	/// <summary>
	/// 全コライダーの接触判定を行う
	/// </summary>
	void UpdateCollisions();
	/// <summary>
	/// 全コライダーの描画を行う
	/// </summary>
	void DrawColliders();
	/// <summary>
	/// 全コライダーの開放を行う
	/// </summary>
	void ReleaseColliders();

private:
	//コンストラクタ
	CollisionManager();

	//ブロードフェーズ
	bool IsHitBroadPhase(Collider* a, Collider* b);

	//大雑把な衝突チェック
	bool CheckCollisionRough(Collider* a, Collider* b);

	//接触判定を行う
	CollisionData IsHit(Collider* a, Collider* b);

	//箱型同士の衝突判定
	//引数：boxA	１つ目の箱型判定
	//引数：boxB	２つ目の箱型判定
	void IsHitBoxVsBox(Collider* boxA, Collider* boxB,CollisionData& data);

	//箱型と球体の衝突判定
	//引数：box	箱型判定
	//引数：sphere	球体型判定
	void IsHitBoxVsSphere(Collider* box, Collider* sphere, CollisionData& data);

	//球体同士の衝突判定
	//引数：sphereA	１つ目の球体判定
	//引数：sphereB	２つ目の球体判定
	void IsHitSphereVsSphere(Collider* sphereA, Collider* sphereB, CollisionData& data);

	//OBBと球体の衝突判定
	//引数：obb	OBB判定
	//引数：sphere	球体型判定
	void IsHitOBBVsSphere(Collider* obb, Collider* sphere, CollisionData& data);

	//OBBと箱型の衝突判定
	//引数：obb	OBB判定
	//引数：box	箱型判定
	void IsHitOBBVsBox(Collider* obb, Collider* box, CollisionData& data);

	//OBB同士の衝突判定
	//引数：obbA	１つ目のOBB判定
	//引数：obbB	２つ目のOBB判定
	void IsHitOBBVsOBB(Collider* obbA, Collider* obbB, CollisionData& data);
private:
	//コライダーマトリックス
	CollisionMatrix collisionMatrix_;
	//コライダーのリスト
	std::vector<Collider*> colliders_;
};