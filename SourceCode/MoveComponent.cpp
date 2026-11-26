#include "MoveComponent.h"
#include"../Engine/Timer.h"
#include"../Engine/Transform.h"

MoveComponent::MoveComponent()
    :speed_(0.0f),
    direction_(XMVectorSet(0.0f,0.0f,0.0f,0.0f)),
    velocity_(XMVectorSet(0.0f,0.0f,0.0f,0.0f)),
    useGravity_(false),
    isGrounded_(false)
{
}

MoveComponent::~MoveComponent()
{
}

XMFLOAT3 MoveComponent::Move(Transform ownerTransform)
{
    //水平方向の移動速度
    XMVECTOR horizontalVelocity = direction_ * speed_;
    //もし速度が0以下ならゼロベクトルで上書き
    if (speed_ < 0.0f)horizontalVelocity = XMVectorZero();
    
    //垂直方向の移動速度
    float verticalVelocity = XMVectorGetY(velocity_);
    if (useGravity_)
    {
        //重力を適応
        verticalVelocity += GRAVITY * Timer::GetDeltaTime();

        velocity_ = XMVectorSet(
            XMVectorGetX(horizontalVelocity),
            verticalVelocity,
            XMVectorGetZ(horizontalVelocity),
            0.0f
        );
    }
    else if (isGrounded_)
    {
        verticalVelocity = 0.0f;

        velocity_ = XMVectorSet(
            XMVectorGetX(horizontalVelocity),
            verticalVelocity,
            XMVectorGetZ(horizontalVelocity),
            0.0f
        );
    }
    else    
    {
        velocity_ = horizontalVelocity;
    }

    //現在座標取得
    XMVECTOR pos = XMLoadFloat3(&ownerTransform.position_);
    pos += velocity_ * Timer::GetDeltaTime();
    XMFLOAT3 newPos;
    XMStoreFloat3(&newPos, pos);
    return newPos;
}

void MoveComponent::Jump(float jumpSpeed)
{
    //地面に触れていないなら抜ける
    if (!isGrounded_)return;

    velocity_ = XMVectorSet(
        XMVectorGetX(velocity_),
        jumpSpeed,
        XMVectorGetZ(velocity_),
        0.0f
    );
    isGrounded_ = false;
    useGravity_ = true;
}

void MoveComponent::OnCelingHit()
{
    if (XMVectorGetY(velocity_) > 0.0f)
    {
        velocity_ = XMVectorSet(
            XMVectorGetX(velocity_),
            0.0f,
            XMVectorGetZ(velocity_),
            0.0f
        );
    }
}

void MoveComponent::SetGrounded(bool isGrounded)
{
    isGrounded_ = isGrounded;

    if (isGrounded_ && XMVectorGetY(velocity_) < 0.0f)
    {
        velocity_ = XMVectorSet(
            XMVectorGetX(velocity_),
            0.0f,
            XMVectorGetZ(velocity_),
            0.0f
        );
    }
}

void MoveComponent::ResetGravity()
{
    if (!useGravity_)return;

    velocity_ = direction_ * speed_;
}

void MoveComponent::ResetVelocity()
{
    velocity_ = direction_ * speed_;
}
