#include "CollisionManager.h"
#include"CollisionFilter.h"
#include"Collider.h"
#include"BoxCollider.h"
#include"SphereCollider.h"
#include"OBBCollider.h"
#include"GameObject.h"
#include"Direct3D.h"
#include"Global.h"

void CollisionManager::Initialize()
{
    //マトリックスの初期化
    collisionMatrix_.Initialize();
    colliders_.clear();
}

void CollisionManager::AddCollider(Collider* col)
{
    colliders_.push_back(col);
}

void CollisionManager::ClearCollider(Collider * col)
{
    auto it = std::remove(colliders_.begin(), colliders_.end(), col);
    colliders_.erase(it, colliders_.end());
}

void CollisionManager::UpdateCollisions()
{
    //全組み合わせを検索
    for (int i = 0; i < colliders_.size(); i++)
    {
        //コライダーを取得
        Collider* colA = colliders_[i];
        for (int j = i + 1; j < colliders_.size(); j++)
        {
            //コライダーを取得
            Collider* colB = colliders_[j];

            CollisionFilter filterA = colA->GetCollisionFilter();
            CollisionFilter filterB = colB->GetCollisionFilter();

            //グローバルマトリックスのチェック
            if (collisionMatrix_.ShouldCollide(filterA.layer, filterB.layer) == false)
            {
                continue;
            }

            //個別のマスクのチェック
            if ((filterA.mask & LayerBit(filterB.layer)) == 0)continue;
            if ((filterB.mask & LayerBit(filterA.layer)) == 0)continue;

            //接触判定
            if (IsHit(colA, colB))
            {
                //各オブジェクトのコールバック
                GameObject* objA = colA->GetGameObject();
                GameObject* objB = colB->GetGameObject();
                objA->OnCollision(objB, colB);
                objB->OnCollision(objA, colA);
            }
        }
    }
}

void CollisionManager::DrawColliders()
{
    //リリース時は削除
#ifdef _DEBUG
        //コリジョンの描画
    if (Direct3D::isDrawCollision_ == false)return;

    Direct3D::SetShader(Direct3D::SHADER_UNLIT);
    
    //全コライダーをチェック
    for (Collider* col : colliders_)
    {
        GameObject* obj = col->GetGameObject();
        col->Draw(obj->GetWorldPosition(), obj->GetWorldRotate());
    }

    Direct3D::SetShader(Direct3D::SHADER_3D);

#endif
}

void CollisionManager::ReleaseColliders()
{
    for (Collider* col : colliders_)
    {
        //安全に削除
        SAFE_DELETE(col);
    }

    colliders_.clear();
}

CollisionManager::CollisionManager()
{
    collisionMatrix_ = CollisionMatrix();
    colliders_.clear();
}

bool CollisionManager::IsHit(Collider* a, Collider* b)
{
    ColliderType typeA = a->GetColliderType();
    ColliderType typeB = b->GetColliderType();

    if (typeA == COLLIDER_BOX)
    {
        if (typeB == COLLIDER_BOX)return IsHitBoxVsBox((BoxCollider*)a, (BoxCollider*)b);
        else if (typeB == COLLIDER_CIRCLE)return IsHitBoxVsCircle((BoxCollider*)a, (SphereCollider*)b);
        else return IsHitOBBVsBox((OBBCollider*)b, (BoxCollider*)a);
    }
    else if (typeA == COLLIDER_CIRCLE)
    {
        if (typeB == COLLIDER_BOX)return IsHitBoxVsCircle((BoxCollider*)b, (SphereCollider*)a);
        else if (typeB == COLLIDER_CIRCLE)return IsHitCircleVsCircle((SphereCollider*)a, (SphereCollider*)b);
        else return IsHitOBBVsCircle((OBBCollider*)b, (SphereCollider*)a);
    }
    else
    {
        if (typeB == COLLIDER_BOX)return IsHitOBBVsBox((OBBCollider*)a, (BoxCollider*)b);
        else if (typeB == COLLIDER_CIRCLE)return IsHitOBBVsCircle((OBBCollider*)a, (SphereCollider*)b);
        else return IsHitOBBVsOBB((OBBCollider*)a, (OBBCollider*)b);
    }

    return false;
}

bool CollisionManager::IsHitBoxVsBox(BoxCollider* boxA, BoxCollider* boxB)
{

    XMFLOAT3 boxPosA = Transform::Float3Add(boxA->GetGameObject()->GetWorldPosition(), boxA->GetCenter());
    XMFLOAT3 boxPosB = Transform::Float3Add(boxB->GetGameObject()->GetWorldPosition(), boxB->GetCenter());
    XMFLOAT3 boxSizeA = boxA->GetSize();
    XMFLOAT3 boxSizeB = boxB->GetSize();

    if ((boxPosA.x + boxSizeA.x / 2) > (boxPosB.x - boxSizeB.x / 2) &&
        (boxPosA.x - boxSizeA.x / 2) < (boxPosB.x + boxSizeB.x / 2) &&
        (boxPosA.y + boxSizeA.y / 2) > (boxPosB.y - boxSizeB.y / 2) &&
        (boxPosA.y - boxSizeA.y / 2) < (boxPosB.y + boxSizeB.y / 2) &&
        (boxPosA.z + boxSizeA.z / 2) > (boxPosB.z - boxSizeB.z / 2) &&
        (boxPosA.z - boxSizeA.z / 2) < (boxPosB.z + boxSizeB.z / 2))
    {
        return true;
    }
    return false;
}

bool CollisionManager::IsHitBoxVsCircle(BoxCollider* box, SphereCollider* sphere)
{
    XMFLOAT3 circlePos = Transform::Float3Add(sphere->GetGameObject()->GetWorldPosition(), sphere->GetCenter());
    XMFLOAT3 boxPos = Transform::Float3Add(box->GetGameObject()->GetWorldPosition(), box->GetCenter());
    XMFLOAT3 circleSize = sphere->GetSize();
    XMFLOAT3 boxSize = box->GetSize();

    if (circlePos.x > boxPos.x - boxSize.x / 1.7f - circleSize.x &&
        circlePos.x < boxPos.x + boxSize.x / 1.7f + circleSize.x &&
        circlePos.y > boxPos.y - boxSize.y / 1.7f - circleSize.x &&
        circlePos.y < boxPos.y + boxSize.y / 1.7f + circleSize.x &&
        circlePos.z > boxPos.z - boxSize.z / 1.7f - circleSize.x &&
        circlePos.z < boxPos.z + boxSize.z / 1.7f + circleSize.x)
    {
        return true;
    }

    return false;
}

bool CollisionManager::IsHitCircleVsCircle(SphereCollider* circleA, SphereCollider* circleB)
{
    XMFLOAT3 centerA = circleA->GetCenter();
    XMFLOAT3 positionA = circleA->GetGameObject()->GetWorldPosition();
    XMFLOAT3 centerB = circleB->GetCenter();
    XMFLOAT3 positionB = circleB->GetGameObject()->GetWorldPosition();

    XMVECTOR v = (XMLoadFloat3(&centerA) + XMLoadFloat3(&positionA))
        - (XMLoadFloat3(&centerB) + XMLoadFloat3(&positionB));

    if (XMVector3Length(v).m128_f32[0] <= circleA->GetSize().x + circleB->GetSize().x)
    {
        return true;
    }

    return false;
}

bool CollisionManager::IsHitOBBVsCircle(OBBCollider* obb, SphereCollider* sphere)
{
    return false;
}

bool CollisionManager::IsHitOBBVsBox(OBBCollider* obb, BoxCollider* box)
{
    return false;
}

bool CollisionManager::IsHitOBBVsOBB(OBBCollider* obbA, OBBCollider* obbB)
{
    return false;
}
