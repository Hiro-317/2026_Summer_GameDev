#pragma once

#include "ShaderBase.h"
#include "../../../Manager/Shader/ShaderTypeDefine.h"

class ToonShader : public ShaderBase
{
public:
    explicit ToonShader(SHADER_VERTEX_TYPE vertexType = SHADER_VERTEX_TYPE::Frame8)
        : ShaderBase(), vertexType(vertexType) {}
    ~ToonShader() override = default;

    void Init(void) override;

private:
    SHADER_VERTEX_TYPE vertexType;
};
