#include "FlameBreathActor.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

#include "../Shader/FlameBreathShader.h"

FlameBreathActor::FlameBreathActor(const Vector3& startPoint, const Vector3& direction,
    const FlameBreathSettings& settings) :
    EffectBase(settings.durationSeconds), direction(), settings(settings), breathShader(nullptr)
{
    SetStartPoint(startPoint);
    SetDirection(direction);
    SetStrength(settings.distance, settings.radius, settings.intensity);
    SetFlowSpeed(settings.flowSpeed);
    SetColor(settings.color);
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
}

void FlameBreathActor::SetDirection(const Vector3& value)
{
    const float lengthSquared = value.LengthSq();
    if (!std::isfinite(lengthSquared) || lengthSquared <= 0.00000001f) {
        throw std::invalid_argument("Flame breath direction must be finite and nonzero");
    }
    direction = value.Normalized();
}

void FlameBreathActor::SetStrength(float distance, float radius, float intensity)
{
    // 不正な値の場合は現在の設定を変更せず、呼び出し側へエラーを返す。
    if (!std::isfinite(distance) || distance <= 0.0f ||
        !std::isfinite(radius) || radius <= 0.0f ||
        !std::isfinite(intensity) || intensity < 0.0f) {
        throw std::invalid_argument("Invalid flame breath strength");
    }
    settings.distance = distance;
    settings.radius = radius;
    settings.intensity = intensity;
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
    const float reach = 0.05f + 0.95f * growth;
    const float distance = settings.distance * reach;
    const float radius = settings.radius * (0.25f + 0.75f * growth);
    const Vector3 tip = trans.pos + direction * distance;

    // 中心軸上の座標だけを渡し、横幅は頂点シェーダーのカメラ座標上で付ける。
    // NORMALには描画用の法線ではなく、方向×現在の距離を入れる。
    // TEXCOORD1はxが本体/補助炎の区別、yが幅オフセットまたは補助炎の半径。
    VERTEX3DSHADER vertices[8]{};
    const float signs[4] = {-1.0f, 1.0f, -1.0f, 1.0f};
    for (int i = 0; i < 4; ++i) {
        const bool isTip = i >= 2;
        vertices[i].pos = (isTip ? tip : trans.pos).ToVECTOR();
        vertices[i].u = isTip ? 1.0f : 0.0f;
        vertices[i].v = (signs[i] + 1.0f) * 0.5f;
        vertices[i].su = 0.0f; // 本体の描画モード
        vertices[i].sv = radius * signs[i]; // カメラ座標で付ける符号付きの横幅
    }

    // 軸方向から見て本体の長さが縮む場合に備え、先端側に画面正対の補助炎を置く。
    // 表示量は描画時のカメラ行列からVSが求めるため、CPUのカメラ軸へ依存しない。
    const Vector3 capCenter = trans.pos + direction * (distance * 0.85f);
    const float corners[4][2] = {{-1, 1}, {1, 1}, {-1, -1}, {1, -1}};
    for (int i = 0; i < 4; ++i) {
        auto& vertex = vertices[i + 4];
        vertex.pos = capCenter.ToVECTOR();
        vertex.u = (corners[i][0] + 1.0f) * 0.5f;
        vertex.v = (corners[i][1] + 1.0f) * 0.5f;
        vertex.su = 1.0f; // 正面用の描画モード
        vertex.sv = radius * 0.75f; // 補助炎の半径
    }
    for (auto& vertex : vertices) {
        vertex.norm = (direction * distance).ToVECTOR();
        vertex.dif = GetColorU8(255, 255, 255, 255);
    }
    const unsigned short indices[12] = {0, 1, 2, 1, 3, 2, 4, 5, 6, 5, 7, 6};

    int previousBlend = 0, previousBlendParameter = 0;
    GetDrawBlendMode(&previousBlend, &previousBlendParameter);
    const int previousCulling = GetUseBackCulling();
    SetDrawBlendMode(DX_BLENDMODE_PMA_ADD, 255);
    SetUseBackCulling(DX_CULLING_NONE);
    // 壁には隠れるが、透明部分で後続の物体を遮らないよう深度を書き込まない。
    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(false);
    DrawPolygonIndexed3DToShader(vertices, 8, indices, 4);
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
