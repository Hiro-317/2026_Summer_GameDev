#include "../Common/VertexToPixelHeader.hlsli"

#define PS_INPUT VertexToPixelLit

#include "../Common/Pixel/PixelShader3DHeader.hlsli"
#include "../Common/Pixel/ModelPixelCommon.hlsli"

// 影の境界
static const float SHADOW_THRESHOLD = 0.25f;
static const float LIGHT_THRESHOLD = 0.65f;

// 各段階の明るさ
static const float SHADOW_LEVEL = 0.35f;
static const float MID_LEVEL = 0.70f;
static const float LIGHT_LEVEL = 1.0f;

float4 main(PS_INPUT i) : SV_TARGET
{
    float4 tex = diffuseMapTexture.Sample(diffuseMapSampler, i.uv);

    float4 base = tex * i.diffuse;

    ModelLightingResult light = CalcModelLighting(i.vwPos, i.normal);
    float3 lighting = max(light.diffuse, 0.0f);

    // 環境光による最低限の明るさを追加
    float ambient = 0.25f;

    float intensity = dot(lighting, float3(0.2126f, 0.7152f, 0.0722f));

    intensity = saturate(intensity + ambient);

    // 明るさを段階化
    float band = SHADOW_LEVEL;

    if (intensity >= LIGHT_THRESHOLD)
    {
        band = LIGHT_LEVEL;
    }
    else if (intensity >= SHADOW_THRESHOLD)
    {
        band = MID_LEVEL;
    }

    // 通常ライティングの明るさ
    float smoothLight = saturate(intensity);

    // 通常ライティングを10%混ぜる
    float finalLight = lerp(band, smoothLight, 0.10f);

    // 暗くなりすぎないように補正
    finalLight = max(finalLight, 0.25f);

    float3 color = base.rgb * finalLight;

    color = FinalizeModelColor(color, i.vwPos);

    return float4(saturate(color), base.a);
}