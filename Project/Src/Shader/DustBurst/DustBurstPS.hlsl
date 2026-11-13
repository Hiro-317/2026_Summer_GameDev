// UTF-8 BOMなし。C++のParametersと同じ48バイト。
cbuffer DustParameters : register(b4)
{
    float elapsed;
    float opacity;
    float seed;
    float padding;
    float3 colorRGB;
    float clearProgress;
    float3 origin;
    float radius;
};
struct DustInput
{
    float4 position : SV_POSITION;
    float2 local : TEXCOORD0;
    nointerpolation float2 puffData : TEXCOORD1;
    float3 worldPosition : TEXCOORD2;
};
float DustHash(float2 p) { return frac(sin(dot(p, float2(127.1, 311.7))) * 43758.5453); }
float DustNoise(float2 p)
{
    float2 cell = floor(p);
    float2 f = frac(p);
    f = f * f * (3.0 - 2.0 * f);
    return lerp(lerp(DustHash(cell), DustHash(cell + float2(1, 0)), f.x),
                lerp(DustHash(cell + float2(0, 1)), DustHash(cell + float2(1, 1)), f.x), f.y);
}
float DustDetail(float2 p)
{
    return DustNoise(p) * 0.6 + DustNoise(p * 2.07 + 13.7) * 0.28 +
           DustNoise(p * 4.13 - 7.3) * 0.12;
}
float4 main(DustInput input) : SV_TARGET
{
    float2 offsetXZ = input.worldPosition.xz - origin.xz;
    float angle = atan2(offsetXZ.y, offsetXZ.x);
    float warm = 0.5 + 0.5 * sin(angle + seed * 0.3);
    float3 warmTint = float3(1.0, lerp(0.42, 1.0, warm), lerp(0.35, 0.8, warm));
    // 白や青などの指定色は保ち、赤・橙系のときだけ参考画像の暖色差を強める。
    float warmAmount = saturate((colorRGB.r - max(colorRGB.g, colorRGB.b)) * 3.0);
    float3 tint = lerp(float3(1, 1, 1), warmTint, warmAmount);
    float3 color = saturate(colorRGB * tint * 1.35);
    float alpha;
    if (input.puffData.y > 0.5) {
        // 地面を走る柔らかい帯。内外の端だけをぼかし、輪のつながりを残す。
        float across = input.local.y * 2.0 - 1.0;
        float2 circle = float2(cos(input.local.x * 6.2831853), sin(input.local.x * 6.2831853));
        float noise = DustDetail(circle * 6.0 + elapsed * 0.35 + seed);
        float band = 1.0 - smoothstep(0.28, 1.0, abs(across + (noise - 0.5) * 0.20));
        float core = 1.0 - smoothstep(0.0, 0.45, abs(across));
        alpha = band * (0.72 + noise * 0.28);
        color = saturate(color * (1.0 + core * 0.28));
    } else {
        // 煙を塗りつぶさず、ノイズの細い稜線を崩した破片として残す。
        float2 p = float2(input.local.x * 2.0 - 1.0, 1.0 - input.local.y * 2.0);
        float id = input.puffData.x + seed * 1.7;
        float2 curl = p + float2(sin(p.y * 4.0 + id), cos(p.x * 4.5 - id)) * 0.20;
        float noise = DustDetail(curl * 3.8 + float2(id * 2.3, -elapsed * 0.7));
        float fine = DustNoise(curl * 10.0 + id - elapsed * 0.3);
        float ridges = 1.0 - smoothstep(0.07, 0.20, abs(noise - 0.51));
        float breakup = smoothstep(0.35, 0.62, fine);
        float silhouette = 1.0 - smoothstep(0.55, 1.0, length(float2(p.x, p.y * 1.08)));
        float top = 1.0 - smoothstep(0.15, 1.0, p.y);
        alpha = ridges * breakup * silhouette * top * 0.82;
        color *= 0.72 + noise * 0.28;
    }
    alpha *= saturate(opacity) * smoothstep(0.0, 0.045, elapsed);
    if (clearProgress >= 0.0) {
        float r = length(offsetXZ) / max(radius, 0.001);
        float clearRadius = clearProgress * 1.45;
        alpha *= smoothstep(clearRadius - 0.08, clearRadius + 0.06, r);
    }
    // 乗算済みα合成。中央を透明に保ち、輪と破片だけを地面へ重ねる。
    return float4(color * alpha, alpha);
}
