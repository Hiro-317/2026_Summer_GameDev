#pragma once

#include "ShaderBase.h"

// 共有VS/PSと、オーラごとの時間・流速・色を持つ定数バッファを管理する。
class RushAuraShader : public ShaderBase
{
public:
    RushAuraShader();
    ~RushAuraShader() override;
    RushAuraShader(const RushAuraShader&) = delete;
    RushAuraShader& operator=(const RushAuraShader&) = delete;
    void Init(void) override;
    void Apply(void) override;
    void ResetApply(void) override;
    void Release(void) override;
    void SetParameters(float elapsedSeconds, float intensity, float flowSpeed, unsigned int color);

private:
    struct Parameters
    {
        float elapsedSeconds;
        float intensity;
        float flowSpeed;
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