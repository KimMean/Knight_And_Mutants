#pragma once

class Collider
{
protected:
	enum Type
	{
		BOX, SPHERE, CAPSULE
	};
	struct ObbDesc
	{
		Vector3 pos;
		Vector3 axis[3];
		Vector3 halfSize;
	};
	const float PUSH_SPEED = 10.0f;

public :
	Collider();
	~Collider();

	void Render();

	bool IsCollision(Collider* collider);

	virtual bool IsRayCollision(IN Ray ray, OUT Contact* contact = nullptr) = 0;
	virtual bool IsBoxCollision(class BoxCollider* collider) = 0;
	virtual bool IsSphereCollision(class SphereCollider* collider) = 0;
	virtual bool IsCapsuleCollision(class CapsuleCollider* collider) = 0;

	bool PushCollision(Collider* collider);
	void SetColor(Float4 color);

	static void ToggleRender() { isRender = !isRender; }

private:
	virtual void MakeMesh() = 0;

public :
	Transform* transform;

protected :
	Type type;

	Mesh<VertexColor>* mesh;

	VertexShader* vertexShader = nullptr;
	PixelShader* pixelShader = nullptr;

private :
	static bool isRender;
};