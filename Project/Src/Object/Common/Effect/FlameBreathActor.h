#pragma once

#include "EffectBase.h"

class FlameBreathShader;

// ブレスの距離・大きさ・寿命・色などをまとめた生成設定。
struct FlameBreathSettings
{
    // 開始点からの最大到達距離（正のワールド単位）。
    float distance = 600.0f;
    // 円錐の終端円の半径（正のワールド単位）。展開完了時の外周と一致する。
    float radius = 90.0f;
    // 発生・消滅を含む寿命（秒）。0.0fならStopするまで無期限。
    float durationSeconds = 1.5f;
    // 発光の強さ（0以上）。0.0fで発光なし。
    float intensity = 2.0f;
    // 炎の模様が先端へ流れる速さ（0以上）。0.0fで模様の移動を止める。
    float flowSpeed = 3.0f;
    // 0xRRGGBB形式の色。例: 0xff7020は橙、0x00aaffは青。
    unsigned int color = 0xff7020;
    // 炎の模様を変える有限の値。同じ値で同じ模様になる。
    float seed = 1.0f;
    // Stop後、根元から先端へ炎が消え切るまでの秒数。0なら即時停止。
    float stopSeconds = 0.24f;
};

// 開始点から指定方向へまっすぐ伸びる炎ブレスのアクター。
// EffectBaseが寿命・停止・再利用を管理する。
class FlameBreathActor : public EffectBase
{
public:
    /// <summary>
    /// 指定した開始点・向き・設定でブレスを生成する。
    /// </summary>
    /// <param name="startPoint">開始点（ワールド座標）。各成分に有限の値を指定する。</param>
    /// <param name="direction">向き。内部で正規化するため長さは到達距離に影響しない。ゼロベクトルは不可。</param>
    /// <param name="settings">距離・半径・寿命・発光・流速・色・模様の設定。寿命0.0fなら無期限。</param>
    FlameBreathActor(const Vector3& startPoint, const Vector3& direction,
        const Vector3& center, float radius,
        const Vector3& normal = Vector3(0.0f, 1.0f, 0.0f),
        const FlameBreathSettings& settings = FlameBreathSettings{});
    /// ブレスアクターを破棄する。
    ~FlameBreathActor() override = default;

    // 現在の向き・大きさ・色・時間でブレスを描画する。終了済みなら描画しない。
    void Draw(void) override;

    // 停止中の炎の後端もTimeScaleに従って進める。消え切るまで更新を続ける。
    void Update(void) override;
    // 伸び・停止状態と経過時間を全て戻し、根元から新しく再生する。
    void Restart(void) override;
    // 根元から先端へ順に消す。無限再生中もstopSeconds後に完全停止する。
    // 繰り返し呼んでも停止の進みは戻らない。完全停止まではIsFinishedがfalseになる。
    void Stop(void) override;
    // 残っている炎と停止演出をその場で消す。Playで再利用できる。
    void StopImmediately(void);

    // ブレスの開始点を変更する。口元などへの毎フレームの追従にも使用できる。
    // startPoint: 新しい開始点（ワールド座標）。各成分に有限の値を指定する。
    void SetStartPoint(const Vector3& startPoint);
    // ブレスの向きを変更する。終端円の固定を解除し、円錐の底面はこの方向へ垂直にする。
    // direction: 新しい向き。内部で正規化する。ゼロベクトル・有限でない値は指定不可。
    void SetDirection(const Vector3& direction);
    /// <summary>
    /// ブレスの距離・終端円の半径・明るさを変更し、終端円の固定を解除する。
    /// </summary>
    /// <param name="distance">最大到達距離（正のワールド単位）。</param>
    /// <param name="radius">最大半径（正のワールド単位）。</param>
    /// <param name="intensity">発光の強さ（0以上）。1.0fが基準、2.0fで明るく、0.0fで発光なし。</param>
    void SetStrength(float distance, float radius, float intensity);

    /// <summary>
    /// 円錐の終端を円の中心・半径・面の向きで指定する。床の予告円へ合わせる場合に使う。
    /// SetStartPointで口元が動いても終端円はこの座標に固定される。
    /// SetDirectionまたはSetStrengthを呼ぶと、通常の方向・距離指定へ戻る。
    /// </summary>
    /// <param name="center">終端円の中心（ワールド座標）。開始点と異なり、開始点を含む面は不可。</param>
    /// <param name="radius">終端円の半径（正のワールド単位）。直径ではない。</param>
    /// <param name="normal">終端円の面に垂直な向き。標準は上向きで床と平行。正規化不要、ゼロは不可。</param>
    void SetEndCircle(const Vector3& center, float radius,
                      const Vector3& normal = Vector3(0.0f, 1.0f, 0.0f));
    // 炎の模様が流れる速さを変更する。
    // speed: 0以上の有限値。0.0fで模様の移動を止める。
    void SetFlowSpeed(float speed);
    // 次の描画から使用するブレスの色を変更する。
    // rgb: 0xRRGGBB形式の色。例: 0xff8020は橙、0x00aaffは青。
    void SetColor(unsigned int rgb) { settings.color = rgb & 0xffffff; }

    // 現在のブレスの向きを取得する。
    // 戻り値: 正規化済み方向ベクトルへの読み取り専用参照。
    const Vector3& GetDirection(void) const { return direction; }
    // 現在のブレス設定を取得する。
    // 戻り値: 設定への読み取り専用参照。変更には各Set関数を使用する。
    const FlameBreathSettings& GetSettings(void) const { return settings; }

private:
    void DefaultCreateShader(void) override;
    void SubDraw(void) override;
    void SubRelease(void) override;

    Vector3 direction;
    FlameBreathSettings settings;
    bool stopping;
    float stopElapsedSeconds;
    float stopFrontFraction;
    bool useEndCircle;
    Vector3 endCircleCenter;
    Vector3 endCircleNormal;
    // 所有権はActorBaseへ渡す。このポインターは描画前の値更新にだけ使用する。
    FlameBreathShader* breathShader;
};

