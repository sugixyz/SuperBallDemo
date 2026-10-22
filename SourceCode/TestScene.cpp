#include"TestScene.h"
#include"../SourceCode/Fighter.h"
#include"../SourceCode/PlayerController.h"
#include"Obstacle.h"

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

	const int OBSTACLE_COUNT = 5;
	XMFLOAT3 positions[OBSTACLE_COUNT] =
	{
		{0,0,0},
		{-4,1,0},
		{4,1,0},
		{0,1,4},
		{0,1,-4}
	};
	XMFLOAT3 scales[OBSTACLE_COUNT]
	{
		{8,0.1f,8},
		{0.1f,4,8},
		{0.1f,4,8},
		{8,4,0.1f},
		{8,4,0.1f}
	};

	for (int i = 0;i < OBSTACLE_COUNT;i++)
	{
		Obstacle* obs = Instantiate<Obstacle>(this);
		obs->SetObject(positions[i], scales[i]);
	}
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
