#include "GrapeBossIdleState.h"

#include "../../../../../../Manager/Net/NetWorkManager.h"

GrapeBossIdleState::GrapeBossIdleState(
	const std::function<void(void)>& ownChangeState, 
	const std::function<bool(void)>& isOwnState, 
	Vector3& pos, const std::vector<const Vector3*> playerPos,
	const std::function<int(void)> CoolTime,
	const std::function<int(void)> GetTarget,
	const std::function<void(void)> headbuttChangeState,
	const std::function<void(void)> moveChangeState, 
	const std::function<void(void)> stampChangeState,
	const std::function<void(void)> tackleChangeState
)
	:CharacterStateBase(ownChangeState, isOwnState),
	pos(pos), playerPos(playerPos),
	CoolTime(CoolTime),
	GetTarget(GetTarget),
	headbuttChangeState(headbuttChangeState),
	moveChangeState(moveChangeState),
	stampChangeState(stampChangeState),
	tackleChangeState(tackleChangeState)
{
}

void GrapeBossIdleState::Enter(void)
{
	cnt = CoolTime();
	target = GetTarget();
}

void GrapeBossIdleState::Update(void)
{
	if (cnt > 0) {
		cnt--;
		return;
	}
	float distance = (*playerPos.at(target) - pos).Length();
	int luck = GetRand(10000);

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

			tackleChangeState();
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

void GrapeBossIdleState::Exit(void)
{
}

void GrapeBossIdleState::AlwaysUpdate(void)
{
}
