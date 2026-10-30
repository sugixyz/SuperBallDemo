#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include"CollisionFilter.h"

using namespace DirectX;

//クラスの前方宣言
class GameObject;

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

public:
	//コンストラクタ
	Collider();

	//デストラクタ
	virtual ~Collider();

	//テスト表示用の枠を描画
	//引数：position	オブジェクトの位置
	//引数：rotation	オブジェクトの回転
	void Draw(XMFLOAT3 position,XMFLOAT3 rotation);

	//セッター
	void SetGameObject(GameObject* gameObject) { pGameObject_ = gameObject; }

	GameObject* GetGameObject() { return pGameObject_; }
	XMFLOAT3 GetCenter() { return center_; }
	XMFLOAT3 GetSize() { return size_; }
	CollisionFilter GetCollisionFilter() { return filter_; }
	ColliderType GetColliderType() { return type_; }
};

