#pragma once

// �������� ������Ƽ��(Primitive) - ��/��/�/�ٰ����� ���� �׷��� �������� â���ϴ� �� �ʿ��� ��� ���
class GeometryShader : public Shader
{
private:
    friend class Shader;

    GeometryShader(wstring filePath);
    ~GeometryShader();

private:
    // Shader��(��) ���� ��ӵ�
    virtual void CompileFromFile(wstring filePath) override;
    void CreateShader();

public:
    virtual void SetShader() override;

private:
    ID3D11GeometryShader* geometryShader;
};