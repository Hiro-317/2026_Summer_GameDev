#pragma once

#include "EffectBase.h"

class HitSparkShader;

// 衝突点を値で保持し、短い閃光と火花を出して消えるヒット演出。
class HitSparkActor : public EffectBase
{
public:
    explicit HitSparkActor(float seed = 1.0f);
    void Emit(const Vector3& point, bool critical = false);
    void Draw(void) override;

private:
    void DefaultCreateShader(void) override;
    void SubDraw(void) override;
    void SubRelease(void) override;
    float seed;
    bool critical;
    HitSparkShader* hitShader;

    
};