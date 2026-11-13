#include "DustBurstActor.h"
#include <algorithm>
#include <array>
#include <cmath>
#include <stdexcept>
#include "../Shader/DustBurstShader.h"
#include "../../../Manager/TimeScale/TimeScale.h"

namespace {
// 乱数状態やテクスチャを持たず、同じシードで同じ広がり方にする。
float DustRandom(float value)
{
    const float v = std::sin(value * 12.9898f) * 43758.5453f;
    return v - std::floor(v);
}
}
DustBurstActor::DustBurstActor(const Vector3& position, const DustBurstSettings& value) :
    EffectBase(value.durationSeconds), settings(value), stopping(false), stopElapsed(0.0f),
    stopDuration(0.0f), stoppedSpreadTime(0.0f), dustShader(nullptr)
{
    if (!std::isfinite(settings.radius) || settings.radius <= 0.0f ||
        !std::isfinite(settings.height) || settings.height <= 0.0f ||
        !std::isfinite(settings.opacity) || settings.opacity < 0.0f || settings.opacity > 1.0f ||
        !std::isfinite(settings.stopSeconds) || settings.stopSeconds < 0.0f ||
        !std::isfinite(settings.seed)) {
        throw std::invalid_argument("Invalid dust burst settings");
    }
    settings.color &= 0xffffff;
    SetPosition(position);
}
void DustBurstActor::SetPosition(const Vector3& position)
{
    if (!std::isfinite(position.x) || !std::isfinite(position.y) || !std::isfinite(position.z)) {
        throw std::invalid_argument("Dust burst position must be finite");
    }
    SetPos(position);
}
void DustBurstActor::DefaultCreateShader(void)
{
    dustShader = new DustBurstShader();
    CreateShader(dustShader);
}
void DustBurstActor::Restart(void)
{
    stopping = false;
    stopElapsed = stopDuration = stoppedSpreadTime = 0.0f;
    EffectBase::Restart();
}
float DustBurstActor::GetSpreadTime(void) const
{
    // Windowsのmin/maxマクロを展開させないため、標準関数名を括弧で囲む。
    // 有限寿命の最後を消滅演出に割り当てる。無期限では1秒で展開して残る。
    const float expansionSeconds = IsInfinite() ? 1.0f :
        (std::max)(GetDurationSeconds() - (std::min)(settings.stopSeconds, GetDurationSeconds() * 0.5f), 0.05f);
    return std::clamp(GetElapsedSeconds() / expansionSeconds, 0.0f, 1.0f);
}
void DustBurstActor::BeginStop(float seconds)
{
    if (IsFinished() || stopping) { return; }
    if (seconds <= 0.0f || dustShader == nullptr || GetElapsedSeconds() <= 0.0f) { StopImmediately(); return; }
    stoppedSpreadTime = GetSpreadTime();
    stopping = true;
    stopElapsed = 0.0f;
    stopDuration = seconds;
}
void DustBurstActor::Stop(void) { BeginStop(settings.stopSeconds); }
void DustBurstActor::StopImmediately(void)
{
    stopping = false;
    stopElapsed = stopDuration = 0.0f;
    EffectBase::Stop();
}
void DustBurstActor::Update(void)
{
    if (IsFinished()) { return; }
    const float dt = (1.0f / 60.0f) * TimeScale::Get();
    if (!stopping && !IsInfinite()) {
        const float release = (std::min)(settings.stopSeconds, GetDurationSeconds() * 0.5f);
        const float releaseAt = GetDurationSeconds() - release;
        if (GetElapsedSeconds() + dt >= releaseAt) {
            BeginStop(release);
            // フレーム境界をまたいだ時間だけ演出へ進め、寿命内に消滅するようにする。
            if (stopping) {
                stopElapsed = -(std::max)(releaseAt - GetElapsedSeconds(), 0.0f);
                stoppedSpreadTime = 1.0f;
            }
        }
    }
    if (stopping) {
        stopElapsed += dt;
        if (stopElapsed >= stopDuration) { StopImmediately(); return; }
    }
    EffectBase::Update();
}
void DustBurstActor::Draw(void)
{
    if (!GetIsDraw() || IsFinished() || dustShader == nullptr) { return; }
    const float clear = stopping ? std::clamp(stopElapsed / stopDuration, 0.0f, 1.0f) : -1.0f;
    // 高い煙を斜め・真上から見た場合の板の張り出しも、消滅範囲へ含める。
    const float clearScale = settings.radius + settings.height * 0.7f;
    dustShader->SetParameters(GetElapsedSeconds(), settings.opacity, settings.seed, settings.color,
                             clear, trans.pos, clearScale);
    ActorBase::Draw();
}
void DustBurstActor::SubDraw(void)
{
    constexpr int puffCount = 20;
    constexpr float twoPi = 6.28318530718f;
    struct Puff { Vector3 center; float width, height, seed, depth; };
    std::array<Puff, puffCount> puffs{};
    const float t = stopping ? stoppedSpreadTime : GetSpreadTime();
    const float expansion = 1.0f - std::pow(1.0f - t, 3.0f);
    const float swelling = 0.08f + 0.92f * std::sqrt(t);
    const Vector3 camera(GetCameraPosition());
    const Vector3 front(GetCameraFrontVector());
    for (int i = 0; i < puffCount; ++i) {
        const bool outer = i < 12;
        const float id = static_cast<float>(i);
        const float random = DustRandom(id + settings.seed * 3.7f);
        const float angle = twoPi * (outer ? id / 12.0f : (id - 12.0f) / 8.0f + 0.04f) +
            (random - 0.5f) * 0.32f;
        const float speed = outer ? 0.72f + random * 0.28f : 0.30f + random * 0.22f;
        const float reach = settings.radius * expansion * speed;
        auto& puff = puffs[i];
        // 低く横へ重ね、同じ高さの山が並ぶ見た目を避ける。
        puff.width = settings.radius * (outer ? 0.30f : 0.23f) * swelling * (0.85f + random * 0.3f);
        puff.height = settings.height * (outer ? 0.42f : 0.24f) * swelling * (0.85f + random * 0.3f);
        puff.center = trans.pos + Vector3(std::cos(angle) * reach,
            puff.height * 0.55f + settings.height * 0.055f * t, std::sin(angle) * reach);
        puff.seed = id + settings.seed;
        puff.depth = (puff.center - camera).Dot(front);
    }
    // 不透明な加算ではなく、奥から半透明の煙を重ねる。
    std::sort(puffs.begin(), puffs.end(), [](const Puff& a, const Puff& b) { return a.depth > b.depth; });
    std::array<VERTEX3DSHADER, puffCount * 4> vertices{};
    std::array<unsigned short, puffCount * 6> indices{};
    const float corners[4][2] = {{-1, 1}, {1, 1}, {-1, -1}, {1, -1}};
    for (int i = 0; i < puffCount; ++i) {
        const auto& puff = puffs[i];
        for (int j = 0; j < 4; ++j) {
            auto& v = vertices[i * 4 + j];
            v.pos = puff.center.ToVECTOR();
            v.norm = VGet(puff.seed, 0.0f, 0.0f); // 法線ではなく、煙ごとの模様ID。
            v.dif = GetColorU8(255, 255, 255, 255);
            v.u = (corners[j][0] + 1.0f) * 0.5f;
            v.v = (1.0f - corners[j][1]) * 0.5f;
            v.su = corners[j][0] * puff.width;
            v.sv = corners[j][1] * puff.height;
        }
        const unsigned short b = static_cast<unsigned short>(i * 4);
        const unsigned short face[6] = {b, static_cast<unsigned short>(b+1), static_cast<unsigned short>(b+2),
            static_cast<unsigned short>(b+1), static_cast<unsigned short>(b+3), static_cast<unsigned short>(b+2)};
        std::copy(face, face + 6, indices.begin() + i * 6);
    }
    int blend = 0, blendValue = 0;
    GetDrawBlendMode(&blend, &blendValue);
    const int culling = GetUseBackCulling();
    SetDrawBlendMode(DX_BLENDMODE_PMA_ALPHA, 255);
    SetUseBackCulling(DX_CULLING_NONE);
    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(false);
    DrawPolygonIndexed3DToShader(vertices.data(), static_cast<int>(vertices.size()), indices.data(), puffCount * 2);
    SetWriteZBuffer3D(true);
    SetUseBackCulling(culling);
    SetDrawBlendMode(blend, blendValue);
}
void DustBurstActor::SubRelease(void) { dustShader = nullptr; }
