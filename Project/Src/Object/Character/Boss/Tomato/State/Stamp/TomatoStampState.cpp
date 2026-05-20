#include "TomatoStampState.h"

#include "../../../../../ActorBase.h"

TomatoStampState::TomatoStampState(
	const std::function<void(void)>& ownChangeState,
	const std::function<bool(void)>& isOwnState,
	TomatoStampCollOperator* collOperator,
	Vector3& pos, const bool& isGround,
	const std::function<void(void)> DefaultChangeState,
	const std::function<void(void)> offCollider,
	const std::function<void(void)> onCollider
) :CharacterStateBase(ownChangeState, isOwnState),
	pos(pos), collOperator(collOperator), isGround(isGround),
	DefaultChangeState(DefaultChangeState),
	offCollider(offCollider),
	onCollider(onCollider)
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

	if (nowAttackTime < TIME_RATE) {

		attackPos = collOperator->GetAttackPos();
		attackDistRate = Vector3::XZonly(attackPos.x - pos.x, attackPos.z - pos.z) / (TIME_RATE - nowAttackTime);

		nowAttackTime++;
		pos += attackDistRate;
	}

	if (collOperator->End()) {
		Exit();
		DefaultChangeState();
	}
}

void TomatoStampState::Exit(void)
{
	collOperator->SetDrawArea(false);
}

void TomatoStampState::AlwaysUpdate(void)
{
}
