#include "PurpleExplosionActor.h"

#include <cmath>
#include <stdexcept>

#include "../Shader/PurpleExplosionShader.h"

PurpleExplosionActor::PurpleExplosionActor(const Vector3& position, float radius,
    float durationSeconds, float intensity, float seed, unsigned int color) :
    EffectBase(durationSeconds), radius(radius), intensity(intensity), seed(seed),
    color(color & 0xffffff),
    explosionShader(nullptr)
{
    if (!std::isfinite(radius) || radius <= 0.0f ||
        !std::isfinite(intensity) || intensity < 0.0f || !std::isfinite(seed)) {
        throw std::invalid_argument("Invalid purple explosion parameters");
    }
    SetPos(position);
}

void PurpleExplosionActor::DefaultCreateShader(void)
{
    explosionShader = new PurpleExplosionShader(GetDurationSeconds(), intensity, seed, color);
    CreateShader(explosionShader);
}

void PurpleExplosionActor::Draw(void)
{
    if (!GetIsDraw() || IsFinished() || explosionShader == nullptr) { return; }
    explosionShader->SetElapsedSeconds(GetElapsedSeconds());
    // 色はアクターごとに保持する。共有シェーダーを使う別の爆発には影響しない。
    explosionShader->SetColor(color);
    ActorBase::Draw();
}

void PurpleExplosionActor::SubDraw(void)
{
    // カメラの向きに合わせて板ポリゴンを毎回組み直す（ビルボード）。
    const Vector3 right(GetCameraRightVector());
    const Vector3 up(GetCameraUpVector());
    const Vector3 normal(GetCameraBackVector());
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

void PurpleExplosionActor::SubRelease(void)
{
    explosionShader = nullptr;
}
