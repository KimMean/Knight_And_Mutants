#pragma once

// 기하학적 프리미티브(Primitive) - 선/원/곡선/다각형과 같은 그래픽 디자인을 창작하는 데 필요한 요소 담당
class GeometryShader : public Shader
{
private:
    friend class Shader;

    GeometryShader(wstring filePath);
    ~GeometryShader();

private:
    // Shader을(를) 통해 상속됨
    virtual void CompileFromFile(wstring filePath) override;
    void CreateShader();

public:
    virtual void SetShader() override;

private:
    ID3D11GeometryShader* geometryShader;
};