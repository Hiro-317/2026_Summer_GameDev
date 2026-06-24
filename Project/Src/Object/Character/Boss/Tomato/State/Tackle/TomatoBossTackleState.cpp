#include "TomatoBossTackleState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Manager/Sound/SoundManager.h"

TomatoBossTackleState::TomatoBossTackleState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	float MOVE_SPEED, float ROTATION_POW,
	Vector3& pos, Vector3& angle,
	const std::vector<const Vector3*> playerPos,
	TomatoTackleCollOperator* collOperator,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> ResetAngle,
	const std::function<void(void)> DeleteColl,
	const std::function<void(void)> ReviveColl,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> SetCoolTime
) 
	:CharacterStateBase(ownChangeState, isOwnState),
	MOVE_SPEED(MOVE_SPEED), ROTATION_POW(ROTATION_POW),
	pos(pos), angle(angle), playerPos(playerPos),
	collOperator(collOperator),
	GetTarget(GetTarget),
	ResetAngle(ResetAngle),
	DeleteColl(DeleteColl),
	ReviveColl(ReviveColl),
	DefaultChangeState(DefaultChangeState),
	SetCoolTime(SetCoolTime)
{
	rotPow = ROTATION_POW;
}

void TomatoBossTackleState::Enter(void)
{
	time = 0;
	rotPow = ROTATION_POW;
	DeleteColl();
	target = GetTarget();
	collOperator->ResetStageHit();
	collOperator->SetDrawArea(true);
	SetCoolTime();
	
	SoundManager::GetIns().Play("TackleCharge");

	if (Net::GetIns().IsHost()) {
		Net::GetIns().Send(MsgDataBossInform(MsgDataBossInform::INFORM_TYPE::ChangeAttackB));
	}
}

void TomatoBossTackleState::Update(void)
{
	angle.x += rotPow;

	// 一秒の溜め
	if (time < CHARGE_POW) {
		
		time++;

		// 回転の更新
		moveDir = (*playerPos.at(target) - pos).Normalized();
		angle.y = atan2f(moveDir.x, moveDir.z);
		rotPow += ROTATION_POW;
		collOperator->SetViewPos(Vector3::XZonly(pos.x, pos.z));
		collOperator->SetAngle(Vector3::Yonly(angle.y));
		collOperator->SetScale(Vector3::Xonly(((float)time - 180.0f) / 180.0f + 1.0f));
	}
	else {
		// 位置の更新
		pos += moveDir * MOVE_SPEED;

		if (Net::GetIns().IsHost()) {
			collOperator->CollSet(true);
		}

		if (time < FORCE_MOVE_TIME) {
			time++;
			SoundManager::GetIns().Play("TackleMove");
			collOperator->ResetStageHit();
		}
		else if (collOperator->GetStageHit()) {
			SoundManager::GetIns().Play("TackleRock");
			DefaultChangeState();
			return;
		}
	}
	angle.x += rotPow;
	collOperator->SetPos(Vector3::XZonly(pos.x, pos.z));
}

void TomatoBossTackleState::Exit(void)
{
	ResetAngle();
	ReviveColl();
	collOperator->SetDrawArea(false);
	if (Net::GetIns().IsHost()) {
		collOperator->CollSet(false);
	}
}

void TomatoBossTackleState::AlwaysUpdate(void)
{
}
