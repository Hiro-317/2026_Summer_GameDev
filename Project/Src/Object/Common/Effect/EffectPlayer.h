#pragma once

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <memory>
#include <vector>

#include "EffectBase.h"
#include "FlameBreathActor.h"

// 一度AddActorするだけで、複数種類・複数地点のエフェクトを同時再生する管理アクター。
// EffectBaseを継承するが、自分自身の寿命は無期限。生成直後には何も表示しない。
class EffectPlayer : public EffectBase
{
public:
    enum class Type { Explosion, FlameBreath };
    using PlaybackId = std::uint64_t;
    static constexpr PlaybackId INVALID_ID = 0;

    // preloadTypes: AddActor時に事前ロードする種類だけを指定する。
    // 空なら事前ロードなし。未指定の種類は最初に再生するときだけロードする。
    // 例: new EffectPlayer({EffectPlayer::Type::Explosion})
    explicit EffectPlayer(std::initializer_list<Type> preloadTypes = {});
    ~EffectPlayer() override = default;

    void Load(void) override;

    // 呼ぶたびに別の再生IDを返す。前の再生を消さず、同時に何個でも追加できる。
    PlaybackId PlayExplosion(
        const Vector3& position,        // 中心のワールド座標
        float radius = 150.0f,          // 最大半径（正のワールド単位）
        float durationSeconds = 1.2f,   // 寿命（秒）。0.0fなら停止するまで無期限
        float intensity = 2.0f,         // 発光の強さ（0以上）
        float seed = 1.0f,              // 炎・火花の模様
        unsigned int color = 0x7509ff   // 0xRRGGBB形式。省略時は紫
    );

    PlaybackId PlayFlameBreath(
        const Vector3& startPoint,      // 開始点のワールド座標
        const Vector3& direction,       // 向き。長さは内部で正規化する
        const FlameBreathSettings& settings = FlameBreathSettings{}
    );

    // 指定IDだけを即時停止し、その再生用アクター・定数バッファを回収する。
    // 無効・終了済みのIDならfalse。別の再生には影響しない。
    bool Stop(PlaybackId id);
    void StopAll(void);
    // EffectBase*経由で呼ばれた場合も全再生を停止する。次のPlayで再び使用できる。
    void Stop(void) override;

    bool IsPlaying(PlaybackId id) const;
    // シーン初期化前にPlayした場合の、開始待ちの予約も含む。
    std::size_t GetPlayingCount(void) const { return instances.size(); }

    // 戻り値falseは無効・終了済みID、または対象の種類が違う場合。
    // 再生中のブレスを口元や照準へ追従させる用途にも使える。
    bool SetPosition(PlaybackId id, const Vector3& position);
    bool SetDirection(PlaybackId id, const Vector3& direction);
    bool SetColor(PlaybackId id, unsigned int color);
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
