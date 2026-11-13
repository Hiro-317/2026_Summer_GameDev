#include "SupportParticleActor.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <stdexcept>

#include "../Shader/SupportParticleShader.h"

SupportParticleActor::SupportParticleActor(unsigned int color, float radius) :
    EffectBase(1.3f), targetPosition(nullptr), color(color & 0xffffff), radius(radius), particleShader(nullptr)
{
    if (!std::isfinite(radius) || radius <= 0.0f) {
        throw std::invalid_argument("回復・バフの粒子半径には正の有限値を指定してください");
    }
}

void SupportParticleActor::Emit(const Vector3* target)
{
    if (target == nullptr) { return; }
    targetPosition = target;
    Restart();
}

void SupportParticleActor::DefaultCreateShader(void)
{
    particleShader = new SupportParticleShader();
    CreateShader(particleShader);
}

void SupportParticleActor::Draw(void)
{
    if (!GetIsDraw() || IsFinished() || particleShader == nullptr || targetPosition == nullptr) { return; }
    particleShader->SetParameters(GetElapsedSeconds(), GetDurationSeconds(), color);
    ActorBase::Draw();
}

void SupportParticleActor::SubDraw(void)
{
    constexpr int particleCount = 80;
    constexpr int quadCount = particleCount + 1;
    constexpr float goldenAngle = 2.39996323f;
    std::array<VERTEX3DSHADER, quadCount * 4> vertices{};
    std::array<unsigned short, quadCount * 6> indices{};
    const float elapsed = GetElapsedSeconds();
    const float duration = GetDurationSeconds();
    const float life = std::clamp(elapsed / duration, 0.0f, 1.0f);
    const Vector3 center = *targetPosition;
    const float corners[4][2] = {{-1, 1}, {1, 1}, {-1, -1}, {1, -1}};
    int quads = 0;

    auto addQuad = [&](const Vector3& pos, float width, float height, float kind, float phase, float seed) {
        const unsigned short base = static_cast<unsigned short>(quads * 4);
        for (int edge = 0; edge < 4; ++edge) {
            auto& v = vertices[base + edge];
            v.pos = pos.ToVECTOR();
            // 法線欄は描画用情報として渡し、標準照明には使用しない。
            v.norm = VGet(kind, phase, seed);
            v.dif = GetColorU8(255, 255, 255, 255);
            v.u = (corners[edge][0] + 1.0f) * 0.5f;
            v.v = (1.0f - corners[edge][1]) * 0.5f;
            v.su = corners[edge][0] * width;
            v.sv = corners[edge][1] * height;
        }
        const int index = quads * 6;
        indices[index] = base; indices[index + 1] = base + 1; indices[index + 2] = base + 2;
        indices[index + 3] = base + 2; indices[index + 4] = base + 1; indices[index + 5] = base + 3;
        ++quads;
    };

    // 薄い輪郭光が中心から広がり、周囲の粒子と一緒に体を包む。
    addQuad(center, radius * 1.2f, radius * 1.45f, 2.0f, life, 0.0f);
    for (int particle = 0; particle < particleCount; ++particle) {
        const float seed = static_cast<float>(particle) + 0.41f;
        const float delay = (0.5f + 0.5f * std::sin(seed * 8.7f)) * 0.12f;
        if (elapsed < delay) { continue; }
        const float phase = std::clamp((elapsed - delay) / (duration - delay), 0.0f, 1.0f);
        const float expansion = 1.0f - std::pow(1.0f - (std::min)(1.0f, phase / 0.48f), 3.0f);
        const float y = 1.0f - 2.0f * (static_cast<float>(particle) + 0.5f) / particleCount;
        const float horizontal = std::sqrt((std::max)(0.0f, 1.0f - y * y));
        const float angle = particle * goldenAngle + elapsed * (0.6f + 0.2f * std::sin(seed));
        const Vector3 outward(std::cos(angle) * horizontal, y * 1.15f, std::sin(angle) * horizontal);
        const float variation = 0.78f + 0.22f * std::sin(seed * 2.7f);
        Vector3 pos = center + outward * (radius * expansion * variation);
        pos.y += phase * 18.0f + std::sin(elapsed * 4.0f + seed) * expansion * 5.0f;
        const float size = (5.0f + 3.0f * (0.5f + 0.5f * std::sin(seed * 3.4f))) * (0.6f + expansion * 0.4f);
        addQuad(pos, size, size, particle % 5 == 0 ? 1.0f : 0.0f, phase, seed);
    }

    int previousBlend = 0, previousBlendParameter = 0;
    GetDrawBlendMode(&previousBlend, &previousBlendParameter);
    const int previousCulling = GetUseBackCulling();
    SetDrawBlendMode(DX_BLENDMODE_PMA_ADD, 255);
    SetUseBackCulling(DX_CULLING_NONE);
    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(false);
    DrawPolygonIndexed3DToShader(vertices.data(), quads * 4, indices.data(), quads * 2);
    SetWriteZBuffer3D(true);
    SetUseBackCulling(previousCulling);
    SetDrawBlendMode(previousBlend, previousBlendParameter);
}

void SupportParticleActor::SubRelease(void)
{
    particleShader = nullptr;
    targetPosition = nullptr;
}