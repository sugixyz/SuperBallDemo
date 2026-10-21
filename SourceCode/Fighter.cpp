#include "Fighter.h"
#include"../Engine/Model.h"
#include"Controller.h"
#include"../Engine/BoxCollider.h"
#include"HealthComponent.h"
#include"MoveComponent.h"

Fighter::Fighter(GameObject* parent)
	:GameObject(parent, "Fighter"),
	hModel_(-1),
	controller_(nullptr),
	health_(nullptr),
	move_(nullptr)
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

	//health_ = new HealthComponent();
	move_ = new MoveComponent();
	assert(move_ != nullptr);

	move_->SetSpeed(MOVE_SPEED);
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
	UpdateCamera(input);
	Move(input);
	Shoot(input);
}

void Fighter::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Fighter::Release()
{
	delete controller_;
	delete health_;
	delete move_;
}

void Fighter::Move(const FighterInput& input)
{
	move_->SetDirection(input.moveInput);
	transform_.position_ = move_->Move(transform_);
}

void Fighter::Shoot(const FighterInput& input)
{
}

void Fighter::UpdateCamera(const FighterInput& input)
{
	XMFLOAT2 cameraInput = input.cameraInput;
	if (cameraInput.x < 0)transform_.rotate_.y--;
	else if (cameraInput.x > 0)transform_.rotate_.y++;
}
