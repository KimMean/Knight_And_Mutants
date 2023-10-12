#include "Framework.h"

//#include "Scenes/InitializedScene.h"
//#include "Scenes/QuadScene.h"
#include "Scenes/DebugScene.h"
#include "Scenes/GridScene.h"
#include "Scenes/SkyScene.h"
#include "Scenes/TerrainEditorScene.h"
#include "Scenes/ModelExportScene.h"
#include "Scenes/ModelRenderScene.h"
#include "Scenes/GameScene.h"

GameManager::GameManager()
{
    Create();

    Environment::Get()->SetViewport();  // 뷰포트 설정
    // 현재 UI 패널을 안쓰고 있으니 여기서 설정
    Environment::Get()->SetPerspective();   // 원근 투영행렬 설정

    SceneManager::Get()->Create("Debug", new DebugScene());
    //SceneManager::Get()->Create("SkyBox", new SkyScene());
    //SceneManager::Get()->Create("Grid", new GridScene());
    //SceneManager::Get()->Create("TerrainEditor", new TerrainEditorScene());
    //SceneManager::Get()->Create("ModelExport", new ModelExportScene());
    //SceneManager::Get()->Create("ModelRender", new ModelRenderScene());
    //SceneManager::Get()->Create("Start", new QuadScene());
    SceneManager::Get()->Create("Game", new GameScene());
    

    SceneManager::Get()->Add("Debug");
    //SceneManager::Get()->Add("SkyBox");
    //SceneManager::Get()->Add("Grid");
    //SceneManager::Get()->Add("TerrainEditor");
    //SceneManager::Get()->Add("ModelExport");
    //SceneManager::Get()->Add("ModelRender");
    //SceneManager::Get()->Add("Start");
    SceneManager::Get()->Add("Game");
    
}

GameManager::~GameManager()
{
    Delete();
}

void GameManager::Update()
{
    Keyboard::Get()->Update();
    Timer::Get()->Update();

    //scene->Update();
    SceneManager::Get()->Update();

    Environment::Get()->Update();
    Audio::Get()->Update();
}

void GameManager::Render()
{
    SceneManager::Get()->PreRender();

    Device::Get()->Clear();         // 화면 클리어

    Font::Get()->GetDC()->BeginDraw();

    Environment::Get()->SetState(); // Default State 셋팅
    SceneManager::Get()->Render();

    //scene->Render();
    Environment::Get()->SetPostRender();
    SceneManager::Get()->PostRender();

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //string fps = "FPS : " + to_string(Timer::Get()->GetFPS());
    //Font::Get()->RenderText(fps, { 100, WIN_HEIGHT - 10 });

    static bool isActive = true;

    if (Environment::Get()->isGUIRender)
    {
        ImGui::Begin("Inspector", &isActive);
        Environment::Get()->GUIRender();
        SceneManager::Get()->GUIRender();
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    Font::Get()->GetDC()->EndDraw();

    Device::Get()->Present();
    //swapChain->Present(0, 0);     // 백 버퍼(Back Buffer)에 그려진 화면을 프론트 버퍼(Front Buffer)로 스왑
}

void GameManager::Create()
{
    Keyboard::Get();
    Timer::Get();

    Device::Get();
    Environment::Get();
    Audio::Get();

    Font::Get()->AddColor("White", 1, 1, 1);
    Font::Get()->AddStyle("Default", L"배달의민족 주아");

    Font::Get()->SetColor("White");
    Font::Get()->SetStyle("Default");

    Texture::AddTexture(L"Textures/Color/White.png");

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(hWnd);
    ImGui_ImplDX11_Init(DEVICE, DC);
}

void GameManager::Delete()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();

    ImGui::DestroyContext();

    Keyboard::Delete();
    Timer::Delete();

    Shader::Delete();
    Texture::Delete();

    Audio::Delete();
    Font::Delete();

    SceneManager::Delete();

    Environment::Delete();
    Device::Delete();

}
