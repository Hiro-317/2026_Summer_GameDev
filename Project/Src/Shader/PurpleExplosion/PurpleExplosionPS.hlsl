#include "PurpleExplosion.hlsl"

cbuffer PurpleExplosionParameters : register(b4)
{
    float elapsedSeconds;
    float durationSeconds;
    float intensity;
    float seed;
};

struct ExplosionInput
{
    float4 position : SV_POSITION;
    float2 localUV : TEXCOORD0;
};

float4 main(ExplosionInput input) : SV_TARGET
{
    return EvaluatePurpleExplosion(input.localUV, elapsedSeconds,
                                   durationSeconds, intensity, seed);
}
