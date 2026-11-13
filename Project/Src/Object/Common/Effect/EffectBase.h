#pragma once

#include "../ActorBase/ActorBase.h"

/// <summary>
/// 寿命・停止・再利用を管理するエフェクト用中間基底クラス。
/// </summary>
/// <remarks>通常のActorBaseと同じLoad→Init→Update/Draw→Releaseで扱う。</remarks>
class EffectBase : public ActorBase
{
public:

    /// <summary>
    /// エフェクトの寿命を設定する。
    /// </summary>
    /// <param name="durationSeconds">寿命（秒）。正の値で自動終了、0.0fでStopするまで無期限。負値・NaN・無限大は指定不可。</param>
    explicit EffectBase(float durationSeconds);
    /// <summary>
    /// エフェクトオブジェクトを破棄する。
    /// </summary>
    ~EffectBase() override = default;

    EffectBase(const EffectBase&) = delete;
    EffectBase& operator=(const EffectBase&) = delete;

    /// <summary>
    /// 読み込み処理。標準では何もしないため、必要な派生先で上書きする。
    /// </summary>
    void Load(void) override {}
    /// <summary>
    /// 終了前のアクターを更新し、TimeScaleに従って寿命を進める。
    /// </summary>
    void Update(void) override;

    /// <summary>
    /// 経過時間を0へ戻して更新・描画を再開する。
    /// </summary>
    /// <remarks>座標や色の設定は保持する。再生数は増えない。</remarks>
    void Restart(void);

    /// <summary>
    /// 残り寿命やフェードを待たずに更新・描画を即時停止する。
    /// </summary>
    /// <remarks>標準ではdeleteしない。オブジェクト自体は所有者が解放する。</remarks>
    virtual void Stop(void);

    /// <summary>
    /// 寿命終了または明示停止済みか調べる。
    /// </summary>
    /// <returns>終了・停止済みならtrue、再生中ならfalse。</returns>
    bool IsFinished(void) const { return finished; }
    /// <summary>
    /// 寿命が無期限か調べる。
    /// </summary>
    /// <returns>durationSecondsが0.0fならtrue。</returns>
    bool IsInfinite(void) const { return durationSeconds == 0.0f; }
    /// <summary>
    /// 再生開始またはRestartからの経過時間を取得する。
    /// </summary>
    /// <returns>TimeScaleを反映した経過時間（秒）。</returns>
    float GetElapsedSeconds(void) const { return elapsedSeconds; }
    /// <summary>
    /// 設定された寿命を取得する。
    /// </summary>
    /// <returns>寿命（秒）。0.0fは無期限。</returns>
    float GetDurationSeconds(void) const { return durationSeconds; }

private:
    // ActorBaseの中間基底クラス用フックを使い、派生先のSubInit/SubUpdateを残す。
    void BaseInit(void) override;
    void BaseUpdate(void) override;

    float durationSeconds;
    float elapsedSeconds;
    bool finished;
};
