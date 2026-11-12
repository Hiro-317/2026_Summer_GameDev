#include "ShaderBase.h"

#include "../../../pch.h"

ShaderBase::ShaderBase() :
    ActorBase(),
    vertexShaderHandle(-1),
    pixelShaderHandle(-1),
    actorInitialized(false),
    actorReleased(false)
{
    // 描画を制御するアクターなので、標準では移動・重力・押し出しを使用しない。
    // 描画タイプや位置はActorBaseの機能を通して派生先で設定できる。
    SetDynamicFlg(false);
    SetGravityFlg(false);
    SetPushFlg(false);
}

void ShaderBase::Init(void)
{
    if (actorInitialized) { return; }
    ActorBase::Init();
    actorInitialized = true;
    actorReleased = false;
}

void ShaderBase::Update(void)
{
    if (!actorInitialized || actorReleased || IsEnd()) { return; }
    ActorBase::Update();
}

void ShaderBase::Draw(void)
{
    if (!actorInitialized || actorReleased || !GetIsDraw() || IsEnd()) { return; }

    // 自分自身をCreateShader(this)で所有させず、その場で直接適用する。
    // ActorBase::DrawからSubDraw・モデル描画などの通常の描画処理を通す。
    Apply();
    try {
        ActorBase::Draw();
    }
    catch (...) {
        // 描画中に例外が発生しても、後続オブジェクトへシェーダー設定を残さない。
        ResetApply();
        throw;
    }
    ResetApply();
}

void ShaderBase::Apply(void)
{
    MV1SetUseOrigShader(true);
    if (vertexShaderHandle != -1) { SetUseVertexShader(vertexShaderHandle); }
    if (pixelShaderHandle != -1) { SetUsePixelShader(pixelShaderHandle); }
}

void ShaderBase::ResetApply(void)
{
    SetUseVertexShader(-1);
    SetUsePixelShader(-1);
    MV1SetUseOrigShader(false);
}

void ShaderBase::Release(void)
{
    if (actorReleased) { return; }
    // 自分が持つ子アクター・コライダー・モデルなどはActorBaseに解放を任せる。
    // 共有シェーダーは削除せず、借りていたハンドルだけを解除する。
    ActorBase::Release();
    vertexShaderHandle = -1;
    pixelShaderHandle = -1;
    actorInitialized = false;
    actorReleased = true;
}
