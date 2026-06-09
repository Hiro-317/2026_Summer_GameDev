#include "TomatoBossHeadbuttState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

TomatoBossHeadbuttState::TomatoBossHeadbuttState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	const float MOVE_SPEED, const float ATTACK_TIME,
	Vector3& pos, Vector3& angle, const Vector3& playerPos,
	TomatoHeadbuttCollOperator* collOperator,
	const std::function<void(void)> DeleteColl,
	const std::function<void(void)> ReviveColl,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> SetCoolTime
)
	:CharacterStateBase(ownChangeState, isOwnState),
	MOVE_SPEED(MOVE_SPEED), ATTACK_TIME(ATTACK_TIME),
	pos(pos), angle(angle), playerPos(playerPos),
	collOperator(collOperator),
	DeleteColl(DeleteColl),
	ReviveColl(ReviveColl),
	DefaultChangeState(DefaultChangeState),
	SetCoolTime(SetCoolTime)
{
}

void TomatoBossHeadbuttState::Enter(void)
{
	moveDir = (playerPos - pos).Normalized();
	time = -100;
	DeleteColl();
	collOperator->SetDrawArea(true);
	SetCoolTime();
	if (Net::GetIns().IsHost()) {
		Net::GetIns().Send(MsgDataBossInform(MsgDataBossInform::INFORM_TYPE::ChangeAttackA));
	}
}

void TomatoBossHeadbuttState::Update(void)
{
	time++;
	if (time < 0) {
		collOperator->SetScale(((float)time + 100.0f) / 100.0f);
		collOperator->SetViewPos(pos);
		collOperator->SetAngle(Vector3::Yonly(atan2f(moveDir.x, moveDir.z)));
		return;
	}
	if (time == 0) {
		if (Net::GetIns().IsHost()) {
			collOperator->CollSet(true);
		}
	}
	if (time > ATTACK_TIME) {
		DefaultChangeState();
	}
	else {
		// à íuÇÃçXêV
		float cnt = time <= ATTACK_TIME / 2.0f ? time : time - (ATTACK_TIME + 1.0f);
		pos += moveDir * MOVE_SPEED * cnt;
	}
	collOperator->SetPos(pos);
}

void TomatoBossHeadbuttState::Exit(void)
{
	collOperator->SetDrawArea(false);
	ReviveColl();
	if (Net::GetIns().IsHost()) {
		collOperator->CollSet(false);
	}
}

void TomatoBossHeadbuttState::AlwaysUpdate(void)
{
}
