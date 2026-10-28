#include "ToonShader.h"
#include "../../../pch.h"
#include "../../../Manager/Shader/ShaderResourceManager.h"

void ToonShader::Init(void)
{
    ShaderResourceManager& manager = ShaderResourceManager::GetIns();

    // Reuse the existing vertex shader family used by DefaultShader.
    VERTEX_SHADER_TYPE vsType = static_cast<VERTEX_SHADER_TYPE>(
        static_cast<int>(VERTEX_SHADER_TYPE::Default1Frame) + static_cast<int>(vertexType));

    manager.CreateVertexShader(vsType);
    manager.CreatePixelShader(PIXEL_SHADER_TYPE::Toon);

    vertexShaderHandle = manager.GetVertexShader(vsType);
    pixelShaderHandle = manager.GetPixelShader(PIXEL_SHADER_TYPE::Toon);
}
