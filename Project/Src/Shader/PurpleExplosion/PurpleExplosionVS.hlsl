#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_VERTEX3DSHADER
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

struct ExplosionOutput
{
    float4 position : SV_POSITION;
    float2 localUV : TEXCOORD0;
};

ExplosionOutput main(VS_INPUT input)
{
    ExplosionOutput output;
    // 頂点はCPU側でワールド座標へ配置済みなので、ローカル→ワールド変換は不要。
    // DxLibが用意するビュー・射影行列で画面座標へ変換する。
    float3 viewPosition = mul(float4(input.pos, 1.0f), g_base.viewMatrix);
    output.position = mul(float4(viewPosition, 1.0f), g_base.projectionMatrix);
    output.localUV = float2(input.TexCoords0.x * 2.0f - 1.0f,
                            1.0f - input.TexCoords0.y * 2.0f);
    return output;
}
