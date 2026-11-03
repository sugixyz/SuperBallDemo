#pragma once
#include"CollisionMatrix.h"
#include<vector>

class CollisionFilter;
class Collider;
class BoxCollider;
class SphereCollider;
class OBBCollider;

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
	bool IsHit(Collider* a, Collider* b);

	//箱型同士の衝突判定
	//引数：boxA	１つ目の箱型判定
	//引数：boxB	２つ目の箱型判定
	//戻値：接触していればtrue
	bool IsHitBoxVsBox(Collider* boxA, Collider* boxB);

	//箱型と球体の衝突判定
	//引数：box	箱型判定
	//引数：sphere	球体型判定
	//戻値：接触していればtrue
	bool IsHitBoxVsSphere(Collider* box, Collider* sphere);

	//球体同士の衝突判定
	//引数：sphereA	１つ目の球体判定
	//引数：sphereB	２つ目の球体判定
	//戻値：接触していればtrue
	bool IsHitSphereVsSphere(Collider* sphereA, Collider* sphereB);

	//OBBと球体の衝突判定
	//引数：obb	OBB判定
	//引数：sphere	球体型判定
	//戻値：接触していればtrue
	bool IsHitOBBVsSphere(Collider* obb, Collider* sphere);

	//OBBと箱型の衝突判定
	//引数：obb	OBB判定
	//引数：box	箱型判定
	//戻値：接触していればtrue
	bool IsHitOBBVsBox(Collider* obb, Collider* box);

	//OBB同士の衝突判定
	//引数：obbA	１つ目のOBB判定
	//引数：obbB	２つ目のOBB判定
	//戻値：接触していればtrue
	bool IsHitOBBVsOBB(Collider* obbA, Collider* obbB);
private:
	//コライダーマトリックス
	CollisionMatrix collisionMatrix_;
	//コライダーのリスト
	std::vector<Collider*> colliders_;
};