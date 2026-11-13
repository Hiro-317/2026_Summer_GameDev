#pragma once
#include "EffectBase.h"

class DustBurstShader;

// 地面を広がる輪と、細かな砂煙の設定。距離はワールド単位。
struct DustBurstSettings
{
    // 煙の中心が外側へ移動する最大半径。正の有限値。
    float radius = 180.0f;
    // 煙の立ち上がる高さの基準。正の有限値。輪とは別に時間とともに上昇する。
    float height = 100.0f;
    // 消滅演出を含む寿命（秒）。0なら展開後も無期限で残る。
    float durationSeconds = 1.0f;
    // 中心から外周へ消える時間（秒）。0なら即時停止。
    float stopSeconds = 0.3f;
    // 煙の不透明度（0～1）。加算発光ではなく半透明合成を使う。
    float opacity = 0.9f;
    // 輪と砂煙の基本色（0xRRGGBB）。標準は橙。砂色なら0xbca078など。
    unsigned int color = 0xffa04a;
    // 煙の形・配置を変える有限の値。
    float seed = 1.0f;
};

// 地面の輪と崩れた砂煙が中心から周囲へ広がる土埃。EffectBaseが所有・寿命管理を担当する。
// 最初にAddActor/AddChildActorで準備し、発生時はRestartを一度呼ぶ。
class DustBurstActor : public EffectBase
{
public:
    /// <summary>土埃を準備する。生成だけでは再生しない。</summary>
    /// <param name="position">土埃が広がる中心（地面上のワールド座標）。値をコピーして保持する。</param>
    /// <param name="settings">半径・高さ・寿命・消滅時間・不透明度・色・模様。</param>
    DustBurstActor(const Vector3& position, const DustBurstSettings& settings = DustBurstSettings{});
    // 所有者の解放処理から破棄する。
    ~DustBurstActor() override = default;
    // 時間と中心から外周への消滅演出を進める。停止演出が完了するまで更新する。
    void Update(void) override;
    // 終了後・消滅途中なら中心から新しく再生し、通常の再生中なら時間を戻さない。
    void Play(void) override;
    // 経過時間・出現・消滅の状態を全て戻し、中心から再生する。
    void Restart(void) override;
    // 寿命を待たず、中心から外周へ順に消す。繰り返し呼んでも進みは戻らない。
    void Stop(void) override;
    // 煙と消滅演出をその場で消す。Restartで再利用できる。
    void StopImmediately(void);
    // 煙の描画パラメーターを反映して描画する。待機・終了時は何もしない。
    void Draw(void) override;
    // 中心座標を変更する。
    // position: 新しい地面上のワールド座標。各成分は有限値。
    void SetPosition(const Vector3& position);
    // 次の描画から煙の基本色を変更する。経過時間は戻さない。
    // rgb: 0xRRGGBB形式の色。
    void SetColor(unsigned int rgb) { settings.color = rgb & 0xffffff; }
    // 現在の土埃の設定を取得する。
    // 戻り値: 読み取り専用の設定への参照。
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
