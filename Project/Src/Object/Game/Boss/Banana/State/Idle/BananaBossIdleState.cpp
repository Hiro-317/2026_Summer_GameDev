#include "BananaBossIdleState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

BananaBossIdleState::BananaBossIdleState(
	Vector3& pos, Quaternion& rotation,
	const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],
	const std::function<int(void)> CoolTime,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> PlayIdleAnim,
	const std::function<void(void)> ScratchChangeState,
	const std::function<void(void)> FireChangeState,
	const std::function<void(void)> BanamerangChangeState,
	const std::function<void(void)> BanadrillChangeState
)
	:CharacterStateBase(),
	pos(pos), rotation(rotation),
	playerPos(playerPos),
	CoolTime(CoolTime),
	GetTarget(GetTarget),
	PlayIdleAnim(PlayIdleAnim),
	ScratchChangeState(ScratchChangeState),
	FireChangeState(FireChangeState),
	BanamerangChangeState(BanamerangChangeState),
	BanadrillChangeState(BanadrillChangeState),
	remainingCooldownFrames(0), targetPlayerIndex(0), needsFacingUpdate(true)
{
}

void BananaBossIdleState::Enter(void)
{
	remainingCooldownFrames = CoolTime();
	targetPlayerIndex = GetTarget();
	PlayIdleAnim();
	needsFacingUpdate = true;
}

void BananaBossIdleState::Update(void)
{
	targetPlayerIndex = GetTarget();
	if (targetPlayerIndex < 0 || targetPlayerIndex >= (int)MSG_SENDER_ID::Max || playerPos[targetPlayerIndex] == nullptr) { return; }

	// クールタイムの間
	if (remainingCooldownFrames > 0) {
		remainingCooldownFrames--;

		return;
	}
	// 攻撃を抽選する
	int luck = GetRand(RANDOM);

	if (luck <= SCRATCH_LUCK) {
		ScratchChangeState();
	}
	else if (luck <= BREATH_LUCK){
		FireChangeState();
	}
	else if (luck <= BANAMERANG_LUCK){
		BanamerangChangeState();
	}
	else {
		BanadrillChangeState();
	}

}

void BananaBossIdleState::Exit(void)
{
}

void BananaBossIdleState::AlwaysUpdate(void)
{
}
