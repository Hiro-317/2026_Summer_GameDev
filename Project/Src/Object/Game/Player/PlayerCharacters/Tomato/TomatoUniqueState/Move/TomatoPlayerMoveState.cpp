#include "TomatoPlayerMoveState.h"

#include <cmath>

#include "../../../../../../../Manager/TimeScale/TimeScale.h"
#include "../../../../../../../Manager/Input/InputManager.h"

#include "../../../../../../../Manager/Camera/CurrentCamera.h"

#include "../../../../../../Common/CharacterBase/CharacterStatsDefine.h"
#include "../../../../../../Common/Transform/Transform.h"

TomatoPlayerMoveState::TomatoPlayerMoveState(
    const CharacterStats& stats,
    float DASH_SPEED_RATE,
    short DASH_STAMINA_MAX,
    std::function<void(const Vector3&)> moveAccel,
    float& ACCEL_MAX,
    Transform& playerTrans,
    float ROLL_RADIUS
) :
    stats(stats),
    DASH_SPEED_RATE(DASH_SPEED_RATE),
    DASH_STAMINA_MAX(DASH_STAMINA_MAX),
    moveAccel(moveAccel),
    ACCEL_MAX(ACCEL_MAX),
    playerTrans(playerTrans),
    ROLL_RADIUS(ROLL_RADIUS),
    isDash(false),
    dashStamina(static_cast<float>(DASH_STAMINA_MAX)),
    isTired(false),
    spinAngle(0.0f)
{
}

void TomatoPlayerMoveState::OwnStateConditionUpdate(void)
{
    if (InputVec() != 0.0f) { OwnChangeState(); }
}

void TomatoPlayerMoveState::Enter(void)
{
    // rotationは保持する。再入場時も転がり姿勢を引き継ぐ。
}

void TomatoPlayerMoveState::Update(void)
{
    Vector3 inputVec = InputVec();

    isDash = !isTired &&
        Input::GetIns().GetInfo(KEY_TYPE::PlayerDash).now &&
        inputVec != 0.0f;

    ACCEL_MAX = MOVE_SPEED_MAX(isDash);

    if (inputVec != 0.0f) {
        inputVec.TransMatOwn(MGetRotY(CurrentCamera::Get().GetRotation().GetYaw()));
        moveAccel(inputVec);

        if (isDash) {
            dashStamina -= TimeScale::Get();
            if (dashStamina <= 0.0f) {
                dashStamina = 0.0f;
                isTired = true;
            }
        }
    }

    // 加速度ではなく、実際に移動した距離で転がる。
    // 転がり回転は、所有するプレイヤーの移動処理後に更新する。
}

void TomatoPlayerMoveState::Exit(void)
{
    isDash = false;
    // rotationは戻さない。転がり姿勢を維持する。
}

void TomatoPlayerMoveState::AlwaysUpdate(void)
{
    if (!isDash) {
        dashStamina += TimeScale::Get();
        if (dashStamina >= static_cast<float>(DASH_STAMINA_MAX)) {
            dashStamina = static_cast<float>(DASH_STAMINA_MAX);
            isTired = false;
        }
    }
}

Vector3 TomatoPlayerMoveState::InputVec(void) const
{
    // 返却用一時変数
    Vector3 vec = Vector3();

    // コントローラーの入力を取得
    vec = Input::GetIns().GetLeftStickVec(false).ToVector3XZInvertY();

    // 入力がなければ次にキーボードの入力を取得
    if (vec == 0.0f) {
        if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveRight).now) { vec.x++; }
        if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveLeft).now) { vec.x--; }
        if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveFront).now) { vec.z++; }
        if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveBack).now) { vec.z--; }
        vec.Normalize();
    }

    return vec;
}

float TomatoPlayerMoveState::MOVE_SPEED_MAX(bool dash) const
{
    return stats.speedPower.Value() * (dash ? DASH_SPEED_RATE : 1.0f);
}

void TomatoPlayerMoveState::UpdateRolling(void)
{
    if (ROLL_RADIUS <= 0.0f) { return; }

    // Y成分は除外する。ジャンプ・落下では転がらない。
    const Vector3 velocity = playerTrans.Velocity();
    const Vector3 move(velocity.x, 0.0f, velocity.z);
    const float distance = move.Length();
    if (distance <= 0.0001f) { return; }

    // 前転する面が進行方向を向くように、水平の向きを合わせる。
    const float heading = std::atan2(move.x, move.z);

    // 左右軸まわりの前転を合成してから、進行方向へ向ける。
    spinAngle = std::fmod(spinAngle + distance / ROLL_RADIUS, 2.0f * DX_PI_F);
    const MATRIX newRotationMat = MMult(MGetRotX(spinAngle), MGetRotY(heading));
    playerTrans.SetRotation(Quaternion::FromMatrix(newRotationMat));
}
