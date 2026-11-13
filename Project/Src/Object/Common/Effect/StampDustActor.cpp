#include "StampDustActor.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <stdexcept>

#include "../Shader/StampDustShader.h"

StampDustActor::StampDustActor(float radius, float footOffset, float durationSeconds) :
    EffectBase(durationSeconds), origin(), radius(radius), footOffset(footOffset), dustShader(nullptr)
{
    if (!std::isfinite(radius) || radius <= 0.0f || !std::isfinite(footOffset) || footOffset < 0.0f) {
        throw std::invalid_argument("着地の砂埃には正の半径と非負の足元補正を指定してください");
    }
}

void StampDustActor::Emit(const Vector3& playerPosition)
{
    // 中心座標をコピーし、プレイヤーが次の行動へ移っても着地点に残す。
    origin = playerPosition - Vector3::Yonly(footOffset) + Vector3::Yonly(3.0f);
    Restart();
}

void StampDustActor::DefaultCreateShader(void)
{
    dustShader = new StampDustShader();
    CreateShader(dustShader);
}

void StampDustActor::Draw(void)
{
    if (!GetIsDraw() || IsFinished() || dustShader == nullptr) { return; }
    dustShader->SetParameters(GetElapsedSeconds(), GetDurationSeconds());
    ActorBase::Draw();
}

void StampDustActor::SubDraw(void)
{
    constexpr int puffCount = 20;
    constexpr int quadCount = puffCount + 1;
    constexpr float twoPi = 6.28318530718f;
    std::array<VERTEX3DSHADER, quadCount * 4> vertices{};
    std::array<unsigned short, quadCount * 6> indices{};
    const float life = std::clamp(GetElapsedSeconds() / GetDurationSeconds(), 0.0f, 1.0f);
    const float spread = 1.0f - std::pow(1.0f - life, 3.0f);
    const float corners[4][2] = {{-1, 1}, {1, 1}, {-1, -1}, {1, -1}};

    // 地面に沿う衝撃波用の面。床と重ならないよう、発生点は少し浮かせる。
    for (int edge = 0; edge < 4; ++edge) {
        auto& v = vertices[edge];
        v.pos = (origin + Vector3(corners[edge][0] * radius, 0.0f, corners[edge][1] * radius)).ToVECTOR();
        v.norm = VGet(0.0f, 0.0f, 0.0f);
        v.u = (corners[edge][0] + 1.0f) * 0.5f;
        v.v = (corners[edge][1] + 1.0f) * 0.5f;
    }

    // 砂煙を円周方向へ押し出し、少し舞い上げる。左右に揺らして均一な輪を崩す。
    for (int puff = 0; puff < puffCount; ++puff) {
        const float seed = static_cast<float>(puff) + 0.37f;
        const float angle = twoPi * static_cast<float>(puff) / puffCount;
        const float variation = 0.82f + 0.18f * std::sin(seed * 7.1f);
        const Vector3 outward(std::cos(angle), 0.0f, std::sin(angle));
        const Vector3 sideways(-outward.z, 0.0f, outward.x);
        const float travel = radius * (0.1f + 0.72f * spread) * variation;
        const Vector3 center = origin + outward * travel
            + sideways * (radius * 0.04f * std::sin(life * 12.0f + seed))
            + Vector3::Yonly(radius * (0.045f + 0.19f * std::sin(DX_PI_F * life)));
        const float width = radius * (0.13f + life * 0.13f) * variation;
        const float height = width * 0.78f;
        for (int edge = 0; edge < 4; ++edge) {
            auto& v = vertices[(puff + 1) * 4 + edge];
            v.pos = center.ToVECTOR();
            // 法線欄は形状種別と模様の種として渡し、VSでそのままPSへ渡す。
            v.norm = VGet(1.0f, seed, 0.0f);
            v.u = (corners[edge][0] + 1.0f) * 0.5f;
            v.v = (1.0f - corners[edge][1]) * 0.5f;
            v.su = corners[edge][0] * width;
            v.sv = corners[edge][1] * height;
        }
    }
    for (auto& vertex : vertices) { vertex.dif = GetColorU8(255, 255, 255, 255); }
    for (int quad = 0; quad < quadCount; ++quad) {
        const unsigned short a = static_cast<unsigned short>(quad * 4);
        const int index = quad * 6;
        indices[index] = a; indices[index + 1] = a + 1; indices[index + 2] = a + 2;
        indices[index + 3] = a + 2; indices[index + 4] = a + 1; indices[index + 5] = a + 3;
    }

    int previousBlend = 0, previousBlendParameter = 0;
    GetDrawBlendMode(&previousBlend, &previousBlendParameter);
    const int previousCulling = GetUseBackCulling();
    SetUseBackCulling(DX_CULLING_NONE);
    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(false);
    // 砂埃は半透明、混ざった赤い光は加算で重ねて質感を分ける。
    SetDrawBlendMode(DX_BLENDMODE_PMA_ALPHA, 255);
    DrawPolygonIndexed3DToShader(vertices.data(), static_cast<int>(vertices.size()), indices.data(), quadCount * 2);
    dustShader->SetRenderMode(1.0f);
    dustShader->Apply();
    SetDrawBlendMode(DX_BLENDMODE_PMA_ADD, 255);
    DrawPolygonIndexed3DToShader(vertices.data(), static_cast<int>(vertices.size()), indices.data(), quadCount * 2);
    SetWriteZBuffer3D(true);
    SetUseBackCulling(previousCulling);
    SetDrawBlendMode(previousBlend, previousBlendParameter);
}

void StampDustActor::SubRelease(void)
{
    dustShader = nullptr;
}