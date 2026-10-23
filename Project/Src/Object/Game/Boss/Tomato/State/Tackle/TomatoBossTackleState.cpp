#include "TomatoBossTackleState.h"

#include "../../../../../Common/DataLoad/ParameterLoad.h"
#include "../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Manager/Sound/SoundManager.h"

TomatoBossTackleState::TomatoBossTackleState(
	float MOVE_SPEED, float ROTATION_POW,
	Vector3& pos, Quaternion& rotation, const Transform* trans,
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
	pos(pos), rotation(rotation), trans(trans),
	playerPos(playerPos),
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
	SetCoolTime();
	
	SoundManager::GetIns().Play("TackleCharge");

	collOperator->SetDrawArea(true);
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC));
}

void TomatoBossTackleState::Update(void)
{
	rotation.AddAngle(Vector3(rotPow, 0.0f, 0.0f));

	// 一秒の溜め
	if (time < CHARGE_POW) {
		
		time++;

		// 回転の更新
		moveDir = (*playerPos[target] - pos).Normalized();
		rotation = Quaternion::FromRotationY(atan2f(moveDir.x, moveDir.z));
		rotPow += ROTATION_POW;

		// 予測線の更新
		auto p = Vector3::XZonly(pos.x, pos.z);
		auto a = rotation.ToEulerXZY().y;
		auto s = ((float)time - 180.0f) / 180.0f + 1.0f;

		collOperator->SetViewPos(p);
		collOperator->SetAngle(a);
		collOperator->SetScale(s);

		// 送信
		Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackC, p, s, a));
	}
	else {
		// エフェクトの再生
		if (time == CHARGE_POW) {
		}
		// 位置の更新
		pos += moveDir * MOVE_SPEED;

		collOperator->CollSet(true);
		
		if (time < FORCE_MOVE_TIME) {
			time++;
			SoundManager::GetIns().Stop("TackleCharge");
			SoundManager::GetIns().Play("TackleMove");
			collOperator->ResetStageHit();
		}
		else if (collOperator->GetStageHit()) {
			SoundManager::GetIns().Play("TackleRock");
			DefaultChangeState();
			return;
		}
	}
	rotation.AddAngle(Vector3(rotPow, 0.0f, 0.0f));
	collOperator->SetPos(Vector3::XZonly(trans->pos.x, trans->pos.z));
}

void TomatoBossTackleState::Exit(void)
{
	ResetAngle();
	ReviveColl();
	collOperator->SetDrawArea(false);
	collOperator->CollSet(false);
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackC, false));
}

void TomatoBossTackleState::AlwaysUpdate(void)
{
}
