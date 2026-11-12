#include "TomatoBossMoveState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

#include "../../../../../Common/Transform/Transform.h"

TomatoBossMoveState::TomatoBossMoveState(
    float MOVE_SPEED, float ROTATION_POW,
    Transform& bossTrans, const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],
    const std::function<int(void)> GetTarget,
    const std::function<void(void)> ResetAngle,
    const std::function<void(void)> HeadbuttChangeState
)
    :CharacterStateBase(),
    MOVE_SPEED(MOVE_SPEED), ROTATION_POW(ROTATION_POW),
    bossTrans(bossTrans), playerPos(playerPos),
    GetTarget(GetTarget),
    ResetAngle(ResetAngle),
    HeadbuttChangeState(HeadbuttChangeState)
{

}

void TomatoBossMoveState::Enter(void)
{
    target = GetTarget();
    // ステート開始時の向きを基準にする（転がりはリセット）
    facingRotation = bossTrans.rotation;
    rollAngle = 0.0f;
}

void TomatoBossMoveState::Update(void)
{
    if (target < 0 || target >= (int)MSG_SENDER_ID::Max || playerPos[target] == nullptr) {
        return;
    }

    Vector3 diff = *playerPos[target] - bossTrans.pos;
    diff.y = 0.0f;
    if (diff.Length() <= 0.0001f) {
        return;
    }
    moveDir = diff.Normalized();

    // 転がりを含まない「正面方向」だけを補間する
    const float targetAngle = atan2f(moveDir.x, moveDir.z);
    const Quaternion targetRotation = Quaternion::FromRotationY(targetAngle);
    const float t = 0.25f * TimeScale::Get();
    facingRotation = Quaternion::Slerp(
        facingRotation, targetRotation,
        t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t)
    );

    // X軸回転は累積して保持する
    rollAngle += ROTATION_POW * TimeScale::Get();
    if (rollAngle > DX_TWO_PI_F) rollAngle -= DX_TWO_PI_F;
    if (rollAngle < -DX_TWO_PI_F) rollAngle += DX_TWO_PI_F;
    const Quaternion rollRotation = Quaternion::FromEulerXZY(
        Vector3(rollAngle, 0.0f, 0.0f)
    );

    // ワールドで向きを決め、そのローカルX軸に沿って転がす
    bossTrans.SetRotation(facingRotation * rollRotation);

    bossTrans.pos += moveDir * (MOVE_SPEED * TimeScale::Get());

    if ((*playerPos[target] - bossTrans.pos).Length() <= 350.0f) {
        HeadbuttChangeState();
    }
}

void TomatoBossMoveState::Exit(void)
{
    ResetAngle();
}

void TomatoBossMoveState::AlwaysUpdate(void)
{
}
