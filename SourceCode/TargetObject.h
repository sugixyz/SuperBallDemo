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
	void OnCollision(GameObject* other, Collider* col) override;
private:
	int hModel_;
	Text* text_;
private:
	static constexpr XMFLOAT3 COL_SIZE = { 1.0f,1.0f,0.1f };
};