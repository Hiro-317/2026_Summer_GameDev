#pragma once

#include "ShaderBase.h"

// 砂埃の半透明描画と、赤いオーラの加算描画を切り替える。
class StampDustShader : public ShaderBase
{
public:
    StampDustShader();
    ~StampDustShader() override;
    StampDustShader(const StampDustShader&) = delete;
    StampDustShader& operator=(const StampDustShader&) = delete;
    void Init(void) override;
    void Apply(void) override;
    void ResetApply(void) override;
    void Release(void) override;
    void SetParameters(float elapsedSeconds, float durationSeconds);
    void SetRenderMode(float mode) { parameters.renderMode = mode; }

private:
    struct Parameters
    {
        float elapsedSeconds;
        float durationSeconds;
        float intensity;
        float renderMode;
    };
    static_assert(sizeof(Parameters) == 16);
    Parameters parameters;
    int constantBufferHandle;
};