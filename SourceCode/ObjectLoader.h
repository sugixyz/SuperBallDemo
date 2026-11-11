#pragma once
#include<string>
#include<vector>
#include<DirectXMath.h>

using namespace DirectX;

struct ObjectData
{
	std::string modelName;
	XMFLOAT3 position;
	XMFLOAT3 rotate;
	XMFLOAT3 scale;
	bool isStatic;
};

class ObjectLoader
{
public:
	static ObjectLoader& GetInstance()
	{
		static ObjectLoader instance;
		return instance;
	}
	//コピー、移動を禁止
	ObjectLoader(const ObjectLoader&) = delete;
	ObjectLoader& operator=(const ObjectLoader&) = delete;
	ObjectLoader(ObjectLoader&&) = delete;
	ObjectLoader& operator=(ObjectLoader&&) = delete;

	~ObjectLoader();

	std::vector<ObjectData> Load(std::string filename);
private:
	ObjectLoader();
};