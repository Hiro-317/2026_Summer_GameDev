#pragma once

#include "ShaderBase.h"

class PurpleExplosionShader : public ShaderBase
{
public:
    PurpleExplosionShader(float durationSeconds, float intensity, float seed,
                          unsigned int color = 0x7509ff);
    ~PurpleExplosionShader() override;

    PurpleExplosionShader(const PurpleExplosionShader&) = delete;
    PurpleExplosionShader& operator=(const PurpleExplosionShader&) = delete;

    void Init(void) override;
    void Apply(void) override;
    void ResetApply(void) override;
    void Release(void) override;
    void SetElapsedSeconds(float seconds) { parameters.elapsedSeconds = seconds; }
    // 0xRRGGBBをGPUへ渡す0～1のRGBへ変換する。
    void SetColor(unsigned int rgb);

private:
    // ピクセルシェーダーのb4へ渡すデータ。
    // DxLibの標準3D描画用定数バッファと重ならないスロットを使う。
    struct Parameters
    {
        float elapsedSeconds;
        float durationSeconds;
        float intensity;
        float seed;

        // HLSLのfloat3 + floatと同じ配置。定数バッファは16バイト単位で揃える。
        float colorR;
        float colorG;
        float colorB;
        float padding;
    };
    static_assert(sizeof(Parameters) == 32);

    Parameters parameters;
    int constantBufferHandle;
};
