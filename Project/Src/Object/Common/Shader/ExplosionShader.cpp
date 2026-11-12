#include "ExplosionShader.h"

#include <stdexcept>

#include "../../../pch.h"
#include "../../../Manager/Shader/ShaderResourceManager.h"

ExplosionShader::ExplosionShader(float durationSeconds, float intensity, float seed,
    unsigned int color) :
    parameters{0.0f, durationSeconds, intensity, seed, 0.0f, 0.0f, 0.0f, 0.0f},
    constantBufferHandle(-1)
{
    SetColor(color);
}

void ExplosionShader::SetColor(unsigned int rgb)
{
    parameters.colorR = static_cast<float>((rgb >> 16) & 0xff) / 255.0f;
    parameters.colorG = static_cast<float>((rgb >> 8) & 0xff) / 255.0f;
    parameters.colorB = static_cast<float>(rgb & 0xff) / 255.0f;
}

ExplosionShader::~ExplosionShader()
{
    Release();
}

void ExplosionShader::Init(void)
{
    if (constantBufferHandle >= 0) { return; }

    // 初回だけ共有シェーダーを読み込む。複数の爆発アクターでハンドルを共有する。
    auto& resources = ShaderResourceManager::GetIns();
    resources.CreateVertexShader(VERTEX_SHADER_TYPE::PurpleExplosion);
    resources.CreatePixelShader(PIXEL_SHADER_TYPE::PurpleExplosion);
    vertexShaderHandle = resources.GetVertexShader(VERTEX_SHADER_TYPE::PurpleExplosion);
    pixelShaderHandle = resources.GetPixelShader(PIXEL_SHADER_TYPE::PurpleExplosion);
    if (vertexShaderHandle < 0 || pixelShaderHandle < 0) {
        throw std::runtime_error("Could not load PurpleExplosionVS/PS.cso; build the shaders first");
    }

    // 時間・明るさ・色は爆発ごとに異なるため、定数バッファはインスタンスごとに持つ。
    constantBufferHandle = CreateShaderConstantBuffer(sizeof(Parameters));
    if (constantBufferHandle < 0) {
        throw std::runtime_error("Could not create purple explosion constant buffer");
    }

    // GPUリソースの準備後に、ActorBaseの初期化・子アクターの初期化を通す。
    ShaderBase::Init();
}

void ExplosionShader::Apply(void)
{
    auto* buffer = static_cast<Parameters*>(GetBufferShaderConstantBuffer(constantBufferHandle));
    if (buffer == nullptr) {
        throw std::runtime_error("Could not map purple explosion constant buffer");
    }
    // CPU側の値をGPUへ反映してから、ShaderBaseの共通処理でシェーダーを適用する。
    *buffer = parameters;
    UpdateShaderConstantBuffer(constantBufferHandle);
    ShaderBase::Apply();
    SetShaderConstantBuffer(constantBufferHandle, DX_SHADERTYPE_PIXEL, 4);
}

void ExplosionShader::ResetApply(void)
{
    SetShaderConstantBuffer(-1, DX_SHADERTYPE_PIXEL, 4);
    ShaderBase::ResetApply();
}

void ExplosionShader::Release(void)
{
    if (constantBufferHandle >= 0) {
        DeleteShaderConstantBuffer(constantBufferHandle);
        constantBufferHandle = -1;
    }
    // 共有する頂点・ピクセルシェーダーの実体はShaderResourceManagerが所有する。
    // このクラスでは定数バッファだけを解放し、共有シェーダーは削除しない。
    ShaderBase::Release();
}
