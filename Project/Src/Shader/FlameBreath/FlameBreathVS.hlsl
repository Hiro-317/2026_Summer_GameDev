#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_VERTEX3DSHADER
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

// 型を直接宣言し、VS_INPUTの条件付き別名に依存しないようにする。
// DxLibの固定入力レイアウトと同じ9要素・同じ順番を必ず宣言する。
// 未使用項目を省くと、固定レイアウトのUV入力位置とシェーダー側のレジスターがずれる。
// POSITION0=v0、TEXCOORD0=v7、TEXCOORD1=v8。使わない値も削除しないこと。
struct FlameBreathVertexInput
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

struct FlameBreathOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    // x: 本体/正面用の区別、y: 表示量。三角形内で補間する必要はない。
    nointerpolation float2 drawParameters : TEXCOORD1;
};

FlameBreathOutput main(FlameBreathVertexInput input)
{
    FlameBreathOutput output;
    // CPUが配置したワールド座標を、DxLibのカメラ行列で変換する。
    float3 viewPosition = mul(float4(input.positionWS, 1.0f), g_base.viewMatrix);
    output.position = mul(float4(viewPosition, 1.0f), g_base.projectionMatrix);
    output.uv = input.uv;
    output.drawParameters = input.drawParameters;
    return output;
}
