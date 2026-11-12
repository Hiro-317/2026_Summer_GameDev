#include "TomatoBossHeadbuttState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Manager/Sound/SoundManager.h"

TomatoBossHeadbuttState::TomatoBossHeadbuttState(
	const float MOVE_SPEED, const float ATTACK_TIME,
	Vector3& pos, Quaternion& rotation,
	const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],
	TomatoHeadbuttCollOperator* collOperator,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> DeleteColl,
	const std::function<void(void)> ReviveColl,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> SetCoolTime
)
	:CharacterStateBase(),
	MOVE_SPEED(MOVE_SPEED), ATTACK_TIME(ATTACK_TIME),
	pos(pos), rotation(rotation), playerPos(playerPos),
	collOperator(collOperator),
	GetTarget(GetTarget),
	DeleteColl(DeleteColl),
	ReviveColl(ReviveColl),
	DefaultChangeState(DefaultChangeState),
	SetCoolTime(SetCoolTime)
{
}

void TomatoBossHeadbuttState::Enter(void)
{
	target = GetTarget();
	moveDir = (*playerPos[target] - pos).Normalized();
	time = -START_CNT;
	DeleteColl();
	SetCoolTime();
	collOperator->SetDrawArea(true);
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackA));
}

void TomatoBossHeadbuttState::Update(void)
{
	// 前隙用の時間加算
	time++;
	// 前隙中は予測線の更新
	if (time < 0) {

		rotation = Quaternion::FromRotationY(atan2f(moveDir.x, moveDir.z));
		float s = (((float)time + START_CNT) / START_CNT);

		collOperator->SetScale(s);
		collOperator->SetViewPos(pos);
		collOperator->SetAngle(-rotation.ToEulerXZY().y);
		Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackA, pos, s, -rotation.ToEulerXZY().y));
		return;
	}
	// 初めなら音と当たり判定を出す
	if (time == 0) {
		SoundManager::GetIns().Play("Headbutt");
		if (Net::GetIns().IsHost()) {
			collOperator->CollSet(true);
		}
	}
	// 持続時間を超えたらステートを変える
	if (time > ATTACK_TIME) {
		DefaultChangeState();
	}
	else {
		// 位置の更新
		float cnt = time <= ATTACK_TIME / 2.0f ? time : time - (ATTACK_TIME + 1.0f);
		pos += moveDir * MOVE_SPEED * cnt;
	}
	collOperator->SetPos(pos);
}

void TomatoBossHeadbuttState::Exit(void)
{
	collOperator->SetDrawArea(false);
	ReviveColl();
	collOperator->CollSet(false);
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackA, false));
}

void TomatoBossHeadbuttState::AlwaysUpdate(void)
{
}
