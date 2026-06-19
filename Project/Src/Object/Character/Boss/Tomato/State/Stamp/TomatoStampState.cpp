#include "TomatoStampState.h"

#include "../../../../../ActorBase.h"
#include "../../../../../../Manager/Net/NetWorkManager.h"
#include "../../../../../../Manager/Sound/SoundManager.h"

TomatoStampState::TomatoStampState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	TomatoStampCollOperator* collOperator,
	Vector3& pos, const bool& isGround,
	const std::function<int(void)> PlayerNum, 
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> offCollider,
	const std::function<void(void)> onCollider,
	const std::function<void(void)> SetCoolTime
) :CharacterStateBase(ownChangeState, isOwnState),
	pos(pos), collOperator(collOperator), isGround(isGround),
	PlayerNum(PlayerNum),
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

void TomatoStampState::Enter(void)
{
	attackPos = Vector3();
	attackDistRate = Vector3();
	collOperator->SetDrawArea(true);
	nowAttackTime = 0;
	prevPos = pos.y - 0.5f;
	attackCnt = 0;
	isAttack = true;
	SetCoolTime();
	
	SoundManager::GetIns().Play("StampJump");
	
	if (Net::GetIns().IsHost()) {
		Net::GetIns().Send(MsgDataBossInform(MsgDataBossInform::INFORM_TYPE::ChangeAttackC));
	}
}

void TomatoStampState::Update(void)
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
				if (Net::GetIns().IsHost()) {
					collOperator->CollSet(PlayerNum(), true);
				}
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

		collOperator->SetScale(1.0f - (TIME_RATE - (float)nowAttackTime) / TIME_RATE);
	}
}

void TomatoStampState::Exit(void)
{
	collOperator->SetDrawArea(false);
	if (Net::GetIns().IsHost()) {
		collOperator->CollSet(PlayerNum(), false);
	}
}

void TomatoStampState::AlwaysUpdate(void)
{
}
