// 紫色の爆発の模様・色・消滅を計算する共通処理。
// PurpleExplosionPS.hlslから読み込むため、このファイル単体ではコンパイルしない。
#ifndef PURPLE_EXPLOSION_HLSL
#define PURPLE_EXPLOSION_HLSL

float ExplosionHash(float2 p)
{
    return frac(sin(dot(p, float2(127.1, 311.7))) * 43758.5453);
}

float ExplosionNoise(float2 p)
{
    float2 cell = floor(p);
    float2 f = frac(p);
    float2 u = f * f * (3.0 - 2.0 * f);
    return lerp(lerp(ExplosionHash(cell), ExplosionHash(cell + float2(1, 0)), u.x),
                lerp(ExplosionHash(cell + float2(0, 1)),
                     ExplosionHash(cell + float2(1, 1)), u.x), u.y);
}

float ExplosionFBM(float2 p)
{
    float value = 0.0;
    float weight = 0.5;
    [unroll]
    for (int octave = 0; octave < 4; ++octave)
    {
        value += weight * ExplosionNoise(p);
        p = p * 2.03 + float2(17.1, 9.2);
        weight *= 0.5;
    }
    return value;
}

// 中心を原点とする-1～1の座標から爆発色を計算する。
// 戻り値のRGBにはフェードを乗算済み。描画側は乗算済みαの加算ブレンドを使う。
float4 EvaluatePurpleExplosion(float2 p, float elapsed, float duration,
                               float intensity, float seed)
{
    if (duration <= 0.0 || elapsed < 0.0 || elapsed >= duration)
        return float4(0, 0, 0, 0);

    float t = elapsed / duration;
    float r = length(p);
    float edge = 1.0 - smoothstep(0.90, 1.0, r);
    float fade = 1.0 - smoothstep(0.45, 1.0, t);
    float onset = smoothstep(0.0, 0.025, t);
    float expansion = 1.0 - pow(1.0 - t, 3.0);
    float noise = ExplosionFBM(p * 7.0 + float2(seed, -t * 3.0));

    // ノイズで輪郭を揺らす火球と、発生直後だけ強く光る薄紫の中心。
    float fireRadius = lerp(0.07, 0.65, expansion);
    float distortedRadius = r + (noise - 0.45) * 0.16;
    float fire = 1.0 - smoothstep(fireRadius * 0.3, fireRadius, distortedRadius);
    fire *= (0.55 + noise) * pow(1.0 - t, 1.4);
    float flash = exp(-r * r * 95.0) * exp(-t * 16.0);

    // 外側へ広がる衝撃波。板ポリゴンの端で切れない範囲に収める。
    float ringRadius = lerp(0.03, 0.85, expansion);
    float ringWidth = lerp(0.035, 0.018, t);
    float aa = max(fwidth(r), 0.001);
    float ring = 1.0 - smoothstep(ringWidth, ringWidth + aa,
                                  abs(r - ringRadius));
    ring *= (1.0 - t) * 0.8;

    // シードに応じた方向へ火花を飛ばす。同じシードなら毎回同じ模様になる。
    float sparks = 0.0;
    [unroll]
    for (int i = 0; i < 20; ++i)
    {
        float id = (float)i;
        float h = ExplosionHash(float2(id + seed, 31.7));
        float angle = h * 6.2831853;
        float2 direction = float2(cos(angle), sin(angle));
        float speed = lerp(0.45, 0.86, ExplosionHash(float2(id, seed + 8.1)));
        float2 sparkPosition = direction * speed * expansion;
        sparkPosition.y -= 0.12 * t * t;
        float2 delta = p - sparkPosition;
        float along = dot(delta, direction);
        float across = dot(delta, float2(-direction.y, direction.x));
        float spark = exp(-along * along / 0.0012 - across * across / 0.00006);
        sparks += spark * (1.0 - smoothstep(0.35, 0.95, t));
    }

    float3 violet = float3(0.46, 0.035, 1.0);
    float3 pink = float3(0.9, 0.16, 1.0);
    float3 pale = float3(0.85, 0.65, 1.0);
    float3 radiance = lerp(violet, pink, saturate(noise)) * fire * 2.0;
    radiance += violet * ring * 1.5 + pale * (flash * 3.5 + sparks * 2.0);
    float envelope = edge * fade * onset;
    float alpha = saturate((fire + ring + flash + sparks) * envelope);
    return float4(radiance * envelope * max(intensity, 0.0), alpha);
}

#endif
