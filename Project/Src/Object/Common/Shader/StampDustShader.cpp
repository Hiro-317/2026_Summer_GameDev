#include "StampDustShader.h"

#include <stdexcept>

#include "../../../Manager/Shader/ShaderResourceManager.h"

StampDustShader::StampDustShader() : parameters{}, constantBufferHandle(-1) {}
StampDustShader::~StampDustShader() { Release(); }

void StampDustShader::SetParameters(float elapsedSeconds, float durationSeconds)
{
    parameters.elapsedSeconds = elapsedSeconds;
    parameters.durationSeconds = durationSeconds;
    parameters.intensity = 1.8f;
    parameters.renderMode = 0.0f;
}
void StampDustShader::Init(void)
{
    if (constantBufferHandle >= 0) { return; }
    auto& resources = ShaderResourceManager::GetIns();
    resources.CreateVertexShader(VERTEX_SHADER_TYPE::StampDust);
    resources.CreatePixelShader(PIXEL_SHADER_TYPE::StampDust);
    vertexShaderHandle = resources.GetVertexShader(VERTEX_SHADER_TYPE::StampDust);
    pixelShaderHandle = resources.GetPixelShader(PIXEL_SHADER_TYPE::StampDust);
    if (vertexShaderHandle < 0 || pixelShaderHandle < 0) {
        throw std::runtime_error("StampDustVS/PS.csoの読み込みに失敗しました。シェーダーをビルドしてください");
    }
    constantBufferHandle = CreateShaderConstantBuffer(sizeof(Parameters));
    if (constantBufferHandle < 0) { throw std::runtime_error("着地の砂埃の定数バッファ生成に失敗しました"); }
    ShaderBase::Init();
}

void StampDustShader::Apply(void)
{
    auto* buffer = static_cast<Parameters*>(GetBufferShaderConstantBuffer(constantBufferHandle));
    if (buffer == nullptr) { throw std::runtime_error("着地の砂埃の定数バッファ取得に失敗しました"); }
    *buffer = parameters;
    UpdateShaderConstantBuffer(constantBufferHandle);
    ShaderBase::Apply();
    // 経過時間と描画モードを砂埃・オーラ共通のPSへ渡す。

    SetShaderConstantBuffer(constantBufferHandle, DX_SHADERTYPE_PIXEL, 4);
}

void StampDustShader::ResetApply(void)
{

    SetShaderConstantBuffer(-1, DX_SHADERTYPE_PIXEL, 4);
    ShaderBase::ResetApply();
}

void StampDustShader::Release(void)
{
    if (constantBufferHandle >= 0) {
        DeleteShaderConstantBuffer(constantBufferHandle);
        constantBufferHandle = -1;
    }
    // VS/PS本体は共有管理側が所有するため、個別には削除しない。
    ShaderBase::Release();
}