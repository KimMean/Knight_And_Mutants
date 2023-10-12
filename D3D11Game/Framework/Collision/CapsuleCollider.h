#pragma once

class CapsuleCollider : public Collider
{
public :
	CapsuleCollider(float radius = 1.0f, float height = 2.0f,
		UINT stackCount = 8, UINT sliceCount = 16);
	~CapsuleCollider();


	// Collider을(를) 통해 상속됨
	virtual bool IsRayCollision(IN Ray ray, OUT Contact* contact) override;
	virtual bool IsBoxCollision(BoxCollider* collider) override;
	virtual bool IsSphereCollision(SphereCollider* collider) override;
	virtual bool IsCapsuleCollision(CapsuleCollider* collider) override;

	float Radius() { return radius * max(transform->GlobalScale().x, max(transform->GlobalScale().y, transform->GlobalScale().z)); }
	float Height() { return height * transform->GlobalScale().y; }

	// Collider을(를) 통해 상속됨
	virtual void MakeMesh() override;

private:
	float radius, height;
	UINT stackCount, sliceCount;
};