#include "Framework.h"

BoxCollider::BoxCollider(Vector3 size) : size(size)
{
	type = BOX;

    MakeMesh();
    mesh->CreateMesh();
}

BoxCollider::~BoxCollider()
{
}


bool BoxCollider::IsRayCollision(IN Ray ray, OUT Contact* contact)
{
    ObbDesc box;
    GetObb(box);    // 트랜스폼 정보

    Vector3 min = box.halfSize * -1.0f;
    Vector3 max = box.halfSize;
    // 방향 정보
    Vector3 delta = box.pos - ray.pos;
    // 레이 정보
    Vector3 D = ray.dir.GetNormalized();

    float tMin = 0.0f;
    float tMax = FLT_MAX;

    for (UINT i = 0; i < 3; i++)
    {
        Vector3 axis = box.axis[i];
        float e = Dot(axis, delta);
        float f = Dot(D, axis);

        //레이와 축이 수직인 상황
        if (NearlyEqual(f, 0.0f))
        {
            if (min[i] > e || max[i] < e) return false;
        }
        else
        {
            float t1 = (e + min[i]) / f;
            float t2 = (e + max[i]) / f;

            if (t1 > t2) swap(t1, t2);

            if (t2 < tMax) tMax = t2;
            if (t1 > tMin) tMin = t1;
            if (tMin > tMax) return false;
        }
    }

    if (contact != nullptr)
    {
        contact->tag = transform->GetTag();
        contact->distance = tMin;
        contact->hitPoint = ray.pos + ray.dir * tMin;
    }

    return true;
}

bool BoxCollider::IsBoxCollision(BoxCollider* collider)
{
    ObbDesc box1, box2;
    GetObb(box1);
    collider->GetObb(box2);
    // 박스끼리 방향
    Vector3 D = box2.pos - box1.pos;

    for (UINT i = 0; i < 3; i++)
    {
        if (IsSeperateAxis(D, box1.axis[i], box1, box2)) return false;
        if (IsSeperateAxis(D, box2.axis[i], box1, box2)) return false;
    }

    for (UINT i = 0; i < 3; i++)
    {
        for (UINT j = 0; j < 3; j++)
        {
            if (box1.axis[i] == box2.axis[j]) return true;
        }
    }

    for (UINT i = 0; i < 3; i++)
    {
        for (UINT j = 0; j < 3; j++)
        {
            Vector3 cross = Cross(box1.axis[i], box2.axis[j]);
            if (IsSeperateAxis(D, cross, box1, box2)) return false;
        }
    }

    return true;
}

bool BoxCollider::IsSphereCollision(SphereCollider* collider)
{
    ObbDesc box;
    GetObb(box);

    Vector3 closetPointToSphere = box.pos;
    Vector3 direction = collider->transform->GlobalPos() - box.pos;

    for (UINT i = 0; i < 3; i++)
    {
        // 축별 내적
        float length = Dot(box.axis[i], direction);
        // 방향 반전
        float mult = (length < 0.0f) ? -1.0f : 1.0f;
        // 더 짧은거리 비교
        length = min(abs(length), box.halfSize[i]);
        // 최종 거리 합산
        closetPointToSphere += box.axis[i] * length * mult;
    }

    float distance = Distance(collider->transform->GlobalPos(), closetPointToSphere);

    return distance <= collider->Radius();
}

bool BoxCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
    return collider->IsBoxCollision(this);
}

void BoxCollider::GetObb(ObbDesc& obbDesc)
{
    obbDesc.pos = transform->GlobalPos();

    obbDesc.axis[0] = transform->Right();
    obbDesc.axis[1] = transform->Up();
    obbDesc.axis[2] = transform->Forward();

    obbDesc.halfSize = size * 0.5f * transform->GlobalScale();
}

bool BoxCollider::IsSeperateAxis(Vector3 D, Vector3 axis, ObbDesc box1, ObbDesc box2)
{
    // 각 방향 벡터와 내적
    float distance = abs(Dot(D, axis));

    float a = 0.0f;
    float b = 0.0f;

    // 축 별 방향과 사이즈를 내적해서 더해서 비교
    for (UINT i = 0; i < 3; i++)
    {
        Vector3 temp = box1.axis[i] * box1.halfSize[i];
        a += abs(Dot(temp, axis));
        temp = box2.axis[i] * box2.halfSize[i];
        b += abs(Dot(temp, axis));
    }

    return distance > a + b;
}

void BoxCollider::MakeMesh()
{
    Vector3 halfSize = size * 0.5f;

    vector<VertexColor>& vertices = mesh->GetVertices();

    vertices.emplace_back(-halfSize.x, +halfSize.y, -halfSize.z, COLOR_GREEN);
    vertices.emplace_back(+halfSize.x, +halfSize.y, -halfSize.z, COLOR_GREEN);
    vertices.emplace_back(-halfSize.x, -halfSize.y, -halfSize.z, COLOR_GREEN);
    vertices.emplace_back(+halfSize.x, -halfSize.y, -halfSize.z, COLOR_GREEN);

    vertices.emplace_back(-halfSize.x, +halfSize.y, +halfSize.z, COLOR_GREEN);
    vertices.emplace_back(+halfSize.x, +halfSize.y, +halfSize.z, COLOR_GREEN);
    vertices.emplace_back(-halfSize.x, -halfSize.y, +halfSize.z, COLOR_GREEN);
    vertices.emplace_back(+halfSize.x, -halfSize.y, +halfSize.z, COLOR_GREEN);

    vector<UINT>& indices = mesh->GetIndices();

    indices = {
        0, 1, 1, 3, 3, 2, 2, 0,
        4, 5, 5, 7, 7, 6, 6, 4,
        0, 4, 1, 5, 2, 6, 3, 7
    };

}