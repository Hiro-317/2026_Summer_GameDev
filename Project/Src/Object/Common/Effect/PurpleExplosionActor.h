#pragma once

#include "EffectBase.h"

class PurpleExplosionShader;

// 指定したワールド座標で紫色の火球・衝撃波・火花を描画するアクター。
// position: 中心座標、radius: 最大半径、durationSeconds: 再生時間（秒）。
// intensity: 明るさ（0以上）、seed: 火花とノイズの模様を決める値。
class PurpleExplosionActor : public EffectBase
{
public:
    PurpleExplosionActor(const Vector3& position, float radius = 150.0f,
                         float durationSeconds = 1.2f, float intensity = 2.0f,
                         float seed = 1.0f);
    ~PurpleExplosionActor() override = default;

    void Draw(void) override;

private:
    void DefaultCreateShader(void) override;
    void SubDraw(void) override;
    void SubRelease(void) override;

    float radius;
    float intensity;
    float seed;
    // 参照用ポインター。所有権はCreateShaderでActorBaseへ渡す。
    // ActorBase::Releaseがシェーダーの解放とdeleteを行う。
    PurpleExplosionShader* explosionShader;
};
