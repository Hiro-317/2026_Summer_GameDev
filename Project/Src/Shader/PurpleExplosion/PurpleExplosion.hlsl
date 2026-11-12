// Purple explosion on a camera-facing world-space quad. Shader Model 5.0+.
// Compile VSMain as vs_5_0 and PSMain as ps_5_0.
// Draw 6 vertices without a vertex buffer (triangle list).
// Blend: Src=ONE, Dest=ONE (additive). Depth test ON, depth write OFF, cull NONE.
// Bind constants at b0. Matrix below is row-major, using mul(vector, matrix).
// CameraRight/Up must be normalized world-space camera basis vectors.
// Example: CenterWS=(0,1,0), Radius=3, ElapsedSeconds=0.35,
// DurationSeconds=1.2, Intensity=2, Seed=1.
// Update ElapsedSeconds each frame; reset to zero to trigger another explosion.

#ifndef PURPLE_EXPLOSION_HLSL
#define PURPLE_EXPLOSION_HLSL

cbuffer PurpleExplosionConstants : register(b0)
{
    row_major float4x4 ViewProjection;
    float3 CenterWS;
    float Radius;                // Maximum world-space radius; must be > 0.
    float3 CameraRightWS;
    float ElapsedSeconds;
    float3 CameraUpWS;
    float DurationSeconds;       // Must be > 0.
    float Intensity;             // HDR brightness, typically 1 to 4.
    float Seed;
    float2 ExplosionPadding;
};

struct ExplosionVaryings
{
    float4 positionCS : SV_POSITION;
    float2 localUV : TEXCOORD0;   // Centered coordinates, from -1 to +1.
};

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

ExplosionVaryings VSMain(uint vertexID : SV_VertexID)
{
    const float2 corners[6] = {
        float2(-1, -1), float2(-1, 1), float2(1, 1),
        float2(-1, -1), float2(1, 1), float2(1, -1)
    };
    ExplosionVaryings output;
    float2 p = corners[vertexID];
    float3 positionWS = CenterWS + max(Radius, 0.0) *
        (CameraRightWS * p.x + CameraUpWS * p.y);
    output.positionCS = mul(float4(positionWS, 1.0), ViewProjection);
    output.localUV = p;
    return output;
}

// Can also be called from an engine-specific fragment shader.
// Returns premultiplied HDR radiance, ready for ONE/ONE additive blending.
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

    // Turbulent purple fireball, with a brief pale violet core.
    float fireRadius = lerp(0.07, 0.65, expansion);
    float distortedRadius = r + (noise - 0.45) * 0.16;
    float fire = 1.0 - smoothstep(fireRadius * 0.3, fireRadius, distortedRadius);
    fire *= (0.55 + noise) * pow(1.0 - t, 1.4);
    float flash = exp(-r * r * 95.0) * exp(-t * 16.0);

    // Outward shockwave, kept inside the quad to avoid hard clipping.
    float ringRadius = lerp(0.03, 0.85, expansion);
    float ringWidth = lerp(0.035, 0.018, t);
    float aa = max(fwidth(r), 0.001);
    float ring = 1.0 - smoothstep(ringWidth, ringWidth + aa,
                                  abs(r - ringRadius));
    ring *= (1.0 - t) * 0.8;

    // Deterministic sparks, each travelling outwards along a seeded ray.
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

float4 PSMain(ExplosionVaryings input) : SV_TARGET
{
    return EvaluatePurpleExplosion(input.localUV, ElapsedSeconds,
                                   DurationSeconds, Intensity, Seed);
}

#endif
