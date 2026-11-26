#include "ObjectSpawner.h"
#include"ObjectLoader.h"
#include"Obstacle.h"
#include"Balloon.h"
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
        if (data.name == "Ground")
        {
            Obstacle* obs = Instantiate<Obstacle>(parent_);
            obs->SetObject(data);
            obs->SetGround();
        }
        else if (data.name == "Obstacle")
        {
            Obstacle* obs = Instantiate<Obstacle>(parent_);
            obs->SetObject(data);
        }
        else if (data.name == "Balloon")
        {
            Balloon* bln = Instantiate<Balloon>(parent_);
            bln->SetObject(data);
        }
    }

    return true;
}
