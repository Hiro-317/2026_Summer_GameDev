#include "TomatoPlayerStampState.h"
#include "../../../../../../Common/Effect/StampDustActor.h"

#include <algorithm>
#include <cmath>

#include "../../../../../../../Manager/Input/InputManager.h"
#include "../../../../../../../Manager/Camera/CurrentCamera.h"
#include "../../../../../../../Manager/TimeScale/TimeScale.h"
#include "../../../../CommonPlayerState/SimpleAttack/PlayerSimpleAttackCollOperator.h"

TomatoPlayerStampState::TomatoPlayerStampState(
    Transform& playerTrans,
    PlayerSimpleAttackCollOperator& collOperator,
    StampDustActor& landingDust,
    const std::function<void(const Vector3&)>& moveAccel,
    float& accelMax,
    Vector3& velocity,
    int coolTime,
    float jumpPower,
    float fallSpeed,
    const std::function<void(bool)>& setIsGravity,
    const std::function<void(void)>& defaultChangeState
) :
    COOL_TIME(coolTime),
    JUMP_POWER(jumpPower),
    FALL_SPEED(fallSpeed),
    playerTrans(playerTrans),
    collOperator(collOperator),
    landingDust(landingDust),
    moveAccel(moveAccel),
    accelMax(accelMax),
    velocity(velocity),
    setIsGravity(setIsGravity),
    defaultChangeState(defaultChangeState),
    defaultAccelMax(0.0f),
    step(STEP::Rising),
    landingCounter(0.0f),
    coolTimeCounter(0.0f)
{
}

void TomatoPlayerStampState::OwnStateConditionUpdate(void)
{
    if (coolTimeCounter > 0.0f) { return; }
    if (Input::GetIns().GetInfo(KEY_TYPE::PlayerSkill3).down) { OwnChangeState(); }
}

void TomatoPlayerStampState::Enter(void)
{
    collOperator.CollOff();
    collOperator.ResetIsHit();
    collOperator.SetAttackWorldOffset(Vector3());
    step = STEP::Rising;
    landingCounter = 0.0f;
    coolTimeCounter = static_cast<float>(COOL_TIME);
    defaultAccelMax = accelMax;
    accelMax = JUMP_MOVE_SPEED;

    // 通常移動の慣性を持ち込まず、空中の位置調整だけを受け付ける。
    velocity.x = velocity.z = 0.0f;
    velocity.y = JUMP_POWER;
    setIsGravity(false);

    // 通常移動の前転姿勢を起こして跳び上がる。
    const Vector3 right = playerTrans.VTrans(Vector3(1.0f, 0.0f, 0.0f));
    playerTrans.SetRotation(Quaternion::FromRotationY(std::atan2(-right.z, right.x)));
}

void TomatoPlayerStampState::Update(void)
{
    if (step == STEP::Landed) {
        landingCounter += TimeScale::Get();
        if (landingCounter >= LAND_ATTACK_TIME) { defaultChangeState(); }
        return;
    }

    // 共通重力との二重適用を避け、頂点からはスタンプ用の速度で落下する。
    if (step == STEP::Rising) {
        velocity.y -= GRAVITY * TimeScale::Get();
        if (velocity.y <= 0.0f) {
            step = STEP::Falling;
            velocity.y = -FALL_SPEED;
        }
    }
    else {
        velocity.y = -FALL_SPEED;
    }

    JumpMove();
}

void TomatoPlayerStampState::OnGrounded(void)
{
    // 跳躍直前の接地や、上昇中に段差に触れた通知では発動しない。
    if (step != STEP::Falling || velocity.y > 0.0f) { return; }

    step = STEP::Landed;
    landingCounter = 0.0f;
    velocity = Vector3();
    UpdateColliderPosition();
    collOperator.CollOn();
    // FallingからLandedへの切り替え時だけ、着地点に砂埃と赤いオーラを残す。
    landingDust.Emit(playerTrans.pos);
}

void TomatoPlayerStampState::UpdateColliderPosition(void)
{
    collOperator.SetPos(playerTrans.pos);
}

void TomatoPlayerStampState::Exit(void)
{
    // 被ダメージなどによる中断時も、判定・速度上限・重力を確実に戻す。
    collOperator.CollOff();
    collOperator.ResetIsHit();
    collOperator.SetAttackWorldOffset(Vector3());
    accelMax = defaultAccelMax;
    setIsGravity(true);
    landingCounter = 0.0f;
}

void TomatoPlayerStampState::AlwaysUpdate(void)
{
    if (!IsOwnState() && coolTimeCounter > 0.0f) {
        coolTimeCounter = (std::max)(0.0f, coolTimeCounter - TimeScale::Get());
    }
}

void TomatoPlayerStampState::JumpMove(void)
{
    Vector3 vec = Input::GetIns().GetLeftStickVec(false).ToVector3XZInvertY();
    if (vec == 0.0f) {
        if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveRight).now) { vec.x++; }
        if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveLeft).now) { vec.x--; }
        if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveFront).now) { vec.z++; }
        if (Input::GetIns().GetInfo(KEY_TYPE::PlayerMoveBack).now) { vec.z--; }
        vec.Normalize();
    }

    if (vec != 0.0f) {
        vec.TransMatOwn(MGetRotY(CurrentCamera::Get().GetRotation().GetYaw()));
        moveAccel(vec);
    }
}