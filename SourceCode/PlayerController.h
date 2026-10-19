#pragma once
#include"Controller.h"

class PlayerController : public Controller
{
public:
	PlayerController();
	~PlayerController() override;

	void Update() override;

};