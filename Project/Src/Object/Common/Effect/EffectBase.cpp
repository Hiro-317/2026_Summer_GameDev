#include "EffectBase.h"

#include <cmath>
#include <stdexcept>

#include "../../../Manager/TimeScale/TimeScale.h"

EffectBase::EffectBase(float durationSeconds) :
    durationSeconds(durationSeconds), elapsedSeconds(0.0f), finished(false)
{
    if (!std::isfinite(durationSeconds) || durationSeconds <= 0.0f) {
        throw std::invalid_argument("Effect duration must be finite and positive");
    }
    SetDynamicFlg(false);
    SetGravityFlg(false);
    SetPushFlg(false);
    SetDrawType(ACTOR_DRAW_TYPE::Alpha);
}

void EffectBase::BaseInit(void)
{
    Restart();
}

void EffectBase::Update(void)
{
    if (!finished) { ActorBase::Update(); }
}

void EffectBase::BaseUpdate(void)
{
    elapsedSeconds += (1.0f / 60.0f) * TimeScale::Get();
    if (elapsedSeconds >= durationSeconds) {
        elapsedSeconds = durationSeconds;
        Stop();
    }
}

void EffectBase::Restart(void)
{
    elapsedSeconds = 0.0f;
    finished = false;
    SetIsDraw(true);
}

void EffectBase::Stop(void)
{
    finished = true;
    SetIsDraw(false);
}
