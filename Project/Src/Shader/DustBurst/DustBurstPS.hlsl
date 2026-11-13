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
    // 左右反転と上部の塊の違いで、同じ形の山が並ぶのを避ける。
    p.x *= DustHash(id + 7.0) > 0.5 ? -1.0 : 1.0;
    p.x += p.y * p.y * jitter;
    // 低い土煙から、上へ丸く巻く大きな塊と小さな先端をつなげる。
    float d = 100.0;
    float2 lobe = 0.0;
    DustLobe(p, float2(0.0, -0.52), float2(0.92, 0.30), d, lobe);
    DustLobe(p, float2(-0.49, -0.18), float2(0.38, 0.42), d, lobe);
    DustLobe(p, float2(-0.12 + jitter, 0.08), float2(0.49, 0.43), d, lobe);
    DustLobe(p, float2(0.39, -0.15), float2(0.43, 0.44), d, lobe);
    DustLobe(p, float2(0.15 + jitter, 0.34), float2(0.34, 0.31), d, lobe);
    if (DustHash(id + 3.1) > 0.55) {
        DustLobe(p, float2(-0.02 + jitter, 0.65), float2(0.20, 0.19), d, lobe);
        DustLobe(p, float2(-0.16 + jitter, 0.85), float2(0.10, 0.09), d, lobe);
    }
    // 微かなうねりだけを付け、写実ノイズより参考画像の丸い輪郭を優先する。
    d += sin(p.x * 18.0 + id) * sin(p.y * 15.0 - elapsed * 1.5) * 0.018;
    float aa = max(fwidth(d), 0.012);
    float alpha = (1.0 - smoothstep(-aa, aa, d)) * saturate(opacity);
    alpha *= smoothstep(0.0, 0.045, elapsed);
    if (clearProgress >= 0.0) {
        // 実際の地面上の距離で消すので、手前・奥の煙も中心から外周へ消える。
        float r = length(input.worldPosition.xz - origin.xz) / max(radius, 0.001);
        float clearRadius = clearProgress * 1.45;
        alpha *= smoothstep(clearRadius - 0.08, clearRadius + 0.06, r);
    }
    float light = 0.68 + 0.32 * smoothstep(-0.65, 0.45, p.y);
    light -= 0.10 * smoothstep(0.15, 0.85, -lobe.y);
    light += 0.05 * saturate(-lobe.x + lobe.y);
    float3 color = saturate(colorRGB * light);
    // 乗算済みαで背景を隠し、灰色の影を残す。加算で白飛びさせない。
    return float4(color * alpha, alpha);
}
