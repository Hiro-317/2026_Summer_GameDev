// 署名なしのUTF-8で保存する。FXCがBOMを不正文字として扱うことを避ける。
cbuffer FlameBreathParameters : register(b4)
{
    float elapsedSeconds;
    float durationSeconds;
    float intensity;
    float flowSpeed;
    float3 colorRGB;
    float seed;
};

struct FlameBreathInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    nointerpolation float2 drawParameters : TEXCOORD1;
    float3 normalVS : TEXCOORD2;
    float3 viewPosition : TEXCOORD3;
};

float BreathHash(float2 p)
{
    return frac(sin(dot(p, float2(127.1, 311.7))) * 43758.5453);
}

float BreathNoise(float2 p)
{
    float2 cell = floor(p);
    float2 f = frac(p);
    float2 blend = f * f * (3.0 - 2.0 * f);
    return lerp(lerp(BreathHash(cell), BreathHash(cell + float2(1, 0)), blend.x),
                lerp(BreathHash(cell + float2(0, 1)),
                     BreathHash(cell + float2(1, 1)), blend.x), blend.y);
}

float BreathFBM(float2 p)
{
    float value = 0.0;
    float weight = 0.5;
    [unroll]
    for (int i = 0; i < 4; ++i) {
        value += BreathNoise(p) * weight;
        p = p * 2.03 + float2(13.7, 8.1);
        weight *= 0.5;
    }
    return value;
}

float4 main(FlameBreathInput input) : SV_TARGET
{
    // 寿命0は無期限。有限の場合だけ、指定時間を超えたら光を消す。
    if (durationSeconds < 0.0 || elapsedSeconds < 0.0 ||
        (durationSeconds > 0.0 && elapsedSeconds >= durationSeconds) ||
        input.drawParameters.y <= 0.0)
        return float4(0, 0, 0, 0);

    // 無期限では立ち上がり後の光量を保ち、模様だけを流し続ける。
    // durationSecondsで割る処理は有限寿命の分岐内だけで行う。
    float envelope = smoothstep(0.0, 0.09, elapsedSeconds);
    if (durationSeconds > 0.0) {
        float life = saturate(elapsedSeconds / durationSeconds);
        envelope = smoothstep(0.0, 0.06, life) * (1.0 - smoothstep(0.72, 1.0, life));
    }
    float time = elapsedSeconds * flowSpeed;
    float fire;
    float core;
    float glow;

    if (input.drawParameters.x < 0.5) {
        float u = saturate(input.uv.x);
        float angle = input.uv.y * 6.2831853;
        // 周方向の模様をsin/cosでつなぎ、円周のUVの継ぎ目を隠す。
        float2 circle = float2(cos(angle), sin(angle));
        float noise = BreathFBM(circle * 3.2 + float2(u * 6.0 - time * 2.0, u * 2.0) + seed);
        float detail = BreathFBM(circle * 8.5 + float2(u * 15.0 - time * 3.5, -u * 4.0) + seed * 2.3);
        float root = smoothstep(0.0, 0.02, u);
        // 側面は終端まで描く。旧処理のu=0.84～1で消すフェードを使わない。
        float facing = saturate(abs(dot(input.normalVS, -input.viewPosition)) *
            rsqrt(max(dot(input.normalVS, input.normalVS) *
                      dot(input.viewPosition, input.viewPosition), 0.000001)));
        fire = (0.35 + noise * 0.65 + detail * 0.25) * root * (0.55 + facing * 0.45);
        core = smoothstep(0.50, 0.88, noise + detail * 0.25) * root * 0.45;
        glow = fire * 0.10;
        // 裏表の側面を加算しても、平板の白い塊になりすぎないよう調整する。
        // 円錐の表面は薄い発光だけにし、硬い塗りつぶしを避ける。
        float shell = facing * facing * 0.12;
        fire *= shell;
        core *= shell;
        glow *= shell;
    } else if (input.drawParameters.x < 1.5) {
        // 補助の板ではなく、設定した終端円そのものを塗る。
        float2 p = input.uv * 2.0 - 1.0;
        float radius = length(p);
        float noise = BreathFBM(p * 3.5 + float2(-time, time * 0.3) + seed);
        // ノイズで輪郭を縮めず、円周まで炎を残す。揺れは明るさに付ける。
        float edge = 1.0 - smoothstep(0.985, 1.015, radius);
        fire = edge * (0.20 + noise * 0.30);
        core = (1.0 - smoothstep(0.15, 0.85, radius)) * (0.20 + noise * 0.20);
        glow = edge * 0.10;
    } else {
        // 初期のブレスと同じ、流れるノイズ・白い芯・柔らかい横端を内部へ重ねる。
        float u = saturate(input.uv.x);
        float y = input.uv.y * 2.0 - 1.0;
        float noise = BreathFBM(float2(u * 7.0 - time * 2.0, y * 4.0) + seed);
        float detail = BreathFBM(float2(u * 18.0 - time * 3.5, y * 9.0) + seed * 2.3);
        // 形状自体が円錐なので、UVの横幅をもう一度先細りにしない。
        float ripple = (noise - 0.45) * 0.20;
        float radial = abs(y - ripple) / 0.86;
        float turbulentEdge = 0.82 + noise * 0.30 + detail * 0.16;
        float aa = max(fwidth(radial), 0.015);
        float mask = 1.0 - smoothstep(turbulentEdge - 0.25, turbulentEdge + aa, radial);
        float ends = smoothstep(0.0, 0.012, u) * (1.0 - smoothstep(0.90, 1.0, abs(y)));
        // 3面の重なりを考慮した明るさ。円の手前で消す終端フェードは入れない。
        fire = mask * (0.55 + noise * 0.8 + detail * 0.35) * ends * 0.38;
        core = (1.0 - smoothstep(0.12, 0.48, radial)) * (0.75 + detail * 0.35) * ends * 0.38;
        glow = (1.0 - smoothstep(0.5, 1.25, radial)) * 0.16 * ends * 0.38;
    }

    // 外側を指定色にし、内側は白に近づけて高温の炎らしく光らせる。
    // 黒を指定した場合は、中心も含めて光を出さない。
    float3 baseColor = saturate(colorRGB);
    float peak = max(baseColor.r, max(baseColor.g, baseColor.b));
    float3 hotColor = lerp(baseColor, float3(peak, peak, peak), 0.40);
    float3 coreColor = lerp(baseColor, float3(peak, peak, peak), 0.96);
    float3 radiance = hotColor * fire + coreColor * core * 1.8 + baseColor * glow;
    envelope *= saturate(input.drawParameters.y);
    float alpha = saturate((fire + core + glow) * envelope);
    // 乗算済みαの加算合成。フェードはRGBにも掛け、透明になっても光が残るのを防ぐ。
    return float4(radiance * envelope * max(intensity, 0.0), alpha);
}
