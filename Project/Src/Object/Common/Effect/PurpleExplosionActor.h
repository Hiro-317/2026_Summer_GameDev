#pragma once

#include "EffectBase.h"

class ExplosionShader;

/// <summary>
/// 指定座標へ火球・衝撃波・火花を描画する爆発アクター。
/// </summary>
/// <remarks>色の既定値は紫。EffectBaseが寿命と停止・再利用を管理する。</remarks>
class PurpleExplosionActor : public EffectBase
{
public:

    /// <summary>
    /// 指定した位置・大きさ・色で爆発アクターを生成する。
    /// </summary>
    /// <param name="position">爆発の中心（ワールド座標）。</param>
    /// <param name="radius">最大半径（ワールド単位、0より大きい値）。</param>
    /// <param name="durationSeconds">寿命（秒）。正の値で自動終了、0.0fでStopするまで無期限。負値・NaN・無限大は指定不可。</param>
    /// <param name="intensity">発光の強さ（0以上）。1.0fが基準、2.0fで明るく、0.0fで発光なし。</param>
    /// <param name="seed">炎・火花の模様を決める有限の値。同じ値で同じ模様になる。</param>
    /// <param name="color">色（0xRRGGBB）。例: 0xff8020は橙、0x00aaffは青。省略時は紫。</param>
    PurpleExplosionActor(
        const Vector3& position,
        float radius = 150.0f,
        float durationSeconds = 1.2f,
        float intensity = 2.0f,
        float seed = 1.0f,
        unsigned int color = 0x7509ff
    );
    /// <summary>
    /// 爆発アクターを破棄する。
    /// </summary>
    ~PurpleExplosionActor() override = default;

    /// <summary>
    /// 時間・色をシェーダーへ反映して爆発を描画する。終了済みなら描画しない。
    /// </summary>
    void Draw(void) override;

    /// <summary>
    /// 次の描画から使用する色を変更する。
    /// </summary>
    /// <remarks>初期化前・再生中・再利用時のいずれでも変更可能。時間はリセットしない。</remarks>
    /// <param name="rgb">0xRRGGBB形式の色。例: 0xff4000は赤橙、0x00aaffは青。上位8ビットは使用しない。</param>
    void SetColor(unsigned int rgb) { color = rgb & 0xffffff; }
    /// <summary>
    /// 現在の設定色を取得する。
    /// </summary>
    /// <returns>0xRRGGBB形式の色。</returns>
    unsigned int GetColor(void) const { return color; }

private:
    void DefaultCreateShader(void) override;
    void SubDraw(void) override;
    void SubRelease(void) override;

    float radius;
    float intensity;
    float seed;
    unsigned int color;
    // 参照用ポインター。所有権はCreateShaderでActorBaseへ渡す。
    // ActorBase::Releaseがシェーダーの解放とdeleteを行う。
    ExplosionShader* explosionShader;
};
