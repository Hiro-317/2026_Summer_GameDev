cbuffer HitSparkParameters : register(b4)
{
    float elapsedSeconds;
    float durationSeconds;
    float intensity;
    float seed;
    float3 colorRGB;
    float padding;
};
struct HitSparkInput
{
    float4 position : SV_POSITION;
    float2 localUV : TEXCOORD0;
};
float HitHash(float value)
{
    return frac(sin(value * 127.1 + seed * 31.7) * 43758.5453);
}
float4 main(HitSparkInput input) : SV_TARGET
{
    float life = saturate(elapsedSeconds / max(durationSeconds, 0.001));
    if (life >= 1.0) { return float4(0, 0, 0, 0); }
    float2 p = input.localUV;
    float radial = length(p);
    // ヒットごとに偏りを変え、衝撃輪を歪ませて途切れさせる。
    float phase = seed * 2.39996;
    float angleAround = atan2(p.y, p.x);
    float distortion = 1.0 + sin(angleAround * 3.0 + phase) * 0.15;
    distortion += sin(angleAround * 7.0 - phase + life * 2.0) * 0.07;
    float2 impactAxis = float2(cos(phase), sin(phase));
    float2 corePos = p - impactAxis * life * 0.08;
    float flash = exp(-dot(corePos, corePos) * 30.0 * distortion) * exp(-life * 9.0);
    float ringRadius = 0.1 + 0.64 * (1.0 - pow(1.0 - life, 3.0));
    float ringDistance = (radial * distortion - ringRadius) / 0.04;
    float brokenRing = smoothstep(-0.55, 0.35, sin(angleAround * 5.0 + phase));
    float ring = exp(-ringDistance * ringDistance) * pow(1.0 - life, 2.0) * brokenRing * 0.65;
    float sparks = 0.0;
    [unroll]
    for (int i = 0; i < 12; ++i) {
        float angle = i * 6.2831853 / 12.0 + phase + (HitHash(i + 0.7) - 0.5) * 0.85;
        float2 direction = float2(cos(angle), sin(angle));
        float speed = 0.3 + HitHash(i + 2.1) * 0.55;
        float travel = 0.07 + speed * (1.0 - pow(1.0 - life, 2.0));
        float2 sideways = float2(-direction.y, direction.x);
        float bend = sin(life * 7.0 + i) * life * (HitHash(i + 5.0) - 0.5) * 0.12;
        float2 delta = p - direction * travel - sideways * bend;
        float along = dot(delta, direction);
        float across = dot(delta, float2(-direction.y, direction.x));
        // 火花だけ少し太く長くして、コミカルな勢いを残す。
        float lengthScale = 0.035 + (1.0 - life) * (0.1 + HitHash(i + 6.0) * 0.14);
        float width = 0.016 + HitHash(i + 3.4) * 0.02;
        sparks += exp(-along * along / (lengthScale * lengthScale) - across * across / (width * width)) * (0.55 + HitHash(i + 7.0) * 0.7);
    }
    sparks *= pow(1.0 - life, 1.4);
    // 整った十字の輝きの代わりに、傾いた非対称な衝撃の筋を出す。
    float alongImpact = dot(p, impactAxis);
    float acrossImpact = dot(p, float2(-impactAxis.y, impactAxis.x));
    float cross = exp(-acrossImpact * acrossImpact * 1100.0 - alongImpact * alongImpact * 12.0);
    cross *= (0.5 + 0.5 * smoothstep(-0.2, 0.4, alongImpact)) * exp(-life * 11.0) * 0.7;
    float edge = 1.0 - smoothstep(0.84, 1.0, radial);
    float energy = (flash + ring + sparks + cross) * edge;
    // 中心は暖色の閃光、周囲は赤い衝撃にして白一色への飽和を抑える。
    float3 radiance = float3(1.0, 0.42, 0.13) * flash * 1.5;
    radiance += colorRGB * (sparks + cross);
    radiance += float3(1.0, 0.035, 0.008) * ring;
    return float4(radiance * edge * max(intensity, 0.0), saturate(energy));
}
