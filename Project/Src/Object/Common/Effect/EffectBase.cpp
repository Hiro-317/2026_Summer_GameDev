#include "EffectBase.h"

#include <cmath>
#include <stdexcept>

#include "../../../Manager/TimeScale/TimeScale.h"

EffectBase::EffectBase(float durationSeconds) :
    durationSeconds(durationSeconds), elapsedSeconds(0.0f), finished(true)
{
    if (!std::isfinite(durationSeconds) || durationSeconds < 0.0f) {
        throw std::invalid_argument("Effect duration must be finite and nonnegative (zero means unlimited)");
    }
    SetDynamicFlg(false);
    SetGravityFlg(false);
    SetPushFlg(false);
    SetDrawType(ACTOR_DRAW_TYPE::Alpha);
    SetIsDraw(false);
}

void EffectBase::BaseInit(void)
{
    // Initは下準備だけにする。初期化前にPlay/Restartした場合だけ開始要求を引き継ぐ。
    if (finished) { SetIsDraw(false); }
    else { Restart(); }
}

void EffectBase::Update(void)
{
    if (!finished) { ActorBase::Update(); }
}

void EffectBase::BaseUpdate(void)
{
    elapsedSeconds += (1.0f / 60.0f) * TimeScale::Get();
    // 無期限の場合も時間は進め、ノイズなどのアニメーションを継続する。
    if (!IsInfinite() && elapsedSeconds >= durationSeconds) {
        // 終了時は寿命の値で止め、再生を開始するまで0へ戻さない。
        elapsedSeconds = 0.0f;
        Stop();
    }
}

void EffectBase::Play(void)
{
    // Playは「一度の再生開始」。呼び出し側はイベントの発生時に一度だけ呼ぶ。
    finished = false;
    SetIsDraw(true);
}

void EffectBase::Restart(void)
{
    elapsedSeconds = 0.0f;
    Play();
}

void EffectBase::Stop(void)
{
    finished = true;
    SetIsDraw(false);
}
