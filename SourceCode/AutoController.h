#pragma once
#include"Controller.h"

class AutoController : public Controller
{
public:
	AutoController();
	~AutoController() override;

	void Update() override;
};