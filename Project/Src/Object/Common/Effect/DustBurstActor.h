#pragma once
#include "EffectBase.h"

class DustBurstShader;

/// <summary>中心から地面の周囲へ広がる土埃の設定。距離はワールド単位。</summary>
struct DustBurstSettings
{
    /// <summary>煙の中心が外側へ移動する最大半径。正の有限値。</summary>
    float radius = 180.0f;
    /// <summary>煙の立ち上がる高さの基準。正の有限値。</summary>
    float height = 100.0f;
    /// <summary>消滅演出を含む寿命（秒）。0なら展開後も無期限で残る。</summary>
    float durationSeconds = 1.0f;
    /// <summary>中心から外周へ消える時間（秒）。0なら即時停止。</summary>
    float stopSeconds = 0.3f;
    /// <summary>煙の不透明度（0～1）。加算発光ではなく半透明合成を使う。</summary>
    float opacity = 0.9f;
    /// <summary>煙の基本色（0xRRGGBB）。白なら0xffffff、茶色なら0xbca078など。</summary>
    unsigned int color = 0xeee6d7;
    /// <summary>煙の形・配置を変える有限の値。</summary>
    float seed = 1.0f;
};

/// <summary>丸い煙の塊が中心から周囲へ広がる土埃。EffectBaseが所有・寿命管理を担当する。</summary>
/// <remarks>最初にAddActor/AddChildActorで準備し、発生時はRestartを一度呼ぶ。</remarks>
class DustBurstActor : public EffectBase
{
public:
    /// <summary>土埃を準備する。生成だけでは再生しない。</summary>
    /// <param name="position">土埃が広がる中心（地面上のワールド座標）。値をコピーして保持する。</param>
    /// <param name="settings">半径・高さ・寿命・消滅時間・不透明度・色・模様。</param>
    DustBurstActor(const Vector3& position, const DustBurstSettings& settings = DustBurstSettings{});
    /// <summary>所有者の解放処理から破棄する。</summary>
    ~DustBurstActor() override = default;
    /// <summary>時間と中心から外周への消滅演出を進める。停止演出が完了するまで更新する。</summary>
    void Update(void) override;
    /// <summary>途中ならそのまま、中心から再生する。</summary>
    void Play(void) override;
    /// <summary>経過時間・出現・消滅の状態を全て戻し、中心から再生する。</summary>
    void Restart(void) override;
    /// <summary>寿命を待たず、中心から外周へ順に消す。繰り返し呼んでも進みは戻らない。</summary>
    void Stop(void) override;
    /// <summary>煙と消滅演出をその場で消す。Restartで再利用できる。</summary>
    void StopImmediately(void);
    /// <summary>煙の描画パラメーターを反映して描画する。待機・終了時は何もしない。</summary>
    void Draw(void) override;
    /// <summary>中心座標を変更する。</summary>
    /// <param name="position">新しい地面上のワールド座標。各成分は有限値。</param>
    void SetPosition(const Vector3& position);
    /// <summary>次の描画から煙の基本色を変更する。経過時間は戻さない。</summary>
    /// <param name="rgb">0xRRGGBB形式の色。</param>
    void SetColor(unsigned int rgb) { settings.color = rgb & 0xffffff; }
    /// <summary>現在の土埃の設定を取得する。</summary>
    /// <returns>読み取り専用の設定への参照。</returns>
    const DustBurstSettings& GetSettings(void) const { return settings; }
private:
    void DefaultCreateShader(void) override;
    void SubDraw(void) override;
    void SubRelease(void) override;
    void BeginStop(float seconds);
    float GetSpreadTime(void) const;
    DustBurstSettings settings;
    bool stopping;
    float stopElapsed;
    float stopDuration;
    float stoppedSpreadTime;
    DustBurstShader* dustShader;
};
