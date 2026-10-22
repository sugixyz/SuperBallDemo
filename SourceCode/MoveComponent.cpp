#include "MoveComponent.h"
#include"../Engine/Timer.h"
#include"../Engine/Transform.h"

MoveComponent::MoveComponent()
    :speed_(0.0f),
    direction2D_(0.0f,0.0f),
    direction3D_(XMVectorSet(0.0f,0.0f,0.0f,0.0f)),
    isSetDirection3D_(false),
    velocity_(XMVectorSet(0.0f,0.0f,0.0f,0.0f))
{
}

MoveComponent::~MoveComponent()
{
}

XMFLOAT3 MoveComponent::Move(Transform ownerTransform)
{
    //現在座標取得
    XMVECTOR pos = XMLoadFloat3(&ownerTransform.position_);

    if (isSetDirection3D_)
    {
        XMVECTOR moveDirection = XMVector3Normalize(direction3D_);
        velocity_ = moveDirection * speed_ * Timer::GetDeltaTime();
    }
    else
    {
        //前方ベクトル、右ベクトルを計算
        XMVECTOR forward = CalculateForwardVector(ownerTransform.rotate_.y);
        XMVECTOR right = CalculateRightVector(forward);
        velocity_ = CalculateVelocity(forward, right);
    }

    pos += velocity_;
    XMFLOAT3 newPos;
    XMStoreFloat3(&newPos, pos);
    return newPos;
}

XMVECTOR MoveComponent::CalculateVelocity(XMVECTOR forward, XMVECTOR right)
{
    //移動ベクトルを計算 
    XMVECTOR moveDirection = (forward * direction2D_.y) + (right * direction2D_.x);
    moveDirection = XMVector3Normalize(moveDirection);

    //速度を適応
    return moveDirection * speed_ * Timer::GetDeltaTime();
}

XMVECTOR MoveComponent::CalculateForwardVector(float rotationY)
{
    float rad = XMConvertToRadians(rotationY);
    float x = sinf(rad);
    float y = 0.0f;
    float z = cosf(rad);
    float w = 0.0f;
    XMVECTOR forward = XMVectorSet(x, y, z, w);
    forward = XMVector3Normalize(forward);

    return forward;
}

XMVECTOR MoveComponent::CalculateRightVector(XMVECTOR forward)
{
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMVECTOR right = XMVector3Cross(up, forward);
    right = XMVector3Normalize(right);

    return right;
}
