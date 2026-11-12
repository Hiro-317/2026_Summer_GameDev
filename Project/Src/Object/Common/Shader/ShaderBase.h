#pragma once

#include "../ActorBase/ActorBase.h"

// アクターとして直接登録できる、シェーダー描画の中間基底クラス。
// 派生先はLoad/SubInit/SubUpdate/SubDraw/SubReleaseなどで固有処理を追加する。
// 既存のCreateShaderによるモデルへの適用にも使用できる。
class ShaderBase : public ActorBase
{
public:
    ShaderBase();
    ~ShaderBase() override = default;

    // GPUシェーダーの読み込みは派生先のInitで行う。モデルが必要ならLoadを上書きする。
    void Load(void) override {}

    // アクター共通処理を呼び出す。Init/Update/Releaseを上書きする派生先も、
    // ShaderBase側の同名関数を呼び、子アクター・モデルなどの処理を通すこと。
    void Init(void) override;
    void Update(void) override;
    void Release(void) override;

    // 直接登録された場合は、適用→アクター描画→解除の順で描画する。
    // 他のActorにCreateShaderで保持された場合は、所有者がApply/ResetApplyを呼ぶ。
    void Draw(void) override;

    virtual void Apply(void);
    virtual void ResetApply(void);
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
