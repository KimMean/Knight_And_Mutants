#pragma once

// 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// 외부 MFC 오버헤더를 포함하지 않는다. 게임 프로그래밍을 할 땐 MFC를 쓰지 않는다.
#define WIN32_LEAN_AND_MEAN

#define WIN_START_X 0
#define WIN_START_Y 0
#define WIN_WIDTH 1824
#define WIN_HEIGHT 1026

#define CENTER_X (WIN_WIDTH * 0.5f)
#define CENTER_Y (WIN_HEIGHT * 0.5f)

#define MAX_LIGHT 10
#define MAX_BONE 512
#define MAX_FRAME 1024
#define MAX_INSTANCE 30


#define DEVICE Device::Get()->GetDevice()
#define DC Device::Get()->GetDeviceContext()

#define DELTA Timer::Get()->GetElapsedTime()

#define KEY_DOWN(k) Keyboard::Get()->Down(k)
#define KEY_UP(k) Keyboard::Get()->Up(k)
#define KEY_PRESS(k) Keyboard::Get()->Press(k)

#define CAM Environment::Get()->GetMainCamera()

#define DIALOG ImGuiFileDialog::Instance()

#include <Windows.h>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

// DirectX11
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <DirectXCollision.h>	// triangleTests
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTex.lib")
#pragma comment(lib, "dxguid.lib")

#include <d2d1_2.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

// ImGui
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <ImGuiFileDialog.h>

#pragma comment(lib, "ImGui.lib")

// Assimp
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

#pragma comment(lib, "assimp-vc143-mtd.lib")

// FMOD
#include <fmod.hpp>

#pragma comment(lib, "fmod_vc.lib")

using namespace std;
using namespace DirectX;
using namespace DirectX::TriangleTests; // triangleTests는 삼각형과 관련된 함수들을 다룸

typedef XMFLOAT2 Float2;
typedef XMFLOAT3 Float3;
typedef XMFLOAT4 Float4;
typedef XMVECTOR Vector4;
typedef XMMATRIX Matrix;
typedef XMFLOAT4X4 Float4x4;

#define COLOR_WHITE Float4(1.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_BLACK Float4(0.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_GRAY Float4(0.5f, 0.5f, 0.5f, 1.0f)
#define COLOR_RED Float4(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_GREEN Float4(0.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_BLUE Float4(0.0f, 0.0f, 1.0f, 1.0f)

typedef function<void()> Event;

// Utility
#include "Framework/Utilities/Singleton.h"
#include "Framework/Utilities/BinaryReader.h"
#include "Framework/Utilities/BinaryWriter.h"
#include "Framework/Utilities/Utility.h"
#include "Framework/Utilities/Keyboard.h"
#include "Framework/Utilities/Timer.h"
#include "Framework/Utilities/tinyxml2.h"
#include "Framework/Utilities/Audio.h"
#include "Framework/Utilities/Font.h"

using namespace Utility;

// System
#include "Framework/System/Device.h"


// Buffer
#include "Framework/Buffer/VertexBuffer.h"
#include "Framework/Buffer/IndexBuffer.h"
#include "Framework/Buffer/ConstBuffer.h"
#include "Framework/Buffer/GlobalBuffer.h"
#include "Framework/Buffer/ComputeBuffer.h"
#include "Framework/Buffer/VertexLayouts.h"


// Shader
#include "Framework/Shader/Shader.h"
#include "Framework/Shader/VertexShader.h"
#include "Framework/Shader/PixelShader.h"
#include "Framework/Shader/ComputeShader.h"
#include "Framework/Shader/GeometryShader.h"
#include "Framework/Shader/HullShader.h"
#include "Framework/Shader/DomainShader.h"


// Math
#include "Framework/Math/Vector2.h"
#include "Framework/Math/Vector3.h"
#include "Framework/Math/Transform.h"
#include "Framework/Math/GameMath.h"

using namespace GameMath;

// State
#include "Framework/State/SamplerState.h"
#include "Framework/State/RasterizerState.h"
#include "Framework/State/DepthStencilState.h"
#include "Framework/State/BlendState.h"

// Render
#include "Framework/Render/Texture.h"
#include "Framework/Render/Material.h"
#include "Framework/Render/Mesh.h"
#include "Framework/Render/DepthStencil.h"
#include "Framework/Render/RenderTarget.h"

// Collision
#include "Framework/Collision/Ray.h"
#include "Framework/Collision/Collider.h"
#include "Framework/Collision/BoxCollider.h"
#include "Framework/Collision/SphereCollider.h"
#include "Framework/Collision/LineCollider.h"
#include "Framework/Collision/CapsuleCollider.h"

#include "Framework/Model/ModelData.h"
#include "Framework/Model/ModelExporter.h"
#include "Framework/Model/ModelMesh.h"
#include "Framework/Model/Model.h"
#include "Framework/Model/ModelClip.h"
#include "Framework/Model/ModelAnimator.h"
#include "Framework/Model/ModelInstancing.h"
#include "Framework/Model/ModelAnimatorInstancing.h"

// Environment
#include "Framework/Environment/Camera.h"
#include "Framework/Environment/DirectLight.h"
#include "Framework/Environment/Shadow.h"
#include "Framework/Environment/Reflection.h"
#include "Framework/Environment/Environment.h"

// Objects
#include "Objects/Basic/GameObject.h"
#include "Objects/Basic/Cube.h"
#include "Objects/Basic/Sphere.h"
#include "Objects/Basic/Quad.h"

#include "Objects/Algorithm/Node.h"
#include "Objects/Algorithm/Heap.h"
#include "Objects/Algorithm/AStar.h"

#include "Objects/Builds/Build.h"

#include "Objects/UI/Slot.h"
#include "Objects/UI/Item.h"
#include "Objects/UI/Inventory.h"
#include "Objects/UI/ProgressBar.h"
#include "Objects/UI/Interaction.h"
#include "Objects/UI/TextBox.h"

#include "Objects/Weapons/Sword.h"
#include "Objects/Weapons/Hammer.h"
#include "Objects/Weapons/Axe.h"
#include "Objects/Weapons/IceAxe.h"

#include "Objects/Characters/Knight.h"
#include "Objects/Characters/Monster.h"
#include "Objects/Characters/Mutant.h"
#include "Objects/Characters/NPC.h"

#include "Objects/Landscape/Tree.h"
#include "Objects/Landscape/Rocks.h"
#include "Objects/Landscape/Water.h"
#include "Objects/Landscape/Terrain.h"
#include "Objects/Landscape/TerrainEditor.h"
#include "Objects/Landscape/SkyBox.h"

#include "Objects/Managers/ModelInstancingManager.h"
#include "Objects/Managers/TreeManager.h"
#include "Objects/Managers/RockManager.h"
#include "Objects/Managers/NPCManager.h"
#include "Objects/Managers/MonsterManager.h"
#include "Objects/Managers/UIManager.h"

// Scene
#include "Scenes/Scene.h"

// Manager
#include "Manager/SceneManager.h"
#include "Manager/GameManager.h"

extern HWND hWnd;
extern Vector3 mousePos;
extern Vector3 screenMousePos;