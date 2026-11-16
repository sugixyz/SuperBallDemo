#pragma once
#include<string>
#include<DirectXMath.h>

using namespace DirectX;

struct ObjectData
{
	std::string name;
	std::string color;
	XMFLOAT3 position;
	XMFLOAT3 rotate;
	XMFLOAT3 scale;
	bool isStatic;
};