#pragma once

#include "ShaderBase.h"

// 時間・色・明るさを受け取り、爆発の見た目を制御するシェーダー。
// GPUシェーダー本体はShaderResourceManagerから取得する。
class ExplosionShader : public ShaderBase
{
public:

    /// <summary>
    /// 爆発の描画パラメーターと個別定数バッファの管理状態を設定する。
    /// </summary>
    /// <remarks>例: ExplosionShader(1.2f, 2.0f, 1.0f, 0xff8020)で橙色の爆発。</remarks>
    /// <param name="durationSeconds">見た目に使う寿命（秒）。0.0fなら無期限。所有するエフェクトアクターの寿命と揃える。</param>
    /// <param name="intensity">発光の強さ（0以上）。1.0fが基準、2.0fで明るく、0.0fで発光なし。</param>
    /// <param name="seed">炎・火花の模様を決める有限の値。同じ値で同じ模様になる。</param>
    /// <param name="color">色（0xRRGGBB）。例: 0xff8020は橙、0x00aaffは青。省略時は紫。</param>
    ExplosionShader(
        float durationSeconds,
        float intensity,
        float seed,
        unsigned int color = 0x7509ff
    );
    // 自身の定数バッファを解放して破棄する。
    ~ExplosionShader() override;

    ExplosionShader(const ExplosionShader&) = delete;
    ExplosionShader& operator=(const ExplosionShader&) = delete;

    // 共有VS/PSを取得し、爆発ごとの定数バッファとアクター共通処理を初期化する。
    void Init(void) override;
    // 時間・色・明るさをGPUへ反映し、爆発のVS/PSとPSのb4を適用する。
    void Apply(void) override;
    // 爆発用の定数バッファとシェーダーを解除する。
    void ResetApply(void) override;
    // 個別定数バッファとアクター所有リソースを解放する。共有VS/PSは削除しない。
    void Release(void) override;
    // 次の描画に使う経過時間を設定する。
    // seconds: 開始または再起動からの経過時間（秒）。通常はEffectBaseの値を渡す。
    void SetElapsedSeconds(float seconds) { parameters.elapsedSeconds = seconds; }

    // 0xRRGGBBをGPU用の0～1のRGBへ変換して保持する。
    // rgb: 0xRRGGBB形式の色。例: 0xff8020は橙、0x00aaffは青。上位8ビットは使用しない。
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
