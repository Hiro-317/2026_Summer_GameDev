#pragma once

#include "EffectBase.h"

class StampDustShader;

// 着地点から広がる砂埃と赤いオーラ。再生中はプレイヤーを追従しない。
class StampDustActor : public EffectBase
{
public:
    StampDustActor(float radius = 300.0f, float footOffset = 50.0f, float durationSeconds = 0.9f);
    void Emit(const Vector3& playerPosition);
    void Draw(void) override;

private:
    void DefaultCreateShader(void) override;
    void SubDraw(void) override;
    void SubRelease(void) override;
    Vector3 origin;
    float radius;
    float footOffset;
    StampDustShader* dustShader;
};