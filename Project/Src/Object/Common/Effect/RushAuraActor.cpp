#include "RushAuraActor.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <stdexcept>

#include "../Shader/RushAuraShader.h"

RushAuraActor::RushAuraActor(const Transform& target, float radius,
    float trailLength, unsigned int color) :
    EffectBase(0.0f), target(target), direction(Vector3::Zonly(1.0f)),
    radius(radius), trailLength(trailLength), color(color & 0xffffff), auraShader(nullptr)
{
    if (!std::isfinite(radius) || radius <= 0.0f ||
        !std::isfinite(trailLength) || trailLength <= 0.0f) {
        throw std::invalid_argument("突進オーラの半径と長さには正の有限値を指定してください");
    }
}

void RushAuraActor::SetDirection(const Vector3& value)
{
    const float lengthSquared = value.LengthSq();
    if (!std::isfinite(lengthSquared) || lengthSquared <= 0.000001f) { return; }
    direction = value.Normalized();
}

void RushAuraActor::DefaultCreateShader(void)
{
    auraShader = new RushAuraShader();
    CreateShader(auraShader);
}

void RushAuraActor::Draw(void)
{
    if (!GetIsDraw() || IsFinished() || auraShader == nullptr) { return; }
    auraShader->SetParameters(GetElapsedSeconds(), 3.0f, 6.0f, color);
    // 既存の爆発・ブレスと同様、共通描画でシェーダーを適用してSubDrawへ進む。
    ActorBase::Draw();
}

void RushAuraActor::SubDraw(void)
{
    constexpr int radialSegments = 32;
    constexpr int lengthSegments = 28;
    constexpr int ringStride = radialSegments + 1;
    constexpr int shellVertices = (lengthSegments + 1) * ringStride;
    constexpr int corePlanes = 3;
    constexpr int coreStride = (lengthSegments + 1) * 2;
    constexpr int vertexCount = shellVertices + corePlanes * coreStride;
    constexpr int indexCount = lengthSegments * radialSegments * 6 + corePlanes * lengthSegments * 6;
    constexpr float twoPi = 6.28318530718f;
    std::array<VERTEX3DSHADER, vertexCount> vertices{};
    std::array<unsigned short, indexCount> indices{};
    const Vector3 helper = std::abs(direction.y) < 0.9f
        ? Vector3::Yonly(1.0f) : Vector3::Xonly(1.0f);
    const Vector3 right = helper.Cross(direction).Normalized();
    const Vector3 up = direction.Cross(right).Normalized();

    // 前側の丸みは残し、後方ほど広がって散る外殻にする。
    auto profile = [&](float u) {
        constexpr float headEnd = 0.25f;
        if (u <= headEnd) {
            const float axial = 1.0f - u / headEnd;
            return Vector3(radius * axial, radius * std::sqrt((std::max)(0.25f, 1.0f - axial * axial)), 0.0f);
        }
        const float tail = (u - headEnd) / (1.0f - headEnd);
        const float width = radius * (1.0f + 0.65f * tail * tail);
        return Vector3(-trailLength * tail, width, 0.0f);
    };

    // 体を包む丸い外殻。前転の回転は頂点へ適用しない。
    for (int ring = 0; ring <= lengthSegments; ++ring) {
        const float u = static_cast<float>(ring) / lengthSegments;
        const Vector3 shape = profile(u);
        const Vector3 center = target.pos + direction * shape.x;
        const float width = shape.y;
        for (int segment = 0; segment <= radialSegments; ++segment) {
            const float v = static_cast<float>(segment) / radialSegments;
            const Vector3 radial = right * std::cos(twoPi * v) + up * std::sin(twoPi * v);
            auto& vertex = vertices[ring * ringStride + segment];
            vertex.pos = (center + radial * width).ToVECTOR();
            // 丸い面の接線から法線を求め、シェーダーの輪郭光も球面へ合わせる。
            const Vector3 slope = profile((std::min)(1.0f, u + 0.001f)) - profile((std::max)(0.0f, u - 0.001f));
            vertex.norm = (radial * (-slope.x) + direction * slope.y).Normalized().ToVECTOR();
            vertex.u = u;
            vertex.v = v;
            vertex.su = 0.0f;
        }
    }

    // 外殻だけでは裏から薄くなるため、内部に柔らかい炎の面を重ねる。
    for (int plane = 0; plane < corePlanes; ++plane) {
        const float angle = DX_PI_F * static_cast<float>(plane) / corePlanes;
        const Vector3 radial = right * std::cos(angle) + up * std::sin(angle);
        const Vector3 normal = radial.Cross(direction).Normalized();
        for (int ring = 0; ring <= lengthSegments; ++ring) {
            const float u = static_cast<float>(ring) / lengthSegments;
            const Vector3 shape = profile(u);
            const Vector3 center = target.pos + direction * shape.x;
            const float width = shape.y;
            for (int edge = 0; edge < 2; ++edge) {
                auto& vertex = vertices[shellVertices + plane * coreStride + ring * 2 + edge];
                vertex.pos = (center + radial * (width * (edge == 0 ? -1.0f : 1.0f))).ToVECTOR();
                vertex.norm = normal.ToVECTOR();
                vertex.u = u;
                vertex.v = static_cast<float>(edge);
                vertex.su = 1.0f;
            }
        }
    }
    for (auto& vertex : vertices) {
        vertex.dif = GetColorU8(255, 255, 255, 255);
        // シェーダーで位置を揺らすため、流れと直交する基底と大きさを渡す。
        vertex.tan = right.ToVECTOR();
        vertex.binorm = up.ToVECTOR();
        vertex.sv = radius;
    }

    int count = 0;
    for (int ring = 0; ring < lengthSegments; ++ring) {
        for (int segment = 0; segment < radialSegments; ++segment) {
            const unsigned short a = static_cast<unsigned short>(ring * ringStride + segment);
            const unsigned short b = a + ringStride;
            indices[count++] = a; indices[count++] = b; indices[count++] = a + 1;
            indices[count++] = a + 1; indices[count++] = b; indices[count++] = b + 1;
        }
    }
    for (int plane = 0; plane < corePlanes; ++plane) {
        for (int ring = 0; ring < lengthSegments; ++ring) {
            const unsigned short a = static_cast<unsigned short>(shellVertices + plane * coreStride + ring * 2);
            indices[count++] = a; indices[count++] = a + 2; indices[count++] = a + 1;
            indices[count++] = a + 1; indices[count++] = a + 2; indices[count++] = a + 3;
        }
    }

    int previousBlend = 0, previousBlendParameter = 0;
    GetDrawBlendMode(&previousBlend, &previousBlendParameter);
    const int previousCulling = GetUseBackCulling();
    SetDrawBlendMode(DX_BLENDMODE_PMA_ADD, 255);
    SetUseBackCulling(DX_CULLING_NONE);
    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(false);
    DrawPolygonIndexed3DToShader(vertices.data(), vertexCount, indices.data(), count / 3);
    SetWriteZBuffer3D(true);
    SetUseBackCulling(previousCulling);
    SetDrawBlendMode(previousBlend, previousBlendParameter);
}

void RushAuraActor::SubRelease(void)
{
    auraShader = nullptr;
}
