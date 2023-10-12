#pragma once

class Transform
{
public :
	Transform();
	~Transform();

	void UpdateWorld();
	void GUIRender();

	void SetBuffer();

	Vector3 GlobalPos() { return globalPosition; }
	Vector3 GlobalScale() { return globalScale; }

	Vector3& Pos() { return localPosition; }
	Vector3& Rot() { return localRotation; }
	Vector3& Scale() { return localScale; }

	Vector3 Right() const { return right.GetNormalized(); }
	Vector3 Left() const { return right.GetNormalized() * -1.0f; }
	Vector3 Up() const { return up.GetNormalized(); }
	Vector3 Down() const { return up.GetNormalized() * -1.0f; }
	Vector3 Forward() const { return forward.GetNormalized(); }
	Vector3 Back() const { return forward.GetNormalized() * -1.0f; }

	Transform* GetParent();
	void SetParent(Transform* transform) { parent = transform; }
	void SetPivot(Vector3 pivot) { this->pivot = pivot; }
	void SetWorld(Matrix matrix) { world = matrix; }

	bool Active();
	bool ActiveSelf() { return isActive; }
	void SetActive(bool isActive) { this->isActive = isActive; }

	string GetTag() { return tag; }
	void SetTag(string tag) { this->tag = tag; }

	Matrix GetWorld() { return world; }

	void Save();
	void Load();
	void Load(string folder);

private :
	bool isActive = true;
	string tag = "Unkonwn";

	Vector3 localPosition;
	Vector3 localRotation = { 0, 0, 0 };
	Vector3 localScale = { 1, 1, 1 };

	Transform* parent = nullptr;	// 부모

	Vector3 pivot;	// 기준 축

	Vector3 right, up, forward;				// 방향 벡터
	Vector3 globalPosition, globalScale;	// 글로벌 값

	Matrix world;	// World 행렬
	MatrixBuffer* worldBuffer;
};