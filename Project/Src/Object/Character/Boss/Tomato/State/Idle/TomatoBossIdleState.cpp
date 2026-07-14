#include "TomatoBossIdleState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

TomatoBossIdleState::TomatoBossIdleState(
	const std::function<void(void)>& ownChangeState, 
	const std::function<bool(void)>& isOwnState, 
	Vector3& pos, const std::vector<const Vector3*> playerPos,
	const std::function<int(void)> CoolTime,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> headbuttChangeState,
	const std::function<void(void)> moveChangeState, 
	const std::function<void(void)> stampChangeState,
	const std::function<void(void)> tackleChangeState,
	const std::function<bool(void)> hitRock,
	const std::function<void(void)> rockReset
)
	:CharacterStateBase(ownChangeState, isOwnState),
	pos(pos), playerPos(playerPos),
	CoolTime(CoolTime),
	GetTarget(GetTarget),
	headbuttChangeState(headbuttChangeState),
	moveChangeState(moveChangeState),
	stampChangeState(stampChangeState),
	tackleChangeState(tackleChangeState),
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
	float distance = (*playerPos.at(target) - pos).Length();
	int luck = GetRand(10000);

	// 確率で変える
	if (luck <= 4000) {
		// 距離が近いなら頭突き
		if (distance <= 350.0f) {
			headbuttChangeState();
		}
		// 遠いなら移動
		else {
			moveChangeState();
		}
	}
	else if (luck <= 8000) {

		stampChangeState();
	}
	else {
		// 岩に当たっていないならタックル
		if (!hitRock()) {

			tackleChangeState();
		}
		// 当たってたら再抽選
		else {
			if (luck <= 9000) {
				if (distance <= 400.0f) {
					headbuttChangeState();
				}
				else {
					moveChangeState();
				}
			}
			else {
				stampChangeState();

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
