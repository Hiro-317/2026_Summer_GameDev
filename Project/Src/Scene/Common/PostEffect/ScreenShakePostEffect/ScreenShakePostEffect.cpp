#include "ScreenShakePostEffect.h"

#include <algorithm>
#include <cmath>

#include "../../../../Manager/TimeScale/TimeScale.h"
#include "../../../../Manager/Shader/ShaderResourceManager.h"

namespace
{
    constexpr float FRAME_DELTA_TIME = 1.0f / 60.0f;
    constexpr int SHADER_CONSTANT_SLOT = 4;
    constexpr const TCHAR* SHADER_PATH = _T("Data/Shader/ScreenShakePS.cso");
}

ScreenShakePostEffect::ScreenShakePostEffect(float strength, float frequency, float duration) :
    constantBufferHandle(-1),
    strength((std::max)(0.0f, strength)),
    frequency((std::max)(0.0f, frequency)),
    duration((std::max)(0.0f, duration)),
    elapsedTime(0.0f),
    offsetX(0.0f),
    offsetY(0.0f),
    finished(false)
{
}

void ScreenShakePostEffect::Init(void)
{
    elapsedTime = 0.0f;
    offsetX = 0.0f;
    offsetY = 0.0f;
    finished = (duration <= 0.0f);

    pixelShaderHandle = LoadPixelShader(SHADER_PATH);
    if (pixelShaderHandle < 0)
    {
        finished = true;
        return;
    }

    constantBufferHandle = CreateShaderConstantBuffer(sizeof(ShakeConstantBuffer));
    if (constantBufferHandle < 0)
    {
        Release();
        finished = true;
    }
}

void ScreenShakePostEffect::Update(void)
{
    if (finished) return;

    // 固定60FPS想定。実時間制御する場合はdeltaTimeを外部から渡す設計に変更する。
    elapsedTime += FRAME_DELTA_TIME * TimeScale::Get();

    if (elapsedTime >= duration)
    {
        elapsedTime = duration;
        offsetX = 0.0f;
        offsetY = 0.0f;
        finished = true;
        return;
    }

    const float progress = elapsedTime / duration;
    const float attenuation = (1.0f - progress) * (1.0f - progress);
    const float phase = elapsedTime * frequency;

    offsetX = std::sin(phase * 1.7f) * strength * attenuation;
    offsetY = std::cos(phase * 2.3f) * strength * attenuation;
}

bool ScreenShakePostEffect::IsFinished(void) const
{
    return finished;
}

void ScreenShakePostEffect::ApplyParameter(void)
{
    if (constantBufferHandle < 0) return;

    auto* buffer = static_cast<ShakeConstantBuffer*>(
        GetBufferShaderConstantBuffer(constantBufferHandle));
    if (buffer == nullptr) return;

    buffer->offsetX = offsetX;
    buffer->offsetY = offsetY;
    buffer->padding[0] = 0.0f;
    buffer->padding[1] = 0.0f;

    UpdateShaderConstantBuffer(constantBufferHandle);
    SetShaderConstantBuffer(constantBufferHandle, DX_SHADERTYPE_PIXEL, SHADER_CONSTANT_SLOT);
}

void ScreenShakePostEffect::ResetParameter(void)
{
    SetShaderConstantBuffer(-1, DX_SHADERTYPE_PIXEL, SHADER_CONSTANT_SLOT);
}

void ScreenShakePostEffect::Release(void)
{
    if (constantBufferHandle >= 0)
    {
        DeleteShaderConstantBuffer(constantBufferHandle);
        constantBufferHandle = -1;
    }

    pixelShaderHandle = -1;
}
