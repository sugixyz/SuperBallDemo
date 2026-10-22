#pragma once
#include"../Engine/GameObject.h"

class BoxCollider;

class Obstacle : public GameObject
{
public:
	Obstacle(GameObject* parent);
	~Obstacle() override;
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetObject(XMFLOAT3 position, XMFLOAT3 scale);
private:
	int hModel_;
	BoxCollider* col_;
};
