#include "DustBurstShader.h"
#include <stdexcept>
#include "../../../pch.h"
#include "../../../Manager/Shader/ShaderResourceManager.h"

DustBurstShader::DustBurstShader() : parameters{}, constantBufferHandle(-1) {}
DustBurstShader::~DustBurstShader() { Release(); }
void DustBurstShader::Init(void)
{
    if (constantBufferHandle >= 0) { return; }
    auto& resources = ShaderResourceManager::GetIns();
    resources.CreateVertexShader(VERTEX_SHADER_TYPE::DustBurst);
    resources.CreatePixelShader(PIXEL_SHADER_TYPE::DustBurst);
    vertexShaderHandle = resources.GetVertexShader(VERTEX_SHADER_TYPE::DustBurst);
    pixelShaderHandle = resources.GetPixelShader(PIXEL_SHADER_TYPE::DustBurst);
    if (vertexShaderHandle < 0 || pixelShaderHandle < 0) {
        throw std::runtime_error("Could not load DustBurstVS/PS.cso; rebuild the shaders");
    }
    constantBufferHandle = CreateShaderConstantBuffer(sizeof(Parameters));
    if (constantBufferHandle < 0) { throw std::runtime_error("Could not create dust burst buffer"); }
    ShaderBase::Init();
}
void DustBurstShader::SetParameters(float elapsed, float opacity, float seed, unsigned int color,
    float clearProgress, const Vector3& origin, float radius)
{
    parameters = {elapsed, opacity, seed, 0.0f,
        static_cast<float>((color >> 16) & 255) / 255.0f,
        static_cast<float>((color >> 8) & 255) / 255.0f,
        static_cast<float>(color & 255) / 255.0f, clearProgress,
        origin.x, origin.y, origin.z, radius};
}
void DustBurstShader::Apply(void)
{
    auto* buffer = static_cast<Parameters*>(GetBufferShaderConstantBuffer(constantBufferHandle));
    if (buffer == nullptr) { throw std::runtime_error("Could not map dust burst buffer"); }
    *buffer = parameters;
    UpdateShaderConstantBuffer(constantBufferHandle);
    ShaderBase::Apply();
    SetShaderConstantBuffer(constantBufferHandle, DX_SHADERTYPE_PIXEL, 4);
}
void DustBurstShader::ResetApply(void)
{
    SetShaderConstantBuffer(-1, DX_SHADERTYPE_PIXEL, 4);
    ShaderBase::ResetApply();
}
void DustBurstShader::Release(void)
{
    if (constantBufferHandle >= 0) {
        DeleteShaderConstantBuffer(constantBufferHandle);
        constantBufferHandle = -1;
    }
    ShaderBase::Release();
}
