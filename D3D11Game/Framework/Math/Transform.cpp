#include "Framework.h"
#include "Transform.h"

Transform::Transform()
{
    world = XMMatrixIdentity();     // 4X4 단위행렬 생성
    worldBuffer = new MatrixBuffer();
}

Transform::~Transform()
{
    delete worldBuffer;
}

void Transform::UpdateWorld()
{
    // 현재 위치, 회전, 크기값을 기준으로 월드행렬을 구함
    world = XMMatrixTransformation( // 변환 행렬(world)을 생성
        pivot,                      // 스케일링 변환의 중심점을 지정합니다.
        XMQuaternionIdentity(),     // 스케일링 변환의 방향을 지정합니다.
        localScale,                 // X, Y, Z 축에 대한 스케일링 값을 지정합니다.
        pivot,                      // 회전 변환의 중심점을 지정합니다.
        XMQuaternionRotationRollPitchYawFromVector(localRotation),  // 회전 변환을 지정합니다.
        localPosition               // 이동 변환을 지정합니다.
    );

    // 부모로 지정한 오브젝트가 있다면 곱해줌
    if (parent)
        world *= parent->world;

    // 월드행렬을 기준으로 3개 축 벡터를 구함
    XMFLOAT4X4 matWorld;
    XMStoreFloat4x4(&matWorld, world);
    right = Vector3(matWorld._11, matWorld._12, matWorld._13);
    up = Vector3(matWorld._21, matWorld._22, matWorld._23);
    forward = Vector3(matWorld._31, matWorld._32, matWorld._33);

    // 월드행렬에서 Scale, Rotation, Transform 값을 Vector값으로 받음
    XMVECTOR outS, outR, outT;
    XMMatrixDecompose(&outS, &outR, &outT, world);

    // 위에서 받은 Vector값을 Float3 값으로 변환
    Float3 tempPos, tempScale;
    XMStoreFloat3(&tempPos, outT);
    XMStoreFloat3(&tempScale, outS);

    // 변환 된 값을 Global 값으로 저장
    globalPosition = tempPos;
    globalScale = tempScale;
}

void Transform::GUIRender()
{
    if (ImGui::TreeNode((tag + "_Transform").c_str()))
    {
        ImGui::Text(tag.c_str());
    
        ImGui::Checkbox("Active", &isActive);
    
        string temp = tag + "_Pos";
        ImGui::DragFloat3(temp.c_str(), (float*)&localPosition, 0.1f);
    
        temp = tag + "_Rot";
        Float3 rot;
        rot.x = XMConvertToDegrees(localRotation.x);
        rot.y = XMConvertToDegrees(localRotation.y);
        rot.z = XMConvertToDegrees(localRotation.z);
    
        ImGui::DragFloat3(temp.c_str(), (float*)&rot, 1.0f, -180, 180);
    
        localRotation.x = XMConvertToRadians(rot.x);
        localRotation.y = XMConvertToRadians(rot.y);
        localRotation.z = XMConvertToRadians(rot.z);
    
        temp = tag + "_Scale";
        ImGui::DragFloat3(temp.c_str(), (float*)&localScale, 0.1f);
    
        if (ImGui::Button("Save"))
            Save();
        
        ImGui::SameLine();
        
        if (ImGui::Button("Load"))
            Load();
    
        ImGui::TreePop();
    }
}

void Transform::SetBuffer()
{
    worldBuffer->SetMatrix(world);
    worldBuffer->SetVertexShaderBuffer(0);
}

Transform* Transform::GetParent()
{
    if (parent != nullptr)
        return parent;

    return nullptr;
}

bool Transform::Active()
{
    if (parent == nullptr)
        return isActive;

    if (isActive == false)
        return false;

    return parent->Active();
}


void Transform::Save()
{
    BinaryWriter* writer = new BinaryWriter("TextData/Transforms/" + tag + ".srt");

    writer->Float(localPosition.x);
    writer->Float(localPosition.y);
    writer->Float(localPosition.z);

    writer->Float(localRotation.x);
    writer->Float(localRotation.y);
    writer->Float(localRotation.z);

    writer->Float(localScale.x);
    writer->Float(localScale.y);
    writer->Float(localScale.z);

    delete writer;
}

void Transform::Load()
{
    BinaryReader* reader = new BinaryReader("TextData/Transforms/" + tag + ".srt");

    if (reader->IsFailed()) return;

    localPosition.x = reader->Float();
    localPosition.y = reader->Float();
    localPosition.z = reader->Float();

    localRotation.x = reader->Float();
    localRotation.y = reader->Float();
    localRotation.z = reader->Float();

    localScale.x = reader->Float();
    localScale.y = reader->Float();
    localScale.z = reader->Float();

    delete reader;
}

void Transform::Load(string folder)
{
    BinaryReader* reader = new BinaryReader("TextData/Transforms/" + folder + "/" + tag + ".srt");

    if (reader->IsFailed()) return;

    localPosition.x = reader->Float();
    localPosition.y = reader->Float();
    localPosition.z = reader->Float();

    localRotation.x = reader->Float();
    localRotation.y = reader->Float();
    localRotation.z = reader->Float();

    localScale.x = reader->Float();
    localScale.y = reader->Float();
    localScale.z = reader->Float();

    delete reader;
}
