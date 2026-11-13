// 球の内部の密度・発光を計算する共通処理。
// PurpleExplosionPS.hlslから読み込む。UTF-8の署名なしで保存する。
#ifndef PURPLE_EXPLOSION_HLSL
#define PURPLE_EXPLOSION_HLSL

float ExplosionHash(float2 p)
{
    return frac(sin(dot(p, float2(127.1, 311.7))) * 43758.5453);
}

// 体積サンプリングでは何度も使うので、三角関数を使わない3次元ハッシュにする。
float ExplosionHash3D(float3 p)
{
    p = frac(p * 0.1031);
    p += dot(p, p.yzx + 33.33);
    return frac((p.x + p.y) * p.z);
}

float ExplosionNoise3D(float3 p)
{
    float3 cell = floor(p);
    float3 f = frac(p);
    float3 u = f * f * (3.0 - 2.0 * f);
    float nearPlane = lerp(
        lerp(ExplosionHash3D(cell), ExplosionHash3D(cell + float3(1, 0, 0)), u.x),
        lerp(ExplosionHash3D(cell + float3(0, 1, 0)),
             ExplosionHash3D(cell + float3(1, 1, 0)), u.x), u.y);
    float farPlane = lerp(
        lerp(ExplosionHash3D(cell + float3(0, 0, 1)),
             ExplosionHash3D(cell + float3(1, 0, 1)), u.x),
        lerp(ExplosionHash3D(cell + float3(0, 1, 1)),
             ExplosionHash3D(cell + float3(1, 1, 1)), u.x), u.y);
    return lerp(nearPlane, farPlane, u.z);
}

float ExplosionVolumeNoise(float3 p)
{
    // 大きな炎の塊と細かい凹凸を重ねる。8回の体積サンプリングでも負荷を抑える。
    return ExplosionNoise3D(p) * 0.68 +
           ExplosionNoise3D(p * 2.03 + float3(17.1, 9.2, 5.7)) * 0.32;
}

/// <summary>
/// 視線が球を通る長さに沿って炎を重ね、球状の火球を描画する。
/// </summary>
/// <param name="p">中心を原点とする-1～1の板ポリゴン座標。</param>
/// <param name="elapsed">再生開始からの経過時間（秒）。</param>
/// <param name="duration">寿命（秒）。0なら無期限。</param>
/// <param name="intensity">発光の強さ。0なら表示しない。</param>
/// <param name="seed">炎・火花の模様を決める値。</param>
/// <param name="colorRGB">0～1のRGB。</param>
/// <param name="rightWS">カメラの右方向（ワールド空間）。</param>
/// <param name="upWS">カメラの上方向（ワールド空間）。</param>
/// <param name="frontWS">カメラの前方向（ワールド空間）。</param>
/// <returns>乗算済みRGBと、球の密度から計算した不透明度。</returns>
float4 EvaluatePurpleExplosion(float2 p, float elapsed, float duration,
    float intensity, float seed, float3 colorRGB,
    float3 rightWS, float3 upWS, float3 frontWS)
{
    if (duration < 0.0 || elapsed < 0.0 ||
        (duration > 0.0 && elapsed >= duration) || intensity <= 0.0)
        return float4(0, 0, 0, 0);

    float3 baseColor = saturate(colorRGB);
    float peak = max(baseColor.r, max(baseColor.g, baseColor.b));
    if (peak <= 0.0) { return float4(0, 0, 0, 0); }

    float t = 0.35;
    if (duration > 0.0) { t = elapsed / duration; }
    float expansion = 1.0 - pow(1.0 - t, 3.0);
    float fade = 1.0 - smoothstep(0.45, 1.0, t);
    float onset = duration > 0.0 ? smoothstep(0.0, 0.025, t) : smoothstep(0.0, 0.03, elapsed);
    float envelope = fade * onset;
    float edge = 1.0 - smoothstep(0.94, 1.0, length(p));

    // 見えている半球の位置を3次元で求め、炎の輪郭にも小さな凹凸を作る。
    float ballRadius = lerp(0.07, 0.82, expansion);
    float2 initialXY = p / ballRadius;
    float initialZ = -sqrt(saturate(1.0 - dot(initialXY, initialXY)));
    float3 frontPointWS = rightWS * initialXY.x + upWS * initialXY.y + frontWS * initialZ;
    float3 drift = float3(seed * 1.7, -elapsed * 0.9, seed * 2.3);
    float contourNoise = ExplosionVolumeNoise(frontPointWS * 2.8 + drift);
    ballRadius *= 0.94 + contourNoise * 0.12;

    float2 rayXY = p / ballRadius;
    float radialSquared = dot(rayXY, rayXY);
    float3 hotColor = lerp(baseColor, float3(peak, peak, peak), 0.30);
    float3 coreColor = lerp(baseColor, float3(peak, peak, peak), 0.75);
    float3 radiance = float3(0, 0, 0);
    float opacity = 0.0;

    if (radialSquared < 1.0) {
        // 中央ほど球を通る距離が長く、縁ほど短い。この差が円盤ではない厚みになる。
        float halfDepth = sqrt(1.0 - radialSquared);
        float stepLength = halfDepth * 2.0 / 8.0;
        float transmittance = 1.0;
        float3 lightDirection = normalize(float3(-0.45, 0.65, -0.60));

        [unroll]
        for (int sampleIndex = 0; sampleIndex < 8; ++sampleIndex) {
            float z = -halfDepth + (sampleIndex + 0.5) * stepLength;
            float3 pointVS = float3(rayXY, z);
            // ワールド方向で模様を計算するため、カメラを回しても炎が板に貼り付かない。
            float3 pointWS = rightWS * pointVS.x + upWS * pointVS.y + frontWS * pointVS.z;
            float radius = length(pointVS);
            float noise = ExplosionVolumeNoise(pointWS * 3.4 + drift);
            float density = (1.0 - smoothstep(0.45, 1.0, radius)) *
                            saturate((noise - 0.18) * 2.1);
            float sampleOpacity = 1.0 - exp(-density * stepLength * 4.2);

            // 球面の向きで明暗を変え、炎の隙間と白く熱い塊を作る。
            float3 normal = pointWS / max(radius, 0.001);
            float light = 0.28 + 0.72 * saturate(dot(normal, lightDirection));
            float temperature = saturate(noise * 1.35 + (1.0 - radius) * 0.22);
            float hotSpots = smoothstep(0.72, 0.98, temperature);
            float3 sampleColor = lerp(baseColor * 0.32, hotColor, temperature);
            sampleColor = lerp(sampleColor, coreColor, hotSpots * 0.65);
            sampleColor *= light * (0.30 + temperature * 0.75);

            radiance += transmittance * sampleOpacity * sampleColor;
            transmittance *= 1.0 - sampleOpacity;
        }
        opacity = 1.0 - transmittance;

        // 発生直後の閃光は球の中心から出す。輪状の衝撃波は描画しない。
        float flash = exp(-radialSquared * 7.0) * exp(-t * 15.0);
        radiance += coreColor * flash * opacity * 1.8;
    }

    // 薄い発光だけを球の外へ広げる。硬い円の輪郭は作らない。
    float glow = exp(-radialSquared * 3.2) * 0.08;
    radiance += baseColor * glow;
    opacity = saturate(opacity + glow * 0.35);

    // 火花も3次元方向へ配置し、カメラ方向へ投影する。
    [unroll]
    for (int i = 0; i < 12; ++i) {
        float id = (float)i;
        float angle = ExplosionHash(float2(id + seed, 31.7)) * 6.2831853;
        float height = ExplosionHash(float2(id + 4.1, seed + 8.7)) * 2.0 - 1.0;
        float horizontal = sqrt(saturate(1.0 - height * height));
        float3 directionWS = float3(cos(angle) * horizontal, height, sin(angle) * horizontal);
        float speed = lerp(0.55, 0.90, ExplosionHash(float2(id, seed + 8.1)));
        float3 sparkWS = directionWS * speed * expansion + float3(0, -0.10 * t * t, 0);
        float2 sparkPosition = float2(dot(sparkWS, rightWS), dot(sparkWS, upWS));
        float2 delta = p - sparkPosition;
        float spark = exp(-dot(delta, delta) / 0.00014);
        spark *= 1.0 - smoothstep(0.35, 0.95, t);
        // 球の裏側の火花は、球の密度に応じて隠す。
        spark *= dot(sparkWS, frontWS) > 0.0 ? 1.0 - opacity : 1.0;
        radiance += coreColor * spark * 1.4;
        opacity = saturate(opacity + spark);
    }

    // 体積の不透明度で背景を遮る。陰の側も残り、白い床の上でも球体の厚みが分かる。
    return float4(radiance * intensity * envelope * edge, opacity * envelope * edge);
}

#endif
