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

	const int OBSTACLE_COUNT = 7;
	XMFLOAT3 positions[OBSTACLE_COUNT] =
	{
		{0,0,0},
		{-8,2,0},
		{8,2,0},
		{0,2,8},
		{0,2,-8},
		{0,4,0},
		{0,1,4}
	};
	XMFLOAT3 scales[OBSTACLE_COUNT]
	{
		{16,0.5f,16},
		{0.5f,4,16},
		{0.5f,4,16},
		{16,4,0.5f},
		{16,4,0.5f},
		{16,0.5f,16},
		{2,3,2}
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
