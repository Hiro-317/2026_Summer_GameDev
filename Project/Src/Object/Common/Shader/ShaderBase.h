#pragma once

#include "../ActorBase/ActorBase.h"

// ActorBaseを継承するシェーダー描画の中間基底クラス。
// 直接AddActorする使い方と、CreateShaderで別のアクターへ適用する使い方に対応する。
class ShaderBase : public ActorBase
{
public:
    // シェーダー制御アクターを生成する。標準では移動・重力・押し出しを使用しない。
    ShaderBase();
    // シェーダー制御アクターを破棄する。
    ~ShaderBase() override = default;

    // 読み込み処理。標準では何もしない。モデルが必要な派生先で上書きする。
    void Load(void) override {}

    // ActorBaseの共通初期化と子アクターの初期化を行う。
    // 派生先でInitを上書きする場合も、この関数を呼ぶ。GPUリソースの準備は派生先で行う。
    void Init(void) override;
    // アクターの共通更新を行う。未初期化・解放済み・終了済みなら更新しない。
    void Update(void) override;
    // アクター所有リソースを解放し、借りている共有シェーダーハンドルを解除する。
    // 共有GPUシェーダーの削除はShaderResourceManagerが行う。重複解放は抑止する。
    void Release(void) override;

    // 直接登録されたシェーダーを、適用→アクター描画→解除の順で描画する。
    // 別のアクターに保持された場合のApplyとResetApplyは、その所有者が呼ぶ。
    void Draw(void) override;

    // 保持している頂点・ピクセルシェーダーを描画へ適用する。
    virtual void Apply(void);
    // 適用したシェーダーを解除し、後続の描画へ設定を残さないようにする。
    virtual void ResetApply(void);
    // シェーダーの再生・適用が終了したか調べる。
    // 戻り値: 標準ではfalse。期限のある派生シェーダーで上書きする。
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
