#include "TomatoPlayerDamageState.h"

#include <algorithm>
#include <cmath>

#include "../../../../../../../Common/Quaternion.h"
#include "../../../../../../../Common/Vector3.h"
#include "../../../../../../../Manager/TimeScale/TimeScale.h"

TomatoPlayerDamageState::TomatoPlayerDamageState(
    Quaternion& rotation,
    Vector3& velocity,
    std::function<void(void)> inviStart,
    std::function<void(void)> defaultChangeState
) :
    rotation(rotation),
    velocity(velocity),
    inviStart(inviStart),
    defaultChangeState(defaultChangeState),
    elapsedTime(0.0f),
    heading(0.0f)
{
}

void TomatoPlayerDamageState::Enter(void)
{
    inviStart();
    elapsedTime = 0.0f;
    velocity.x = velocity.z = 0.0f;

    // 前転中でも反転しない左右軸から、被弾前の水平の向きを保存する。
    const Vector3 right = rotation.Rotate(Vector3(1.0f, 0.0f, 0.0f));
    heading = std::atan2(-right.z, right.x);
    rotation = Quaternion::FromRotationY(heading);
}

void TomatoPlayerDamageState::Update(void)
{
    elapsedTime = (std::min)(DAMAGE_TIME, elapsedTime + TimeScale::Get());
    velocity.x = velocity.z = 0.0f;
    // 最初の8フレームで軽くのけぞり、残りの時間でゆっくり起き上がる。
    const float peakTime = DAMAGE_TIME * 0.2f;
    const float phase = elapsedTime < peakTime
        ? elapsedTime / peakTime
        : (DAMAGE_TIME - elapsedTime) / (DAMAGE_TIME - peakTime);
    const float smooth = phase * phase * (3.0f - 2.0f * phase);
    const float leanAngle = -(25.0f * DX_PI_F / 180.0f) * smooth;
    rotation = Quaternion::FromMatrix(MMult(MGetRotX(leanAngle), MGetRotY(heading)));

    // Quaternionの角度の折り返しや単位に依存せず、確実に通常移動へ戻る。
    if (elapsedTime >= DAMAGE_TIME) { defaultChangeState(); }
}

void TomatoPlayerDamageState::Exit(void)
{
    rotation = Quaternion::FromRotationY(heading);
    elapsedTime = 0.0f;
}
