#include "Framework.h"

Camera::Camera()
{
    transform = new Transform();

    viewBuffer = new ViewBuffer();
    viewBuffer->SetVertexShaderBuffer(1);
}

Camera::~Camera()
{
    delete transform;
    delete viewBuffer;
}

void Camera::Update()
{
    projection = Environment::Get()->GetProjection();
    Frustum();

    if (target)
    {
        if (Environment::Get()->isFreeMode)
            FreeMode();
        else
            FollowMode();
    }
    else
        FreeMode();

    SetView();

    transform->UpdateWorld();
}

void Camera::GUIRender()
{
    if (ImGui::TreeNode("CameraOption"))
    {
        ImGui::DragFloat("MoveSpeed", &moveSpeed);
        ImGui::DragFloat("RotSpeed", &rotSpeed);

        if (target && ImGui::TreeNode("TargetOption"))
        {
            ImGui::DragFloat("Distance", &distance, 0.1f);
            ImGui::DragFloat("Height", &height, 0.1f);
            ImGui::DragFloat3("FocusOffset", (float*)&focusOffset, 0.1f);

            float degree = XMConvertToDegrees(rotY);
            ImGui::DragFloat("RotY", &degree, 0.1f);
            rotY = XMConvertToRadians(degree);

            ImGui::DragFloat("MoveDamping", &moveDamping, 0.1f);
            ImGui::DragFloat("RotDamping", &rotDamping, 0.1f);

            ImGui::Checkbox("LookAtTargetX", &isLookAtTargetX);
            ImGui::Checkbox("LookAtTargetY", &isLookAtTargetY);

            if (ImGui::Button("Save"))
                TargetOptionSave("Option");
            ImGui::SameLine();
            if (ImGui::Button("Load"))
                TargetOptionLoad("Option");

            ImGui::TreePop();
        }

        transform->GUIRender();

        ImGui::TreePop();
    }
}

void Camera::SetView()
{
    transform->UpdateWorld();

    // 월드의 역행렬을 구함
    // 세계 좌표계에서 카메라의 위치와 방향을 계산하는 것보다는 카메라 자체의 좌표축과 방향을 기준으로 계산하는 것이 더욱 간단합니다.
    view = XMMatrixInverse(nullptr, transform->GetWorld());

    viewBuffer->SetMatrix(view, transform->GetWorld());
    viewBuffer->SetVertexShaderBuffer(1);
    viewBuffer->SetPixelShaderBuffer(1);
}


void Camera::LookAtTarget()
{
    rotMatrix = XMMatrixRotationY(target->Rot().y + rotY);

    Vector3 forward = XMVector3TransformNormal(Vector3::Forward(), rotMatrix);

    transform->Pos() = target->GlobalPos() + forward * -distance;
    transform->Pos().y += height;

    Vector3 offset = XMVector3TransformCoord(focusOffset, rotMatrix);
    Vector3 targetPos = target->GlobalPos() + offset;

    Vector3 dir = (targetPos - transform->Pos()).GetNormalized();
    forward = Vector3(dir.x, 0.0f, dir.z).GetNormalized();

    transform->Rot().x = acos(Dot(forward, dir));
    transform->Rot().y = atan2(dir.x, dir.z);
}

Vector3 Camera::ScreenToWorld(Vector3 screenPos)
{
    return XMVector3TransformCoord(screenPos, transform->GetWorld());
}

Vector3 Camera::WorldToScreen(Vector3 worldPos)
{
    Vector3 sPos = {};

    sPos = XMVector3TransformCoord(worldPos, view);
    sPos = XMVector3TransformCoord(sPos, projection);
    //NDC : -1 ~ 1

    sPos = (sPos + Vector3::One()) * 0.5f;//0~1

    sPos.x *= WIN_WIDTH;
    sPos.y *= WIN_HEIGHT;

    return sPos;
}

Ray Camera::ScreenPointToRay(Vector3 screenPoint)
{
    Vector3 screenSize(WIN_WIDTH, WIN_HEIGHT, 1.0f);
    // NDC 좌표에서 -1~1 까지로 변환
    Vector2 point;
    point.x = (screenPoint.x / screenSize.x) * 2.0f - 1.0f;
    point.y = (screenPoint.y / screenSize.y) * 2.0f - 1.0f;

    // 프로젝션 행렬을 곱함
    Float4x4 temp;
    XMStoreFloat4x4(&temp, projection);

    // 프로젝션 행렬과 
    screenPoint.x = point.x / temp._11;
    screenPoint.y = point.y / temp._22;
    screenPoint.z = 1.0f;

    screenPoint = XMVector3TransformNormal(screenPoint, transform->GetWorld());

    Ray ray;
    ray.pos = transform->Pos();
    ray.dir = screenPoint.GetNormalized();

    return ray;
}
bool Camera::ContainPoint(Vector3 point)
{
    for (UINT i = 0; i < 6; i++)
    {
        Vector3 dot = XMPlaneDotCoord(planes[i], point);

        if (dot.x < 0.0f)
            return false;
    }

    return true;
}
void Camera::TargetOptionSave(string file)
{
    string path = "TextData/Camera/" + file + ".cam";

    BinaryWriter* writer = new BinaryWriter(path);

    writer->Float(distance);
    writer->Float(height);
    writer->Float(moveDamping);
    writer->Float(rotDamping);
    writer->Float(rotY);
    writer->Vector(focusOffset);
    writer->Bool(isLookAtTargetX);
    writer->Bool(isLookAtTargetY);

    delete writer;
}

void Camera::TargetOptionLoad(string file)
{
    string path = "TextData/Camera/" + file + ".cam";

    BinaryReader* reader = new BinaryReader(path);

    distance = reader->Float();
    height = reader->Float();
    moveDamping = reader->Float();
    rotDamping = reader->Float();
    rotY = reader->Float();
    focusOffset = reader->Vector();
    isLookAtTargetX = reader->Bool();
    isLookAtTargetY = reader->Bool();

    delete reader;
}


void Camera::FreeMode()
{
    //ShowCursor(true);
    if (KEY_PRESS(VK_RBUTTON))
    {
        if (KEY_PRESS('W'))
            transform->Pos() += transform->Forward() * moveSpeed * DELTA;
        if (KEY_PRESS('S'))
            transform->Pos() += transform->Back() * moveSpeed * DELTA;
        if (KEY_PRESS('A'))
            transform->Pos() += transform->Left() * moveSpeed * DELTA;
        if (KEY_PRESS('D'))
            transform->Pos() += transform->Right() * moveSpeed * DELTA;
        if (KEY_PRESS('E'))
            transform->Pos() += transform->Down() * moveSpeed * DELTA;
        if (KEY_PRESS('Q'))
            transform->Pos() += transform->Up() * moveSpeed * DELTA;

        ImVec2 delta = ImGui::GetIO().MouseDelta;

        transform->Rot().x += delta.y * rotSpeed * DELTA;
        transform->Rot().y += delta.x * rotSpeed * DELTA;
    }
}

void Camera::FollowMode()
{
    //if (UIManager::Get()->InventoryIsShow())
    //    ShowCursor(true);
    //else
    //    ShowCursor(false);

    destRot = Lerp(destRot, target->Rot().y, rotDamping * DELTA);
    rotMatrix = XMMatrixRotationY(destRot + rotY);

    Vector3 forward = XMVector3TransformNormal(Vector3::Forward(), rotMatrix);

    destPos = target->GlobalPos() + forward * -distance;
    destPos.y += height;

    transform->Pos() = Lerp(transform->Pos(), destPos, moveDamping * DELTA);

    Vector3 offset = XMVector3TransformCoord(focusOffset, rotMatrix);
    Vector3 targetPos = target->GlobalPos() + offset;

    Vector3 dir = (targetPos - transform->Pos()).GetNormalized();
    forward = Vector3(dir.x, 0.0f, dir.z).GetNormalized();

    if (isLookAtTargetX)
    {
        transform->Rot().x = acos(Dot(forward, dir));
    }
    if (isLookAtTargetY)
    {
        transform->Rot().y = atan2(dir.x, dir.z);
    }
}

void Camera::Frustum()
{
    Float4x4 VP;
    XMStoreFloat4x4(&VP, view * projection);

    //Left
    a = VP._14 + VP._11;
    b = VP._24 + VP._21;
    c = VP._34 + VP._31;
    d = VP._44 + VP._41;
    planes[0] = XMVectorSet(a, b, c, d);

    //Right
    a = VP._14 - VP._11;
    b = VP._24 - VP._21;
    c = VP._34 - VP._31;
    d = VP._44 - VP._41;
    planes[1] = XMVectorSet(a, b, c, d);

    //Bottom
    a = VP._14 + VP._12;
    b = VP._24 + VP._22;
    c = VP._34 + VP._32;
    d = VP._44 + VP._42;
    planes[2] = XMVectorSet(a, b, c, d);

    //Top
    a = VP._14 - VP._12;
    b = VP._24 - VP._22;
    c = VP._34 - VP._32;
    d = VP._44 - VP._42;
    planes[3] = XMVectorSet(a, b, c, d);

    //Near
    a = VP._14 + VP._13;
    b = VP._24 + VP._23;
    c = VP._34 + VP._33;
    d = VP._44 + VP._43;
    planes[4] = XMVectorSet(a, b, c, d);

    //Far
    a = VP._14 - VP._13;
    b = VP._24 - VP._23;
    c = VP._34 - VP._33;
    d = VP._44 - VP._43;
    planes[5] = XMVectorSet(a, b, c, d);

    for(UINT i = 0; i < 6; i++)
        planes[i] = XMPlaneNormalize(planes[i]);
}
