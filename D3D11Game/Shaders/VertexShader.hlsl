#include "WVPMatrix.hlsli"
#include "VertexLayout.hlsli"


struct PixelOutput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

PixelOutput VS(VertexUV input)
{
    PixelOutput output;
    output.pos = mul(input.pos, world);         // position�� world ����� ����
    output.pos = mul(output.pos, view);         // ī�޶� view ����� ����
    output.pos = mul(output.pos, projection);   // ������� projection�� ����
    
    output.uv = input.uv;

	return output;
}