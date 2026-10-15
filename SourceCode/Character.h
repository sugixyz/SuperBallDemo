#pragma once
#include"../Engine/GameObject.h"

//キャラクター用のインターフェース
class Character : public GameObject
{
public:
	virtual ~Character() = default;
	virtual void Update() override = 0;
	virtual void Draw() override = 0;

	void SetVelocity(XMFLOAT3 velocity) { velocity_ = velocity; }
	//XMFLOAT3 
protected:
	XMFLOAT3 velocity_;
};