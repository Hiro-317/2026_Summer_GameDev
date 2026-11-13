#include "SupportParticleShader.h"

#include <stdexcept>

#include "../../../Manager/Shader/ShaderResourceManager.h"

SupportParticleShader::SupportParticleShader() : parameters{}, constantBufferHandle(-1) {}
SupportParticleShader::~SupportParticleShader() { Release(); }

void SupportParticleShader::SetParameters(float elapsedSeconds, float durationSeconds, unsigned int color)
{
    parameters.elapsedSeconds = elapsedSeconds;
    parameters.durationSeconds = durationSeconds;
    parameters.intensity = 1.5f;
    parameters.colorR = static_cast<float>((color >> 16) & 0xff) / 255.0f;
    parameters.colorG = static_cast<float>((color >> 8) & 0xff) / 255.0f;
    parameters.colorB = static_cast<float>(color & 0xff) / 255.0f;
}
void SupportParticleShader::Init(void)
{
    if (constantBufferHandle >= 0) { return; }
    auto& resources = ShaderResourceManager::GetIns();
    resources.CreateVertexShader(VERTEX_SHADER_TYPE::SupportParticle);
    resources.CreatePixelShader(PIXEL_SHADER_TYPE::SupportParticle);
    vertexShaderHandle = resources.GetVertexShader(VERTEX_SHADER_TYPE::SupportParticle);
    pixelShaderHandle = resources.GetPixelShader(PIXEL_SHADER_TYPE::SupportParticle);
    if (vertexShaderHandle < 0 || pixelShaderHandle < 0) {
        throw std::runtime_error("SupportParticleVS/PS.csoの読み込みに失敗しました。シェーダーをビルドしてください");
    }
    constantBufferHandle = CreateShaderConstantBuffer(sizeof(Parameters));
    if (constantBufferHandle < 0) { throw std::runtime_error("回復・バフの粒子の定数バッファ生成に失敗しました"); }
    ShaderBase::Init();
}

void SupportParticleShader::Apply(void)
{
    auto* buffer = static_cast<Parameters*>(GetBufferShaderConstantBuffer(constantBufferHandle));
    if (buffer == nullptr) { throw std::runtime_error("回復・バフの粒子の定数バッファ取得に失敗しました"); }
    *buffer = parameters;
    UpdateShaderConstantBuffer(constantBufferHandle);
    ShaderBase::Apply();
    // 粒子の色と時間を、インスタンスごとの定数バッファで反映する。

    SetShaderConstantBuffer(constantBufferHandle, DX_SHADERTYPE_PIXEL, 4);
}

void SupportParticleShader::ResetApply(void)
{

    SetShaderConstantBuffer(-1, DX_SHADERTYPE_PIXEL, 4);
    ShaderBase::ResetApply();
}

void SupportParticleShader::Release(void)
{
    if (constantBufferHandle >= 0) {
        DeleteShaderConstantBuffer(constantBufferHandle);
        constantBufferHandle = -1;
    }
    // VS/PS本体は共有管理側が所有するため、個別には削除しない。
    ShaderBase::Release();
}