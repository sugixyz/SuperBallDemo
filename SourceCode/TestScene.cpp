#include"TestScene.h"
#include"Fighter.h"
#include"PlayerController.h"
#include"AutoController.h"
#include"TargetObject.h"
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

	Fighter* ai = Instantiate<Fighter>(this);
	ai->SetController(new AutoController());


	TargetObject* target = Instantiate<TargetObject>(this);
	target->SetPosition(XMFLOAT3(0.0f, 2.0f, 3.49f));

	const int OBSTACLE_COUNT = 6;
	XMFLOAT3 positions[OBSTACLE_COUNT] =
	{
		{0,0,0},
		{-8,2,0},
		{8,2,0},
		{0,2,8},
		{0,2,-8},
		{0,2.0f,4}
	};
	XMFLOAT3 scales[OBSTACLE_COUNT]
	{
		{16,1,16},
		{1,4,16},
		{1,4,16},
		{16,4,1},
		{16,4,1},
		{1,3,1}
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
