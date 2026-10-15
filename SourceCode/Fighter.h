#pragma once
#include"Character.h"

class Fighter : public Character
{
public:
	Fighter();
	Fighter(XMFLOAT3);
	~Fighter();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//操作用関数
	void OnAttack();
	void OnRun();
	void OnCrouch();
	void OnReadyWeapon();
};