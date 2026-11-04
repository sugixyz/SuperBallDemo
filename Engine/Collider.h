#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include"CollisionFilter.h"

using namespace DirectX;

//クラスの前方宣言
class GameObject;
class Transform;

//あたり判定のタイプ
enum ColliderType
{
	COLLIDER_BOX,		//箱型
	COLLIDER_OBB,		//OBB
	COLLIDER_CIRCLE,	//球体
};

//-----------------------------------------------------------
//あたり判定を管理するクラス
//-----------------------------------------------------------
class Collider
{
protected:
	GameObject*		pGameObject_;	//この判定をつけたゲームオブジェクト
	ColliderType	type_;			//種類
	XMFLOAT3		center_;		//中心位置（ゲームオブジェクトの原点から見た位置）
	XMFLOAT3		size_;			//判定サイズ（幅、高さ、奥行き）
	int				hDebugModel_;	//デバッグ表示用のモデルのID
	CollisionFilter filter_;		//判定用のレイヤーフィルタ
	float			boundingRadius_;//外接球の半径

public:
	//コンストラクタ
	Collider();

	//デストラクタ
	virtual ~Collider();

	//テスト表示用の枠を描画
	//引数：transform	オブジェクトのトランスフォーム
	void Draw(Transform transform);

	//セッター
	void SetGameObject(GameObject* gameObject) { pGameObject_ = gameObject; }
	void SetSize(const XMFLOAT3& size) { size_ = size; boundingRadius_ = CalculateBoundingRadius(size); }

	//ゲッター
	GameObject* GetGameObject() { return pGameObject_; }
	XMFLOAT3 GetCenter() { return center_; }
	XMFLOAT3 GetSize() { return size_; }
	CollisionFilter GetCollisionFilter() { return filter_; }
	ColliderType GetColliderType() { return type_; }
	float GetBoundingRadius() { return boundingRadius_; }
private:
	//外接球の半径の２乗を計算する
	virtual float CalculateBoundingRadius(const XMFLOAT3& size) = 0;
};

