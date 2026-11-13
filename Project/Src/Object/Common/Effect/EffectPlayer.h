#pragma once

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <vector>

#include "EffectBase.h"
#include "FlameBreathActor.h"

/// <summary>
/// 一度AddActorして準備し、再生関数から複数地点へ同時表示するエフェクト管理アクター。
/// </summary>
/// <remarks>自分自身の寿命は無期限。生成・初期化だけでは何も表示しない。</remarks>
class EffectPlayer : public EffectBase
{
public:
    /// <summary>
    /// 事前ロードするエフェクトの種類。
    /// </summary>
    enum class Type { Explosion, FlameBreath };
    /// <summary>
    /// 個々の再生を識別するID。終了済みIDは再利用しない。
    /// </summary>
    using PlaybackId = std::uint64_t;
    /// <summary>
    /// 無効な再生ID。正常な再生には割り当てない。
    /// </summary>
    static constexpr PlaybackId INVALID_ID = 0;

    /// <summary>
    /// 必要な種類だけを事前ロードする管理アクターを生成する。
    /// </summary>
    /// <remarks>例: new EffectPlayer({EffectPlayer::Type::Explosion})</remarks>
    /// <param name="preloadTypes">AddActor時に準備する種類の一覧。空なら事前ロードなし。未指定の種類は初回再生時に読み込む。</param>
    explicit EffectPlayer(std::initializer_list<Type> preloadTypes = {});
    /// <summary>
    /// 管理中の再生を回収して破棄する。
    /// </summary>
    ~EffectPlayer() override = default;

    /// <summary>
    /// 指定種類の共有VS/PSだけを準備する。再生用アクター・定数バッファはまだ作らない。
    /// </summary>
    void Load(void) override;

    /// <summary>
    /// 新しい爆発を追加再生する。既存の再生はリセットしない。
    /// </summary>
    /// <remarks>シーン初期化前の呼び出しは予約され、初期化完了後に再生が進む。</remarks>
    /// <param name="position">爆発の中心（ワールド座標）。</param>
    /// <param name="radius">最大半径（ワールド単位、0より大きい値）。</param>
    /// <param name="durationSeconds">寿命（秒）。正の値で自動終了、0.0fでStopするまで無期限。負値・NaN・無限大は指定不可。</param>
    /// <param name="intensity">発光の強さ（0以上）。1.0fが基準、2.0fで明るく、0.0fで発光なし。</param>
    /// <param name="seed">炎・火花の模様を決める有限の値。同じ値で同じ模様になる。</param>
    /// <param name="color">色（0xRRGGBB）。例: 0xff8020は橙、0x00aaffは青。省略時は紫。</param>
    /// <returns>今回の再生ID。個別停止や色・位置の変更に使用する。</returns>
    PlaybackId PlayExplosion(
        const Vector3& position,
        float radius = 150.0f,
        float durationSeconds = 1.2f,
        float intensity = 2.0f,
        float seed = 1.0f,
        unsigned int color = 0x7509ff
    );

    /// <summary>
    /// 開始点から指定方向へ伸びるブレスを追加再生する。
    /// </summary>
    /// <remarks>初期化前の呼び出しは開始待ちの予約として保持する。</remarks>
    /// <param name="startPoint">開始点（ワールド座標）。</param>
    /// <param name="direction">向き。内部で正規化する。ゼロベクトル・有限でない値は指定不可。</param>
    /// <param name="settings">距離・半径・寿命・発光・色・模様・流速の設定。寿命0.0fなら無期限。</param>
    /// <returns>今回の再生ID。追従更新や個別停止に使用する。</returns>
    PlaybackId PlayFlameBreath(
        const Vector3& startPoint,
        const Vector3& direction,
        const FlameBreathSettings& settings = FlameBreathSettings{}
    );

    /// <summary>
    /// 指定した再生だけを、寿命やフェードを待たず即時停止・回収する。
    /// </summary>
    /// <param name="id">PlayExplosionまたはPlayFlameBreathから返された再生ID。</param>
    /// <returns>停止できた場合はtrue。無効・終了済みのIDならfalse。</returns>
    bool Stop(PlaybackId id);
    /// <summary>
    /// 全再生と開始待ち予約を即時回収する。管理アクターは引き続きPlayできる。
    /// </summary>
    void StopAll(void);

    /// <summary>
    /// 全再生を回収し、管理アクターの更新・描画も停止する。
    /// </summary>
    /// <remarks>EffectBase*経由でも有効。次のPlay呼び出しで管理アクターが再開する。</remarks>
    void Stop(void) override;

    /// <summary>
    /// 指定IDが再生中または開始待ちか調べる。
    /// </summary>
    /// <param name="id">確認する再生ID。</param>
    /// <returns>再生中・開始待ちならtrue。無効・終了済みならfalse。</returns>
    bool IsPlaying(PlaybackId id) const;

    /// <summary>
    /// 管理中の再生数を取得する。
    /// </summary>
    /// <returns>再生中とシーン初期化前の開始待ち予約を合計した数。</returns>
    std::size_t GetPlayingCount(void) const { return instances.size(); }

    /// <summary>
    /// 指定した爆発の中心、またはブレスの開始点を変更する。
    /// </summary>
    /// <param name="id">変更する再生ID。</param>
    /// <param name="position">新しいワールド座標。各成分に有限の値を指定する。</param>
    /// <returns>変更できた場合はtrue。無効・終了済みIDならfalse。</returns>
    bool SetPosition(PlaybackId id, const Vector3& position);
    /// <summary>
    /// 指定したブレスの向きを変更する。
    /// </summary>
    /// <param name="id">ブレスの再生ID。</param>
    /// <param name="direction">新しい向き。内部で正規化する。ゼロベクトル・有限でない値は指定不可。</param>
    /// <returns>変更できた場合はtrue。無効・終了済みID、または爆発のIDならfalse。</returns>
    bool SetDirection(PlaybackId id, const Vector3& direction);
    /// <summary>
    /// 指定した再生の色を変更する。
    /// </summary>
    /// <param name="id">変更する再生ID。</param>
    /// <param name="color">0xRRGGBB形式の色。例: 0xff8020は橙、0x00aaffは青。上位8ビットは使用しない。</param>
    /// <returns>変更できた場合はtrue。無効・終了済みIDならfalse。</returns>
    bool SetColor(PlaybackId id, unsigned int color);
    /// <summary>
    /// 指定したブレスの距離・半径・明るさを変更する。
    /// </summary>
    /// <param name="id">ブレスの再生ID。</param>
    /// <param name="distance">最大到達距離（正のワールド単位）。</param>
    /// <param name="radius">最大半径（正のワールド単位）。</param>
    /// <param name="intensity">発光の強さ（0以上）。1.0fが基準、2.0fで明るく、0.0fで発光なし。</param>
    /// <returns>変更できた場合はtrue。無効・終了済みID、または爆発のIDならfalse。</returns>
    bool SetBreathStrength(PlaybackId id, float distance, float radius, float intensity);

private:
    // deleteの前にActorBaseの解放処理を必ず通す。途中で初期化に失敗した場合も同じ。
    struct EffectReleaser
    {
        void operator()(EffectBase* effect) const;
    };
    using EffectPointer = std::unique_ptr<EffectBase, EffectReleaser>;

    struct Instance
    {
        PlaybackId id;
        Type type;
        EffectPointer effect;
        bool initialized;
    };

    PlaybackId Start(Type type, EffectPointer effect);
    void EnsurePrepared(Type type);
    Instance* Find(PlaybackId id);
    void SubInit(void) override;
    void SubUpdate(void) override;
    void SubDraw(void) override;
    void SubRelease(void) override;

    unsigned int preloadMask;
    bool explosionPrepared;
    bool breathPrepared;
    bool initialized;
    bool released;
    PlaybackId nextId;
    std::vector<Instance> instances;
};
