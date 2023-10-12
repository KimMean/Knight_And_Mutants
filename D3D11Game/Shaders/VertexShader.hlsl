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
    output.pos = mul(input.pos, world);         // position에 world 행렬을 곱함
    output.pos = mul(output.pos, view);         // 카메라 view 행렬을 곱함
    output.pos = mul(output.pos, projection);   // 투영행렬 projection을 곱함
    
    output.uv = input.uv;

	return output;
}