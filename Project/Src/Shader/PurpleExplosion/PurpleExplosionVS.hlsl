#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_VERTEX3DSHADER
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

// DxLibのVERTEX3DSHADERから使う頂点属性を直接定義する。
// 条件付きインクルードのVS_INPUT別名に依存せず、エディターでも型を解決できるようにする。
// DxLibの固定入力レイアウトと同じ9要素・同じ順番を必ず宣言する。
// 未使用項目を省くと、固定レイアウトのUV入力位置とシェーダー側のレジスターがずれる。
// POSITION0=v0、TEXCOORD0=v7、TEXCOORD1=v8。使わない値も削除しないこと。
struct PurpleExplosionVertexInput
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

struct ExplosionOutput
{
    float4 position : SV_POSITION;
    float2 localUV : TEXCOORD0;
};

ExplosionOutput main(PurpleExplosionVertexInput input)
{
    ExplosionOutput output;
    // 頂点はCPU側でワールド座標へ配置済みなので、ローカル→ワールド変換は不要。
    // DxLibが用意するビュー・射影行列で画面座標へ変換する。
    float3 viewPosition = mul(float4(input.positionWS, 1.0f), g_base.viewMatrix);
    output.position = mul(float4(viewPosition, 1.0f), g_base.projectionMatrix);
    output.localUV = float2(input.uv.x * 2.0f - 1.0f,
                            1.0f - input.uv.y * 2.0f);
    return output;
}
