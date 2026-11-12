#include "TomatoPlayerTackleState.h"

#include <algorithm>
#include <cmath>

#include "../../../../../../../Manager/Input/InputManager.h"
#include "../../../../../../../Manager/Sound/SoundManager.h"
#include "../../../../../../../Manager/TimeScale/TimeScale.h"
#include "../../../../../../../Manager/Camera/CurrentCamera.h"
#include "../../../../CommonPlayerState/SimpleAttack/PlayerSimpleAttackCollOperator.h"

TomatoPlayerTackleState::TomatoPlayerTackleState(
    PlayerSimpleAttackCollOperator& collOperator,
    Transform& playerTrans,
    Vector3& velocity,
    float moveSpeed,
    float rotationPower,
    int coolTime,
    std::function<void(void)> chargeAttackBuff,
    std::function<void(void)> chargeAttackBuffDelete,
    std::function<void(void)> defaultChangeState
) :
    MOVE_SPEED(moveSpeed),
    ROTATION_POW(rotationPower),
    COOL_TIME(coolTime),
    playerTrans(playerTrans),
    velocity(velocity),
    collOperator(collOperator),
    moveDir(),
    heading(0.0f),
    spinAngle(0.0f),
    timeCounter(0.0f),
    chargeCounter(0.0f),
    coolTimeCounter(0.0f),
    step(STEP::Charge),
    chargeAttackBuff(chargeAttackBuff),
    chargeAttackBuffDelete(chargeAttackBuffDelete),
    defaultChangeState(defaultChangeState)
{
}

void TomatoPlayerTackleState::OwnStateConditionUpdate(void)
{
    if (coolTimeCounter > 0.0f) { return; }
    if (Input::GetIns().GetInfo(KEY_TYPE::PlayerSkill2).down) { OwnChangeState(); }
}

void TomatoPlayerTackleState::Enter(void)
{
    collOperator.CollOff();
    collOperator.ResetIsHit();
    collOperator.SetAttackWorldOffset(Vector3());

    // 前転中でも左右軸は水平なので、前後が反転しない向きを取得できる。
    const Vector3 right = playerTrans.VTrans(Vector3(1.0f, 0.0f, 0.0f));
    heading = std::atan2(-right.z, right.x);
    moveDir = Vector3(std::sin(heading), 0.0f, std::cos(heading));
    spinAngle = 0.0f;
    timeCounter = 0.0f;
    chargeCounter = 0.0f;
    coolTimeCounter = 0.0f;
    step = STEP::Charge;

    // 通常移動の慣性だけを止める。重力・接地は共通基盤に任せる。
    velocity.x = velocity.z = 0.0f;
    SoundManager::GetIns().Play("TackleCharge");
}

void TomatoPlayerTackleState::Update(void)
{
    velocity.x = velocity.z = 0.0f;

    if (step == STEP::Charge) {
        Vector3 input = InputVec();
        if (input != 0.0f) {
            input.TransMatOwn(MGetRotY(CurrentCamera::Get().GetRotation().GetYaw()));
            moveDir = input.Normalized();
            heading = std::atan2(moveDir.x, moveDir.z);
        }

        // 入力がない場合は最後に向いた方向を維持する。
        if (Input::GetIns().GetInfo(KEY_TYPE::PlayerSkill2).now) {
            chargeCounter = (std::min)(CHARGE_TIME, chargeCounter + TimeScale::Get());
            coolTimeCounter = static_cast<float>(COOL_TIME) * chargeCounter / CHARGE_TIME;
            UpdateRotation();
            // 長押し中は音声が終わったら再生し直す。
            SoundManager::GetIns().Play("TackleCharge");
            return;
        }

        StartTackle();
    }

    Tackle();
}

void TomatoPlayerTackleState::StartTackle(void)
{
    step = STEP::Tackle;
    timeCounter = COUNT_MAX;
    coolTimeCounter = static_cast<float>(COOL_TIME);
    SoundManager::GetIns().Stop("TackleCharge");
    SoundManager::GetIns().Play("TackleMove");

    // 最大チャージ時の補正は開始時に一度だけ適用する。
    if (chargeCounter >= CHARGE_TIME) { chargeAttackBuff(); }
    collOperator.CollOn();
}

void TomatoPlayerTackleState::Tackle(void)
{
    // 最後の突進フレームの衝突判定を終えてから、次の更新で終了する。
    if (timeCounter <= 0.0f) {
        defaultChangeState();
        return;
    }

    const float elapsed = (std::min)(TimeScale::Get(), timeCounter);
    playerTrans.pos += moveDir * (MOVE_SPEED * elapsed);
    timeCounter -= elapsed;
    UpdateRotation();
}

void TomatoPlayerTackleState::UpdateRotation(void)
{
    // 方向転換しても前転角を失わず、突進開始後も同じ速度で回り続ける。
    spinAngle = std::fmod(spinAngle + ROTATION_POW * TimeScale::Get(), 2.0f * DX_PI_F);
    playerTrans.SetRotation(Quaternion::FromMatrix(MMult(MGetRotX(spinAngle), MGetRotY(heading))));
}

void TomatoPlayerTackleState::Exit(void)
{
    collOperator.CollOff();
    collOperator.ResetIsHit();
    collOperator.SetAttackWorldOffset(Vector3());
    velocity.x = velocity.z = 0.0f;
    timeCounter = 0.0f;
    chargeCounter = 0.0f;
    coolTimeCounter = static_cast<float>(COOL_TIME);
    chargeAttackBuffDelete();
    SoundManager::GetIns().Stop("TackleCharge");
    SoundManager::GetIns().Stop("TackleMove");
}

void TomatoPlayerTackleState::AlwaysUpdate(void)
{
    if (!IsOwnState() && coolTimeCounter > 0.0f) {
        coolTimeCounter = (std::max)(0.0f, coolTimeCounter - TimeScale::Get());
    }
}

Vector3 TomatoPlayerTackleState::InputVec(void) const
{
    Vector3 vec = Input::GetIns().GetLeftStickVec(false).ToVector3XZInvertY();
    if (vec == 0.0f) {
        if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveRight).now) { vec.x++; }
        if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveLeft).now) { vec.x--; }
        if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveFront).now) { vec.z++; }
        if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveBack).now) { vec.z--; }
    }
    return vec.Normalized();
}