#include "TomatoBossIdleState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

TomatoBossIdleState::TomatoBossIdleState(
	Vector3& pos, const Vector3* const (&playerPos)[(int)MSG_SENDER_ID::Max],
	const std::function<int(void)> CoolTime,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> HeadbuttChangeState,
	const std::function<void(void)> MoveChangeState, 
	const std::function<void(void)> StampChangeState,
	const std::function<void(void)> TackleChangeState,
	const std::function<bool(void)> hitRock,
	const std::function<void(void)> rockReset
)
	:CharacterStateBase(),
	pos(pos), playerPos(playerPos),
	CoolTime(CoolTime),
	GetTarget(GetTarget),
	HeadbuttChangeState(HeadbuttChangeState),
	MoveChangeState(MoveChangeState),
	StampChangeState(StampChangeState),
	TackleChangeState(TackleChangeState),
	hitRock(hitRock),
	rockReset(rockReset)
{
}

void TomatoBossIdleState::Enter(void)
{
	cnt = CoolTime();
	target = GetTarget();
}

void TomatoBossIdleState::Update(void)
{
	// クールタイムまで待つ
	if (cnt > 0) {
		cnt--;
		return;
	}
	// 距離と確率を出す
	float distance = (*playerPos[target] - pos).Length();
	int luck = GetRand(RANDOM);

	// 確率で変える
	if (luck <= HEADBUTT_LUCK) {
		// 距離が近いなら頭突き
		if (distance <= DISTANCE) {
			HeadbuttChangeState();
		}
		// 遠いなら移動
		else {
			MoveChangeState();
		}
	}
	else if (luck <= STAMP_LUCK) {

		StampChangeState();
	}
	else {
		// 岩に当たっていないならタックル
		if (!hitRock()) {

			TackleChangeState();
		}
		// 当たってたら再抽選
		else {
			if (luck <= RELOOT_LUCK) {
				if (distance <= DISTANCE) {
					HeadbuttChangeState();
				}
				else {
					MoveChangeState();
				}
			}
			else {
				StampChangeState();
			}
		}
	}
}

void TomatoBossIdleState::Exit(void)
{
}

void TomatoBossIdleState::AlwaysUpdate(void)
{
	rockReset();
}
