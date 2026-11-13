#include "HitSparkShader.h"

#include <stdexcept>

#include "../../../Manager/Shader/ShaderResourceManager.h"

HitSparkShader::HitSparkShader() : parameters{}, constantBufferHandle(-1) {}
HitSparkShader::~HitSparkShader() { Release(); }

void HitSparkShader::SetParameters(float elapsedSeconds, float durationSeconds, float seed, bool critical)
{
    parameters.elapsedSeconds = elapsedSeconds;
    parameters.durationSeconds = durationSeconds;
    parameters.intensity = critical ? 5.2f : 4.0f;
    parameters.seed = seed;
    parameters.colorR = 1.0f;
    parameters.colorG = critical ? 0.14f : 0.085f;
    parameters.colorB = critical ? 0.025f : 0.012f;
}
void HitSparkShader::Init(void)
{
    if (constantBufferHandle >= 0) { return; }
    auto& resources = ShaderResourceManager::GetIns();
    resources.CreateVertexShader(VERTEX_SHADER_TYPE::HitSpark);
    resources.CreatePixelShader(PIXEL_SHADER_TYPE::HitSpark);
    vertexShaderHandle = resources.GetVertexShader(VERTEX_SHADER_TYPE::HitSpark);
    pixelShaderHandle = resources.GetPixelShader(PIXEL_SHADER_TYPE::HitSpark);
    if (vertexShaderHandle < 0 || pixelShaderHandle < 0) {
        throw std::runtime_error("HitSparkVS/PS.csoの読み込みに失敗しました。シェーダーをビルドしてください");
    }
    constantBufferHandle = CreateShaderConstantBuffer(sizeof(Parameters));
    if (constantBufferHandle < 0) { throw std::runtime_error("ヒット火花の定数バッファ生成に失敗しました"); }
    ShaderBase::Init();
}

void HitSparkShader::Apply(void)
{
    auto* buffer = static_cast<Parameters*>(GetBufferShaderConstantBuffer(constantBufferHandle));
    if (buffer == nullptr) { throw std::runtime_error("ヒット火花の定数バッファ取得に失敗しました"); }
    *buffer = parameters;
    UpdateShaderConstantBuffer(constantBufferHandle);
    ShaderBase::Apply();
    // 各ヒットの時間・色・模様を個別の定数バッファで反映する。

    SetShaderConstantBuffer(constantBufferHandle, DX_SHADERTYPE_PIXEL, 4);
}

void HitSparkShader::ResetApply(void)
{

    SetShaderConstantBuffer(-1, DX_SHADERTYPE_PIXEL, 4);
    ShaderBase::ResetApply();
}

void HitSparkShader::Release(void)
{
    if (constantBufferHandle >= 0) {
        DeleteShaderConstantBuffer(constantBufferHandle);
        constantBufferHandle = -1;
    }
    // VS/PS本体は共有管理側が所有するため、個別には削除しない。
    ShaderBase::Release();
}