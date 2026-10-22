#include "MoveComponent.h"
#include"../Engine/Timer.h"
#include"../Engine/Transform.h"

MoveComponent::MoveComponent()
    :speed_(0.0f),
    direction_(XMVectorSet(0.0f,0.0f,0.0f,0.0f)),
    velocity_(XMVectorSet(0.0f,0.0f,0.0f,0.0f)),
    useGravity_(false)
{
}

MoveComponent::~MoveComponent()
{
}

XMFLOAT3 MoveComponent::Move(Transform ownerTransform)
{
    //現在座標取得
    XMVECTOR pos = XMLoadFloat3(&ownerTransform.position_);

    XMVECTOR moveDirection = XMVector3Normalize(direction_);
    velocity_ = moveDirection * speed_ * Timer::GetDeltaTime();
    
    pos += velocity_;
    XMFLOAT3 newPos;
    XMStoreFloat3(&newPos, pos);
    return newPos;
}

XMFLOAT3 MoveComponent::UpdatePhysics(Transform ownerTransform)
{
    //現在座標取得
    XMVECTOR pos = XMLoadFloat3(&ownerTransform.position_);

    if (useGravity_)
    {
        velocity_ += GRAVITY * Timer::GetDeltaTime();
    }

    pos += velocity_;
    XMFLOAT3 newPos;
    XMStoreFloat3(&newPos, pos);
    return newPos;
}
