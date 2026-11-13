#pragma once

#include "ShaderBase.h"

/// <summary>
/// ShaderResourceManagerの共有VS/PSを使用する、ブレス用のシェーダー制御。
/// </summary>
class FlameBreathShader : public ShaderBase
{
public:
    /// <summary>
    /// ブレスのシェーダー制御を生成する。GPUリソースはInitで準備する。
    /// </summary>
    FlameBreathShader();
    /// <summary>
    /// 自身の定数バッファを解放して破棄する。
    /// </summary>
    ~FlameBreathShader() override;
    FlameBreathShader(const FlameBreathShader&) = delete;
    FlameBreathShader& operator=(const FlameBreathShader&) = delete;

    /// <summary>
    /// 共有VS/PSを取得し、ブレスごとの定数バッファとアクター共通処理を初期化する。
    /// </summary>
    void Init(void) override;
    /// <summary>
    /// 現在のパラメーターをGPUへ反映し、ブレスのVS/PSとPSのb4を適用する。
    /// </summary>
    void Apply(void) override;
    /// <summary>
    /// ブレス用の定数バッファとシェーダーを解除する。
    /// </summary>
    void ResetApply(void) override;
    /// <summary>
    /// 個別定数バッファとアクター所有リソースを解放する。共有VS/PSは削除しない。
    /// </summary>
    void Release(void) override;

    /// <summary>
    /// 次の描画に使うブレス固有の値をまとめて設定する。
    /// </summary>
    /// <remarks>時間・設定はアクターから受け取り、このインスタンスの定数バッファにだけ反映する。</remarks>
    /// <param name="elapsedSeconds">再生開始からの経過時間（秒）。</param>
    /// <param name="durationSeconds">寿命（秒）。0.0fなら停止するまで無期限。</param>
    /// <param name="intensity">発光の強さ（0以上）。1.0fが基準、2.0fで明るく、0.0fで発光なし。</param>
    /// <param name="flowSpeed">炎の模様が先端へ流れる速さ（0以上）。0.0fで模様の移動を止める。</param>
    /// <param name="color">0xRRGGBB形式の色。例: 0xff7020は橙、0x00aaffは青。</param>
    /// <param name="seed">炎の模様を決める有限の値。同じ値なら同じ模様になる。</param>
    void SetParameters(float elapsedSeconds, float durationSeconds, float intensity,
                       float flowSpeed, unsigned int color, float seed);

private:
    // PSのb4。HLSLと同じ順番で、16バイトを2組にして配置する。
    struct Parameters
    {
        float elapsedSeconds;
        float durationSeconds;
        float intensity;
        float flowSpeed;
        float colorR;
        float colorG;
        float colorB;
        float seed;
    };
    static_assert(sizeof(Parameters) == 32);

    Parameters parameters;
    int constantBufferHandle;
};
