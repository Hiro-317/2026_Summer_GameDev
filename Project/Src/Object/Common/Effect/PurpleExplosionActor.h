#pragma once

#include "EffectBase.h"

class PurpleExplosionShader;

// 指定したワールド座標で紫色の火球・衝撃波・火花を描画するアクター。
// position: 中心座標、radius: 最大半径、durationSeconds: 再生時間（秒）。
// intensity: 明るさ（0以上）、seed: 火花とノイズの模様を決める値。
// color: 0xRRGGBB形式の色。省略すると紫色になる。
class PurpleExplosionActor : public EffectBase
{
public:
    PurpleExplosionActor(const Vector3& position, float radius = 150.0f,
                         float durationSeconds = 1.2f, float intensity = 2.0f,
                         float seed = 1.0f, unsigned int color = 0x7509ff);
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
    PurpleExplosionShader* explosionShader;
};
