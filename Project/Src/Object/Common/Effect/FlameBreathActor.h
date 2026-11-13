#pragma once

#include "EffectBase.h"

class FlameBreathShader;

/// <summary>
/// ブレスの距離・大きさ・寿命・色などをまとめた生成設定。
/// </summary>
struct FlameBreathSettings
{
    /// <summary>
    /// 開始点からの最大到達距離（正のワールド単位）。
    /// </summary>
    float distance = 600.0f;
    /// <summary>
    /// 先端付近の最大半径（正のワールド単位）。最大の太さはおよそこの2倍。
    /// </summary>
    float radius = 90.0f;
    /// <summary>
    /// 発生・消滅を含む寿命（秒）。0.0fならStopするまで無期限。
    /// </summary>
    float durationSeconds = 1.5f;
    /// <summary>
    /// 発光の強さ（0以上）。0.0fで発光なし。
    /// </summary>
    float intensity = 2.0f;
    /// <summary>
    /// 炎の模様が先端へ流れる速さ（0以上）。0.0fで模様の移動を止める。
    /// </summary>
    float flowSpeed = 3.0f;
    /// <summary>
    /// 0xRRGGBB形式の色。例: 0xff7020は橙、0x00aaffは青。
    /// </summary>
    unsigned int color = 0xff7020;
    /// <summary>
    /// 炎の模様を変える有限の値。同じ値で同じ模様になる。
    /// </summary>
    float seed = 1.0f;
};

/// <summary>
/// 開始点から指定方向へまっすぐ伸びる炎ブレスのアクター。
/// </summary>
/// <remarks>EffectBaseが寿命・停止・再利用を管理する。</remarks>
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
                     const FlameBreathSettings& settings = FlameBreathSettings{});
    /// <summary>
    /// ブレスアクターを破棄する。
    /// </summary>
    ~FlameBreathActor() override = default;

    /// <summary>
    /// 現在の向き・大きさ・色・時間でブレスを描画する。終了済みなら描画しない。
    /// </summary>
    void Draw(void) override;

    /// <summary>
    /// ブレスの開始点を変更する。口元などへの毎フレームの追従にも使用できる。
    /// </summary>
    /// <param name="startPoint">新しい開始点（ワールド座標）。各成分に有限の値を指定する。</param>
    void SetStartPoint(const Vector3& startPoint);
    /// <summary>
    /// ブレスの向きを変更する。
    /// </summary>
    /// <param name="direction">新しい向き。内部で正規化する。ゼロベクトル・有限でない値は指定不可。</param>
    void SetDirection(const Vector3& direction);
    /// <summary>
    /// ブレスの距離・最大半径・明るさをまとめて変更する。
    /// </summary>
    /// <param name="distance">最大到達距離（正のワールド単位）。</param>
    /// <param name="radius">最大半径（正のワールド単位）。</param>
    /// <param name="intensity">発光の強さ（0以上）。1.0fが基準、2.0fで明るく、0.0fで発光なし。</param>
    void SetStrength(float distance, float radius, float intensity);
    /// <summary>
    /// 炎の模様が流れる速さを変更する。
    /// </summary>
    /// <param name="speed">0以上の有限値。0.0fで模様の移動を止める。</param>
    void SetFlowSpeed(float speed);
    /// <summary>
    /// 次の描画から使用するブレスの色を変更する。
    /// </summary>
    /// <param name="rgb">0xRRGGBB形式の色。例: 0xff8020は橙、0x00aaffは青。</param>
    void SetColor(unsigned int rgb) { settings.color = rgb & 0xffffff; }

    /// <summary>
    /// 現在のブレスの向きを取得する。
    /// </summary>
    /// <returns>正規化済み方向ベクトルへの読み取り専用参照。</returns>
    const Vector3& GetDirection(void) const { return direction; }
    /// <summary>
    /// 現在のブレス設定を取得する。
    /// </summary>
    /// <returns>設定への読み取り専用参照。変更には各Set関数を使用する。</returns>
    const FlameBreathSettings& GetSettings(void) const { return settings; }

private:
    void DefaultCreateShader(void) override;
    void SubDraw(void) override;
    void SubRelease(void) override;

    Vector3 direction;
    FlameBreathSettings settings;
    // 所有権はActorBaseへ渡す。このポインターは描画前の値更新にだけ使用する。
    FlameBreathShader* breathShader;
};
