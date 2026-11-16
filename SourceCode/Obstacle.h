#pragma once
#include"../Engine/GameObject.h"
#include"ObjectData.h"

class Obstacle : public GameObject
{
public:
	Obstacle(GameObject* parent);
	~Obstacle() override;
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void OnCollision(const CollisionData& data);

	/// <summary>
	/// オブジェクトの設定
	/// </summary>
	/// <param name="data">オブジェクトの初期化用データ</param>
	void SetObject(const ObjectData& data);
private:
	int hModel_;
private:
	static constexpr XMFLOAT3 COL_OFFSET = { 0.0f,0.0f,0.0f };
};
