#pragma once
#include "ShaderBase.h"

// 共有の土埃VS/PSと、個別の48バイト定数バッファを扱う。
class DustBurstShader : public ShaderBase
{
public:
    // リソースをまだロードせず、管理状態だけを準備する。
    DustBurstShader();
    // 個別定数バッファを解放する。
    ~DustBurstShader() override;
    DustBurstShader(const DustBurstShader&) = delete;
    DustBurstShader& operator=(const DustBurstShader&) = delete;
    // ShaderResourceManagerから共有VS/PSを取得し、個別バッファを準備する。
    void Init(void) override;
    // 現在の土埃のパラメーターをPSのb4へ反映する。
    void Apply(void) override;
    // 個別バッファとシェーダーの適用を解除する。
    void ResetApply(void) override;
    // 個別バッファを解放する。共有VS/PSは管理クラスが所有する。
    void Release(void) override;
    /// <summary>次の描画に使う時間・色・消滅位置を設定する。</summary>
    /// <param name="elapsed">経過時間（秒）。</param>
    /// <param name="opacity">不透明度（0～1）。</param>
    /// <param name="seed">模様を決める値。</param>
    /// <param name="color">煙の色（0xRRGGBB）。</param>
    /// <param name="clearProgress">消滅位置。-1なら通常表示、0～1で中心から外側へ消す。</param>
    /// <param name="origin">土埃の中心座標。</param>
    /// <param name="radius">高さによる板の張り出しを含めた、消滅距離の基準。</param>
    void SetParameters(float elapsed, float opacity, float seed, unsigned int color,
                       float clearProgress, const Vector3& origin, float radius);
private:
    struct Parameters {
        float elapsed, opacity, seed, padding;
        float colorR, colorG, colorB, clearProgress;
        float originX, originY, originZ, radius;
    };
    static_assert(sizeof(Parameters) == 48);
    Parameters parameters;
    int constantBufferHandle;
};
