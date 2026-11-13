#include "FlameBreathActor.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <stdexcept>

#include "../Shader/FlameBreathShader.h"
#include "../../../Manager/TimeScale/TimeScale.h"

FlameBreathActor::FlameBreathActor(const Vector3& startPoint, const Vector3& direction,
    const Vector3& center, float radius, const Vector3& normal,
    const FlameBreathSettings& settings) :
    EffectBase(settings.durationSeconds), direction(), settings(settings),
    stopping(false), stopElapsedSeconds(0.0f), stopFrontFraction(0.0f),
    useEndCircle(false), endCircleCenter(), endCircleNormal(0.0f, 1.0f, 0.0f),
    breathShader(nullptr)
{
    SetStartPoint(startPoint);
    SetDirection(direction);
    SetStrength(settings.distance, settings.radius, settings.intensity);
    SetFlowSpeed(settings.flowSpeed);
    SetColor(settings.color);
    if (!std::isfinite(settings.stopSeconds) || settings.stopSeconds < 0.0f) {
        throw std::invalid_argument("Flame breath stop time must be finite and nonnegative");
    }
    SetEndCircle(center, radius, normal);
    if (!std::isfinite(settings.seed)) {
        throw std::invalid_argument("Flame breath seed must be finite");
    }
}

void FlameBreathActor::SetStartPoint(const Vector3& startPoint)
{
    if (!std::isfinite(startPoint.x) || !std::isfinite(startPoint.y) ||
        !std::isfinite(startPoint.z)) {
        throw std::invalid_argument("Flame breath start point must be finite");
    }
    SetPos(startPoint);
    if (useEndCircle) {
        const Vector3 delta = endCircleCenter - startPoint;
        if (delta.LengthSq() > 0.00000001f) {
            direction = delta.Normalized();
            settings.distance = delta.Length();
        }
    }
}

void FlameBreathActor::SetDirection(const Vector3& value)
{
    const float lengthSquared = value.LengthSq();
    if (!std::isfinite(lengthSquared) || lengthSquared <= 0.00000001f) {
        throw std::invalid_argument("Flame breath direction must be finite and nonzero");
    }
    direction = value.Normalized();
    useEndCircle = false;
}

void FlameBreathActor::SetStrength(float distance, float radius, float intensity)
{
    // 不正な値の場合は現在の設定を変更せず、呼び出し側へエラーを返す。
    if (!std::isfinite(distance) || distance <= 0.0f ||
        !std::isfinite(radius) || radius <= 0.0f ||
        !std::isfinite(intensity) || intensity < 0.0f) {
        throw std::invalid_argument("Invalid flame breath strength");
    }
    useEndCircle = false;
    settings.distance = distance;
    settings.radius = radius;
    settings.intensity = intensity;
}

void FlameBreathActor::SetEndCircle(const Vector3& center, float radius, const Vector3& normal)
{
    const Vector3 delta = center - trans.pos;
    const float distanceSquared = delta.LengthSq();
    const float normalSquared = normal.LengthSq();
    if (!std::isfinite(distanceSquared) || distanceSquared <= 0.00000001f ||
        !std::isfinite(normalSquared) || normalSquared <= 0.00000001f ||
        !std::isfinite(radius) || radius <= 0.0f) {
        throw std::invalid_argument("Invalid flame breath end circle");
    }
    const Vector3 unitNormal = normal.Normalized();
    // 開始点が終端面上にあると、円錐が一枚の平面へ潰れてしまう。
    if (std::abs(delta.Dot(unitNormal)) <= 0.0001f) {
        throw std::invalid_argument("Flame breath start must be outside the end circle plane");
    }
    endCircleCenter = center;
    endCircleNormal = unitNormal;
    direction = delta.Normalized();
    settings.distance = std::sqrt(distanceSquared);
    settings.radius = radius;
    useEndCircle = true;
}

void FlameBreathActor::SetFlowSpeed(float speed)
{
    if (!std::isfinite(speed) || speed < 0.0f) {
        throw std::invalid_argument("Flame breath speed must be finite and nonnegative");
    }
    settings.flowSpeed = speed;
}

void FlameBreathActor::DefaultCreateShader(void)
{
    breathShader = new FlameBreathShader();
    CreateShader(breathShader);
}

void FlameBreathActor::Restart(void)
{
    // EffectBase*からRestartした場合も、毎回ブレス固有の停止状態を初期化する。
    stopping = false;
    stopElapsedSeconds = 0.0f;
    stopFrontFraction = 0.0f;
    EffectBase::Restart();
}

void FlameBreathActor::Stop(void)
{
    if (IsFinished() || stopping) { return; }
    // 初期化前のキャンセル、有限寿命の終了、演出時間0はその場で止める。
    if (breathShader == nullptr || settings.stopSeconds == 0.0f ||
        (!IsInfinite() && GetElapsedSeconds() >= GetDurationSeconds())) {
        StopImmediately();
        return;
    }
    const float growthSeconds = IsInfinite() ? 0.24f : GetDurationSeconds() * 0.16f;
    float growth = std::clamp(GetElapsedSeconds() / growthSeconds, 0.0f, 1.0f);
    stopFrontFraction = growth * growth * (3.0f - 2.0f * growth);
    if (stopFrontFraction <= 0.000001f) { StopImmediately(); return; }
    stopping = true;
    stopElapsedSeconds = 0.0f;
}

void FlameBreathActor::StopImmediately(void)
{
    stopping = false;
    stopElapsedSeconds = 0.0f;
    stopFrontFraction = 0.0f;
    EffectBase::Stop();
}

void FlameBreathActor::Update(void)
{
    if (IsFinished()) { return; }
    if (stopping) {
        stopElapsedSeconds += (1.0f / 60.0f) * TimeScale::Get();
        if (stopElapsedSeconds >= settings.stopSeconds) {
            StopImmediately();
            return;
        }
    }
    EffectBase::Update();
}

void FlameBreathActor::Draw(void)
{
    if (!GetIsDraw() || IsFinished() || breathShader == nullptr) { return; }
    // 描画直前にインスタンス固有の色・時間・強さを渡す。
    // 同じ共有シェーダーを使っている別のブレスには影響しない。
    breathShader->SetParameters(GetElapsedSeconds(), GetDurationSeconds(),
        settings.intensity, settings.flowSpeed, settings.color, settings.seed);
    ActorBase::Draw();
}

void FlameBreathActor::SubDraw(void)
{
    // 出始めは短く、有限寿命なら最初の16%、無期限なら0.24秒で設定距離まで伸びる。
    // 中心軸は常にdirectionの直線上に置き、炎の揺れはシェーダー内で表現する。
    const float growthSeconds = IsInfinite() ? 0.24f : GetDurationSeconds() * 0.16f;
    float growth = std::clamp(GetElapsedSeconds() / growthSeconds, 0.0f, 1.0f);
    growth = growth * growth * (3.0f - 2.0f * growth);
    // 先端と後端を別々に管理する。再生時は先端が伸び、停止時は後端だけが進む。
    const float frontFraction = stopping ? stopFrontFraction : growth;
    const float tailFraction = stopping ? frontFraction *
        std::clamp(stopElapsedSeconds / settings.stopSeconds, 0.0f, 1.0f) : 0.0f;
    if (frontFraction - tailFraction <= 0.000001f) { return; }
    Vector3 axis = direction;
    float maximumDistance = settings.distance;
    if (useEndCircle) {
        // SetPosで口元を移動した場合も、終端円を追いかけて位置を固定する。
        const Vector3 delta = endCircleCenter - trans.pos;
        if (delta.LengthSq() <= 0.00000001f) { return; }
        axis = delta.Normalized();
        maximumDistance = delta.Length();
    }
    const float distance = maximumDistance;
    const float radius = settings.radius;
    const Vector3 tip = trans.pos + axis * (distance * frontFraction);

    // カメラに向けた板ではなく、円周を結んだ立体の円錐を作る。
    // 終端円指定時は床・斜面の向きを使い、通常はブレス軸に垂直な円を使う。
    const Vector3 planeNormal = useEndCircle ? endCircleNormal : axis;
    if (std::abs((tip - trans.pos).Dot(planeNormal)) <= 0.0001f) { return; }
    const Vector3 helper = std::abs(planeNormal.y) < 0.9f ?
        Vector3(0.0f, 1.0f, 0.0f) : Vector3(1.0f, 0.0f, 0.0f);
    const Vector3 side = helper.Cross(planeNormal).Normalized();
    const Vector3 up = planeNormal.Cross(side).Normalized();
    constexpr int radialSegments = 32;
    constexpr int lengthSegments = 8;
    constexpr int ringStride = radialSegments + 1;
    constexpr int bodyVertexCount = (lengthSegments + 1) * ringStride;
    constexpr int capCenterIndex = bodyVertexCount;
    constexpr int capRingStart = capCenterIndex + 1;
    // 円錐内部に3枚の炎の断面を重ねる。どの視点でも白い芯と柔らかい輪郭を残す。
    constexpr int corePlanes = 3;
    constexpr int coreVertexStart = capRingStart + ringStride;
    constexpr int coreVerticesPerPlane = (lengthSegments + 1) * 2;
    constexpr int vertexCount = coreVertexStart + corePlanes * coreVerticesPerPlane;
    constexpr int indexCount = lengthSegments * radialSegments * 6 + radialSegments * 3 +
        corePlanes * lengthSegments * 6;
    constexpr float twoPi = 6.28318530718f;
    std::array<VERTEX3DSHADER, vertexCount> vertices{};
    std::array<unsigned short, indexCount> indices{};

    for (int ring = 0; ring <= lengthSegments; ++ring) {
        const float ringFraction = static_cast<float>(ring) / lengthSegments;
        const float u = tailFraction + (frontFraction - tailFraction) * ringFraction;
        const Vector3 center = trans.pos + axis * (distance * u);
        for (int segment = 0; segment <= radialSegments; ++segment) {
            const float v = static_cast<float>(segment) / radialSegments;
            const float angle = twoPi * v;
            const Vector3 radial = side * std::cos(angle) + up * std::sin(angle);
            auto& vertex = vertices[ring * ringStride + segment];
            // 半径をuに比例させる。末端は指定した半径そのものになり、手前で切れない。
            vertex.pos = (center + radial * (radius * u)).ToVECTOR();
            const Vector3 tangent = -side * std::sin(angle) + up * std::cos(angle);
            vertex.norm = tangent.Cross(axis * distance + radial * radius).Normalized().ToVECTOR();
            vertex.u = u;
            vertex.v = v;
            vertex.su = 0.0f; // 円錐の側面
            vertex.sv = 1.0f;
        }
    }
    // 終端面も円周まで埋める。UVは円の中心が(0.5,0.5)、外周が半径0.5。
    auto& capCenter = vertices[capCenterIndex];
    capCenter.pos = tip.ToVECTOR();
    capCenter.u = capCenter.v = 0.5f;
    for (int segment = 0; segment <= radialSegments; ++segment) {
        const float angle = twoPi * static_cast<float>(segment) / radialSegments;
        auto& vertex = vertices[capRingStart + segment];
        vertex.pos = vertices[lengthSegments * ringStride + segment].pos;
        vertex.u = 0.5f + std::cos(angle) * 0.5f;
        vertex.v = 0.5f + std::sin(angle) * 0.5f;
    }
    for (int i = capCenterIndex; i < coreVertexStart; ++i) {
        vertices[i].norm = planeNormal.ToVECTOR();
        vertices[i].su = 1.0f; // 実際の終端円。視線方向では隠さない。
        vertices[i].sv = 1.0f;
    }
    for (int plane = 0; plane < corePlanes; ++plane) {
        const float angle = twoPi * static_cast<float>(plane) / (corePlanes * 2);
        const Vector3 radial = side * std::cos(angle) + up * std::sin(angle);
        const Vector3 normal = radial.Cross(axis).Normalized();
        for (int ring = 0; ring <= lengthSegments; ++ring) {
            const float fraction = static_cast<float>(ring) / lengthSegments;
            const float u = tailFraction + (frontFraction - tailFraction) * fraction;
            const Vector3 center = trans.pos + axis * (distance * u);
            for (int edge = 0; edge < 2; ++edge) {
                auto& vertex = vertices[coreVertexStart + plane * coreVerticesPerPlane + ring * 2 + edge];
                vertex.pos = (center + radial * (radius * u * (edge == 0 ? -1.0f : 1.0f))).ToVECTOR();
                vertex.norm = normal.ToVECTOR();
                vertex.u = u;
                vertex.v = static_cast<float>(edge);
                vertex.su = 2.0f; // 元の見た目に近い、円錐内部の炎
                vertex.sv = 1.0f;
            }
        }
    }
    for (auto& vertex : vertices) { vertex.dif = GetColorU8(255, 255, 255, 255); }
    int index = 0;
    for (int ring = 0; ring < lengthSegments; ++ring) {
        for (int segment = 0; segment < radialSegments; ++segment) {
            const unsigned short a = static_cast<unsigned short>(ring * ringStride + segment);
            const unsigned short b = a + ringStride;
            // 頂点が一点に集まる根元では、面積0の三角形を作らない。
            if (ring > 0 || tailFraction > 0.0f) {
                indices[index++] = a; indices[index++] = b; indices[index++] = a + 1;
            }
            indices[index++] = a + 1; indices[index++] = b; indices[index++] = b + 1;
        }
    }
    for (int segment = 0; segment < radialSegments; ++segment) {
        indices[index++] = capCenterIndex;
        indices[index++] = static_cast<unsigned short>(capRingStart + segment);
        indices[index++] = static_cast<unsigned short>(capRingStart + segment + 1);
    }

    for (int plane = 0; plane < corePlanes; ++plane) {
        for (int ring = 0; ring < lengthSegments; ++ring) {
            const unsigned short a = static_cast<unsigned short>(coreVertexStart +
                plane * coreVerticesPerPlane + ring * 2);
            const unsigned short b = a + 2;
            if (ring > 0 || tailFraction > 0.0f) {
                indices[index++] = a; indices[index++] = b; indices[index++] = a + 1;
            }
            indices[index++] = a + 1; indices[index++] = b; indices[index++] = b + 1;
        }
    }

    int previousBlend = 0, previousBlendParameter = 0;
    GetDrawBlendMode(&previousBlend, &previousBlendParameter);
    const int previousCulling = GetUseBackCulling();
    SetDrawBlendMode(DX_BLENDMODE_PMA_ADD, 255);
    SetUseBackCulling(DX_CULLING_NONE);
    // 壁には隠れるが、透明部分で後続の物体を遮らないよう深度を書き込まない。
    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(false);
    DrawPolygonIndexed3DToShader(vertices.data(), vertexCount, indices.data(), index / 3);
    // 既存の3Dシーンが使用する通常設定へ戻す。
    SetWriteZBuffer3D(true);
    SetUseBackCulling(previousCulling);
    SetDrawBlendMode(previousBlend, previousBlendParameter);
}

void FlameBreathActor::SubRelease(void)
{
    // シェーダー本体の解放・deleteはActorBase::Releaseに任せる。
    breathShader = nullptr;
}
