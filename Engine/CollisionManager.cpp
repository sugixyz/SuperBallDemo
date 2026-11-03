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
            if (collisionMatrix_.ShouldCollide(filterA.layer, filterB.layer) == false)continue;

            //個別のマスクのチェック
            if ((filterA.mask & LayerBit(filterB.layer)) == 0)continue;
            if ((filterB.mask & LayerBit(filterA.layer)) == 0)continue;

            //ブロードフェーズ
            if (IsHitBroadPhase(colA, colB) == false)continue;

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

bool CollisionManager::IsHitBroadPhase(Collider* a, Collider* b)
{
    //それぞれの最大半径を取得
    float boundingRadiusA = a->GetBoundingRadius();
    float boundingRadiusB = b->GetBoundingRadius();
    float boundingRadius = boundingRadiusA + boundingRadiusB;

    //２つの距離の２乗を計算
    XMFLOAT3 aPos = a->GetGameObject()->GetWorldPosition();
    XMFLOAT3 bPos = b->GetGameObject()->GetWorldPosition();
    XMVECTOR diff = XMLoadFloat3(&aPos) - XMLoadFloat3(&bPos);
    float distSq;
    XMStoreFloat(&distSq, XMVector3LengthSq(diff));

    //距離の２乗が半径の２乗より小さければtrue
    return (distSq < boundingRadius * boundingRadius);
}

bool CollisionManager::IsHit(Collider* a, Collider* b)
{
    ColliderType typeA = a->GetColliderType();
    ColliderType typeB = b->GetColliderType();

    if (typeA == COLLIDER_BOX)
    {
        if (typeB == COLLIDER_BOX)return IsHitBoxVsBox(a, b);
        else if (typeB == COLLIDER_CIRCLE)return IsHitBoxVsSphere(a, b);
        else return IsHitOBBVsBox(b, a);
    }
    else if (typeA == COLLIDER_CIRCLE)
    {
        if (typeB == COLLIDER_BOX)return IsHitBoxVsSphere(b, a);
        else if (typeB == COLLIDER_CIRCLE)return IsHitSphereVsSphere(a, b);
        else return IsHitOBBVsSphere(b, a);
    }
    else
    {
        if (typeB == COLLIDER_BOX)return IsHitOBBVsBox(a, b);
        else if (typeB == COLLIDER_CIRCLE)return IsHitOBBVsSphere(a, b);
        else return IsHitOBBVsOBB(a, b);
    }

    return false;
}

bool CollisionManager::IsHitBoxVsBox(Collider* boxA, Collider* boxB)
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

bool CollisionManager::IsHitBoxVsSphere(Collider* box, Collider* sphere)
{
    XMFLOAT3 spherePos = Transform::Float3Add(sphere->GetGameObject()->GetWorldPosition(), sphere->GetCenter());
    XMFLOAT3 boxPos = Transform::Float3Add(box->GetGameObject()->GetWorldPosition(), box->GetCenter());
    XMFLOAT3 sphereSize = sphere->GetSize();
    XMFLOAT3 boxSize = box->GetSize();

    if (spherePos.x > boxPos.x - boxSize.x / 1.7f - sphereSize.x &&
        spherePos.x < boxPos.x + boxSize.x / 1.7f + sphereSize.x &&
        spherePos.y > boxPos.y - boxSize.y / 1.7f - sphereSize.x &&
        spherePos.y < boxPos.y + boxSize.y / 1.7f + sphereSize.x &&
        spherePos.z > boxPos.z - boxSize.z / 1.7f - sphereSize.x &&
        spherePos.z < boxPos.z + boxSize.z / 1.7f + sphereSize.x)
    {
        return true;
    }

    return false;
}

bool CollisionManager::IsHitSphereVsSphere(Collider* sphereA, Collider* sphereB)
{
    //XMFLOAT3 centerA = sphereA->GetCenter();
    //XMFLOAT3 positionA = sphereA->GetGameObject()->GetWorldPosition();
    //XMFLOAT3 centerB = sphereB->GetCenter();
    //XMFLOAT3 positionB = sphereB->GetGameObject()->GetWorldPosition();

    //XMVECTOR v = (XMLoadFloat3(&centerA) + XMLoadFloat3(&positionA))
    //    - (XMLoadFloat3(&centerB) + XMLoadFloat3(&positionB));

    //if (XMVector3Length(v).m128_f32[0] <= sphereA->GetSize().x + sphereB->GetSize().x)
    //{
    //    return true;
    //}

    //return false;

    //事前に計算済みなためここに入った時点でtrueを返す
    return true;
}

bool CollisionManager::IsHitOBBVsSphere(Collider* obb, Collider* sphere)
{
    //OBBのローカル座標変換用の行列を取得
    GameObject* obbObj = obb->GetGameObject();
    XMMATRIX mWorld = obbObj->GetWorldMatrix();
    XMMATRIX mInvWorld = XMMatrixInverse(nullptr, mWorld);

    //球の座標をOBBのローカル座標に変換
    XMFLOAT3 spherePos = Transform::Float3Add(sphere->GetGameObject()->GetWorldPosition(), sphere->GetCenter());
    XMVECTOR sphereWorldPos = XMLoadFloat3(&spherePos);
    XMVECTOR sphereLocalPos = XMVector3TransformCoord(sphereWorldPos, mInvWorld);

    XMFLOAT3 obbSize = obb->GetSize();
    XMVECTOR halfSize = XMLoadFloat3(&obbSize) * 0.7f;
    XMVECTOR obbMin = -halfSize;
    XMVECTOR obbMax = halfSize;

    float radius = sphere->GetSize().x;

    //最近点を計算
    XMVECTOR closestPoint = XMVectorClamp(sphereLocalPos, obbMin, obbMax);

    XMVECTOR diff = sphereLocalPos - closestPoint;
    float distanceSq = 0.0f;
    XMStoreFloat(&distanceSq, XMVector3LengthSq(diff));

    if (distanceSq <= (radius * radius))
    {
        return true;
    }

    return false;
}

bool CollisionManager::IsHitOBBVsBox(Collider* obb, Collider* box)
{
    return IsHitOBBVsOBB(obb, box);
}

bool CollisionManager::IsHitOBBVsOBB(Collider* obbA, Collider* obbB)
{
    GameObject* obbObjA = obbA->GetGameObject();
    GameObject* obbObjB = obbB->GetGameObject();

    XMMATRIX mWorldA = obbObjA->GetWorldMatrix();
    XMMATRIX mWorldB = obbObjB->GetWorldMatrix();

    XMFLOAT3 obbPosA = Transform::Float3Add(obbObjA->GetWorldPosition(), obbA->GetCenter());
    XMFLOAT3 obbPosB = Transform::Float3Add(obbObjB->GetWorldPosition(), obbB->GetCenter());

    XMVECTOR cA = XMLoadFloat3(&obbPosA);
    XMVECTOR cB = XMLoadFloat3(&obbPosB);

    XMFLOAT3 sizeA = obbA->GetSize();
    XMFLOAT3 sizeB = obbB->GetSize();
    XMVECTOR eA = XMLoadFloat3(&sizeA) * 0.5f;
    XMVECTOR eB = XMLoadFloat3(&sizeB) * 0.5f;

    XMVECTOR uA[3] =
    {
        XMVector3Normalize(mWorldA.r[0]),
        XMVector3Normalize(mWorldA.r[1]),
        XMVector3Normalize(mWorldA.r[2])
    };
    XMVECTOR uB[3]
    {
        XMVector3Normalize(mWorldB.r[0]),
        XMVector3Normalize(mWorldB.r[1]),
        XMVector3Normalize(mWorldB.r[2])
    };

    XMVECTOR T = cB - cA;

    float R[3][3];
    float RA[3];
    float RB[3];

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            XMStoreFloat(&R[i][j], XMVector3Dot(uA[i], uB[j]));
        }
    }

    float tA[3];
    for (int i = 0; i < 3; i++)
    {
        XMStoreFloat(&tA[i], XMVector3Dot(T, uA[i]));
    }

    //15軸でテスト
    for (int i = 0; i < 3; i++)
    {
        float tB = tA[0] * R[0][i] + tA[1] * R[1][i] + tA[2] * R[2][i];

        float rA = XMVectorGetByIndex(eA, i);
        float rB =
            XMVectorGetX(eB) * fabsf(R[i][0]) +
            XMVectorGetY(eB) * fabsf(R[i][1]) +
            XMVectorGetZ(eB) * fabsf(R[i][2]);

        if (fabsf(tA[i]) > rA + rB)return false;
    }

    //ゼロ除算を防ぐ為の値
    const float EPSILON = 1e-5f;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            float rA =
                XMVectorGetByIndex(eA, (i + 1) % 3) * fabsf(R[(i + 2) % 3][j]) +
                XMVectorGetByIndex(eA, (i + 2) % 3) * fabsf(R[(i + 1) % 3][j]);

            float rB =
                XMVectorGetByIndex(eB, (j + 1) % 3) * fabsf(R[i][(j + 2) % 3]) +
                XMVectorGetByIndex(eB, (j + 2) % 3) * fabsf(R[i][(j + 1) % 3]);

            float t = fabsf(tA[(i + 2) % 3] * R[(i + 1) % 3][j] - tA[(i + 1) % 3] * R[(i + 2) % 3][j]);

            float det = 1.0f - fabsf(R[i][j]);
            if (det > EPSILON)
            {
                if (t > rA + rB)return false;
            }
        }
    }

    return true;
}
