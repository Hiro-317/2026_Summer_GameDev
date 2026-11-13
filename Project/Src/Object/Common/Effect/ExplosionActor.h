#pragma once

#include "EffectBase.h"

class ExplosionShader;

// 指定座標へ火球・衝撃波・火花を描画する爆発アクター。
// 色の既定値は紫。EffectBaseが寿命と停止・再利用を管理する。
class ExplosionActor : public EffectBase
{
public:

    /// <summary>
    /// 指定した位置・大きさ・色で爆発アクターを生成する。
    /// </summary>
    /// <param name="position">爆発の中心（ワールド座標）。値をコピーして保持し、参照先には追従しない。</param>
    /// <param name="radius">最大半径（ワールド単位、0より大きい値）。</param>
    /// <param name="durationSeconds">寿命（秒）。正の値で自動終了、0.0fでStopするまで無期限。負値・NaN・無限大は指定不可。</param>
    /// <param name="intensity">発光の強さ（0以上）。1.0fが基準、2.0fで明るく、0.0fで発光なし。</param>
    /// <param name="seed">炎・火花の模様を決める有限の値。同じ値で同じ模様になる。</param>
    /// <param name="color">色（0xRRGGBB）。例: 0xff8020は橙、0x00aaffは青。省略時は紫。</param>
    ExplosionActor(
        const Vector3& position,
        float radius = 150.0f,
        float durationSeconds = 1.2f,
        float intensity = 2.0f,
        float seed = 1.0f,
        unsigned int color = 0x7509ff
    );
    // 爆発アクターを破棄する。
    ~ExplosionActor() override = default;

    // 時間・色をシェーダーへ反映して爆発を描画する。終了済みなら描画しない。
    void Draw(void) override;

    // 次の描画から使用する色を変更する。
    // 初期化前・再生中・再利用時のいずれでも変更可能。時間はリセットしない。
    // rgb: 0xRRGGBB形式の色。例: 0xff4000は赤橙、0x00aaffは青。上位8ビットは使用しない。
    void SetColor(unsigned int rgb) { color = rgb & 0xffffff; }
    // 現在の設定色を取得する。
    // 戻り値: 0xRRGGBB形式の色。
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
