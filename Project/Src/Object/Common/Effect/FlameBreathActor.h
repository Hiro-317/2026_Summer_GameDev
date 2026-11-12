#pragma once

#include "EffectBase.h"

class FlameBreathShader;

// ブレスの強さ・見た目をまとめた生成設定。
// 距離と半径はワールド単位、時間は秒、色は0xRRGGBB形式で指定する。
struct FlameBreathSettings
{
    float distance = 600.0f;       // 開始点からの最大到達距離
    float radius = 90.0f;          // 先端付近の最大半径。太さはおよそこの2倍。
    float durationSeconds = 1.5f;  // 発生・消滅を含む寿命。0.0fならStopまで無期限。
    float intensity = 2.0f;        // 発光の強さ（0以上）
    float flowSpeed = 3.0f;        // 炎の模様が先端へ流れる速さ（0以上）
    unsigned int color = 0xff7020; // 指定を省略した場合は橙色の炎
    float seed = 1.0f;             // ノイズの模様を変える値
};

// 開始点から指定方向へまっすぐ伸びる、炎のブレスアクター。
// 向きは内部で正規化するため、方向ベクトルの長さは距離に影響しない。
// EffectBaseが寿命・半透明描画・停止・再利用を管理する。
class FlameBreathActor : public EffectBase
{
public:
    FlameBreathActor(const Vector3& startPoint, const Vector3& direction,
                     const FlameBreathSettings& settings = FlameBreathSettings{});
    ~FlameBreathActor() override = default;

    void Draw(void) override;

    // 口元に追従させる場合などは、所有者の更新処理から毎フレーム指定できる。
    void SetStartPoint(const Vector3& startPoint);
    void SetDirection(const Vector3& direction);
    void SetStrength(float distance, float radius, float intensity);
    void SetFlowSpeed(float speed);
    void SetColor(unsigned int rgb) { settings.color = rgb & 0xffffff; }

    const Vector3& GetDirection(void) const { return direction; }
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
