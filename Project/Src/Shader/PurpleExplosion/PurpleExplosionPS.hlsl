#include "PurpleExplosion.hlsl"

cbuffer PurpleExplosionParameters : register(b4)
{
    float elapsedSeconds;
    float durationSeconds;
    float intensity;
    float seed;

    // C++側のParametersと順番・サイズを合わせる（合計32バイト）。
    float3 colorRGB;
    float padding;
};

struct ExplosionInput
{
    float4 position : SV_POSITION;
    float2 localUV : TEXCOORD0;
    nointerpolation float3 cameraRightWS : TEXCOORD1;
    nointerpolation float3 cameraUpWS : TEXCOORD2;
    nointerpolation float3 cameraFrontWS : TEXCOORD3;
};

float4 main(ExplosionInput input) : SV_TARGET
{
    return EvaluatePurpleExplosion(input.localUV, elapsedSeconds,
                                   durationSeconds, intensity, seed, colorRGB,
                                   input.cameraRightWS, input.cameraUpWS, input.cameraFrontWS);
}

