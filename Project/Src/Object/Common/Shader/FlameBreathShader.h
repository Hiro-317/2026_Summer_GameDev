#pragma once

#include "ShaderBase.h"

// ShaderResourceManagerの共有シェーダーを適用する、ブレス用のシェーダー制御。
class FlameBreathShader : public ShaderBase
{
public:
    FlameBreathShader();
    ~FlameBreathShader() override;
    FlameBreathShader(const FlameBreathShader&) = delete;
    FlameBreathShader& operator=(const FlameBreathShader&) = delete;

    void Init(void) override;
    void Apply(void) override;
    void ResetApply(void) override;
    void Release(void) override;

    // 時間・設定はアクターから毎回受け取り、個別の定数バッファへ書き込む。
    void SetParameters(float elapsedSeconds, float durationSeconds, float intensity,
                       float flowSpeed, unsigned int color, float seed);

private:
    // PSのb4。HLSLと同じ順番で、16バイトを2組にして配置する。
    struct Parameters
    {
        float elapsedSeconds;
        float durationSeconds;
        float intensity;
        float flowSpeed;
        float colorR;
        float colorG;
        float colorB;
        float seed;
    };
    static_assert(sizeof(Parameters) == 32);

    Parameters parameters;
    int constantBufferHandle;
};
