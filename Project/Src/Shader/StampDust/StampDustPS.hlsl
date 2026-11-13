cbuffer StampDustParameters : register(b4)
{
    float elapsedSeconds;
    float durationSeconds;
    float intensity;
    float renderMode;
};
struct StampDustInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    nointerpolation float2 drawInfo : TEXCOORD1;
};
float DustHash(float2 p)
{
    return frac(sin(dot(p, float2(127.1, 311.7))) * 43758.5453);
}
float DustNoise(float2 p)
{
    float2 cell = floor(p);
    float2 f = frac(p);
    float2 t = f * f * (3.0 - 2.0 * f);
    return lerp(lerp(DustHash(cell), DustHash(cell + float2(1, 0)), t.x),
        lerp(DustHash(cell + float2(0, 1)), DustHash(cell + float2(1, 1)), t.x), t.y);
}
float4 main(StampDustInput input) : SV_TARGET
{
    float life = saturate(elapsedSeconds / max(durationSeconds, 0.001));
    float envelope = smoothstep(0.0, 0.035, elapsedSeconds) * (1.0 - smoothstep(0.42, 1.0, life));
    float2 p = input.uv * 2.0 - 1.0;
    float radial = length(p);
    float time = elapsedSeconds * 3.0;
    float seed = input.drawInfo.y;
    float noise = DustNoise(p * 3.8 + float2(time * 0.7, -time) + seed);
    float detail = DustNoise(p * 9.0 + float2(-time, time * 0.4) + seed * 2.1);

    if (input.drawInfo.x < 0.5) {
        if (renderMode < 0.5) { return float4(0, 0, 0, 0); }
        // 赤い衝撃波が地面を走り、揺らいだ輪郭から薄れていく。
        float ringRadius = 0.12 + 0.82 * (1.0 - pow(1.0 - life, 3.0));
        float ringDistance = abs(radial - ringRadius - (noise - 0.5) * 0.045);
        float ring = exp(-ringDistance * ringDistance / 0.0018);
        float energy = ring * (0.55 + detail * 0.45) * envelope * (1.0 - life);
        return float4(float3(1.0, 0.045, 0.012) * energy * intensity, saturate(energy));
    }

    // 四角い輪郭を残さず、ノイズの塊がほぐれて散る砂煙にする。
    float cloudEdge = 0.68 + noise * 0.25 + detail * 0.12;
    float cloud = 1.0 - smoothstep(cloudEdge - 0.25, cloudEdge, radial);
    if (renderMode < 0.5) {
        float alpha = cloud * (0.27 + noise * 0.24) * envelope;
        float3 sand = lerp(float3(0.30, 0.20, 0.12), float3(0.66, 0.48, 0.28), noise * 0.8 + detail * 0.2);
        return float4(sand * alpha, alpha);
    }

    // 砂の中に細い赤い筋を混ぜ、砂埃自体の色を赤一色にはしない。
    float wisps = smoothstep(0.54, 0.85, noise * 0.7 + detail * 0.45);
    float ripple = 0.5 + 0.5 * sin(radial * 22.0 - time * 8.0 + seed);
    float energy = cloud * wisps * ripple * envelope * 0.55;
    return float4(float3(1.0, 0.055, 0.015) * energy * intensity, saturate(energy));
}