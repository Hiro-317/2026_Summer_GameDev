// UTF-8 BOMなしで保存する。C++のParametersと同じ48バイト。
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
    nointerpolation float puffSeed : TEXCOORD1;
    float3 worldPosition : TEXCOORD2;
};
float DustHash(float x) { return frac(sin(x * 12.9898) * 43758.5453); }

/// <summary>楕円の塊を重ねて、煙の丸い輪郭と内部の陰影を作る。</summary>
/// <param name="p">-1～1の煙のローカル座標。</param>
/// <param name="center">塊の中心。</param>
/// <param name="size">塊の半径。</param>
/// <param name="distance">現在までの最小距離。</param>
/// <param name="lobe">選ばれた塊内の座標。</param>
void DustLobe(float2 p, float2 center, float2 size, inout float distance, inout float2 lobe)
{
    float2 q = (p - center) / size;
    float d = length(q) - 1.0;
    if (d < distance) { distance = d; lobe = q; }
}
float4 main(DustInput input) : SV_TARGET
{
    float2 p = input.local;
    float id = input.puffSeed + seed * 1.7;
    float jitter = (DustHash(id) - 0.5) * 0.28;
    // 横に並んだ大小の塊をつなぐ。上へ段積みする形と細い頂点は作らない。
    p.x *= DustHash(id + 7.0) > 0.5 ? -1.0 : 1.0;
    float d = 100.0;
    float2 lobe = 0.0;
    DustLobe(p, float2(0.0, -0.35), float2(0.86, 0.34), d, lobe);
    DustLobe(p, float2(-0.59, -0.10 + jitter), float2(0.33, 0.36), d, lobe);
    DustLobe(p, float2(-0.23, 0.03 + jitter), float2(0.42, 0.43), d, lobe);
    DustLobe(p, float2(0.26, 0.06 - jitter), float2(0.44, 0.40), d, lobe);
    DustLobe(p, float2(0.62, -0.17), float2(0.30, 0.31), d, lobe);
    if (DustHash(id + 3.1) > 0.65) {
        // 少数の煙だけを横へ巻き上げる。小さな球を縦に連結しない。
        DustLobe(p, float2(0.56, 0.20), float2(0.20, 0.30), d, lobe);
    }
    float detail = sin(p.x * 12.0 + id) * sin(p.y * 10.0 - elapsed * 1.5);
    d += detail * 0.035;
    // 外周を少し広めにぼかし、硬い固形物ではなく薄い土煙にする。
    float aa = max(fwidth(d), 0.055);
    float alpha = (1.0 - smoothstep(-aa, aa, d)) * saturate(opacity);
    float density = 0.76 + 0.18 * sin(p.x * 3.0 + id) * sin(p.y * 4.0 - elapsed * 0.8);
    alpha *= density * smoothstep(0.0, 0.045, elapsed);
    if (clearProgress >= 0.0) {
        // 実際の地面上の距離で消すので、手前・奥の煙も中心から外周へ消える。
        float r = length(input.worldPosition.xz - origin.xz) / max(radius, 0.001);
        float clearRadius = clearProgress * 1.45;
        alpha *= smoothstep(clearRadius - 0.08, clearRadius + 0.06, r);
    }
    // 塊ごとの丸い段差には影を付けず、煙全体に連続した明暗を付ける。
    // 灰色の帯が何段も重なって見えるのを防ぐ。
    float light = 0.80 + 0.20 * smoothstep(-0.55, 0.35, p.y);
    light += detail * 0.025;
    float3 color = saturate(colorRGB * light);
    // 乗算済みαで背景を隠し、灰色の影を残す。加算で白飛びさせない。
    return float4(color * alpha, alpha);
}
