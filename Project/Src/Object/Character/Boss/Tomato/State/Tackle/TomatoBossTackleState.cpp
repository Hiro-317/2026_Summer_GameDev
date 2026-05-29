#include "TomatoBossTackleState.h"

TomatoBossTackleState::TomatoBossTackleState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	float MOVE_SPEED, float ROTATION_POW,
	Vector3& pos, Vector3& angle, const Vector3& playerPos,
	TomatoTackleCollOperator* collOperator,
	const std::function<void(void)> resetAngle,
	const std::function<void(void)> DeleteColl,
	const std::function<void(void)> ReviveColl,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> SetCoolTime
) 
	:CharacterStateBase(ownChangeState, isOwnState),
	MOVE_SPEED(MOVE_SPEED), ROTATION_POW(ROTATION_POW),
	pos(pos), angle(angle), playerPos(playerPos),
	collOperator(collOperator),
	resetAngle(resetAngle),
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
	collOperator->ResetStageHit();
}

void TomatoBossTackleState::Update(void)
{
	angle.x += rotPow;

	// 一秒の溜め
	if (time < 180) {
		
		time++;

		// 回転の更新
		moveDir = (playerPos - pos).Normalized();
		angle.y = atan2f(moveDir.x, moveDir.z);
		rotPow += ROTATION_POW;
		collOperator->SetViewPos(Vector3::XZonly(pos.x, pos.z));
		collOperator->SetAngle(Vector3::Yonly(angle.y));
		collOperator->SetScale(Vector3::Xonly(((float)time - 180.0f) / 180.0f + 1.0f));
	}
	else {
		// 位置の更新
		pos += moveDir * MOVE_SPEED;
		collOperator->CollSet(true);
		if (time < 190) {
			time++;
			collOperator->ResetStageHit();
			return;
		}
		if (collOperator->GetStageHit()) {
			DefaultChangeState();
			return;
		}
	}
	angle.x += rotPow;
	collOperator->SetPos(Vector3::XZonly(pos.x, pos.z));
}

void TomatoBossTackleState::Exit(void)
{
	resetAngle();
	collOperator->CollSet(false);
	ReviveColl();
	SetCoolTime();
}

void TomatoBossTackleState::AlwaysUpdate(void)
{
}
