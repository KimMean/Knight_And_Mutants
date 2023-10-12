#include "Framework.h"
#include "ModelExporter.h"

ModelExporter::ModelExporter()
{
    importer = new Assimp::Importer();
    // 해당 파일에 저장된 뼈대의 피봇(pivot) 정보를 무시하고 뼈대의 전역 위치를 기준으로 계산합니다.
    importer->SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    // 정점의 접선(tangent) 벡터와 이접(binormal) 벡터를 유지합니다.
    importer->SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_TANGENTS_AND_BITANGENTS);


}

ModelExporter::~ModelExporter()
{
    delete importer;
}

void ModelExporter::ReadFile(string modelName, string filePath)
{
    name = modelName;
    // 파일 읽어오기
    // 왼손 좌표계
    // 최대 퀄리티로
    scene = importer->ReadFile(filePath,
        aiProcessPreset_TargetRealtime_MaxQuality | // 모델을 실시간 렌더링에 적합하도록 최적화합니다.
        aiProcess_Triangulate |                 // 3D 모델을 삼각형으로 변환합니다.
        aiProcess_GenSmoothNormals |            // 각 면의 법선 벡터를 계산하여 부드러운 노말을 생성합니다.
        aiProcess_FixInfacingNormals |          // 보정이 필요한 노말 벡터를 수정합니다.
        aiProcess_RemoveRedundantMaterials |    // 사용되지 않는 재질을 제거합니다.
        aiProcess_OptimizeMeshes |              // 메시 최적화를 수행합니다.
        aiProcess_ValidateDataStructure |       // 데이터 구조를 유효성 검사합니다.
        aiProcess_ImproveCacheLocality |        // 캐시 효율을 향상시킵니다.
        aiProcess_JoinIdenticalVertices |       // 동일한 버텍스를 하나의 버텍스로 합칩니다.
        aiProcess_FindInvalidData |             // 유효하지 않은 데이터를 찾습니다.
        aiProcess_TransformUVCoords |           // UV 좌표를 변환합니다.
        aiProcess_FlipUVs |                     // UV 좌표를 상하반전합니다.
        aiProcess_ConvertToLeftHanded           // 왼손 좌표계로 변환합니다.
    );

    assert(scene != nullptr);
}


// 메쉬 추출
void ModelExporter::ExportMesh()
{
    ReadNode(scene->mRootNode, -1, -1); // 루트노드부터 노드 데이터 저장
    ReadMesh(scene->mRootNode);         // 
    WriteMesh();
}

// 메테리얼 추출
void ModelExporter::ExportMaterial()
{
    ReadMaterial();
    WriteMaterial();
}

// 클립 추출
void ModelExporter::ExportClip(string clipName)
{
    for (int i = 0; i < scene->mNumAnimations; i++)
    {
        Clip* clip = ReadClip(scene->mAnimations[i]);
        WriterClip(clip, clipName, i);
    }
}

// 노드 정보 읽기
void ModelExporter::ReadNode(aiNode* node, int index, int parent)
{
    // 노드 인덱스, 부모, 이름, 트랜스폼을 가짐
    NodeData* nodeData = new NodeData();
    nodeData->index = index;
    nodeData->parent = parent;
    nodeData->name = node->mName.C_Str();

    // 노드의 로컬 변환 행렬을 저장합니다.
    Matrix matrix(node->mTransformation[0]);
    nodeData->transform = XMMatrixTranspose(matrix);    // 전치행렬 변환

    nodes.push_back(nodeData);

    // 재귀 함수로 돌면서 모든 노드들의 정보를 저장
    for (int i = 0; i < node->mNumChildren; i++)
        ReadNode(node->mChildren[i], nodes.size(), index);
}


// 읽어온 파일의 루트 노드를 받음
void ModelExporter::ReadMesh(aiNode* node)
{
    // 해당 노드의 자식 노드만큼
    for (UINT i = 0; i < node->mNumMeshes; i++) 
    {
        MeshData* mesh = new MeshData();    // 메쉬 데이터 생성
        mesh->name = node->mName.C_Str();   // 해당 노드의 이름

        UINT index = node->mMeshes[i];              // 노드의 인덱스
        aiMesh* srcMesh = scene->mMeshes[index];    // 파일에서 메쉬 정보 가져옴

        mesh->materialIndex = srcMesh->mMaterialIndex;  // 메테리얼 인덱스 가져오기

        // 뼈대 정보를 읽음
        vector<VertexWeights> vertexWeights(srcMesh->mNumVertices);
        ReadBone(srcMesh, vertexWeights);

        UINT verticesCount = srcMesh->mNumVertices;     // 해당 노드의 버텍스 가져옴
        mesh->vertices.resize(verticesCount);
        for (UINT v = 0; v < verticesCount; v++)
        {
            ModelVertex vertex;
            // 모델의 버텍스를 가져옴
            memcpy(&vertex.pos, &srcMesh->mVertices[v], sizeof(Float3));

            // 모델의 텍스쳐 좌표를 가져옴
            if (srcMesh->HasTextureCoords(0))
                memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Float2));

            // 모델의 법선 벡터를 가져옴
            if (srcMesh->HasNormals())
                memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Float3));

            // 모델의 탄젠트 벡터를 가져옴
            if (srcMesh->HasTangentsAndBitangents())
                memcpy(&vertex.tangent, &srcMesh->mTangents[v], sizeof(Float3));

            // 정점 인덱스와 가중치 값 (보간값에 대한 정보)
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

        mesh->indices.resize(srcMesh->mNumFaces * 3);   // 삼각형의 개수를 가져옴
        for (UINT f = 0; f < srcMesh->mNumFaces; f++)
        {
            aiFace& face = srcMesh->mFaces[f];

            // 각각의 트라이앵글은 3개의 인덱스를 가지므로, mNumIndices는 항상 3의 배수가 됩니다.
            for (UINT k = 0; k < face.mNumIndices; k++)
            {
                // 인덱스 입력
                mesh->indices[f * 3 + k] = face.mIndices[k];
            }
        }

        meshes.push_back(mesh);
    }
    // 재귀로 마지막 노드까지 호출
    for (UINT i = 0; i < node->mNumChildren; i++)
        ReadMesh(node->mChildren[i]);
}
// 뼈대 정보 읽기
void ModelExporter::ReadBone(aiMesh* mesh, vector<VertexWeights>& vertexWeights)
{
    // 뼈의 개수 만큼
    for (int i = 0; i < mesh->mNumBones; i++)
    {
        UINT boneIndex = 0;
        string name = mesh->mBones[i]->mName.C_Str();

        // 같은 이름의 뼈대가 없을 경우
        if (boneMap.count(name) == 0)
        {
            boneIndex = boneCount++;
            boneMap[name] = boneIndex;

            // Bone 이름, 인덱스
            BoneData* boneData = new BoneData();
            boneData->name = name;
            boneData->index = boneIndex;

            // 뼈대의 로컬 공간(local space)에서의 변환 행렬
            // 각 뼈대마다 월드좌표로 바꿔줘야 하기 때문
            Matrix matrix(mesh->mBones[i]->mOffsetMatrix[0]);
            boneData->offset = XMMatrixTranspose(matrix);

            bones.push_back(boneData);
        }
        else
        {
            //뼈대가 같으면 같은 인덱스로
            boneIndex = boneMap[name];
        }

        // 뼈의 인덱스와 가중치 값 입력
        for (UINT j = 0; j < mesh->mBones[i]->mNumWeights; j++)
        {
            UINT index = mesh->mBones[i]->mWeights[j].mVertexId;
            vertexWeights[index].AddWeight(boneIndex, mesh->mBones[i]->mWeights[j].mWeight);
        }
    }
}

// 메쉬 정보 저장
void ModelExporter::WriteMesh()
{
    string path = "Models/Meshes/" + name + ".mesh";

    CreateFolders(path);

    BinaryWriter* writer = new BinaryWriter(path);

    // 메쉬 정점정보와 인덱스 정보를 저장
    writer->UInt(meshes.size());
    for (MeshData* mesh : meshes)
    {
        writer->String(mesh->name);         // 이름
        writer->UInt(mesh->materialIndex);  // 메테리얼 인덱스

        writer->UInt(mesh->vertices.size());    // 버텍스 크기
        writer->Byte(mesh->vertices.data(), sizeof(ModelVertex) * mesh->vertices.size());   // 버텍스 정보

        writer->UInt(mesh->indices.size());     // 인덱스 크기
        writer->Byte(mesh->indices.data(), sizeof(UINT) * mesh->indices.size());    // 인덱스 정보

        delete mesh;
    }
    meshes.clear();

    // 노드 인덱스, 네임, 부모, 트랜스폼 정보 저장
    writer->UInt(nodes.size());
    for (NodeData* node : nodes)
    {
        writer->Int(node->index);       // 인덱스
        writer->String(node->name);     // 이름
        writer->Int(node->parent);      // 부모
        writer->Matrix(node->transform);    // 트랜스폼

        delete node;
    }
    nodes.clear();
    // 뼈대 인덱스, 이름, 위치 정보 저장
    writer->UInt(bones.size());
    for (BoneData* bone : bones)
    {
        writer->Int(bone->index);       // 인덱스
        writer->String(bone->name);     // 네임
        writer->Matrix(bone->offset);   // 위치

        delete bone;
    }
    bones.clear();

    delete writer;
}
// Assimp 라이브러리로 메테리얼 정보 받기
void ModelExporter::ReadMaterial()
{
    for (UINT i = 0; i < scene->mNumMaterials; i++)
    {
        // 메테리얼 생성
        aiMaterial* srcMaterial = scene->mMaterials[i];
        Material* material = new Material();

        // 이름 설정
        material->GetName() = srcMaterial->GetName().C_Str();

        aiColor3D color;
        Material::MaterialData& data = material->GetMaterialData();
        // Diffuse 컬러 값 저장
        srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        data.diffuse = Float4(color.r, color.g, color.b, 1.0f);
        // Specular 컬러 값 저장
        srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
        data.specular = Float4(color.r, color.g, color.b, 1.0f);
        // Ambient 컬러 값 저장
        srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
        data.ambient = Float4(color.r, color.g, color.b, 1.0f);
        // Emissive 컬러 값 저장
        srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
        data.emissive = Float4(color.r, color.g, color.b, 1.0f);
        // shininess 값 저장
        srcMaterial->Get(AI_MATKEY_SHININESS, data.shininess);
        //material->GetMaterialData()

        aiString file;
        // DiffuseMap 설정
        srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
        material->SetDiffuseMap(ToWString(CreateTexture(file.C_Str())));
        file.Clear();
        // SpecularMap 설정
        srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
        material->SetSpecularMap(ToWString(CreateTexture(file.C_Str())));
        file.Clear();
        // NormalMap 설정
        srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
        material->SetNormalMap(ToWString(CreateTexture(file.C_Str())));
        file.Clear();

        materials.push_back(material);
    }
}

// 메테리얼 정보 저장
void ModelExporter::WriteMaterial()
{
    string savePath = "Models/Materials/" + name + "/";
    string file = name + ".mats";

    CreateFolders(savePath);

    BinaryWriter* writer = new BinaryWriter(savePath + file);

    writer->UInt(materials.size());

    // 메테리얼 이름, 경로 저장
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

// FBX가 가지고 있는 텍스쳐 파일 추출, 저장
string ModelExporter::CreateTexture(string file)
{
    if (file.length() == 0)
        return "";

    // png파일로
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

        // Png파일 저장
        SaveToWICFile(image, WIC_FLAGS_NONE,
            GetWICCodec(WIC_CODEC_PNG), ToWString(path).c_str());
    }

    return path;
}

Clip* ModelExporter::ReadClip(aiAnimation* animation)
{
    // 클립 생성
    Clip* clip = new Clip();
    clip->name = animation->mName.C_Str();      // 이름
    clip->tickPerSecond = (float)animation->mTicksPerSecond;    // 초당 틱 수
    clip->frameCount = (UINT)(animation->mDuration) + 1;        // 재생 길이

    // 클립 노드 생성
    vector<ClipNode> clipNodes;
    clipNodes.reserve(animation->mNumChannels);
    for (int i = 0; i < animation->mNumChannels; i++)
    {
        // Animation Node 생성
        aiNodeAnim* srcNode = animation->mChannels[i];

        ClipNode node;  // 클립 노드 생성
        node.name = srcNode->mNodeName;

        KeyData data;   // 키 데이터 생성

        // Vector3 Position값 저장
        data.positions.resize(srcNode->mNumPositionKeys);
        for (UINT k = 0; k < srcNode->mNumPositionKeys; k++)
        {
            KeyVector keyPos;
            aiVectorKey key = srcNode->mPositionKeys[k];
            keyPos.time = key.mTime;
            memcpy_s(&keyPos.value, sizeof(Float3), &key.mValue, sizeof(aiVector3D));

            data.positions[k] = keyPos;
        }

        // Vector4 Rotation 값 저장
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
        // Vector3 Scale 값 저장
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
