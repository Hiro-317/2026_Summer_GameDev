#pragma once

#include "../ActorBase/ActorBase.h"

// 寿命・停止・再利用を管理するエフェクト用中間基底クラス。
// 通常のActorBaseと同じLoad→Init→Update/Draw→Releaseで扱う。
class EffectBase : public ActorBase
{
public:

    // エフェクトの寿命を設定する。生成直後は再生せず待機する。
    // durationSeconds: 寿命（秒）。正の値で自動終了、0.0fでStopするまで無期限。負値・NaN・無限大は指定不可。
    explicit EffectBase(float durationSeconds);
    // エフェクトオブジェクトを破棄する。
    ~EffectBase() override = default;

    EffectBase(const EffectBase&) = delete;
    EffectBase& operator=(const EffectBase&) = delete;

    // 読み込み処理。標準では何もしないため、必要な派生先で上書きする。
    void Load(void) override {}
    // 終了前のアクターを更新し、TimeScaleに従って寿命を進める。
    void Update(void) override;

    // 準備済みのエフェクトを、経過時間0から再生する。
    // AddActorとInitだけでは表示しない。表示したいタイミングで呼ぶ。再生中なら最初からやり直す。
    virtual void Play(void);

    // 経過時間を0へ戻して更新・描画を再開する。
    // 座標や色の設定は保持する。再生数は増えない。
    virtual void Restart(void);

    // 残り寿命やフェードを待たずに更新・描画を即時停止する。
    // 標準ではdeleteしない。オブジェクト自体は所有者が解放する。
    virtual void Stop(void);

    // 寿命終了または明示停止済みか調べる。
    // 戻り値: 再生前の待機中・終了・停止済みならtrue、再生中ならfalse。
    bool IsFinished(void) const { return finished; }
    // 寿命が無期限か調べる。
    // 戻り値: durationSecondsが0.0fならtrue。
    bool IsInfinite(void) const { return durationSeconds == 0.0f; }
    // 再生開始またはRestartからの経過時間を取得する。
    // 戻り値: TimeScaleを反映した経過時間（秒）。
    float GetElapsedSeconds(void) const { return elapsedSeconds; }
    // 設定された寿命を取得する。
    // 戻り値: 寿命（秒）。0.0fは無期限。
    float GetDurationSeconds(void) const { return durationSeconds; }

private:
    // ActorBaseの中間基底クラス用フックを使い、派生先のSubInit/SubUpdateを残す。
    void BaseInit(void) override;
    void BaseUpdate(void) override;

    float durationSeconds;
    float elapsedSeconds;
    bool finished;
};

