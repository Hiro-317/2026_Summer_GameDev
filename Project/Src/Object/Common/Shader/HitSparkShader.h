#pragma once

#include "ShaderBase.h"

// 衝突点で発生する閃光・衝撃輪・火花の描画パラメーター。
class HitSparkShader : public ShaderBase
{
public:
    HitSparkShader();
    ~HitSparkShader() override;
    HitSparkShader(const HitSparkShader&) = delete;
    HitSparkShader& operator=(const HitSparkShader&) = delete;
    void Init(void) override;
    void Apply(void) override;
    void ResetApply(void) override;
    void Release(void) override;
    void SetParameters(float elapsedSeconds, float durationSeconds, float seed, bool critical);

private:
    struct Parameters
    {
        float elapsedSeconds;
        float durationSeconds;
        float intensity;
        float seed;
        float colorR;
        float colorG;
        float colorB;
        float padding;
    };
    static_assert(sizeof(Parameters) == 32);
    Parameters parameters;
    int constantBufferHandle;
};