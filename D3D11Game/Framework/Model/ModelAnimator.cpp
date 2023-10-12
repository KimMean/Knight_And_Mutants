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

// Ŭ�� �б�
void ModelAnimator::ReadClip(string clipName, UINT clipNum, string lockBone)
{
    string path = "Models/Clips/" + name + "/"
        + clipName + to_string(clipNum) + ".clip";

    BinaryReader* reader = new BinaryReader(path);

    ModelClip* clip = new ModelClip();
    clip->name = reader->String();          // �̸�
    clip->lockBone = lockBone;              // �����̴� �ִϸ��̼� ���� ���� ����
    clip->frameCount = reader->UInt();      // ������ ��
    clip->tickPerSecond = reader->Float();  // �ʴ� ƽ

    // ���븶�� Ű �������� �� ������
    UINT boneCount = reader->UInt();        // Bone ����
    for(int i = 0; i < boneCount; i++)
    {
        KeyFrame* keyFrame = new KeyFrame();    // Ű ������
        keyFrame->boneName = reader->String();  // Bone �̸�

        UINT size = reader->UInt();             // KeyFrame ũ��
        if (size > 0)
        {
            keyFrame->transforms.resize(size);
            // Ű ������ ���� ������
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
    // ���� Ŭ�� �־���� ��ȯ�ð� ����
    frameBuffer->Get().next.clip = clip;
    frameBuffer->Get().next.scale = scale;
    frameBuffer->Get().takeTime = takeTime;

    clips[clip]->Init();
}

// ��忡�� �ش� Ʈ������ ã��
Matrix ModelAnimator::GetTransformByNode(int nodeIndex)
{
    if (texture == nullptr) return XMMatrixIdentity();

    Matrix curAnim;

    {//CurAnim
        Frame& curFrame = frameBuffer->Get().cur;   // ���� ������
        // ���� �������� ��ġ�� ���� ������ ��ġ�� ������
        Matrix cur = nodeTransforms[curFrame.clip].transform[curFrame.curFrame][nodeIndex];
        Matrix next = nodeTransforms[curFrame.clip].transform[curFrame.curFrame + 1][nodeIndex];
        // �ִϸ��̼��� ����� ������ ������ ���� �������� ����
        curAnim = Lerp(cur, next, curFrame.time) * transform->GetWorld();
    }

    {//NextAnim
        Frame& nextFrame = frameBuffer->Get().next;
        // ��ȯ���� �ִϸ��̼��� ������ ����
        if (nextFrame.clip == -1)
            return curAnim;
        // ���� �ִϸ��̼��� ������ ��ġ�� ���
        Matrix cur = nodeTransforms[nextFrame.clip].transform[nextFrame.curFrame][nodeIndex];
        Matrix next = nodeTransforms[nextFrame.clip].transform[nextFrame.curFrame + 1][nodeIndex];

        Matrix nextAnim = Lerp(cur, next, nextFrame.time) * transform->GetWorld();
        // ���� �ִϸ��̼ǰ� ��ȯ�� �ִϸ��̼��� ���̰��� ���ؼ� ����
        return Lerp(curAnim, nextAnim, frameBuffer->Get().tweenTime);
    }    
}
// �ִϸ��̼� ������ �̹����� ����
void ModelAnimator::CreateTexture()
{
    UINT clipCount = clips.size();

    // Matrix 2��Ʈ
    clipTransforms = new ClipTransform[clipCount];
    nodeTransforms = new ClipTransform[clipCount];

    // Ʈ������ ��ġ
    for(int i = 0 ; i < clipCount; i++)
        CreateClipTransform(i);

    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = MAX_BONE * 4;      // ���� * 4 
    desc.Height = MAX_FRAME;        // �����Ӹ�ŭ
    desc.MipLevels = 1; 
    desc.ArraySize = clipCount;     // Ŭ�� ��
    desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_IMMUTABLE;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    UINT pitchSize = MAX_BONE * sizeof(Matrix); // Bone���� transform�� �����ϱ� ����
    UINT pageSize = pitchSize * MAX_FRAME;      // 

    void* p = VirtualAlloc(nullptr, pageSize * clipCount,
        MEM_RESERVE, PAGE_READWRITE);   // ���� �޸� ���

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
    // �ش� �ε��� Ŭ�� ��������
    ModelClip* clip = clips[index];

    for (int i = 0; i < clip->frameCount; i++)
    {
        UINT nodeIndex = 0;
        // �ش� �������� ���� ����
        for (NodeData node : nodes)
        {
            Matrix animation;
            // ��� �������� Ű �������� ������
            KeyFrame* frame = clip->GetKeyFrame(node.name);
            if (frame != nullptr)
            {
                // ���� ��ġ ������
                KeyTransform& transform = frame->transforms[i];
                // ��� ��ġ�� �����ϰ� ���� ��
                if (node.name == clip->lockBone)
                {
                    transform.pos = {};
                }
                // 3D ��ü�� ��Ʈ���� ��ȯ 
                animation = XMMatrixTransformation(
                    XMVectorZero(),         // ���� (0, 0, 0)�� ���
                    XMQuaternionIdentity(), // ���� (0, 0, 0, 1)�� ���ʹϾ�
                    Vector3(transform.scale),   // ������ ��
                    XMVectorZero(),         // �߽� ȸ�� ��
                    XMLoadFloat4(&transform.rot),   // ȸ�� ��
                    Vector3(transform.pos)  // �̵� ��
                );
            }
            else
            {
                animation = XMMatrixIdentity(); // �⺻ ���
            }

            Matrix parent;
            int parentIndex = node.parent;  // �θ� �ε���

            if (parentIndex < 0)
                parent = XMMatrixIdentity();    // �θ� ������ �⺻ ���
            else
                parent = nodeTransforms[index].transform[i][parentIndex];
            // ��Ŀ� �θ� ����� ���ؼ� ����
            nodeTransforms[index].transform[i][nodeIndex] = animation * parent;

            int boneIndex = -1;
            if (boneMap.count(node.name))
                boneIndex = boneMap[node.name];
            // ���� ���밡 ���� ��
            if (boneIndex >= 0)
            {
                // ���� ����� ���ϹǷν� ������ ��ġ�� �ְ� ��
                Matrix offset = bones[boneIndex].offset;
                offset *= nodeTransforms[index].transform[i][nodeIndex];

                clipTransforms[index].transform[i][boneIndex] = offset;
            }
            nodeIndex++;
        }
    }
}
// ������ ������Ʈ
void ModelAnimator::UpdateFrame()
{
    if (!isPlay) return;

    FrameBuffer::Data& frameData = frameBuffer->Get();

    {// Cur Clip
        ModelClip* clip = clips[frameData.cur.clip];
        // ��� �ð� ������Ʈ
        clip->playTime += frameData.cur.scale * DELTA;  
        // ������ ������Ʈ
        frameData.cur.time += clip->tickPerSecond * frameData.cur.scale * DELTA;

        // ���� ������ % �ִ� ������
        if (frameData.cur.time >= 1.0f)
        {
            frameData.cur.curFrame = (frameData.cur.curFrame + 1) % (clip->frameCount - 1);
            frameData.cur.time -= 1.0f;
            
            clip->SetLoop();
        }

        clip->Excute();
    }

    {// Next Clip
        // ������ ����� �ִϸ��̼��� ������� ��
        if (frameData.next.clip < 0) return;

        ModelClip* clip = clips[frameData.next.clip];
        // ��ȯ �ð�
        frameData.tweenTime += DELTA / frameData.takeTime;

        // ��ȯ �ð��� �� �Ǿ��� �� Ŭ�� �ٲ���
        if (frameData.tweenTime >= 1.0f)
        {
            frameData.cur = frameData.next;
            frameData.tweenTime = 0.0f;

            frameData.next.clip = -1;
            frameData.next.curFrame = 0;
            frameData.next.time = 0.0f;
            return;
        }        
        // ������ ������Ʈ
        frameData.next.time += clip->tickPerSecond * frameData.next.scale * DELTA;

        if (frameData.next.time >= 1.0f)
        {
            frameData.next.curFrame = (frameData.next.curFrame + 1) % (clip->frameCount - 1);
            frameData.next.time -= 1.0f;
        }
    }
}