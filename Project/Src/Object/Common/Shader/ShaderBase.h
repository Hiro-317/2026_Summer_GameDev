#pragma once

#include "../ActorBase/ActorBase.h"

/// <summary>
/// ActorBaseを継承するシェーダー描画の中間基底クラス。
/// </summary>
/// <remarks>直接AddActorする使い方と、CreateShaderで別のアクターへ適用する使い方に対応する。</remarks>
class ShaderBase : public ActorBase
{
public:
    /// <summary>
    /// シェーダー制御アクターを生成する。標準では移動・重力・押し出しを使用しない。
    /// </summary>
    ShaderBase();
    /// <summary>
    /// シェーダー制御アクターを破棄する。
    /// </summary>
    ~ShaderBase() override = default;

    /// <summary>
    /// 読み込み処理。標準では何もしない。モデルが必要な派生先で上書きする。
    /// </summary>
    void Load(void) override {}

    /// <summary>
    /// ActorBaseの共通初期化と子アクターの初期化を行う。
    /// </summary>
    /// <remarks>派生先でInitを上書きする場合も、この関数を呼ぶ。GPUリソースの準備は派生先で行う。</remarks>
    void Init(void) override;
    /// <summary>
    /// アクターの共通更新を行う。未初期化・解放済み・終了済みなら更新しない。
    /// </summary>
    void Update(void) override;
    /// <summary>
    /// アクター所有リソースを解放し、借りている共有シェーダーハンドルを解除する。
    /// </summary>
    /// <remarks>共有GPUシェーダーの削除はShaderResourceManagerが行う。重複解放は抑止する。</remarks>
    void Release(void) override;

    /// <summary>
    /// 直接登録されたシェーダーを、適用→アクター描画→解除の順で描画する。
    /// </summary>
    /// <remarks>別のアクターに保持された場合のApplyとResetApplyは、その所有者が呼ぶ。</remarks>
    void Draw(void) override;

    /// <summary>
    /// 保持している頂点・ピクセルシェーダーを描画へ適用する。
    /// </summary>
    virtual void Apply(void);
    /// <summary>
    /// 適用したシェーダーを解除し、後続の描画へ設定を残さないようにする。
    /// </summary>
    virtual void ResetApply(void);
    /// <summary>
    /// シェーダーの再生・適用が終了したか調べる。
    /// </summary>
    /// <returns>標準ではfalse。期限のある派生シェーダーで上書きする。</returns>
    virtual bool IsEnd(void) const { return false; }

protected:
    // ハンドルの実体はShaderResourceManagerが所有する。
    int vertexShaderHandle;
    int pixelShaderHandle;

private:
    // Releaseと派生デストラクターからの重複解放を防ぐ。
    bool actorInitialized;
    bool actorReleased;
};
