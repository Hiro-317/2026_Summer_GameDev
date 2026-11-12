#pragma once

#include "ShaderBase.h"

// 色・明るさ・時間を受け取り、爆発の見た目を制御するシェーダー。
// 頂点・ピクセルシェーダーの実体はShaderResourceManagerから取得する。
class ExplosionShader : public ShaderBase
{
public:
    // 使用例: ExplosionShader(1.2f, 2.0f, 1.0f, 0xff8020) で橙色の爆発。
    // durationSecondsは、所有するエフェクトアクターの寿命と同じ値を指定する。
    ExplosionShader(
        float durationSeconds,         // 寿命（秒）。正の値で時間経過により消滅、0.0fなら無期限。
        float intensity,               // 発光の強さ。1.0fが基準、2.0fで明るく、0.0fで発光なし。
        float seed,                    // 炎・火花の模様を決める値。同じ値なら同じ模様になる。
        unsigned int color = 0x7509ff   // 色（0xRRGGBB）。例: 0xff8020=橙、0x00aaff=青。省略時は紫。
    );
    ~ExplosionShader() override;

    ExplosionShader(const ExplosionShader&) = delete;
    ExplosionShader& operator=(const ExplosionShader&) = delete;

    void Init(void) override;
    void Apply(void) override;
    void ResetApply(void) override;
    void Release(void) override;
    void SetElapsedSeconds(float seconds) { parameters.elapsedSeconds = seconds; }
    // 0xRRGGBBをGPUへ渡す0～1のRGBへ変換する。
    void SetColor(unsigned int rgb);

private:
    // ピクセルシェーダーのb4へ渡すデータ。
    // DxLibの標準3D描画用定数バッファと重ならないスロットを使う。
    struct Parameters
    {
        float elapsedSeconds;
        float durationSeconds;
        float intensity;
        float seed;

        // HLSLのfloat3 + floatと同じ配置。定数バッファは16バイト単位で揃える。
        float colorR;
        float colorG;
        float colorB;
        float padding;
    };
    static_assert(sizeof(Parameters) == 32);

    Parameters parameters;
    int constantBufferHandle;
};
