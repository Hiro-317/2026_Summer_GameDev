#pragma once

#include "EffectBase.h"

class ExplosionShader;

// 指定したワールド座標で紫色の火球・衝撃波・火花を描画するアクター。
// position: 中心座標、radius: 最大半径、durationSeconds: 再生時間（秒、0なら無期限）。
// intensity: 明るさ（0以上）、seed: 火花とノイズの模様を決める値。
// color: 0xRRGGBB形式の色。省略すると紫色になる。
class PurpleExplosionActor : public EffectBase
{
public:
    // シーンなどから、表示したいタイミングで生成する。
    PurpleExplosionActor(
        const Vector3& position,        // 爆発の中心（ワールド座標）。
        float radius = 150.0f,          // 最大半径（ワールド単位）。0より大きい値を指定する。
        float durationSeconds = 1.2f,   // 寿命（秒）。0.0fならStop()を呼ぶまで無期限。
        float intensity = 2.0f,         // 発光の強さ（0以上）。大きくすると明るくなる。
        float seed = 1.0f,              // 炎・火花の模様。別々の値を指定すると模様が変わる。
        unsigned int color = 0x7509ff   // 色（0xRRGGBB）。例: 0xff8020=橙。省略時は紫。
    );
    ~PurpleExplosionActor() override = default;

    void Draw(void) override;

    // 初期化前・再生中・再利用時のどのタイミングでも変更できる。
    // 例: 0xff4000（赤橙）、0x00aaff（青）、0xffffff（白）。
    // 変更は次の描画から反映する。上位8ビットは使用しない。
    void SetColor(unsigned int rgb) { color = rgb & 0xffffff; }
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
