Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

cbuffer ShakeBuffer : register(b4)
{
    float2 g_shakeOffset;
    float2 g_padding;
};

struct PS_INPUT
{
    float4 svPos : SV_POSITION;
    float4 diffuse : COLOR0;
    float2 uv : TEXCOORD0;
    float2 suv : TEXCOORD1;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    // 画面全体のUV座標をずらす
    float2 uv = input.uv + g_shakeOffset;

    // 画面外の参照を防ぐ
    uv = saturate(uv);

    // ずらした座標から色を取得
    return g_texture.Sample(g_sampler, uv);
}