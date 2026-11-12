#pragma once

#include "../ActorBase/ActorBase.h"

// エフェクト用アクターの中間基底クラス。
// 再生が終了しても所有者（シーン・親アクター）が保持し、Restartで再利用できる。
// 通常のActorBaseと同じLoad→Init→Update/Draw→Releaseの流れで扱う。
class EffectBase : public ActorBase
{
public:
    explicit EffectBase(float durationSeconds);
    ~EffectBase() override = default;

    EffectBase(const EffectBase&) = delete;
    EffectBase& operator=(const EffectBase&) = delete;

    // モデル・コライダーを使用しないエフェクト向けの空の読み込み処理。
    // 必要な派生エフェクトではLoadをオーバーライドしてリソースを読み込む。
    void Load(void) override {}
    void Update(void) override;

    // 経過時間を0に戻して再生する。座標はActorBase::SetPosで変更できる。
    void Restart(void);
    // 更新と描画を停止する。オブジェクト自体は所有者が解放する。
    void Stop(void);
    // 所有者が回収・再利用を判断するときに使用する。
    bool IsFinished(void) const { return finished; }
    float GetElapsedSeconds(void) const { return elapsedSeconds; }
    float GetDurationSeconds(void) const { return durationSeconds; }

private:
    // ActorBaseの中間基底クラス用フックを使い、派生先のSubInit/SubUpdateを残す。
    void BaseInit(void) override;
    void BaseUpdate(void) override;

    float durationSeconds;
    float elapsedSeconds;
    bool finished;
};
