#include "Framework.h"

ModelAnimator::ModelAnimator(string name)
    : Model(name)
{
    SetShader(L"Shaders/Model/ModelAnimation.hlsl");
    //SetShader(L"Shaders/Light/Light.hlsl");

    frameBuffer = new FrameBuffer();

    //worldBuffer->SetType(1);
}

ModelAnimator::~ModelAnimator()
{
    for (ModelClip* clip : clips)
        delete clip;

    delete frameBuffer;

    delete[] clipTransforms;
    delete[] nodeTransforms;

    texture->Release();
    srv->Release();
}

void ModelAnimator::Update()
{
    UpdateFrame();
    transform->UpdateWorld();
}

void ModelAnimator::Render()
{
    if (texture == nullptr)
        CreateTexture();

    frameBuffer->SetVertexShaderBuffer(3);
    DC->VSSetShaderResources(0, 1, &srv);

    Model::Render();
}

void ModelAnimator::GUIRender()
{
    //int clip = frameBuffer->Get().clip;
    //ImGui::SliderInt("Clip", &frameBuffer->Get().clip, 0, clips.size() - 1);
    //ImGui::SliderInt("Frame", (int*) & frameBuffer->Get().curFrame,
    //    0, clips[clip]->frameCount - 1);
    //ImGui::SliderFloat("Scale", &frameBuffer->Get().scale, 0, 10.0f);

    Model::GUIRender();
}

// 클립 읽기
void ModelAnimator::ReadClip(string clipName, UINT clipNum, string lockBone)
{
    string path = "Models/Clips/" + name + "/"
        + clipName + to_string(clipNum) + ".clip";

    BinaryReader* reader = new BinaryReader(path);

    ModelClip* clip = new ModelClip();
    clip->name = reader->String();          // 이름
    clip->lockBone = lockBone;              // 움직이는 애니메이션 막기 위한 변수
    clip->frameCount = reader->UInt();      // 프레임 수
    clip->tickPerSecond = reader->Float();  // 초당 틱

    // 뼈대마다 키 프레임을 다 가져옴
    UINT boneCount = reader->UInt();        // Bone 개수
    for(int i = 0; i < boneCount; i++)
    {
        KeyFrame* keyFrame = new KeyFrame();    // 키 프레임
        keyFrame->boneName = reader->String();  // Bone 이름

        UINT size = reader->UInt();             // KeyFrame 크기
        if (size > 0)
        {
            keyFrame->transforms.resize(size);
            // 키 프레임 정보 가져옴
            void* ptr = (void*)keyFrame->transforms.data();
            reader->Byte(&ptr, sizeof(KeyTransform) * size);
        }
        clip->keyFrames[keyFrame->boneName] = keyFrame;        
    }

    clips.push_back(clip);

    delete reader;
}

void ModelAnimator::PlayClip(int clip, float scale, float takeTime)
{
    isPlay = true;
    // 다음 클립 넣어놓고 전환시간 저장
    frameBuffer->Get().next.clip = clip;
    frameBuffer->Get().next.scale = scale;
    frameBuffer->Get().takeTime = takeTime;

    clips[clip]->Init();
}

// 노드에서 해당 트랜스폼 찾기
Matrix ModelAnimator::GetTransformByNode(int nodeIndex)
{
    if (texture == nullptr) return XMMatrixIdentity();

    Matrix curAnim;

    {//CurAnim
        Frame& curFrame = frameBuffer->Get().cur;   // 현재 프레임
        // 현재 프레임의 위치와 다음 프레임 위치를 가져옴
        Matrix cur = nodeTransforms[curFrame.clip].transform[curFrame.curFrame][nodeIndex];
        Matrix next = nodeTransforms[curFrame.clip].transform[curFrame.curFrame + 1][nodeIndex];
        // 애니메이션이 진행된 상태의 포지션 값을 가져오기 위함
        curAnim = Lerp(cur, next, curFrame.time) * transform->GetWorld();
    }

    {//NextAnim
        Frame& nextFrame = frameBuffer->Get().next;
        // 전환중인 애니메이션이 없으면 리턴
        if (nextFrame.clip == -1)
            return curAnim;
        // 다음 애니메이션의 프레임 위치를 계산
        Matrix cur = nodeTransforms[nextFrame.clip].transform[nextFrame.curFrame][nodeIndex];
        Matrix next = nodeTransforms[nextFrame.clip].transform[nextFrame.curFrame + 1][nodeIndex];

        Matrix nextAnim = Lerp(cur, next, nextFrame.time) * transform->GetWorld();
        // 현재 애니메이션과 전환될 애니메이션의 사이값을 구해서 리턴
        return Lerp(curAnim, nextAnim, frameBuffer->Get().tweenTime);
    }    
}
// 애니메이션 정보를 이미지로 저장
void ModelAnimator::CreateTexture()
{
    UINT clipCount = clips.size();

    // Matrix 2세트
    clipTransforms = new ClipTransform[clipCount];
    nodeTransforms = new ClipTransform[clipCount];

    // 트랜스폼 위치
    for(int i = 0 ; i < clipCount; i++)
        CreateClipTransform(i);

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = MAX_BONE * 4;      // 뼈대 * 4 
    desc.Height = MAX_FRAME;        // 프레임만큼
    desc.MipLevels = 1; 
    desc.ArraySize = clipCount;     // 클립 수
    desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    UINT pitchSize = MAX_BONE * sizeof(Matrix); // Bone마다 transform을 저장하기 위함
    UINT pageSize = pitchSize * MAX_FRAME;      // 

    void* p = VirtualAlloc(nullptr, pageSize * clipCount,
        MEM_RESERVE, PAGE_READWRITE);   // 가상 메모리 사용

    for(int i = 0; i < clipCount; i++)
    {
        UINT start = i * pageSize;

        for (UINT y = 0; y < MAX_FRAME; y++)
        {
            void* temp = (BYTE*)p + pitchSize * y + start;

            VirtualAlloc(temp, pitchSize, MEM_COMMIT, PAGE_READWRITE);
            memcpy(temp, clipTransforms[i].transform[y], pitchSize);
        }
    }

    D3D11_SUBRESOURCE_DATA* subResource = new D3D11_SUBRESOURCE_DATA[clipCount];

    for (int i = 0; i < clipCount; i++)
    {
        void* temp = (BYTE*)p + i * pageSize;

        subResource[i].pSysMem = temp;
        subResource[i].SysMemPitch = pitchSize;
        subResource[i].SysMemSlicePitch = pageSize;
    }

    DEVICE->CreateTexture2D(&desc, subResource, &texture);

    delete[] subResource;
    VirtualFree(p, 0, MEM_RELEASE);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
    srvDesc.Texture2DArray.MipLevels = 1;
    srvDesc.Texture2DArray.ArraySize = clipCount;

    DEVICE->CreateShaderResourceView(texture, &srvDesc, &srv);
}

void ModelAnimator::CreateClipTransform(UINT index)
{
    // 해당 인덱스 클립 가져오기
    ModelClip* clip = clips[index];

    for (int i = 0; i < clip->frameCount; i++)
    {
        UINT nodeIndex = 0;
        // 해당 프레임의 노드들 접근
        for (NodeData node : nodes)
        {
            Matrix animation;
            // 노드 데이터의 키 프레임을 가져옴
            KeyFrame* frame = clip->GetKeyFrame(node.name);
            if (frame != nullptr)
            {
                // 현재 위치 가져옴
                KeyTransform& transform = frame->transforms[i];
                // 노드 위치를 고정하고 싶을 때
                if (node.name == clip->lockBone)
                {
                    transform.pos = {};
                }
                // 3D 개체의 매트릭스 반환 
                animation = XMMatrixTransformation(
                    XMVectorZero(),         // 원점 (0, 0, 0)을 사용
                    XMQuaternionIdentity(), // 값이 (0, 0, 0, 1)인 쿼터니언
                    Vector3(transform.scale),   // 스케일 값
                    XMVectorZero(),         // 중심 회전 값
                    XMLoadFloat4(&transform.rot),   // 회전 값
                    Vector3(transform.pos)  // 이동 값
                );
            }
            else
            {
                animation = XMMatrixIdentity(); // 기본 행렬
            }

            Matrix parent;
            int parentIndex = node.parent;  // 부모 인덱스

            if (parentIndex < 0)
                parent = XMMatrixIdentity();    // 부모 없으면 기본 행렬
            else
                parent = nodeTransforms[index].transform[i][parentIndex];
            // 행렬에 부모 행렬을 곱해서 저장
            nodeTransforms[index].transform[i][nodeIndex] = animation * parent;

            int boneIndex = -1;
            if (boneMap.count(node.name))
                boneIndex = boneMap[node.name];
            // 기준 뼈대가 있을 때
            if (boneIndex >= 0)
            {
                // 월드 행렬을 곱하므로써 적절한 위치에 있게 됌
                Matrix offset = bones[boneIndex].offset;
                offset *= nodeTransforms[index].transform[i][nodeIndex];

                clipTransforms[index].transform[i][boneIndex] = offset;
            }
            nodeIndex++;
        }
    }
}
// 프레임 업데이트
void ModelAnimator::UpdateFrame()
{
    if (!isPlay) return;

    FrameBuffer::Data& frameData = frameBuffer->Get();

    {// Cur Clip
        ModelClip* clip = clips[frameData.cur.clip];
        // 재생 시간 업데이트
        clip->playTime += frameData.cur.scale * DELTA;  
        // 프레임 업데이트
        frameData.cur.time += clip->tickPerSecond * frameData.cur.scale * DELTA;

        // 현재 프레임 % 최대 프레임
        if (frameData.cur.time >= 1.0f)
        {
            frameData.cur.curFrame = (frameData.cur.curFrame + 1) % (clip->frameCount - 1);
            frameData.cur.time -= 1.0f;
            
            clip->SetLoop();
        }

        clip->Excute();
    }

    {// Next Clip
        // 다음에 재생할 애니메이션이 대기중일 때
        if (frameData.next.clip < 0) return;

        ModelClip* clip = clips[frameData.next.clip];
        // 전환 시간
        frameData.tweenTime += DELTA / frameData.takeTime;

        // 전환 시간이 다 되었을 때 클립 바꿔줌
        if (frameData.tweenTime >= 1.0f)
        {
            frameData.cur = frameData.next;
            frameData.tweenTime = 0.0f;

            frameData.next.clip = -1;
            frameData.next.curFrame = 0;
            frameData.next.time = 0.0f;
            return;
        }        
        // 프레임 업데이트
        frameData.next.time += clip->tickPerSecond * frameData.next.scale * DELTA;

        if (frameData.next.time >= 1.0f)
        {
            frameData.next.curFrame = (frameData.next.curFrame + 1) % (clip->frameCount - 1);
            frameData.next.time -= 1.0f;
        }
    }
}
