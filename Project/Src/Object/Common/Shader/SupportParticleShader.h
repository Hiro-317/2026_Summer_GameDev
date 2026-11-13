#pragma once

#include "ShaderBase.h"

// 回復・バフ共通の、色を指定できるパーティクル用シェーダー。
class SupportParticleShader : public ShaderBase
{
public:
    SupportParticleShader();
    ~SupportParticleShader() override;
    SupportParticleShader(const SupportParticleShader&) = delete;
    SupportParticleShader& operator=(const SupportParticleShader&) = delete;
    void Init(void) override;
    void Apply(void) override;
    void ResetApply(void) override;
    void Release(void) override;
    void SetParameters(float elapsedSeconds, float durationSeconds, unsigned int color);

private:
    struct Parameters
    {
        float elapsedSeconds;
        float durationSeconds;
        float intensity;
        float padding0;
        float colorR;
        float colorG;
        float colorB;
        float padding1;
    };
    static_assert(sizeof(Parameters) == 32);
    Parameters parameters;
    int constantBufferHandle;
};