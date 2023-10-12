#pragma once

class Environment : public Singleton<Environment>
{
private:
    friend class Singleton;

    Environment();
    ~Environment();

public :
    void Update();
    void GUIRender();

    void SetState();
    void SetPostRender();

    // rasterizerState
    void SetRasterizerState();
    void SetCullMode(D3D11_CULL_MODE mode = D3D11_CULL_BACK);
    void SetFillMode(D3D11_FILL_MODE mode = D3D11_FILL_SOLID);
    void SetFrontCounterClockwise(bool value);

    // BlendState
    void SetBlendState();
    void SetAlphaBlend(bool value);
    void SetAlphaToCoverage(bool value);

    // depthStencillState
    void SetDepthStencilState();
    void SetDepthEnable(bool value);

    Camera* GetMainCamera() { return mainCamera; }
    Matrix GetProjection() { return perspective; }
    DirectLight* GetDirectLight() { return directLight; }

    void SetViewport(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT);
    void SetPerspective();
    void SetOrthographic();
    void SetDirectLight();


private :
    void CreateProjection();

    void CreateState();

    void DebugRenderCheck();

private :
    // Buffer
    MatrixBuffer* projectionBuffer;
    DirectLight* directLight;
    ViewBuffer* uiViewBuffer;

    // State, 1개로 돌려써도 될듯한디?
    SamplerState* samplerState;
    RasterizerState* rasterizerState;
    BlendState* blendState;
    DepthStencilState* depthStencilState;


    Camera* mainCamera;

    D3D11_VIEWPORT viewport;    // 뷰포트 생성

    // 원근 매트릭스
    Matrix perspective;
    Matrix orthographic;

public :
    bool isCollider = false;
    bool isAStar = false;
    bool isFreeMode = false;
    bool isGUIRender = false;
};