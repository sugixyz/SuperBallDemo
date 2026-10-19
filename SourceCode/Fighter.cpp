#include "Fighter.h"
#include"../Engine/Model.h"
#include"Controller.h"
#include"../Engine/BoxCollider.h"

Fighter::Fighter(GameObject* parent)
	:GameObject(parent, "Fighter"),
	hModel_(-1),
	velocity_(0.0f, 0.0f, 0.0f),
	controller_(nullptr)
{}

Fighter::~Fighter()
{
}

void Fighter::Initialize()
{
	hModel_ = Model::Load("PlayerDemo.fbx");
	assert(hModel_ >= 0);

	BoxCollider* col = new BoxCollider(COL_POS, COL_SIZE);
	AddCollider(col);
}

void Fighter::Update()
{
	//コントローラーのポインタがなければスルー
	if (controller_ == nullptr)return;

	//コントローラーのアップデート
	controller_->Update();
	//入力情報を取得
	FighterInput input = controller_->GetInput();

	//入力情報をもとに処理を実行
	Move(input);
	Shoot(input);
	UpdateCamera(input);
}

void Fighter::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Fighter::Release()
{
	delete controller_;
	controller_ = nullptr;
}

void Fighter::Move(const FighterInput& input)
{
	velocity_ = XMFLOAT3(input.moveInput.x, 0.0f, input.moveInput.y);

	transform_.position_.x += velocity_.x;
	transform_.position_.y += velocity_.y;
	transform_.position_.z += velocity_.z;
}

void Fighter::Shoot(const FighterInput& input)
{
}

void Fighter::UpdateCamera(const FighterInput& input)
{

}
