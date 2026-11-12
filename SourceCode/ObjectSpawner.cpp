#include "ObjectSpawner.h"
#include"ObjectLoader.h"
#include"Obstacle.h"
#include<string>

ObjectSpawner::ObjectSpawner()
    :parent_(nullptr)
{
}

ObjectSpawner::~ObjectSpawner()
{
}

bool ObjectSpawner::SpawnObject()
{
    std::string filename = "objectData.csv";
    std::vector<ObjectData> spawnDatas = ObjectLoader::GetInstance().Load(filename);
    
    for (auto data : spawnDatas)
    {
        Obstacle* obs = Instantiate<Obstacle>(parent_);
        Transform transform;
        transform.position_ = data.position;
        transform.rotate_ = data.rotate;
        transform.scale_ = data.scale;
        obs->SetObject(transform,data.isStatic);
    }

    return true;
}
