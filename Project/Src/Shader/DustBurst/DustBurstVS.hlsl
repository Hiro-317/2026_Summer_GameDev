#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_VERTEX3DSHADER
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

// DxLibの固定レイアウトの9要素を同じ順番で維持する。UVはv7、su/svはv8。
struct DustInput
{
    float3 centerWS : POSITION0;
    float4 sparePosition : POSITION1;
    float3 puffData : NORMAL0;
    float3 tangent : TANGENT0;
    float3 binormal : BINORMAL0;
    float4 diffuse : COLOR0;
    float4 specular : COLOR1;
    float2 uv : TEXCOORD0;
    float2 offset : TEXCOORD1;
};
struct DustOutput
{
    float4 position : SV_POSITION;
    float2 local : TEXCOORD0;
    nointerpolation float puffSeed : TEXCOORD1;
    float3 worldPosition : TEXCOORD2;
};
DustOutput main(DustInput input)
{
    DustOutput output;
    float3 view = mul(float4(input.centerWS, 1.0), g_base.viewMatrix);
    // カメラ座標で広げ、どの角度からも煙の板が薄くならないようにする。
    view.xy += input.offset;
    output.position = mul(float4(view, 1.0), g_base.projectionMatrix);
    output.local = float2(input.uv.x * 2.0 - 1.0, 1.0 - input.uv.y * 2.0);
    output.puffSeed = input.puffData.x;
    float3 rightWS = float3(g_base.viewMatrix[0][0], g_base.viewMatrix[1][0], g_base.viewMatrix[2][0]);
    float3 upWS = float3(g_base.viewMatrix[0][1], g_base.viewMatrix[1][1], g_base.viewMatrix[2][1]);
    // 煙の各ピクセルの地面上の距離を求めるため、実際の板のワールド座標も渡す。
    output.worldPosition = input.centerWS + rightWS * input.offset.x + upWS * input.offset.y;
    return output;
}
