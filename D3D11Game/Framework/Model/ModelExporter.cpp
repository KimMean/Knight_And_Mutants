#include "Framework.h"
#include "ModelExporter.h"

ModelExporter::ModelExporter()
{
    importer = new Assimp::Importer();
    // �ش� ���Ͽ� ����� ������ �Ǻ�(pivot) ������ �����ϰ� ������ ���� ��ġ�� �������� ����մϴ�.
    importer->SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    // ������ ����(tangent) ���Ϳ� ����(binormal) ���͸� �����մϴ�.
    importer->SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_TANGENTS_AND_BITANGENTS);


}

ModelExporter::~ModelExporter()
{
    delete importer;
}

void ModelExporter::ReadFile(string modelName, string filePath)
{
    name = modelName;
    // ���� �о����
    // �޼� ��ǥ��
    // �ִ� ����Ƽ��
    scene = importer->ReadFile(filePath,
        aiProcessPreset_TargetRealtime_MaxQuality | // ���� �ǽð� �������� �����ϵ��� ����ȭ�մϴ�.
        aiProcess_Triangulate |                 // 3D ���� �ﰢ������ ��ȯ�մϴ�.
        aiProcess_GenSmoothNormals |            // �� ���� ���� ���͸� ����Ͽ� �ε巯�� �븻�� �����մϴ�.
        aiProcess_FixInfacingNormals |          // ������ �ʿ��� �븻 ���͸� �����մϴ�.
        aiProcess_RemoveRedundantMaterials |    // ������ �ʴ� ������ �����մϴ�.
        aiProcess_OptimizeMeshes |              // �޽� ����ȭ�� �����մϴ�.
        aiProcess_ValidateDataStructure |       // ������ ������ ��ȿ�� �˻��մϴ�.
        aiProcess_ImproveCacheLocality |        // ĳ�� ȿ���� ����ŵ�ϴ�.
        aiProcess_JoinIdenticalVertices |       // ������ ���ؽ��� �ϳ��� ���ؽ��� ��Ĩ�ϴ�.
        aiProcess_FindInvalidData |             // ��ȿ���� ���� �����͸� ã���ϴ�.
        aiProcess_TransformUVCoords |           // UV ��ǥ�� ��ȯ�մϴ�.
        aiProcess_FlipUVs |                     // UV ��ǥ�� ���Ϲ����մϴ�.
        aiProcess_ConvertToLeftHanded           // �޼� ��ǥ��� ��ȯ�մϴ�.
    );

    assert(scene != nullptr);
}


// �޽� ����
void ModelExporter::ExportMesh()
{
    ReadNode(scene->mRootNode, -1, -1); // ��Ʈ������ ��� ������ ����
    ReadMesh(scene->mRootNode);         // 
    WriteMesh();
}

// ���׸��� ����
void ModelExporter::ExportMaterial()
{
    ReadMaterial();
    WriteMaterial();
}

// Ŭ�� ����
void ModelExporter::ExportClip(string clipName)
{
    for (int i = 0; i < scene->mNumAnimations; i++)
    {
        Clip* clip = ReadClip(scene->mAnimations[i]);
        WriterClip(clip, clipName, i);
    }
}

// ��� ���� �б�
void ModelExporter::ReadNode(aiNode* node, int index, int parent)
{
    // ��� �ε���, �θ�, �̸�, Ʈ�������� ����
    NodeData* nodeData = new NodeData();
    nodeData->index = index;
    nodeData->parent = parent;
    nodeData->name = node->mName.C_Str();

    // ����� ���� ��ȯ ����� �����մϴ�.
    Matrix matrix(node->mTransformation[0]);
    nodeData->transform = XMMatrixTranspose(matrix);    // ��ġ��� ��ȯ

    nodes.push_back(nodeData);

    // ��� �Լ��� ���鼭 ��� ������ ������ ����
    for (int i = 0; i < node->mNumChildren; i++)
        ReadNode(node->mChildren[i], nodes.size(), index);
}


// �о�� ������ ��Ʈ ��带 ����
void ModelExporter::ReadMesh(aiNode* node)
{
    // �ش� ����� �ڽ� ��常ŭ
    for (UINT i = 0; i < node->mNumMeshes; i++) 
    {
        MeshData* mesh = new MeshData();    // �޽� ������ ����
        mesh->name = node->mName.C_Str();   // �ش� ����� �̸�

        UINT index = node->mMeshes[i];              // ����� �ε���
        aiMesh* srcMesh = scene->mMeshes[index];    // ���Ͽ��� �޽� ���� ������

        mesh->materialIndex = srcMesh->mMaterialIndex;  // ���׸��� �ε��� ��������

        // ���� ������ ����
        vector<VertexWeights> vertexWeights(srcMesh->mNumVertices);
        ReadBone(srcMesh, vertexWeights);

        UINT verticesCount = srcMesh->mNumVertices;     // �ش� ����� ���ؽ� ������
        mesh->vertices.resize(verticesCount);
        for (UINT v = 0; v < verticesCount; v++)
        {
            ModelVertex vertex;
            // ���� ���ؽ��� ������
            memcpy(&vertex.pos, &srcMesh->mVertices[v], sizeof(Float3));

            // ���� �ؽ��� ��ǥ�� ������
            if (srcMesh->HasTextureCoords(0))
                memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Float2));

            // ���� ���� ���͸� ������
            if (srcMesh->HasNormals())
                memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Float3));

            // ���� ź��Ʈ ���͸� ������
            if (srcMesh->HasTangentsAndBitangents())
                memcpy(&vertex.tangent, &srcMesh->mTangents[v], sizeof(Float3));

            // ���� �ε����� ����ġ �� (�������� ���� ����)
            if (!vertexWeights.empty())
            {
                vertexWeights[i].Normalize();

                vertex.indices.x = (float)vertexWeights[v].indices[0];
                vertex.indices.y = (float)vertexWeights[v].indices[1];
                vertex.indices.z = (float)vertexWeights[v].indices[2];
                vertex.indices.w = (float)vertexWeights[v].indices[3];

                vertex.weights.x = vertexWeights[v].weights[0];
                vertex.weights.y = vertexWeights[v].weights[1];
                vertex.weights.z = vertexWeights[v].weights[2];
                vertex.weights.w = vertexWeights[v].weights[3];
            }

            mesh->vertices[v] = vertex;
        }

        mesh->indices.resize(srcMesh->mNumFaces * 3);   // �ﰢ���� ������ ������
        for (UINT f = 0; f < srcMesh->mNumFaces; f++)
        {
            aiFace& face = srcMesh->mFaces[f];

            // ������ Ʈ���̾ޱ��� 3���� �ε����� �����Ƿ�, mNumIndices�� �׻� 3�� ����� �˴ϴ�.
            for (UINT k = 0; k < face.mNumIndices; k++)
            {
                // �ε��� �Է�
                mesh->indices[f * 3 + k] = face.mIndices[k];
            }
        }

        meshes.push_back(mesh);
    }
    // ��ͷ� ������ ������ ȣ��
    for (UINT i = 0; i < node->mNumChildren; i++)
        ReadMesh(node->mChildren[i]);
}
// ���� ���� �б�
void ModelExporter::ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights)
{
    // ���� ���� ��ŭ
    for (int i = 0; i < mesh->mNumBones; i++)
    {
        UINT boneIndex = 0;
        string name = mesh->mBones[i]->mName.C_Str();

        // ���� �̸��� ���밡 ���� ���
        if (boneMap.count(name) == 0)
        {
            boneIndex = boneCount++;
            boneMap[name] = boneIndex;

            // Bone �̸�, �ε���
            BoneData* boneData = new BoneData();
            boneData->name = name;
            boneData->index = boneIndex;

            // ������ ���� ����(local space)������ ��ȯ ���
            // �� ���븶�� ������ǥ�� �ٲ���� �ϱ� ����
            Matrix matrix(mesh->mBones[i]->mOffsetMatrix[0]);
            boneData->offset = XMMatrixTranspose(matrix);

            bones.push_back(boneData);
        }
        else
        {
            //���밡 ������ ���� �ε�����
            boneIndex = boneMap[name];
        }

        // ���� �ε����� ����ġ �� �Է�
        for (UINT j = 0; j < mesh->mBones[i]->mNumWeights; j++)
        {
            UINT index = mesh->mBones[i]->mWeights[j].mVertexId;
            vertexWeights[index].AddWeight(boneIndex, mesh->mBones[i]->mWeights[j].mWeight);
        }
    }
}

// �޽� ���� ����
void ModelExporter::WriteMesh()
{
    string path = "Models/Meshes/" + name + ".mesh";

    CreateFolders(path);

    BinaryWriter* writer = new BinaryWriter(path);

    // �޽� ���������� �ε��� ������ ����
    writer->UInt(meshes.size());
    for (MeshData* mesh : meshes)
    {
        writer->String(mesh->name);         // �̸�
        writer->UInt(mesh->materialIndex);  // ���׸��� �ε���

        writer->UInt(mesh->vertices.size());    // ���ؽ� ũ��
        writer->Byte(mesh->vertices.data(), sizeof(ModelVertex) * mesh->vertices.size());   // ���ؽ� ����

        writer->UInt(mesh->indices.size());     // �ε��� ũ��
        writer->Byte(mesh->indices.data(), sizeof(UINT) * mesh->indices.size());    // �ε��� ����

        delete mesh;
    }
    meshes.clear();

    // ��� �ε���, ����, �θ�, Ʈ������ ���� ����
    writer->UInt(nodes.size());
    for (NodeData* node : nodes)
    {
        writer->Int(node->index);       // �ε���
        writer->String(node->name);     // �̸�
        writer->Int(node->parent);      // �θ�
        writer->Matrix(node->transform);    // Ʈ������

        delete node;
    }
    nodes.clear();
    // ���� �ε���, �̸�, ��ġ ���� ����
    writer->UInt(bones.size());
    for (BoneData* bone : bones)
    {
        writer->Int(bone->index);       // �ε���
        writer->String(bone->name);     // ����
        writer->Matrix(bone->offset);   // ��ġ

        delete bone;
    }
    bones.clear();

    delete writer;
}
// Assimp ���̺귯���� ���׸��� ���� �ޱ�
void ModelExporter::ReadMaterial()
{
    for (UINT i = 0; i < scene->mNumMaterials; i++)
    {
        // ���׸��� ����
        aiMaterial* srcMaterial = scene->mMaterials[i];
        Material* material = new Material();

        // �̸� ����
        material->GetName() = srcMaterial->GetName().C_Str();

        aiColor3D color;
        Material::MaterialData& data = material->GetMaterialData();
        // Diffuse �÷� �� ����
        srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        data.diffuse = Float4(color.r, color.g, color.b, 1.0f);
        // Specular �÷� �� ����
        srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
        data.specular = Float4(color.r, color.g, color.b, 1.0f);
        // Ambient �÷� �� ����
        srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
        data.ambient = Float4(color.r, color.g, color.b, 1.0f);
        // Emissive �÷� �� ����
        srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
        data.emissive = Float4(color.r, color.g, color.b, 1.0f);
        // shininess �� ����
        srcMaterial->Get(AI_MATKEY_SHININESS, data.shininess);
        //material->GetMaterialData()

        aiString file;
        // DiffuseMap ����
        srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
        material->SetDiffuseMap(ToWString(CreateTexture(file.C_Str())));
        file.Clear();
        // SpecularMap ����
        srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
        material->SetSpecularMap(ToWString(CreateTexture(file.C_Str())));
        file.Clear();
        // NormalMap ����
        srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
        material->SetNormalMap(ToWString(CreateTexture(file.C_Str())));
        file.Clear();

        materials.push_back(material);
    }
}

// ���׸��� ���� ����
void ModelExporter::WriteMaterial()
{
    string savePath = "Models/Materials/" + name + "/";
    string file = name + ".mats";

    CreateFolders(savePath);

    BinaryWriter* writer = new BinaryWriter(savePath + file);

    writer->UInt(materials.size());

    // ���׸��� �̸�, ��� ����
    for (Material* material : materials)
    {
        string path = savePath + material->GetName() + ".mat";
        material->SaveMaterial(path);

        writer->String(path);

        delete material;
    }

    materials.clear();

    delete writer;
}

// FBX�� ������ �ִ� �ؽ��� ���� ����, ����
string ModelExporter::CreateTexture(string file)
{
    if (file.length() == 0)
        return "";

    // png���Ϸ�
    string fileName = GetFileNameWithoutExtension(file) + ".png";
    string path = "Textures/Model/" + name + "/" + fileName;

    CreateFolders(path);

    const aiTexture* texture = scene->GetEmbeddedTexture(file.c_str());

    if (texture == nullptr)
        return "";

    if (texture->mHeight < 1)
    {
        BinaryWriter w(path);
        w.Byte(texture->pcData, texture->mWidth);
    }
    else
    {
        Image image;
        image.width = texture->mWidth;
        image.height = texture->mHeight;
        image.pixels = (uint8_t*)(texture->pcData);
        image.rowPitch = image.width * 4;
        image.slicePitch = image.width * image.height * 4;

        // Png���� ����
        SaveToWICFile(image, WIC_FLAGS_NONE,
            GetWICCodec(WIC_CODEC_PNG), ToWString(path).c_str());
    }

    return path;
}

Clip* ModelExporter::ReadClip(aiAnimation* animation)
{
    // Ŭ�� ����
    Clip* clip = new Clip();
    clip->name = animation->mName.C_Str();      // �̸�
    clip->tickPerSecond = (float)animation->mTicksPerSecond;    // �ʴ� ƽ ��
    clip->frameCount = (UINT)(animation->mDuration) + 1;        // ��� ����

    // Ŭ�� ��� ����
    vector<ClipNode> clipNodes;
    clipNodes.reserve(animation->mNumChannels);
    for (int i = 0; i < animation->mNumChannels; i++)
    {
        // Animation Node ����
        aiNodeAnim* srcNode = animation->mChannels[i];

        ClipNode node;  // Ŭ�� ��� ����
        node.name = srcNode->mNodeName;

        KeyData data;   // Ű ������ ����

        // Vector3 Position�� ����
        data.positions.resize(srcNode->mNumPositionKeys);
        for (UINT k = 0; k < srcNode->mNumPositionKeys; k++)
        {
            KeyVector keyPos;
            aiVectorKey key = srcNode->mPositionKeys[k];
            keyPos.time = key.mTime;
            memcpy_s(&keyPos.value, sizeof(Float3), &key.mValue, sizeof(aiVector3D));

            data.positions[k] = keyPos;
        }

        // Vector4 Rotation �� ����
        data.rotations.resize(srcNode->mNumRotationKeys);
        for (UINT k = 0; k < srcNode->mNumRotationKeys; k++)
        {
            KeyQuat keyRot;
            aiQuatKey key = srcNode->mRotationKeys[k];
            keyRot.time = key.mTime;

            keyRot.value.x = (float)key.mValue.x;
            keyRot.value.y = (float)key.mValue.y;
            keyRot.value.z = (float)key.mValue.z;
            keyRot.value.w = (float)key.mValue.w;

            data.rotations[k] = keyRot;
        }
        // Vector3 Scale �� ����
        data.scales.resize(srcNode->mNumScalingKeys);
        for (UINT k = 0; k < srcNode->mNumScalingKeys; k++)
        {
            KeyVector keyScale;
            aiVectorKey key = srcNode->mScalingKeys[k];
            keyScale.time = key.mTime;
            memcpy_s(&keyScale.value, sizeof(Float3), &key.mValue, sizeof(aiVector3D));

            data.scales[k] = keyScale;
        }

        SetClipNode(data, clip->frameCount, node);

        clipNodes.push_back(node);
    }

    ReadKeyFrame(clip, scene->mRootNode, clipNodes);

    return clip;
}

void ModelExporter::ReadKeyFrame(Clip* clip, aiNode* node, vector<ClipNode>& clipNodes)
{
    KeyFrame* keyFrame = new KeyFrame();
    keyFrame->boneName = node->mName.C_Str();
    keyFrame->transforms.reserve(clip->frameCount);
    for (int i = 0; i < clip->frameCount; i++)
    {
        ClipNode* clipNode = nullptr;
        for (ClipNode& temp : clipNodes)
        {
            if (temp.name == node->mName)
            {
                clipNode = &temp;
                break;
            }
        }

        KeyTransform keyTransform;
        if (clipNode == nullptr)
        {
            Matrix transform(node->mTransformation[0]);
            transform = XMMatrixTranspose(transform);

            Vector3 S, R, T;
            XMMatrixDecompose(S.GetValue(), R.GetValue(), T.GetValue(), transform);
            keyTransform.scale = S;
            XMStoreFloat4(&keyTransform.rot, R);
            keyTransform.pos = T;
        }
        else
        {
            keyTransform = clipNode->transforms[i];
        }

        keyFrame->transforms.push_back(keyTransform);
    }

    clip->keyFrame.push_back(keyFrame);

    for (int i = 0; i < node->mNumChildren; i++)
        ReadKeyFrame(clip, node->mChildren[i], clipNodes);
}

void ModelExporter::WriterClip(Clip* clip, string clipName, UINT index)
{
    string file = "Models/Clips/" + name + "/"
        + clipName + to_string(index) + ".clip";

    CreateFolders(file);

    BinaryWriter* writer = new BinaryWriter(file);
    writer->String(clip->name);
    writer->UInt(clip->frameCount);
    writer->Float(clip->tickPerSecond);

    writer->UInt(clip->keyFrame.size());
    for (KeyFrame* keyFrame : clip->keyFrame)
    {
        writer->String(keyFrame->boneName);
        writer->UInt(keyFrame->transforms.size());
        writer->Byte(keyFrame->transforms.data(),
            sizeof(KeyTransform) * keyFrame->transforms.size());

        delete keyFrame;
    }

    delete clip;
    delete writer;
}

void ModelExporter::SetClipNode(const KeyData& keyData, const UINT& frameCount, ClipNode& clipNode)
{
    clipNode.transforms.resize(frameCount);

    UINT posCount = 0;
    UINT rotCount = 0;
    UINT scaleCount = 0;

    for (int i = 0; i < frameCount; i++)
    {
        clipNode.transforms[i].pos = CalcInterpolationVector(keyData.positions, posCount, i);
        clipNode.transforms[i].rot = CalcInterpolationQuat(keyData.rotations, rotCount, i);
        clipNode.transforms[i].scale = CalcInterpolationVector(keyData.scales, scaleCount, i);
    }
}

Float3 ModelExporter::CalcInterpolationVector(const vector<KeyVector>& keyData, UINT& count, int curFrame)
{
    if (keyData.size() <= count || keyData.size() == 1)
    {
        return keyData.back().value;
    }

    KeyVector curValue = keyData[count];
    KeyVector nextValue = curValue;
    if (keyData.size() > count + 1)
        nextValue = keyData[count + 1];

    float t = ((float)curFrame - curValue.time) / (nextValue.time - curValue.time);

    if (curFrame == (int)nextValue.time)
        count++;

    return Lerp(curValue.value, nextValue.value, t);
}

Float4 ModelExporter::CalcInterpolationQuat(const vector<KeyQuat>& keyData, UINT& count, int curFrame)
{
    if (keyData.size() <= count || keyData.size() == 1)
    {
        return keyData.back().value;
    }

    KeyQuat curValue = keyData[count];
    KeyQuat nextValue = curValue;
    if (keyData.size() > count + 1)
        nextValue = keyData[count + 1];

    float t = ((float)curFrame - curValue.time) / (nextValue.time - curValue.time);

    if (curFrame == (int)nextValue.time)
        count++;

    XMVECTOR cur = XMLoadFloat4(&curValue.value);
    XMVECTOR next = XMLoadFloat4(&nextValue.value);

    XMVECTOR rot = XMQuaternionSlerp(cur, next, t);

    Float4 result;
    XMStoreFloat4(&result, rot);

    return result;
}
