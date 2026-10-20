#pragma once
#include"../Engine/GameObject.h"

class Gun : public GameObject
{
public:
	Gun(GameObject* parent);
	~Gun() override;
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;
};