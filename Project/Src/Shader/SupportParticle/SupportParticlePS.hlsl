cbuffer SupportParticleParameters : register(b4)
{
    float elapsedSeconds;
    float durationSeconds;
    float intensity;
    float padding0;
    float3 colorRGB;
    float padding1;
};
struct SupportParticleInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    nointerpolation float3 drawInfo : TEXCOORD1;
};
float4 main(SupportParticleInput input) : SV_TARGET
{
    float2 p = input.uv * 2.0 - 1.0;
    float radial = length(p);
    float life = saturate(input.drawInfo.y);
    float envelope = smoothstep(0.0, 0.045, life) * (1.0 - smoothstep(0.38, 1.0, life));
    float energy;
    float core = 0.0;
    if (input.drawInfo.x > 1.5) {
        // 薄い光の輪で、粒子が対象を包む印象を補う。
        float shellRadius = 0.12 + 0.78 * (1.0 - pow(1.0 - life, 4.0));
        float d = (radial - shellRadius) / 0.16;
        energy = exp(-d * d) * 0.12 * envelope;
    } else {
        float softEdge = 1.0 - smoothstep(0.55, 1.0, radial);
        float flicker = 0.78 + 0.22 * sin(elapsedSeconds * 5.0 + input.drawInfo.z * 2.1);
        energy = exp(-radial * radial * 4.0) * softEdge;
        core = exp(-radial * radial * 28.0) * 0.4;
        if (input.drawInfo.x > 0.5) {
            // 一部だけ十字のきらめきを混ぜ、柔らかい光の粒と区別する。
            float star = exp(-abs(p.x) * 28.0 - abs(p.y) * 3.0)
                + exp(-abs(p.y) * 28.0 - abs(p.x) * 3.0);
            energy += star * softEdge * 0.42;
        }
        energy *= envelope * flicker;
        core *= envelope;
    }
    float3 lightColor = lerp(colorRGB, float3(1.0, 1.0, 1.0), 0.25);
    float3 radiance = colorRGB * energy + lightColor * core;
    return float4(radiance * max(intensity, 0.0), saturate(energy + core));
}