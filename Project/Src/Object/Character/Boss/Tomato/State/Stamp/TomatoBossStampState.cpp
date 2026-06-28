#include "TomatoBossStampState.h"

#include "../../../../../ActorBase.h"
#include "../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Manager/Sound/SoundManager.h"


TomatoBossStampState::TomatoBossStampState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	TomatoStampCollOperator* collOperator,
	Vector3& pos, const bool& isGround,
	const std::vector<const Vector3*>playerPos,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> offCollider,
	const std::function<void(void)> onCollider,
	const std::function<void(void)> SetCoolTime
) :CharacterStateBase(ownChangeState, isOwnState),
	pos(pos), collOperator(collOperator), isGround(isGround),
	playerPos(playerPos),
	GetTarget(GetTarget),
	DefaultChangeState(DefaultChangeState),
	offCollider(offCollider),
	onCollider(onCollider),
	SetCoolTime(SetCoolTime)
{
	prevPos = pos.y;
	nowAttackTime = 0;
	attackCnt = 0;
	isAttack = true;
}

void TomatoBossStampState::Enter(void)
{
	target = GetTarget();
	collOperator->SetPos(*playerPos.at(target)); 
	attackPos = Vector3();
	attackDistRate = Vector3();
	collOperator->SetDrawArea(true);
	nowAttackTime = 0;
	prevPos = pos.y - 0.5f;
	attackCnt = 0;
	isAttack = true;
	SetCoolTime();

	SoundManager::GetIns().Play("StampJump");
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackB));
}

void TomatoBossStampState::Update(void)
{
	if (!isGround || isAttack) {
		if (isAttack) {
			isAttack = false;
		}
		if (prevPos < pos.y) {
			prevPos = pos.y;
			pos.y += JUMP_POW;
			offCollider();
		}
		else {
			prevPos = pos.y;
			onCollider();
		}
	}
	else {
		if (attackCnt <= ATTACK_DURATION) {
			if (attackCnt == 0) {
				SoundManager::GetIns().Play("StampLand");
				collOperator->CollSet(true);
			}
			attackCnt++;
		}
		else {
			DefaultChangeState();
		}
	}

	if (nowAttackTime < TIME_RATE) {

		attackPos = collOperator->GetAttackPos();
		attackDistRate = Vector3::XZonly(attackPos.x - pos.x, attackPos.z - pos.z) / (TIME_RATE - (float)nowAttackTime);

		nowAttackTime++;
		pos += attackDistRate;

		// —\‘ªü‚ÌXV
		float scale = 1.0f - (TIME_RATE - (float)nowAttackTime) / TIME_RATE;
		collOperator->SetScale(scale);
		Net::GetIns().Send(MsgDataBossAttackDraw(MsgDataBossAttackDraw::INFORM_TYPE::ChangeAttackB, attackPos, Vector3::Xonly(scale)));
	}
}

void TomatoBossStampState::Exit(void)
{
	collOperator->SetDrawArea(false);
	collOperator->CollSet(false);
	Net::GetIns().Send(MsgDataBossAttackDrawFlg(MsgDataBossAttackDrawFlg::INFORM_TYPE::ChangeAttackB, false));
}

void TomatoBossStampState::AlwaysUpdate(void)
{
}
