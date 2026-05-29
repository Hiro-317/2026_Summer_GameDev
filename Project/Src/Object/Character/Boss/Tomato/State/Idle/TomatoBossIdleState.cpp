#include "TomatoBossIdleState.h"

TomatoBossIdleState::TomatoBossIdleState(
	const std::function<void(void)>& ownChangeState, 
	const std::function<bool(void)>& isOwnState, 
	Vector3& pos, const Vector3& playerPos,
	const std::function<int(void)> coolTime,
	const std::function<void(void)> headbuttChangeState,
	const std::function<void(void)> moveChangeState, 
	const std::function<void(void)> stampChangeState,
	const std::function<void(void)> tackleChangeState,
	const std::function<bool(void)> hitRock,
	const std::function<void(void)> rockReset
)
	:CharacterStateBase(ownChangeState, isOwnState),
	pos(pos), playerPos(playerPos),
	coolTime(coolTime),
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
	cnt = coolTime();
}

void TomatoBossIdleState::Update(void)
{
	if (cnt > 0) {
		cnt--;
		return;
	}
	float distance = (playerPos - pos).Length();
	int luck = GetRand(10000);

	stampChangeState();
	return;

	if (luck <= 4000) {
		if (distance <= 350.0f) {
			headbuttChangeState();
		}
		else {
			moveChangeState();
		}
	}
	else if (luck <= 8000) {

		stampChangeState();
	}
	else {
		if (!hitRock()) {

			tackleChangeState();
		}
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
