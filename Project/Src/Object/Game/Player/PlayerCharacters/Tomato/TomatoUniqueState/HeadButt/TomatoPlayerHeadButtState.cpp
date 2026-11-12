#include "TomatoPlayerHeadButtState.h"

#include <algorithm>
#include <cmath>

#include "../../../../../../Common/Transform/Transform.h"
#include "../../../../../../../Manager/Input/InputManager.h"
#include "../../../../../../../Manager/TimeScale/TimeScale.h"
#include "../../../../CommonPlayerState/SimpleAttack/PlayerSimpleAttackCollOperator.h"

TomatoPlayerHeadButtState::TomatoPlayerHeadButtState(
    int COOL_TIME,
    float COLL_START_TIME,
    float COLL_END_TIME,
    float STATE_END_TIME,
    PlayerSimpleAttackCollOperator& collOperator,
    Transform& playerTrans,
    const Vector3& defaultCenterDiff,
    float headButtDistance,
    std::function<void(void)> defaultChangeState
) :
    COOL_TIME(COOL_TIME),
    COLL_START_TIME(COLL_START_TIME),
    COLL_END_TIME(COLL_END_TIME),
    STATE_END_TIME(STATE_END_TIME),
    DEFAULT_CENTER_DIFF(defaultCenterDiff),
    collOperator(collOperator),
    playerTrans(playerTrans),
    defaultChangeState(defaultChangeState),
    time(0.0f),
    coolTimeCounter(0.0f),
    HEADBUTT_DISTANCE(headButtDistance),
    attackDirection(),
    collActive(false)
{
}

void TomatoPlayerHeadButtState::OwnStateConditionUpdate(void)
{
    if (coolTimeCounter > 0.0f) { return; }
    if (Input::GetIns().GetInfo(KEY_TYPE::PlayerSkill1).down) { OwnChangeState(); }
}

void TomatoPlayerHeadButtState::Enter(void)
{
    collOperator.CollOff();
    collOperator.ResetIsHit();
    collOperator.SetAttackWorldOffset(Vector3());
    time = 0.0f;
    collActive = false;
    playerTrans.centerDiff = DEFAULT_CENTER_DIFF;
    coolTimeCounter = static_cast<float>(COOL_TIME);

    // 移動時の前転角に影響されない左右軸から、水平の進行方向を取り出す。
    const Vector3 right = playerTrans.VTrans(Vector3(1.0f, 0.0f, 0.0f));
    const float heading = std::atan2(-right.z, right.x);
    playerTrans.SetRotation(Quaternion::FromRotationY(heading));
    attackDirection = playerTrans.VTrans(Vector3::Zonly(1.0f));
}

void TomatoPlayerHeadButtState::Update(void)
{
    time += TimeScale::Get();

    // 判定開始までに前へ突き出し、判定中は前方を保持してから元へ戻る。
    const float windupEnd = COLL_START_TIME * 0.4f;
    float distance = 0.0f;
    if (time < windupEnd && windupEnd > 0.0f) {
        const float t = std::clamp(time / windupEnd, 0.0f, 1.0f);
        distance = -HEADBUTT_DISTANCE * 0.125f * t * t * (3.0f - 2.0f * t);
    }
    else if (time < COLL_START_TIME && COLL_START_TIME > windupEnd) {
        const float t = std::clamp((time - windupEnd) / (COLL_START_TIME - windupEnd), 0.0f, 1.0f);
        const float easeOut = 1.0f - (1.0f - t) * (1.0f - t) * (1.0f - t);
        distance = HEADBUTT_DISTANCE * (-0.125f + 1.125f * easeOut);
    }
    else if (time < COLL_END_TIME) {
        distance = HEADBUTT_DISTANCE;
    }
    else if (time < STATE_END_TIME && STATE_END_TIME > COLL_END_TIME) {
        const float t = std::clamp((time - COLL_END_TIME) / (STATE_END_TIME - COLL_END_TIME), 0.0f, 1.0f);
        distance = HEADBUTT_DISTANCE * (1.0f - t * t * (3.0f - 2.0f * t));
    }

    // 描画時に最終回転が掛かるため、ワールドの移動量をローカルへ戻して加算する。
    // モデル固有の補正回転が180度でも、必ず進行方向へ突き出す。
    const Vector3 worldOffset = attackDirection * distance;
    const Vector3 localOffset(VTransform(worldOffset.ToVECTOR(), MInverse(playerTrans.FinalRotationMat())));
    playerTrans.centerDiff = DEFAULT_CENTER_DIFF + localOffset;
    collOperator.SetAttackWorldOffset(worldOffset);

    const bool shouldCollide = time >= COLL_START_TIME && time < COLL_END_TIME;
    if (shouldCollide && !collActive) {
        collOperator.CollOn();
        collActive = true;
    }
    else if (!shouldCollide && collActive) {
        collOperator.CollOff();
        collActive = false;
    }

    if (time >= STATE_END_TIME) { defaultChangeState(); }
}

void TomatoPlayerHeadButtState::Exit(void)
{
    // 完了時だけでなく、被ダメージや死亡による中断時にも元へ戻す。
    playerTrans.centerDiff = DEFAULT_CENTER_DIFF;
    collOperator.SetAttackWorldOffset(Vector3());
    collOperator.CollOff();
    collOperator.ResetIsHit();
    collActive = false;
}

void TomatoPlayerHeadButtState::AlwaysUpdate(void)
{
    if (!IsOwnState() && coolTimeCounter > 0.0f) {
        coolTimeCounter = (std::max)(0.0f, coolTimeCounter - TimeScale::Get());
    }
}