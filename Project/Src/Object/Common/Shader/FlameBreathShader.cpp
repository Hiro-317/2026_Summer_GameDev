#include "FlameBreathShader.h"

#include <stdexcept>

#include "../../../pch.h"
#include "../../../Manager/Shader/ShaderResourceManager.h"

FlameBreathShader::FlameBreathShader() : parameters{}, constantBufferHandle(-1)
{
}

FlameBreathShader::~FlameBreathShader()
{
    Release();
}

void FlameBreathShader::SetParameters(float elapsedSeconds, float durationSeconds,
    float intensity, float flowSpeed, unsigned int color, float seed)
{
    parameters.elapsedSeconds = elapsedSeconds;
    parameters.durationSeconds = durationSeconds;
    parameters.intensity = intensity;
    parameters.flowSpeed = flowSpeed;
    parameters.colorR = static_cast<float>((color >> 16) & 0xff) / 255.0f;
    parameters.colorG = static_cast<float>((color >> 8) & 0xff) / 255.0f;
    parameters.colorB = static_cast<float>(color & 0xff) / 255.0f;
    parameters.seed = seed;
}

void FlameBreathShader::Init(void)
{
    if (constantBufferHandle >= 0) { return; }
    auto& resources = ShaderResourceManager::GetIns();
    resources.CreateVertexShader(VERTEX_SHADER_TYPE::FlameBreath);
    resources.CreatePixelShader(PIXEL_SHADER_TYPE::FlameBreath);
    vertexShaderHandle = resources.GetVertexShader(VERTEX_SHADER_TYPE::FlameBreath);
    pixelShaderHandle = resources.GetPixelShader(PIXEL_SHADER_TYPE::FlameBreath);
    if (vertexShaderHandle < 0 || pixelShaderHandle < 0) {
        throw std::runtime_error("Could not load FlameBreathVS/PS.cso; build the shaders first");
    }
    constantBufferHandle = CreateShaderConstantBuffer(sizeof(Parameters));
    if (constantBufferHandle < 0) {
        throw std::runtime_error("Could not create flame breath constant buffer");
    }
}

void FlameBreathShader::Apply(void)
{
    auto* buffer = static_cast<Parameters*>(GetBufferShaderConstantBuffer(constantBufferHandle));
    if (buffer == nullptr) {
        throw std::runtime_error("Could not map flame breath constant buffer");
    }
    *buffer = parameters;
    UpdateShaderConstantBuffer(constantBufferHandle);
    ShaderBase::Apply();
    SetShaderConstantBuffer(constantBufferHandle, DX_SHADERTYPE_PIXEL, 4);
}

void FlameBreathShader::ResetApply(void)
{
    SetShaderConstantBuffer(-1, DX_SHADERTYPE_PIXEL, 4);
    ShaderBase::ResetApply();
}

void FlameBreathShader::Release(void)
{
    if (constantBufferHandle >= 0) {
        DeleteShaderConstantBuffer(constantBufferHandle);
        constantBufferHandle = -1;
    }
    // シェーダーは共有リソースなので、このインスタンスでは削除しない。
    ShaderBase::Release();
}
