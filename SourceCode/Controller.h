#pragma once
#include"FighterInput.h"

class Controller
{
public:
	virtual ~Controller() = default;

	virtual void Update() = 0;
	FighterInput GetInput() { return fighterInput_; }
protected:
	FighterInput fighterInput_;
};