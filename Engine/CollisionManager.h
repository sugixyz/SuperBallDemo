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

	//接触判定を行う
	bool IsHit(Collider* a, Collider* b);

	//箱型同士の衝突判定
	//引数：boxA	１つ目の箱型判定
	//引数：boxB	２つ目の箱型判定
	//戻値：接触していればtrue
	bool IsHitBoxVsBox(BoxCollider* boxA, BoxCollider* boxB);

	//箱型と球体の衝突判定
	//引数：box	箱型判定
	//引数：sphere	球体型判定
	//戻値：接触していればtrue
	bool IsHitBoxVsCircle(BoxCollider* box, SphereCollider* sphere);

	//球体同士の衝突判定
	//引数：circleA	１つ目の球体判定
	//引数：circleB	２つ目の球体判定
	//戻値：接触していればtrue
	bool IsHitCircleVsCircle(SphereCollider* circleA, SphereCollider* circleB);

	//OBBと球体の衝突判定
	//引数：obb	OBB判定
	//引数：sphere	球体型判定
	//戻値：接触していればtrue
	bool IsHitOBBVsCircle(OBBCollider* obb, SphereCollider* sphere);

	//OBBと箱型の衝突判定
	//引数：obb	OBB判定
	//引数：box	箱型判定
	//戻値：接触していればtrue
	bool IsHitOBBVsBox(OBBCollider* obb, BoxCollider* box);

	//OBB同士の衝突判定
	//引数：obbA	１つ目のOBB判定
	//引数：obbB	２つ目のOBB判定
	//戻値：接触していればtrue
	bool IsHitOBBVsOBB(OBBCollider* obbA, OBBCollider* obbB);
private:
	//コライダーマトリックス
	CollisionMatrix collisionMatrix_;
	//コライダーのリスト
	std::vector<Collider*> colliders_;
};