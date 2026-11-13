#include "ExplosionActor.h"

#include <cmath>
#include <stdexcept>

#include "../Shader/ExplosionShader.h"

ExplosionActor::ExplosionActor(const Vector3& position, float radius,
    float durationSeconds, float intensity, float seed, unsigned int color) :
    EffectBase(durationSeconds), radius(radius), intensity(intensity), seed(seed),
    color(color & 0xffffff),
    explosionShader(nullptr)
{
    if (!std::isfinite(radius) || radius <= 0.0f ||
        !std::isfinite(intensity) || intensity < 0.0f || !std::isfinite(seed)) {
        throw std::invalid_argument("Invalid purple explosion parameters");
    }
    // 一時変数への参照を保持しない。再生時の着弾位置は呼び出し側がSetPosで指定する。
    SetPos(position);
}

void ExplosionActor::DefaultCreateShader(void)
{
    explosionShader = new ExplosionShader(GetDurationSeconds(), intensity, seed, color);
    CreateShader(explosionShader);
}

void ExplosionActor::Draw(void)
{
    if (!GetIsDraw() || IsFinished() || explosionShader == nullptr) { return; }
    explosionShader->SetElapsedSeconds(GetElapsedSeconds());
    // 色はアクターごとに保持する。共有シェーダーを使う別の爆発には影響しない。
    explosionShader->SetColor(color);
    ActorBase::Draw();
}

void ExplosionActor::SubDraw(void)
{
    // 爆発の中心からカメラ位置への方向を正面にする。
    // カメラの視線方向を流用しないので、画面端の爆発もカメラを直接向く。
    // アクターや親の回転は使わず、描画するたびに向きを更新する。
    Vector3 normal = Vector3(GetCameraPosition()) - trans.pos;
    constexpr float directionEpsilonSq = 0.000001f;
    if (normal.LengthSq() <= directionEpsilonSq) {
        // カメラと爆発が同じ位置では方向を決められないため、カメラの背面方向を使う。
        normal = Vector3(GetCameraBackVector());
    }
    normal.Normalize();

    // カメラの上方向を基準にして、ロールにも追従する直交した左右・上下軸を作る。
    Vector3 right = normal.Cross(Vector3(GetCameraUpVector()));
    if (right.LengthSq() <= directionEpsilonSq) {
        // 真上・真下から見る場合も、外積が0になって板が潰れるのを防ぐ。
        const Vector3 cameraRight(GetCameraRightVector());
        right = cameraRight - normal * cameraRight.Dot(normal);
    }
    right.Normalize();
    const Vector3 up = right.Cross(normal).Normalized();
    const float corners[4][2] = {{-1, 1}, {1, 1}, {-1, -1}, {1, -1}};
    VERTEX3DSHADER vertices[4]{};
    for (int i = 0; i < 4; ++i) {
        vertices[i].pos = (trans.pos + right * (corners[i][0] * radius) +
                           up * (corners[i][1] * radius)).ToVECTOR();
        vertices[i].norm = normal.ToVECTOR();
        vertices[i].dif = GetColorU8(255, 255, 255, 255);
        vertices[i].u = (corners[i][0] + 1.0f) * 0.5f;
        vertices[i].v = (1.0f - corners[i][1]) * 0.5f;
    }
    const unsigned short indices[6] = {0, 1, 2, 1, 3, 2};

    // 半透明描画パスの設定を保持し、このエフェクトの描画後に戻す。
    int previousBlend = 0, previousBlendParameter = 0;
    GetDrawBlendMode(&previousBlend, &previousBlendParameter);
    const int previousCulling = GetUseBackCulling();
    SetDrawBlendMode(DX_BLENDMODE_PMA_ADD, 255);
    SetUseBackCulling(DX_CULLING_NONE);
    // 通常の3Dシーンでは深度判定・書き込みが有効になっている。
    // 爆発は壁の裏に隠すが、透明部分で後続の描画を遮らないよう深度は書き込まない。
    // 描画後はシーンの通常設定（深度書き込み有効）に戻す。
    SetUseZBuffer3D(true);
    SetWriteZBuffer3D(false);
    DrawPolygonIndexed3DToShader(vertices, 4, indices, 2);
    SetWriteZBuffer3D(true);
    SetUseBackCulling(previousCulling);
    SetDrawBlendMode(previousBlend, previousBlendParameter);
}

void ExplosionActor::SubRelease(void)
{
    explosionShader = nullptr;
}
