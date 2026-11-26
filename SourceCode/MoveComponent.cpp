#include "MoveComponent.h"
#include"../Engine/Timer.h"
#include"../Engine/Transform.h"

MoveComponent::MoveComponent()
    :speed_(0.0f),
    direction_(XMFLOAT3(0.0f,0.0f,0.0f)),
    velocity_(XMFLOAT3(0.0f,0.0f,0.0f)),
    useGravity_(false),
    isGrounded_(false)
{
}

MoveComponent::~MoveComponent()
{
}

XMFLOAT3 MoveComponent::Move(Transform ownerTransform)
{
    XMVECTOR direction = XMLoadFloat3(&direction_);

    //水平方向の移動速度
    XMVECTOR horizontalVelocity = direction * speed_;
    //もし速度が0以下ならゼロベクトルで上書き
    if (speed_ < 0.0f)horizontalVelocity = XMVectorZero();
    
    //垂直方向の移動速度
    float verticalVelocity = velocity_.y;
    if (useGravity_)
    {
        //重力を適応
        verticalVelocity += GRAVITY * Timer::GetDeltaTime();

        velocity_ = XMFLOAT3(
            XMVectorGetX(horizontalVelocity),
            verticalVelocity,
            XMVectorGetZ(horizontalVelocity)
        );
    }
    else if (isGrounded_)
    {
        verticalVelocity = 0.0f;

        velocity_ = XMFLOAT3(
            XMVectorGetX(horizontalVelocity),
            verticalVelocity,
            XMVectorGetZ(horizontalVelocity)
        );
    }
    else    
    {
        XMStoreFloat3(&velocity_, horizontalVelocity);
    }

    XMVECTOR velocity = XMLoadFloat3(&velocity_);

    //現在座標取得
    XMVECTOR pos = XMLoadFloat3(&ownerTransform.position_);
    pos += velocity * Timer::GetDeltaTime();
    XMFLOAT3 newPos;
    XMStoreFloat3(&newPos, pos);
    return newPos;
}

void MoveComponent::Jump(float jumpSpeed)
{
    //地面に触れていないなら抜ける
    if (!isGrounded_)return;

    velocity_.y = jumpSpeed;
    isGrounded_ = false;
    useGravity_ = true;
}

void MoveComponent::OnCelingHit()
{
    if (velocity_.y > 0.0f)
    {
        velocity_.y = 0.0f;
    }
}

void MoveComponent::SetGrounded(bool isGrounded)
{
    isGrounded_ = isGrounded;

    if (isGrounded_ && velocity_.y < 0.0f)
    {
        velocity_.y = 0.0f;
    }
}

void MoveComponent::ResetGravity()
{
    if (!useGravity_)return;

    ResetVelocity();
}

void MoveComponent::ResetVelocity()
{
    XMVECTOR direction = XMLoadFloat3(&direction_);
    XMVECTOR velocity = direction * speed_;

    XMStoreFloat3(&velocity_, velocity);
}
