#include "RushAuraShader.h"

#include <stdexcept>

#include "../../../Manager/Shader/ShaderResourceManager.h"

RushAuraShader::RushAuraShader() : parameters{}, constantBufferHandle(-1) {}
RushAuraShader::~RushAuraShader() { Release(); }

void RushAuraShader::SetParameters(float elapsedSeconds, float intensity, float flowSpeed, unsigned int color)
{
    parameters.elapsedSeconds = elapsedSeconds;
    parameters.intensity = intensity;
    parameters.flowSpeed = flowSpeed;
    parameters.colorR = static_cast<float>((color >> 16) & 0xff) / 255.0f;
    parameters.colorG = static_cast<float>((color >> 8) & 0xff) / 255.0f;
    parameters.colorB = static_cast<float>(color & 0xff) / 255.0f;
}

void RushAuraShader::Init(void)
{
    if (constantBufferHandle >= 0) { return; }
    auto& resources = ShaderResourceManager::GetIns();
    resources.CreateVertexShader(VERTEX_SHADER_TYPE::RushAura);
    resources.CreatePixelShader(PIXEL_SHADER_TYPE::RushAura);
    vertexShaderHandle = resources.GetVertexShader(VERTEX_SHADER_TYPE::RushAura);
    pixelShaderHandle = resources.GetPixelShader(PIXEL_SHADER_TYPE::RushAura);
    if (vertexShaderHandle < 0 || pixelShaderHandle < 0) {
        throw std::runtime_error("RushAuraVS/PS.csoの読み込みに失敗しました。シェーダーをビルドしてください");
    }
    constantBufferHandle = CreateShaderConstantBuffer(sizeof(Parameters));
    if (constantBufferHandle < 0) { throw std::runtime_error("突進オーラの定数バッファ生成に失敗しました"); }
    ShaderBase::Init();
}

void RushAuraShader::Apply(void)
{
    auto* buffer = static_cast<Parameters*>(GetBufferShaderConstantBuffer(constantBufferHandle));
    if (buffer == nullptr) { throw std::runtime_error("突進オーラの定数バッファ取得に失敗しました"); }
    *buffer = parameters;
    UpdateShaderConstantBuffer(constantBufferHandle);
    ShaderBase::Apply();
    // VSの位置揺らぎとPSの流れる模様を同じ時間で動かす。
    SetShaderConstantBuffer(constantBufferHandle, DX_SHADERTYPE_VERTEX, 4);
    SetShaderConstantBuffer(constantBufferHandle, DX_SHADERTYPE_PIXEL, 4);
}

void RushAuraShader::ResetApply(void)
{
    SetShaderConstantBuffer(-1, DX_SHADERTYPE_VERTEX, 4);
    SetShaderConstantBuffer(-1, DX_SHADERTYPE_PIXEL, 4);
    ShaderBase::ResetApply();
}

void RushAuraShader::Release(void)
{
    if (constantBufferHandle >= 0) {
        DeleteShaderConstantBuffer(constantBufferHandle);
        constantBufferHandle = -1;
    }
    // VS/PS本体は共有管理側が所有するため、個別には削除しない。
    ShaderBase::Release();
}