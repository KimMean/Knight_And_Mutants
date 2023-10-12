#pragma once

class Camera
{
public :
	Camera();
	~Camera();

	void Update();
	void GUIRender();

	void SetView();

	void SetTarget(Transform* target) { this->target = target; }
	void LookAtTarget();

	Vector3 ScreenToWorld(Vector3 screenPos);
	Vector3 WorldToScreen(Vector3 worldPos);
	Ray ScreenPointToRay(Vector3 screenPoint);

	ViewBuffer* GetViewBuffer() { return viewBuffer; }

	bool ContainPoint(Vector3 point);

	void TargetOptionSave(string file);
	void TargetOptionLoad(string file);

private :
	void FreeMode();
	void FollowMode();

	void Frustum();


public :
	Transform* transform;
private :

	ViewBuffer* viewBuffer;
	Matrix view;
	Matrix projection;

	Vector3 planes[6];
	float a, b, c, d;

	Transform* target = nullptr;

	float moveSpeed = 10.0f;	// 카메라 무브 스피드
	float rotSpeed = 5.0f;		// 카메라 회전 스피드

	// Target FollowMode

	Matrix rotMatrix;
	Vector3 destPos;
	Vector3 focusOffset;
	float distance = 20.0f;
	float height = 20.0f;
	float moveDamping = 5.0f;

	float rotDamping = 1.0f;
	float destRot = 0.0f;
	float rotY = 0;

	bool isLookAtTargetX = true;	// x 동기화
	bool isLookAtTargetY = true;	// y 동기화
};