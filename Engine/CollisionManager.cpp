#include "CollisionManager.h"
#include"CollisionFilter.h"
#include"Collider.h"
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
    col->InitializeWorldCenter();
}

void CollisionManager::ClearCollider(Collider * col)
{
    auto it = std::remove(colliders_.begin(), colliders_.end(), col);
    colliders_.erase(it, colliders_.end());
}

void CollisionManager::UpdateCollisions()
{
    //全コライダーの中心座標を更新
    for (auto col : colliders_)col->UpdateWorldCenter();

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
            CollisionData data = IsHit(colA, colB);
            if (data.isHit)
            {
                //各オブジェクトのコールバック
                GameObject* objA = colA->GetGameObject();
                GameObject* objB = colB->GetGameObject();

                CollisionData dataForA = data;
                CollisionData dataForB = data;

                dataForA.other = objB;
                dataForA.otherCollider = colB;

                dataForB.normal = XMFLOAT3(-data.normal.x, -data.normal.y, -data.normal.z);
                dataForB.other = objA;
                dataForB.otherCollider = colA;

                objA->OnCollision(dataForA);
                objB->OnCollision(dataForB);
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
        col->Draw(obj->GetTransform());
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

    XMFLOAT3 worldCenterA = a->GetWorldCenter();
    XMFLOAT3 worldCenterB = b->GetWorldCenter();
    XMVECTOR posA = XMLoadFloat3(&worldCenterA);
    XMVECTOR posB = XMLoadFloat3(&worldCenterB);

    XMVECTOR diff = posA - posB;
    float distSq;
    XMStoreFloat(&distSq, XMVector3LengthSq(diff));

    //距離の２乗が半径の２乗より小さければtrue
    return (distSq < boundingRadius * boundingRadius);
}

CollisionData CollisionManager::IsHit(Collider* a, Collider* b)
{
    ColliderType typeA = a->GetColliderType();
    ColliderType typeB = b->GetColliderType();

    CollisionData data;

    if (typeA == COLLIDER_BOX)
    {
        if (typeB == COLLIDER_BOX)IsHitBoxVsBox(a, b,data);
        else if (typeB == COLLIDER_CIRCLE)IsHitBoxVsSphere(a, b, data);
        else
        { IsHitOBBVsBox(b, a, data); data.normal = XMFLOAT3(-data.normal.x, -data.normal.y, -data.normal.z); }
    }
    else if (typeA == COLLIDER_CIRCLE)
    {
        if (typeB == COLLIDER_BOX) { IsHitBoxVsSphere(b, a, data); data.normal = XMFLOAT3(-data.normal.x, -data.normal.y, -data.normal.z); }
        else if (typeB == COLLIDER_CIRCLE)IsHitSphereVsSphere(a, b, data);
        else { IsHitOBBVsSphere(b, a, data); data.normal = XMFLOAT3(-data.normal.x, -data.normal.y, -data.normal.z); }
    }
    else
    {
        if (typeB == COLLIDER_BOX)IsHitOBBVsBox(a, b,data);
        else if (typeB == COLLIDER_CIRCLE)IsHitOBBVsSphere(a, b, data);
        else IsHitOBBVsOBB(a, b, data);
    }

    return data;
}

void CollisionManager::IsHitBoxVsBox(Collider* boxA, Collider* boxB, CollisionData& data)
{
    XMFLOAT3 boxPosA = boxA->GetWorldCenter();
    XMFLOAT3 boxPosB = boxB->GetWorldCenter();
    XMFLOAT3 boxSizeA = boxA->GetSize();
    XMFLOAT3 boxSizeB = boxB->GetSize();

    XMVECTOR cA = XMLoadFloat3(&boxPosA);
    XMVECTOR cB = XMLoadFloat3(&boxPosB);
    XMVECTOR eA = XMLoadFloat3(&boxSizeA);
    XMVECTOR eB = XMLoadFloat3(&boxSizeB);

    XMVECTOR diff = cB - cA;
    XMVECTOR absDiff = XMVectorAbs(diff);
    XMVECTOR minDist = eA + eB;

    //重なりチェック
    XMVECTOR overlapVec = minDist - absDiff;
    XMFLOAT3 overlap;
    XMStoreFloat3(&overlap, overlapVec);

    if (overlap.x <= 0.0f)return;
    if (overlap.y <= 0.0f)return;
    if (overlap.z <= 0.0f)return;

    data.isHit = true;

    //最も重なりが小さい軸を法線・深度として設定
    float minOverlap = overlap.x;
    XMVECTOR normal = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    if (overlap.y < minOverlap)
    {
        minOverlap = overlap.y;
        normal = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
        if (XMVectorGetY(diff) < 0.0f)normal = -normal;
    }
    if (overlap.z < minOverlap)
    {
        minOverlap = overlap.z;
        normal = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
        if (XMVectorGetZ(diff) < 0.0f)normal = -normal;
    }

    data.penetrationDepth = minOverlap;
    XMStoreFloat3(&data.normal, normal);

    //接触点は一旦2つの中間地点を設定
    XMVECTOR contact = cA + diff * 0.5f;
    XMStoreFloat3(&data.contactPoint, contact);
}

void CollisionManager::IsHitBoxVsSphere(Collider* box, Collider* sphere, CollisionData& data)
{
    XMFLOAT3 spherePos = sphere->GetWorldCenter();
    XMFLOAT3 boxPos = box->GetWorldCenter();
    
    float radius = sphere->GetSize().x;
    XMFLOAT3 boxSize = box->GetSize();

    XMVECTOR cBox = XMLoadFloat3(&boxPos);
    XMVECTOR wideHalfSize = XMLoadFloat3(&boxSize) / 1.7f;

    XMVECTOR wideMin = cBox - wideHalfSize;
    XMVECTOR wideMax = cBox + wideHalfSize;

    XMVECTOR cSphere = XMLoadFloat3(&spherePos);
    
    XMVECTOR wideClosestPoint = XMVectorClamp(cSphere, wideMin, wideMax);

    XMVECTOR wideDiff = cSphere - wideClosestPoint;
    float wideDistanceSq = 0.0f;
    XMStoreFloat(&wideDistanceSq, XMVector3LengthSq(wideDiff));

    if (wideDistanceSq <= (radius * radius))
    {
        data.isHit = true;

        XMVECTOR trueHalfSize = XMLoadFloat3(&boxSize) * 0.5f;
        XMVECTOR trueMin = cBox - trueHalfSize;
        XMVECTOR trueMax = cBox + trueHalfSize;

        XMVECTOR trueClosestPoint = XMVectorClamp(cSphere, trueMin, trueMax);
        XMVECTOR trueDiff = cSphere - trueClosestPoint;
        float trueDistSq = 0.0f;
        XMStoreFloat(&trueDistSq, XMVector3LengthSq(trueDiff));
        float distance = sqrtf(trueDistSq);

        XMVECTOR normalVec = XMVectorZero();
        if (distance > 0.0001f)
        {
            normalVec = XMVector3Normalize(trueDiff);
            data.penetrationDepth = radius - distance;
        }
        else
        {
            normalVec = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
            data.penetrationDepth = radius;
        }

        normalVec = XMVector3Normalize(trueDiff);
        data.penetrationDepth = radius - distance;

        XMStoreFloat3(&data.normal, normalVec);
        XMStoreFloat3(&data.contactPoint, trueClosestPoint);
    }
}

void CollisionManager::IsHitSphereVsSphere(Collider* sphereA, Collider* sphereB, CollisionData& data)
{
    XMFLOAT3 spherePosA = sphereA->GetWorldCenter();
    XMFLOAT3 spherePosB = sphereB->GetWorldCenter();

    XMVECTOR cA = XMLoadFloat3(&spherePosA);
    XMVECTOR cB = XMLoadFloat3(&spherePosB);

    XMVECTOR diff = cB - cA;
    XMVECTOR distVec = XMVector3Length(diff);
    float distance = XMVectorGetX(distVec);

    float radiusA = sphereA->GetSize().x;
    float radiusB = sphereB->GetSize().x;
    float totalRadius = radiusA + radiusB;

    if (distance <= totalRadius)
    {
        data.isHit = true;

        //めり込み深度
        data.penetrationDepth = totalRadius - distance;

        //AからBに向かう法線
        if (distance > 0.0001f)
        {
            XMVECTOR normalVec = diff / distance;
            XMStoreFloat3(&data.normal, normalVec);
        }
        else
        {
            data.normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
        }

        XMVECTOR contactVec = cA + (diff * (radiusA / totalRadius));
        XMStoreFloat3(&data.contactPoint, contactVec);
    }
}

void CollisionManager::IsHitOBBVsSphere(Collider* obb, Collider* sphere, CollisionData& data)
{
    // OBBのローカル座標変換用の行列を取得
    GameObject* obbObj = obb->GetGameObject();
    XMMATRIX mWorld = obbObj->GetWorldMatrix();

    XMVECTOR axisX = XMVector3Normalize(mWorld.r[0]);
    XMVECTOR axisY = XMVector3Normalize(mWorld.r[1]);
    XMVECTOR axisZ = XMVector3Normalize(mWorld.r[2]);

    XMMATRIX mRotOnly = XMMATRIX(
        axisX,
        axisY,
        axisZ,
        mWorld.r[3] // 平行移動成分はそのまま
    );

    // スケールを除いた逆行列を作る
    XMMATRIX mInvRot = XMMatrixInverse(nullptr, mRotOnly);
    //XMMATRIX mInvWorld = XMMatrixInverse(nullptr, mWorld);


    // 球の座標をOBBのローカル座標に変換
    XMFLOAT3 spherePos = sphere->GetWorldCenter();
    XMVECTOR sphereWorldPos = XMLoadFloat3(&spherePos);
    XMVECTOR sphereLocalPos = XMVector3TransformCoord(sphereWorldPos, mInvRot);

    // コライダーの中心を基準にした正しいローカル座標に補正する
    XMFLOAT3 obbCenter = obb->GetCenter();
    XMVECTOR center = XMLoadFloat3(&obbCenter);
    sphereLocalPos = sphereLocalPos - center;

    XMFLOAT3 obbSize = obb->GetSize();
    //XMVECTOR vScale = XMVectorSet(
    //    XMVectorGetX(XMVector3Length(mWorld.r[0])),
    //    XMVectorGetX(XMVector3Length(mWorld.r[1])),
    //    XMVectorGetX(XMVector3Length(mWorld.r[2])),
    //    1.0f
    //);

    // ボックス側は純粋な原点中心（-halfSize ~ +halfSize）で計算する
    XMVECTOR scaledHalfSize = XMLoadFloat3(&obbSize);
    XMVECTOR wideHalfSize = scaledHalfSize / 1.7f;
    XMVECTOR wideMin = -wideHalfSize;
    XMVECTOR wideMax = wideHalfSize;

    float radius = sphere->GetSize().x;

    // 最近点を計算
    XMVECTOR wideClosestPoint = XMVectorClamp(sphereLocalPos, wideMin, wideMax);

    XMVECTOR localDiff = sphereLocalPos - wideClosestPoint;
    float wideDistSq = 0.0f;
    XMStoreFloat(&wideDistSq, XMVector3LengthSq(localDiff));

    if (wideDistSq <= (radius * radius))
    {
        data.isHit = true;

        XMVECTOR trueHalfSize = XMLoadFloat3(&obbSize) * 0.5f;
        XMVECTOR trueMin = -trueHalfSize;
        XMVECTOR trueMax = trueHalfSize;

        XMVECTOR trueClosestPoint = XMVectorClamp(sphereLocalPos, trueMin, trueMax);
        XMVECTOR trueDiff = sphereLocalPos - trueClosestPoint;
        float trueDistSq = 0.0f;
        XMStoreFloat(&trueDistSq, XMVector3LengthSq(trueDiff));
        float distance = sqrtf(trueDistSq);

        data.penetrationDepth = radius - distance;

        XMVECTOR localNormal = XMVectorZero();

        if (distance > 0.0001f)
        {
            localNormal = XMVector3Normalize(trueDiff);
        }
        else
        {
            localNormal = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
            data.penetrationDepth = radius;
        }

        XMVECTOR worldNormal = XMVector3TransformNormal(localNormal, mRotOnly);
        worldNormal = XMVector3Normalize(worldNormal);
        XMStoreFloat3(&data.normal, worldNormal);

        // 接触点をワールドに戻す際も、Center位置を戻してからワールド変換する
        XMVECTOR contactWithCenter = trueClosestPoint + center;
        XMVECTOR worldContact = XMVector3TransformCoord(contactWithCenter, mWorld);
        XMStoreFloat3(&data.contactPoint, worldContact);
    }
}

void CollisionManager::IsHitOBBVsBox(Collider* obb, Collider* box, CollisionData& data)
{
    IsHitOBBVsOBB(obb, box, data);
}

void CollisionManager::IsHitOBBVsOBB(Collider* obbA, Collider* obbB, CollisionData& data)
{
    GameObject* obbObjA = obbA->GetGameObject();
    GameObject* obbObjB = obbB->GetGameObject();

    XMMATRIX mWorldA = obbObjA->GetWorldMatrix();
    XMMATRIX mWorldB = obbObjB->GetWorldMatrix();

    XMFLOAT3 obbPosA = obbA->GetWorldCenter();
    XMFLOAT3 obbPosB = obbB->GetWorldCenter();

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

    float minOverlap = FLT_MAX;
    XMVECTOR bestAxis = XMVectorZero();

    //Aの3軸でテスト
    for (int i = 0; i < 3; i++)
    {
        float tB = tA[0] * R[0][i] + tA[1] * R[1][i] + tA[2] * R[2][i];

        float rA = XMVectorGetByIndex(eA, i);
        float rB =
            XMVectorGetX(eB) * fabsf(R[i][0]) +
            XMVectorGetY(eB) * fabsf(R[i][1]) +
            XMVectorGetZ(eB) * fabsf(R[i][2]);

        float distance = fabsf(tA[i]);
        float overlap = (rA + rB) - distance;
        if (overlap < 0.0f)return;

        if (overlap < minOverlap)
        {
            minOverlap = overlap;
            bestAxis = uA[i];
            if (tA[i] < 0.0f)bestAxis = -bestAxis;
        }
    }

    //ゼロ除算を防ぐ為の値
    const float EPSILON = 1e-5f;

    //Bの3軸でテスト
    for (int i = 0; i < 3; i++)
    {
        float tBVal = 0.0f;
        XMStoreFloat(&tBVal, XMVector3Dot(T, uB[i]));

        float rA = 
            XMVectorGetX(eA) * fabsf(R[0][i]) +
            XMVectorGetY(eA) * fabsf(R[1][i]) +
            XMVectorGetZ(eA) * fabsf(R[2][i]);
        float rB = XMVectorGetByIndex(eB, i);

        float distance = fabsf(tBVal);
        float overlap = (rA + rB) - distance;
        if (overlap < 0.0f)return;

        if (overlap < minOverlap)
        {
            minOverlap = overlap;
            bestAxis = uB[i];
            if (tBVal < 0.0f)bestAxis = -bestAxis;
        }
    }

    //残りのクロス積の9軸でテスト
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            float dat = 1.0f - fabsf(R[i][j]);
            if (dat <= EPSILON)continue;

            //XMVECTOR axis = XMVector3Cross(uA[i], uB[j]);
            //axis = XMVector3Normalize(axis);

            float rA =
                XMVectorGetByIndex(eA, (i + 1) % 3) * fabsf(R[(i + 2) % 3][j]) +
                XMVectorGetByIndex(eA, (i + 2) % 3) * fabsf(R[(i + 1) % 3][j]);

            float rB =
                XMVectorGetByIndex(eB, (j + 1) % 3) * fabsf(R[i][(j + 2) % 3]) +
                XMVectorGetByIndex(eB, (j + 2) % 3) * fabsf(R[i][(j + 1) % 3]);

            float t = fabsf(tA[(i + 2) % 3] * R[(i + 1) % 3][j] - tA[(i + 1) % 3] * R[(i + 2) % 3][j]);

            float overlap = (rA + rB) - t;
            if (overlap < 0.0f)return;

            //if (overlap < minOverlap)
            //{
            //    minOverlap = overlap;
            //    bestAxis = axis;
            //    if (XMVectorGetX(XMVector3Dot(T, bestAxis)) < 0.0f)
            //    {
            //        bestAxis = -bestAxis;
            //    }
            //}
        }
    }

    data.isHit = true;
    data.penetrationDepth = minOverlap;
    XMStoreFloat3(&data.normal, -bestAxis);

    //接触点は一旦2つの中間地点を設定
    XMVECTOR contact = cA + (T * 0.5f);
    XMStoreFloat3(&data.contactPoint, contact);
}
