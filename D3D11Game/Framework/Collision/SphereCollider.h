#pragma once

class SphereCollider : public Collider
{
public :
	SphereCollider(float radius = 1.0f, UINT stackCount = 8, UINT sliceCount = 16);
	~SphereCollider();

	virtual bool IsRayCollision(IN Ray ray, OUT Contact* contact) override;

	virtual bool IsBoxCollision(BoxCollider* collider) override;
	virtual bool IsSphereCollision(SphereCollider* collider) override;
	virtual bool IsCapsuleCollision(CapsuleCollider* collider) override;

	float Radius() { return radius * max(transform->GlobalScale().x, max(transform->GlobalScale().y, transform->GlobalScale().z)); }

private:
	virtual void MakeMesh() override;

private :
	float radius;

	UINT stackCount, sliceCount;
};