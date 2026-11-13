#include "EffectPlayer.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

#include "PurpleExplosionActor.h"
#include "../../../Manager/Shader/ShaderResourceManager.h"

EffectPlayer::EffectPlayer(std::initializer_list<Type> preloadTypes) :
    EffectBase(0.0f), preloadMask(0), explosionPrepared(false), breathPrepared(false),
    initialized(false), released(false), nextId(1), instances()
{
    for (Type type : preloadTypes) {
        switch (type) {
        case Type::Explosion: preloadMask |= 1u; break;
        case Type::FlameBreath: preloadMask |= 2u; break;
        default: throw std::invalid_argument("Unknown effect type");
        }
    }
}

void EffectPlayer::EffectReleaser::operator()(EffectBase* effect) const
{
    if (effect != nullptr) {
        effect->Release();
        delete effect;
    }
}

void EffectPlayer::EnsurePrepared(Type type)
{
    // シェーダー本体は既存マネージャーで共有する。未使用の種類は読み込まない。
    auto& resources = ShaderResourceManager::GetIns();
    if (type == Type::Explosion) {
        if (explosionPrepared) { return; }
        resources.CreateVertexShader(VERTEX_SHADER_TYPE::PurpleExplosion);
        resources.CreatePixelShader(PIXEL_SHADER_TYPE::PurpleExplosion);
        if (resources.GetVertexShader(VERTEX_SHADER_TYPE::PurpleExplosion) < 0 ||
            resources.GetPixelShader(PIXEL_SHADER_TYPE::PurpleExplosion) < 0) {
            throw std::runtime_error("Could not prepare explosion shaders");
        }
        explosionPrepared = true;
    }
    else {
        if (breathPrepared) { return; }
        resources.CreateVertexShader(VERTEX_SHADER_TYPE::FlameBreath);
        resources.CreatePixelShader(PIXEL_SHADER_TYPE::FlameBreath);
        if (resources.GetVertexShader(VERTEX_SHADER_TYPE::FlameBreath) < 0 ||
            resources.GetPixelShader(PIXEL_SHADER_TYPE::FlameBreath) < 0) {
            throw std::runtime_error("Could not prepare flame breath shaders");
        }
        breathPrepared = true;
    }
}

void EffectPlayer::Load(void)
{
    if (released) { throw std::logic_error("EffectPlayer has been released"); }
    // ここでは指定種類の共有VS/PSだけを準備する。再生アクターや定数バッファは作らない。
    if ((preloadMask & 1u) != 0) { EnsurePrepared(Type::Explosion); }
    if ((preloadMask & 2u) != 0) { EnsurePrepared(Type::FlameBreath); }
}

EffectPlayer::PlaybackId EffectPlayer::Start(Type type, EffectPointer effect)
{
    if (released) { throw std::logic_error("EffectPlayer has been released"); }
    // IDは再利用しない。古いIDで、後から開始した別の再生を止めないようにする。
    if (nextId == INVALID_ID) { throw std::overflow_error("Effect playback IDs exhausted"); }
    effect->Load();
    bool ready = false;
    if (initialized) {
        EnsurePrepared(type);
        effect->Init();
        ready = true;
    }
    const PlaybackId id = nextId;
    instances.push_back(Instance{id, type, std::move(effect), ready});
    ++nextId;
    // Stop()で管理アクター自体を止めていた場合も、次のPlayで使用できる。
    if (IsFinished()) { Restart(); }
    return id;
}

EffectPlayer::PlaybackId EffectPlayer::PlayExplosion(const Vector3& position, float radius,
    float durationSeconds, float intensity, float seed, unsigned int color)
{
    // コンストラクターで設定を検証してから、必要な種類だけを準備する。
    return Start(Type::Explosion, EffectPointer(
        new PurpleExplosionActor(position, radius, durationSeconds, intensity, seed, color)));
}

EffectPlayer::PlaybackId EffectPlayer::PlayFlameBreath(const Vector3& startPoint,
    const Vector3& direction, const FlameBreathSettings& settings)
{
    return Start(Type::FlameBreath, EffectPointer(new FlameBreathActor(startPoint, direction, settings)));
}

void EffectPlayer::SubInit(void)
{
    if (released) { throw std::logic_error("EffectPlayer has been released"); }
    // シーン初期化より前に呼ばれたPlayは予約として保持し、ここで一度だけ初期化する。
    for (auto& instance : instances) {
        if (!instance.initialized) {
            EnsurePrepared(instance.type);
            instance.effect->Init();
            instance.initialized = true;
        }
    }
    initialized = true;
}

void EffectPlayer::SubUpdate(void)
{
    for (auto it = instances.begin(); it != instances.end();) {
        if (it->initialized && !it->effect->IsFinished()) { it->effect->Update(); }
        if (it->effect->IsFinished()) {
            // 終了した再生は残さない。個別アクターとGPU定数バッファをその場で解放する。
            it = instances.erase(it);
        }
        else { ++it; }
    }
}

void EffectPlayer::SubDraw(void)
{
    // 内部の再生をさらにSceneBase::AddActorへ登録しないので、二重に描画されない。
    for (auto& instance : instances) {
        if (instance.initialized && !instance.effect->IsFinished()) { instance.effect->Draw(); }
    }
}

EffectPlayer::Instance* EffectPlayer::Find(PlaybackId id)
{
    auto found = std::find_if(instances.begin(), instances.end(),
        [id](const Instance& instance) { return instance.id == id; });
    return found == instances.end() ? nullptr : &*found;
}

bool EffectPlayer::Stop(PlaybackId id)
{
    auto found = std::find_if(instances.begin(), instances.end(),
        [id](const Instance& instance) { return instance.id == id; });
    if (found == instances.end()) { return false; }
    found->effect->Stop();
    instances.erase(found);
    return true;
}

void EffectPlayer::StopAll(void)
{
    // 個々の寿命・無期限設定にかかわらず即時回収する。共有シェーダーは所有者に任せる。
    instances.clear();
}

void EffectPlayer::Stop(void)
{
    StopAll();
    EffectBase::Stop();
}

bool EffectPlayer::IsPlaying(PlaybackId id) const
{
    return std::any_of(instances.begin(), instances.end(),
        [id](const Instance& instance) { return instance.id == id && !instance.effect->IsFinished(); });
}

bool EffectPlayer::SetPosition(PlaybackId id, const Vector3& position)
{
    auto* instance = Find(id);
    if (instance == nullptr) { return false; }
    if (!std::isfinite(position.x) || !std::isfinite(position.y) || !std::isfinite(position.z)) {
        throw std::invalid_argument("Effect position must be finite");
    }
    instance->effect->SetPos(position);
    return true;
}

bool EffectPlayer::SetDirection(PlaybackId id, const Vector3& direction)
{
    auto* instance = Find(id);
    if (instance == nullptr || instance->type != Type::FlameBreath) { return false; }
    static_cast<FlameBreathActor*>(instance->effect.get())->SetDirection(direction);
    return true;
}

bool EffectPlayer::SetColor(PlaybackId id, unsigned int color)
{
    auto* instance = Find(id);
    if (instance == nullptr) { return false; }
    if (instance->type == Type::Explosion) {
        static_cast<PurpleExplosionActor*>(instance->effect.get())->SetColor(color);
    }
    else { static_cast<FlameBreathActor*>(instance->effect.get())->SetColor(color); }
    return true;
}

bool EffectPlayer::SetBreathStrength(PlaybackId id, float distance, float radius, float intensity)
{
    auto* instance = Find(id);
    if (instance == nullptr || instance->type != Type::FlameBreath) { return false; }
    static_cast<FlameBreathActor*>(instance->effect.get())->SetStrength(distance, radius, intensity);
    return true;
}

void EffectPlayer::SubRelease(void)
{
    StopAll();
    initialized = false;
    released = true;
}
