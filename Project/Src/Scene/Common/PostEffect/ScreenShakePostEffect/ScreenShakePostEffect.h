#pragma once

#include "../PostEffectBase.h"

// 画面全体を揺らす、一度きりのポストエフェクト
class ScreenShakePostEffect : public PostEffectBase
{
public:
    // strength: UV座標基準の最大振幅 / frequency: 振動速度 / duration: 秒
    ScreenShakePostEffect(float strength = 0.015f, float frequency = 40.0f, float duration = 0.5f);
    ~ScreenShakePostEffect() override = default;

    void Init(void) override;
    void Update(void) override;
    bool IsFinished(void) const override;
    void Release(void) override;

protected:
    void ApplyParameter(void) override;
    void ResetParameter(void) override;

private:
    // HLSLのcbuffer(b4)と同じ16バイト配置
    struct ShakeConstantBuffer
    {
        float offsetX;
        float offsetY;
        float padding[2];
    };

    int constantBufferHandle;
    float strength;
    float frequency;
    float duration;
    float elapsedTime;
    float offsetX;
    float offsetY;
    bool finished;
};
