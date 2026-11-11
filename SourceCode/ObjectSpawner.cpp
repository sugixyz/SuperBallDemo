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
        Obstacle obs = Instantiate<Obstacle>(parent_);
        obs.SetPosition(data.position);
        obs.SetRotate(data.rotate);
        obs.SetScale(data.scale);

        obs.SetObject(data.position, data.scale);
    }

    return true;
}
