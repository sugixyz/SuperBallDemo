#pragma once
#include"../Engine/GameObject.h"

class Text;

class TargetObject : public GameObject
{
public:
	TargetObject(GameObject* parent);
	~TargetObject() override;
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void OnCollision(const CollisionData& data) override;
private:
	void OnCollisionBall(CollisionData data);
private:
	int hModel_;
	Text* text_;
	int score_;
private:
	static constexpr XMFLOAT3 COL_SIZE = { 1.0f,0.1f,1.0f };
};