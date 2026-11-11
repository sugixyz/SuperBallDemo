#pragma once

class GameObject;

class ObjectSpawner
{
public:
	ObjectSpawner();
	~ObjectSpawner();

	bool SpawnObject();
	void SetObjectParent(GameObject* parent) { parent_ = parent; }
private:
	GameObject* parent_;
};