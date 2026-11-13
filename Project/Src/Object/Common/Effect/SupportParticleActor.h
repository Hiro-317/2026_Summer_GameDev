#pragma once

#include "EffectBase.h"

class SupportParticleShader;

// 対象の中心から粒子が広がり、体を包んで消える回復・バフ共通エフェクト。
class SupportParticleActor : public EffectBase
{
public:
    explicit SupportParticleActor(unsigned int color, float radius = 85.0f);
    // 発動時の対象を保持し、選択対象が変わっても再生中の対象には追従し続ける。
    void Emit(const Vector3* targetPosition);
    void Draw(void) override;

private:
    void DefaultCreateShader(void) override;
    void SubDraw(void) override;
    void SubRelease(void) override;
    const Vector3* targetPosition;
    unsigned int color;
    float radius;
    SupportParticleShader* particleShader;
};