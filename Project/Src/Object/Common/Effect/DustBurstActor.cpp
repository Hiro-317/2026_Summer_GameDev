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
    // 基底のRestartが仮想Playを呼んでも再びRestartへ入らないよう、先に再生状態にする。
    EffectBase::Play();
    EffectBase::Restart();
}
void DustBurstActor::Play(void)
{
    if (IsFinished() || stopping) {
        // 待機・終了・消滅途中からは、前回の時間と消滅状態を引き継がず新しく出す。
        Restart();
        return;
    }
    // 通常の再生中なら時間は戻さない。繰り返し呼んでも寿命は延びない。
    EffectBase::Play();
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
void DustBurstActor::Play(void)
{
    stopping = false;
    EffectBase::Play();
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
        const float speed = outer ? 0.72f + random * 0.22f : 0.44f + random * 0.23f;
        const float reach = settings.radius * expansion * speed;
        auto& puff = puffs[i];
        // 床の輪とは別に、煙を時間とともに上昇させる。高さを潰して床へ貼り付けない。
        puff.width = settings.radius * (outer ? 0.27f : 0.20f) * swelling * (0.85f + random * 0.3f);
        puff.height = settings.height * (outer ? 0.85f : 0.60f) * swelling * (0.85f + random * 0.3f);
        puff.center = trans.pos + Vector3(std::cos(angle) * reach,
            puff.height * 0.55f + settings.height * 0.30f * t * t, std::sin(angle) * reach);
        puff.seed = id + settings.seed;
        puff.depth = (puff.center - camera).Dot(front);
    }
    // 不透明な加算ではなく、奥から半透明の煙を重ねる。
    std::sort(puffs.begin(), puffs.end(), [](const Puff& a, const Puff& b) { return a.depth > b.depth; });
    constexpr int ringSegments = 64;
    constexpr int ringVertexStart = puffCount * 4;
    constexpr int vertexCount = ringVertexStart + (ringSegments + 1) * 2;
    constexpr int indexCount = ringSegments * 6 + puffCount * 6;
    std::array<VERTEX3DSHADER, vertexCount> vertices{};
    std::array<unsigned short, indexCount> indices{};
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
        std::copy(face, face + 6, indices.begin() + ringSegments * 6 + i * 6);
    }
    // 床上の輪はワールド座標で作る。カメラへ向けず、地面に沿う円を保つ。
    const float ringRadius = settings.radius * expansion;
    const float halfWidth = settings.radius * 0.11f * swelling;
    const float innerRadius = (std::max)(ringRadius - halfWidth, 0.0f);
    const float outerRadius = ringRadius + halfWidth;
    const float groundY = trans.pos.y + settings.radius * 0.012f;
    for (int segment = 0; segment <= ringSegments; ++segment) {
        const float u = static_cast<float>(segment) / ringSegments;
        const float angle = twoPi * u;
        for (int edge = 0; edge < 2; ++edge) {
            auto& v = vertices[ringVertexStart + segment * 2 + edge];
            const float r = edge == 0 ? innerRadius : outerRadius;
            v.pos = VGet(trans.pos.x + std::cos(angle) * r, groundY,
                         trans.pos.z + std::sin(angle) * r);
            v.norm = VGet(settings.seed, 1.0f, 0.0f); // y=1は地面の輪。su/svは0のまま。
            v.dif = GetColorU8(255, 255, 255, 255);
            v.u = u;
            v.v = static_cast<float>(edge);
        }
    }
    for (int segment = 0; segment < ringSegments; ++segment) {
        const unsigned short a = static_cast<unsigned short>(ringVertexStart + segment * 2);
        const unsigned short b = a + 2;
        const int i = segment * 6;
        indices[i] = a; indices[i+1] = b; indices[i+2] = a + 1;
        indices[i+3] = a + 1; indices[i+4] = b; indices[i+5] = b + 1;
    }
    // インデックスは輪→奥から並べた砂煙の順。煙が輪の上へ重なるようにする。
    int blend = 0, blendValue = 0;
    GetDrawBlendMode(&blend, &blendValue);
    const int culling = GetUseBackCulling();
    SetDrawBlendMode(DX_BLENDMODE_PMA_ALPHA, 255);
    SetUseBackCulling(DX_CULLING_NONE);
    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(false);
    DrawPolygonIndexed3DToShader(vertices.data(), static_cast<int>(vertices.size()), indices.data(), indexCount / 3);
    SetWriteZBuffer3D(true);
    SetUseBackCulling(culling);
    SetDrawBlendMode(blend, blendValue);
}
void DustBurstActor::SubRelease(void) { dustShader = nullptr; }
