#pragma once

#include "ShaderBase.h"

class PurpleExplosionShader : public ShaderBase
{
public:
    PurpleExplosionShader(float durationSeconds, float intensity, float seed);
    ~PurpleExplosionShader() override;

    PurpleExplosionShader(const PurpleExplosionShader&) = delete;
    PurpleExplosionShader& operator=(const PurpleExplosionShader&) = delete;

    void Init(void) override;
    void Apply(void) override;
    void ResetApply(void) override;
    void Release(void) override;
    void SetElapsedSeconds(float seconds) { parameters.elapsedSeconds = seconds; }

private:
    // ピクセルシェーダーのb4へ渡すデータ。
    // DxLibの標準3D描画用定数バッファと重ならないスロットを使う。
    struct Parameters
    {
        float elapsedSeconds;
        float durationSeconds;
        float intensity;
        float seed;
    };
    static_assert(sizeof(Parameters) == 16);

    Parameters parameters;
    int constantBufferHandle;
};
