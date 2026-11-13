cbuffer RushAuraParameters : register(b4)
{
    float elapsedSeconds;
    float intensity;
    float flowSpeed;
    float padding0;
    float3 colorRGB;
    float padding1;
};
struct RushAuraInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    nointerpolation float surface : TEXCOORD1;
    float3 normalVS : TEXCOORD2;
    float3 viewPosition : TEXCOORD3;
};
float AuraHash(float2 p)
{
    return frac(sin(dot(p, float2(127.1, 311.7))) * 43758.5453);
}
float AuraNoise(float2 p)
{
    float2 cell = floor(p);
    float2 f = frac(p);
    float2 t = f * f * (3.0 - 2.0 * f);
    return lerp(lerp(AuraHash(cell), AuraHash(cell + float2(1, 0)), t.x),
        lerp(AuraHash(cell + float2(0, 1)), AuraHash(cell + float2(1, 1)), t.x), t.y);
}
float4 main(RushAuraInput input) : SV_TARGET
{
    float u = saturate(input.uv.x);
    float t = elapsedSeconds * flowSpeed;
    // uは前方から後方へ増える。位相を減算することで赤い筋を後方へ高速で流す。
    float envelope = smoothstep(0.0, 0.035, elapsedSeconds);
    float ends = smoothstep(0.0, 0.035, u) * (1.0 - smoothstep(0.68, 1.0, u));
    float noise;
    float mask;
    float stream;
    if (input.surface < 0.5) {
        float angle = input.uv.y * 6.2831853;
        float2 circle = float2(cos(angle), sin(angle));
        noise = AuraNoise(circle * 5.0 + float2(u * 15.0 - t * 3.0, u * 4.0));
        float bands = sin(angle * 12.0 + noise * 3.0 + u * 5.0);
        stream = pow(saturate(0.5 + 0.5 * bands), 5.0);
        float facing = saturate(abs(dot(input.normalVS, -input.viewPosition)) *
            rsqrt(max(dot(input.normalVS, input.normalVS) * dot(input.viewPosition, input.viewPosition), 0.000001)));
        mask = 0.12 + 0.4 * pow(1.0 - facing, 2.0);
    } else {
        float across = abs(input.uv.y * 2.0 - 1.0);
        noise = AuraNoise(float2(u * 13.0 - t * 3.0, input.uv.y * 5.0));
        mask = (1.0 - smoothstep(0.65, 0.98, across)) * 0.18;
        stream = smoothstep(0.3, 0.85, noise);
    }
    float pulse = pow(saturate(0.5 + 0.5 * sin(u * 58.0 - t * 22.0 + noise * 6.0)), 3.0);
    // 中央へ集まる連続した芯を作らず、まばらな光の塊を後方へ流す。
    float breakup = smoothstep(0.28, 0.68, noise);
    float fire = mask * (0.08 + stream * 1.3 + pulse * 0.5) * breakup * ends * envelope;
    float3 hotColor = lerp(colorRGB, float3(1.0, 0.18, 0.035), stream * 0.3);
    // 乗算済みαの加算合成用。透明になる部分ではRGBの発光も消す。
    return float4(hotColor * fire * max(intensity, 0.0), saturate(fire));
}