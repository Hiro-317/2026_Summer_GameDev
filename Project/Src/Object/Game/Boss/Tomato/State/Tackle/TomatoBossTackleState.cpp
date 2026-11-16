#include "TomatoBossTackleState.h"

#include "../../../../../Common/DataLoad/ParameterLoad.h"
#include "../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Manager/Sound/SoundManager.h"

TomatoBossTackleState::TomatoBossTackleState(
	float MOVE_SPEED, float ROTATION_POW,
	Transform& bossTrans,
	const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],
	TomatoTackleCollOperator* collOperator,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> ResetAngle,
	const std::function<void(void)> DeleteColl,
	const std::function<void(void)> ReviveColl,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> SetCoolTime
)
	:CharacterStateBase(),
	MOVE_SPEED(MOVE_SPEED), ROTATION_POW(ROTATION_POW),
	bossTrans(bossTrans),
	playerPos(playerPos),
	collOperator(collOperator),
	GetTarget(GetTarget),
	ResetAngle(ResetAngle),
	DeleteColl(DeleteColl),
	ReviveColl(ReviveColl),
	DefaultChangeState(DefaultChangeState),
	SetCoolTime(SetCoolTime)
{
	rollSpeed = ROTATION_POW;
}

void TomatoBossTackleState::Enter(void)
{
	elapsedFrames = 0;
	facingRotation = bossTrans.rotation;
	rollAngle = 0.0f;
	rollSpeed = ROTATION_POW;
	DeleteColl();
	targetPlayerIndex = GetTarget();
	collOperator->ResetStageHit();
	SetCoolTime();

	SoundManager::GetIns().Play("TackleCharge");

	collOperator->SetDrawArea(true);
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC));
}

void TomatoBossTackleState::Update(void)
{
	if (targetPlayerIndex < 0 || targetPlayerIndex >= (int)MSG_SENDER_ID::Max || playerPos[targetPlayerIndex] == nullptr) {
		return;
	}

	// 一秒の溜め
	if (elapsedFrames < CHARGE_POW) {

		elapsedFrames++;

		// 回転の更新
		movementDirection = (*playerPos[targetPlayerIndex] - bossTrans.pos).Normalized();
		rollSpeed += ROTATION_POW;

		// 予測線の更新
		auto telegraphPosition = Vector3::XZonly(bossTrans.pos.x, bossTrans.pos.z);
		float telegraphYaw = atan2f(movementDirection.x, movementDirection.z);
		const float telegraphScale = static_cast<float>(elapsedFrames) / CHARGE_POW;

		collOperator->SetViewPos(telegraphPosition);
		collOperator->SetAngle(telegraphYaw);
		collOperator->SetScale(telegraphScale);

		// 送信
		Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC, telegraphPosition, telegraphScale, telegraphYaw));
	}
	else {
		// 突進音の切り替えは開始時に一度だけ行う。
		if (elapsedFrames == CHARGE_POW) {
			Snd::GetIns().Stop("TackleCharge");
			Snd::GetIns().Play("TackleMove");
		}
		// 位置の更新
		bossTrans.pos += movementDirection * MOVE_SPEED;

		collOperator->CollSet(true);

		if (elapsedFrames < FORCE_MOVE_TIME) {
			elapsedFrames++;
			collOperator->ResetStageHit();
		}
		else if (collOperator->GetStageHit()) {
			SoundManager::GetIns().Play("TackleRock");
			DefaultChangeState();
			return;
		}
	}

	// 転がりを含まない「正面方向」だけを補間する
	const float targetAngle = atan2f(movementDirection.x, movementDirection.z);
	const Quaternion targetRotation = Quaternion::FromRotationY(targetAngle);
	const float t = 0.25f * TimeScale::Get();
	facingRotation = Quaternion::Slerp(
		facingRotation, targetRotation,
		t < 0.0f ? 0.0f : (t > 1.0f ? 1.0f : t)
	);

	// X軸回転は累積して保持する
	rollAngle += rollSpeed * TimeScale::Get();
	if (rollAngle > DX_TWO_PI_F) rollAngle -= DX_TWO_PI_F;
	if (rollAngle < -DX_TWO_PI_F) rollAngle += DX_TWO_PI_F;
	const Quaternion rollRotation = Quaternion::FromEulerXZY(
		Vector3(rollAngle, 0.0f, 0.0f)
	);

	// ワールドで向きを決め、そのローカルX軸に沿って転がす
	bossTrans.SetRotation(facingRotation * rollRotation);

	collOperator->SetPos(Vector3::XZonly(bossTrans.pos.x, bossTrans.pos.z));
}

void TomatoBossTackleState::Exit(void)
{
	Snd::GetIns().Stop("TackleCharge");
	Snd::GetIns().Stop("TackleMove");
	ResetAngle();
	ReviveColl();
	collOperator->SetDrawArea(false);
	collOperator->CollSet(false);
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC, false));
}

void TomatoBossTackleState::AlwaysUpdate(void)
{
}
