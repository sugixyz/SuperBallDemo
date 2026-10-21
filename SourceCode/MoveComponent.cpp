#include "MoveComponent.h"
#include"../Engine/Timer.h"
#include"../Engine/Transform.h"

MoveComponent::MoveComponent()
    :speed_(0.0f),
    direction_(0.0f,0.0f),
    velocity_(XMVectorSet(0.0f,0.0f,0.0f,0.0f))
{
}

MoveComponent::~MoveComponent()
{
}

XMFLOAT3 MoveComponent::Move(Transform ownerTransform)
{
    //ToDo : 最終的には「座標、方向、前方ベクトル」を受け取り、「移動先の座標」を返す

    //現在座標取得
    XMVECTOR pos = XMLoadFloat3(&ownerTransform.position_);
    //前方ベクトル、右ベクトルを計算
    XMVECTOR forward = CalculateForwardVector(ownerTransform.rotate_.y);
    XMVECTOR right = CalculateRightVector(forward);

    //速度を計算
    velocity_ = CalculateVelocity(forward, right);

    pos += velocity_;
    XMFLOAT3 newPos;
    XMStoreFloat3(&newPos, pos);
    return newPos;
}

XMVECTOR MoveComponent::CalculateVelocity(XMVECTOR forward, XMVECTOR right)
{
    //移動ベクトルを計算 
    XMVECTOR moveDirection = (forward * direction_.y) + (right * direction_.x);
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
