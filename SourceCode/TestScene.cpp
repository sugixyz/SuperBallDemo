#include"TestScene.h"
#include"Fighter.h"
#include"PlayerController.h"
#include"AutoController.h"
#include"ObjectSpawner.h"

//コンストラクタ
TestScene::TestScene(GameObject * parent)
	: GameObject(parent, "TestScene")
{
}

//初期化
void TestScene::Initialize()
{
	Fighter* fighter = Instantiate<Fighter>(this);
	fighter->SetController(new PlayerController());

	//Fighter* ai = Instantiate<Fighter>(this);
	//ai->SetController(new AutoController());

	ObjectSpawner spawner = ObjectSpawner();
	spawner.SetObjectParent(this);
	spawner.SpawnObject();
}

//更新
void TestScene::Update()
{
}

//描画
void TestScene::Draw()
{
}

//開放
void TestScene::Release()
{
}
