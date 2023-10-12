#include "Framework.h"

SphereCollider::SphereCollider(float radius, UINT stackCount, UINT sliceCount)
	: radius(radius), stackCount(stackCount), sliceCount(sliceCount)
{
	type = SPHERE;

	MakeMesh();
	mesh->CreateMesh();
}

SphereCollider::~SphereCollider()
{
}

bool SphereCollider::IsRayCollision(IN Ray ray, OUT Contact* contact)
{
    Vector3 P = ray.pos;
    Vector3 D = ray.dir.GetNormalized();

    Vector3 C = transform->GlobalPos();
    Vector3 A = P - C;

    float b = Dot(D, A);
    float c = Dot(A, A) - pow(Radius(), 2);

    if (pow(b, 2) >= c)
    {
        if (contact != nullptr)
        {
            float t = -b - sqrt(pow(b, 2) - c);

            contact->tag = transform->GetTag();
            contact->distance = t;
            contact->hitPoint = P + D * t;
        }

        return true;
    }

    return false;
}

bool SphereCollider::IsBoxCollision(BoxCollider* collider)
{
    return collider->IsSphereCollision(this);
}

bool SphereCollider::IsSphereCollision(SphereCollider* collider)
{
    float distance = Distance(transform->GlobalPos(), collider->transform->GlobalPos());

    return distance <= (Radius() + collider->Radius());
}

bool SphereCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
    return collider->IsSphereCollision(this);
}

void SphereCollider::MakeMesh()
{
    float thetaStep = XM_2PI / sliceCount;
    float phiStep = XM_PI / stackCount;

    // Vertices
    vector<VertexColor>& vertices = mesh->GetVertices();
    vertices.reserve((sliceCount + 1) * (stackCount + 1));

    for (UINT i = 0; i <= stackCount; i++)
    {
        float phi = i * phiStep;

        for (UINT j = 0; j <= sliceCount; j++)
        {
            float theta = j * thetaStep;

            VertexColor vertex;
            vertex.pos.x = sin(phi) * cos(theta) * radius;
            vertex.pos.y = cos(phi) * radius;
            vertex.pos.z = sin(phi) * sin(theta) * radius;
            vertex.color = COLOR_GREEN;

            vertices.push_back(vertex);
        }
    }

    //Indices
    vector<UINT>& indices = mesh->GetIndices();

    indices.reserve(sliceCount * stackCount * 6);

    for (UINT i = 0; i < stackCount; i++)
    {
        for (UINT j = 0; j < sliceCount; j++)
        {
            indices.push_back((sliceCount + 1) * i + j);//0
            indices.push_back((sliceCount + 1) * (i + 1) + j);//1

            indices.push_back((sliceCount + 1) * i + j);//0            
            indices.push_back((sliceCount + 1) * i + j + 1);//2            
        }
    }
}
