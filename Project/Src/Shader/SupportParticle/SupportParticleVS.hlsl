#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_VERTEX3DSHADER
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

// DxLibの9要素の入力配置を、既存の爆発・ブレスと揃える。
struct SupportParticleVertexInput
{
    float3 positionWS : POSITION0;
    float4 sparePosition : POSITION1;
    float3 drawInfo : NORMAL0;
    float3 tangent : TANGENT0;
    float3 binormal : BINORMAL0;
    float4 diffuse : COLOR0;
    float4 specular : COLOR1;
    float2 uv : TEXCOORD0;
    float2 billboardOffset : TEXCOORD1;
};
struct SupportParticleOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    nointerpolation float3 drawInfo : TEXCOORD1;
};
SupportParticleOutput main(SupportParticleVertexInput input)
{
    SupportParticleOutput output;
    float3 viewPosition = mul(float4(input.positionWS, 1.0), g_base.viewMatrix);
    // 粒子と輪郭光をカメラへ正対させ、見る方向で潰れないようにする。
    viewPosition.xy += input.billboardOffset;
    output.position = mul(float4(viewPosition, 1.0), g_base.projectionMatrix);
    output.uv = input.uv;
    output.drawInfo = input.drawInfo;
    return output;
}