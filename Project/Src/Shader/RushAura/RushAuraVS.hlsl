#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_VERTEX3DSHADER
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

// DxLibの固定入力レイアウトに合わせ、未使用要素も含めた9項目を保持する。
cbuffer RushAuraParameters : register(b4)
{
    float elapsedSeconds;
    float intensity;
    float flowSpeed;
    float padding0;
    float3 colorRGB;
    float padding1;
};

struct RushAuraVertexInput
{
    float3 positionWS : POSITION0;
    float4 sparePosition : POSITION1;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 binormal : BINORMAL0;
    float4 diffuse : COLOR0;
    float4 specular : COLOR1;
    float2 uv : TEXCOORD0;
    float2 drawParameters : TEXCOORD1;
};
struct RushAuraOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    nointerpolation float surface : TEXCOORD1;
    float3 normalVS : TEXCOORD2;
    float3 viewPosition : TEXCOORD3;
};
RushAuraOutput main(RushAuraVertexInput input)
{
    RushAuraOutput output;
    float u = saturate(input.uv.x);
    float angle = input.uv.y * 6.2831853;
    float2 circle = float2(cos(angle), sin(angle));
    float time = elapsedSeconds * flowSpeed;
    float phase = u * 11.0 - time * 2.5;
    // 根元も揺らし、後方では振幅を大きくする。円周の継ぎ目も連続させる。
    float amplitude = input.drawParameters.y * (0.10 + 0.34 * u);
    float2 flutter = float2(
        sin(phase + circle.x * 3.1 + circle.y * 2.3) + 0.4 * sin(phase * 1.7 + time),
        cos(phase * 1.13 + circle.y * 3.7 - circle.x) + 0.35 * sin(phase * 2.1 - time * 0.7));
    float3 worldPosition = input.positionWS +
        (input.tangent * flutter.x + input.binormal * flutter.y) * amplitude;
    float3 viewPosition = mul(float4(worldPosition, 1.0), g_base.viewMatrix);
    output.position = mul(float4(viewPosition, 1.0), g_base.projectionMatrix);
    output.uv = input.uv;
    output.surface = input.drawParameters.x;
    output.normalVS = mul(float4(input.normal, 0.0), g_base.viewMatrix);
    output.viewPosition = viewPosition;
    return output;
}