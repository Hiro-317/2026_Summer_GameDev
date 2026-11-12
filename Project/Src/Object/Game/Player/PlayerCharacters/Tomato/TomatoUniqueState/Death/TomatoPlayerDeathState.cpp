#include "TomatoPlayerDeathState.h"

#include <algorithm>
#include <cmath>

#include "../../../../../../../Manager/TimeScale/TimeScale.h"
#include "../../../../../../Common/Transform/Transform.h"

TomatoPlayerDeathState::TomatoPlayerDeathState(
    Transform& playerTrans,
    Vector3& velocity,
    std::function<void(bool)> setIsGravity,
    std::function<void(bool)> setJudge,
    std::function<void(void)> playerDeathSetting
) :
    playerTrans(playerTrans),
    velocity(velocity),
    setIsGravity(setIsGravity),
    setJudge(setJudge),
    playerDeathSetting(playerDeathSetting),
    elapsedTime(0.0f),
    startHeight(0.0f),
    heading(0.0f),
    finished(false)
{
}

void TomatoPlayerDeathState::Enter(void)
{
    elapsedTime = 0.0f;
    startHeight = playerTrans.pos.y;
    finished = false;
    velocity = Vector3();

    // 演出中は通常重力やステージの押し出しで動きを上書きしない。
    setIsGravity(false);
    setJudge(false);

    const Vector3 right = playerTrans.VTrans(Vector3(1.0f, 0.0f, 0.0f));
    heading = std::atan2(-right.z, right.x);
    playerTrans.SetRotation(Quaternion::FromRotationY(heading));
}

void TomatoPlayerDeathState::Update(void)
{
    if (finished) { return; }

    elapsedTime = (std::min)(DEATH_TIME, elapsedTime + TimeScale::Get());
    const float ratio = elapsedTime / DEATH_TIME;

    // 小さく跳ね上がり、元の高さへ戻る。通常重力との二重移動は行わない。
    playerTrans.pos.y = startHeight + 4.0f * HOP_HEIGHT * ratio * (1.0f - ratio);

    // 跳躍と同時に少しずつ後ろへ倒れ、最後に逆さの姿勢になる。
    const float smooth = ratio * ratio * (3.0f - 2.0f * ratio);
    const float tiltAngle = -DX_PI_F * smooth;
    playerTrans.SetRotation(Quaternion::FromMatrix(MMult(MGetRotX(tiltAngle), MGetRotY(heading))));

    if (elapsedTime >= DEATH_TIME) {
        // 演出完了の通知は一度だけ行い、全滅判定へつなげる。
        finished = true;
        playerDeathSetting();
    }
}

void TomatoPlayerDeathState::Exit(void)
{
    velocity = Vector3();
    setIsGravity(true);
    setJudge(true);
    elapsedTime = 0.0f;
    finished = true;
}